
/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/





#include "../gmpsurf.h"

// gmlib
#include <opengl/gmopengl.h>
#include <opengl/gmopenglmanager.h>
#include <scene/gmscene.h>
#include <scene/camera/gmcamera.h>
#include <scene/light/gmlight.h>
#include <scene/render/gmdefaultrenderer.h>
#include <scene/utils/gmmaterial.h>

//// stl
//#include <set>
//#include <string>


namespace GMlib {



  template <typename T, int n>
  inline
  PSurfDefaultVisualizer<T,n>::PSurfDefaultVisualizer()
    : _no_strips(0), _no_strip_indices(0), _strip_size(0) {

    _mode = GL_TRIANGLE_STRIP;
    _init();
  }


  template <typename T, int n>
  inline
  PSurfDefaultVisualizer<T,n>::PSurfDefaultVisualizer(const PSurfDefaultVisualizer<T,n>& copy)
    : PSurfVisualizer<T,n>(copy), _no_strips(0), _no_strip_indices(0), _strip_size(0) {

    _mode = copy._mode;
    _init();
  }


  template <typename T, int n>
  void PSurfDefaultVisualizer<T,n>::render( const SceneObject* obj, const DefaultRenderer* renderer ) const {

    const Camera* cam = renderer->getCamera();
    const HqMatrix<float,3> &mvmat = obj->getModelViewMatrix(cam);
    const HqMatrix<float,3> &pmat  = obj->getProjectionMatrix(cam);
//    const SqMatrix<float,3> &nmat = obj->getNormalMatrix(cam);

    SqMatrix<float,3>        nmat = mvmat.getRotationMatrix();
//    nmat.invertOrthoNormal();
//    nmat.transpose();

    this->glSetDisplayMode();

    _prog.bind(); {

      // Model view and projection matrices
      _prog.uniform( "u_mvmat", mvmat );
      _prog.uniform( "u_mvpmat", pmat * mvmat );
      _prog.uniform( "u_nmat", nmat );

      // Lights
      _prog.bindBufferBase( "DirectionalLights",  renderer->getDirectionalLightUBO(), 0 );
      _prog.bindBufferBase( "PointLights",        renderer->getPointLightUBO(), 1 );
      _prog.bindBufferBase( "SpotLights",         renderer->getSpotLightUBO(), 2 );

      // Material
      const Material &m = obj->getMaterial();
      _prog.uniform( "u_mat_amb", m.getAmb() );
      _prog.uniform( "u_mat_dif", m.getDif() );
      _prog.uniform( "u_mat_spc", m.getSpc() );
      _prog.uniform( "u_mat_shi", m.getShininess() );

      // Normal map
      _prog.uniform( "u_nmap", _nmap, GLenum(GL_TEXTURE0), 0 );

      // Get vertex and texture attrib locations
      GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );
      GL::AttributeLocation tex_loc  = _prog.getAttributeLocation( "in_tex" );

      // Bind and draw
      _vbo.bind();
          _vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexTex2D), reinterpret_cast<const GLvoid *>(0x0) );
          _vbo.enable( tex_loc,  2, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexTex2D), reinterpret_cast<const GLvoid *>(3*sizeof(GLfloat)) );
             draw();
          _vbo.disable( vert_loc );
          _vbo.disable( tex_loc );
      _vbo.unbind();

    } _prog.unbind();
  }



  template <typename T, int n>
  void PSurfDefaultVisualizer<T,n>::renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const {

    _color_prog.bind();
      _color_prog.uniform( "u_color", color );
      _color_prog.uniform( "u_mvpmat", obj->getModelViewProjectionMatrix(renderer->getCamera()) );
      GL::AttributeLocation vertice_loc = _color_prog.getAttributeLocation( "in_vertex" );

      _vbo.bind();
         _vbo.enable( vertice_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexTex2D), reinterpret_cast<const GLvoid *>(0x0) );
            draw();
         _vbo.disable( vertice_loc );
      _vbo.unbind();

    _color_prog.unbind();
  }



  template <typename T, int n>
  void PSurfDefaultVisualizer<T,n>::replot( const DMatrix< DMatrix< Vector<T, n> > >& p, const DMatrix< Vector<float, 3> >& normals,
                                            int /*m1*/, int /*m2*/, int /*d1*/, int /*d2*/, bool closed_u, bool closed_v ) {

    PSurfVisualizer<T,n>::fillStandardVBO( _vbo, p );
    PSurfVisualizer<T,n>::fillTriangleStripIBO( _ibo, p.getDim1(), p.getDim2(), _no_strips, _no_strip_indices, _strip_size );
    PSurfVisualizer<T,n>::fillNMap( _nmap, normals, closed_u, closed_v );
  }



  template <typename T, int n>
  inline
  void PSurfDefaultVisualizer<T,n>::draw() const {

    _ibo.bind();
    for( unsigned int i = 0; i < _no_strips; ++i )
      _ibo.drawElements( _mode, _no_strip_indices, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(i * _strip_size) );
    _ibo.unbind();
  }



  template<typename T,int n>
  void PSurfDefaultVisualizer<T,n>::initShaderProgram() {

    const std::string prog_name    = "psurf_default_prog";
    if( _prog.acquire(prog_name) ) return;


    std::string vs_src =
        GL::OpenGLManager::glslDefHeaderVersionSource() +

        "uniform mat4 u_mvmat, u_mvpmat;\n"
        "\n"
        "in vec4 in_vertex;\n"
        "in vec2 in_tex;\n"
        "\n"
        "out vec4 gl_Position;\n"
        "\n"
        "smooth out vec3 ex_pos;\n"
        "smooth out vec2 ex_tex;\n"
        "\n"
        "void main() {\n"
        "\n"
        "  vec4 v_pos = u_mvmat * in_vertex;\n"
        "  ex_pos = v_pos.xyz * v_pos.w;\n"
        "\n"
        "  ex_tex = in_tex;\n"
        "\n"
        "  gl_Position = u_mvpmat * in_vertex;\n"
        "}\n"
        ;

    std::string fs_src =
        GL::OpenGLManager::glslDefHeaderVersionSource() +
        GL::OpenGLManager::glslFnComputeBlinnPhongLightingSource() +

        "uniform sampler2D u_nmap;\n"
        "uniform mat4      u_mvmat;\n"
        "uniform mat3      u_nmat;\n"
        "\n"
        "uniform vec4      u_mat_amb;\n"
        "uniform vec4      u_mat_dif;\n"
        "uniform vec4      u_mat_spc;\n"
        "uniform float     u_mat_shi;\n"
        "\n"
        "smooth in vec3    ex_pos;\n"
        "smooth in vec2    ex_tex;\n"
        "\n"
        "out vec4 gl_FragColor;\n"
        "\n"
        "void main() {\n"
        "\n"
        "  vec3 nmap_normal = texture( u_nmap, ex_tex.ts).xyz;\n"
        "  vec3 normal = normalize( u_nmat * nmap_normal );\n"
        "\n"
        "  Material mat;\n"
        "  mat.ambient   = u_mat_amb;\n"
        "  mat.diffuse   = u_mat_dif;\n"
        "  mat.specular  = u_mat_spc;\n"
        "  mat.shininess = u_mat_shi;\n"
        "\n"
        "  gl_FragColor = computeBlinnPhongLighting( mat, ex_pos, normal );\n"
        "\n"
        "}\n"
        ;

    bool compile_ok, link_ok;

    GL::VertexShader vshader;
    vshader.create("psurf_default_vs");
    vshader.setPersistent(true);
    vshader.setSource(vs_src);
    compile_ok = vshader.compile();
    if( !compile_ok ) {
      std::cout << "Src:" << std::endl << vshader.getSource() << std::endl << std::endl;
      std::cout << "Error: " << vshader.getCompilerLog() << std::endl;
    }
    assert(compile_ok);

    GL::FragmentShader fshader;
    fshader.create("psurf_default_fs");
    fshader.setPersistent(true);
    fshader.setSource(fs_src);
    compile_ok = fshader.compile();
    if( !compile_ok ) {
      std::cout << "Src:" << std::endl << fshader.getSource() << std::endl << std::endl;
      std::cout << "Error: " << fshader.getCompilerLog() << std::endl;
    }
    assert(compile_ok);

    _prog.create(prog_name);
    _prog.setPersistent(true);
    _prog.attachShader(vshader);
    _prog.attachShader(fshader);
    link_ok = _prog.link();
    if( !link_ok ) {
      std::cout << "Error: " << _prog.getLinkerLog() << std::endl;
    }
    assert(link_ok);
  }


  template <typename T, int n>
  inline
  void PSurfDefaultVisualizer<T,n>::_init() {

      initShaderProgram();

      _color_prog.acquire("color");
      assert(_color_prog.isValid());

      _vbo.create();
      _ibo.create();
      _nmap.create(GL_TEXTURE_2D);
  }

} // END namespace GMlib






