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





#include "../gmtrianglesystem.h"

// gmlib
#include <opengl/gmopengl.h>
#include <opengl/gmopenglmanager.h>
#include <scene/render/gmdefaultrenderer.h>


namespace GMlib {

  template <typename T>
  TriangleFacetsDefaultVisualizer<T>::TriangleFacetsDefaultVisualizer() :
    _no_elements(0) {

    initShader();

    _color_prog.acquire("color");
    _vbo.create();
    _ibo.create();

    _colors.push_back(GMcolor::blue());
    _colors.push_back(GMcolor::red());
  }

  template <typename T>
  TriangleFacetsDefaultVisualizer<T>::~TriangleFacetsDefaultVisualizer() {}

  template <typename T>
  inline
  void TriangleFacetsDefaultVisualizer<T>::render(const SceneObject *obj, const DefaultRenderer *renderer) const {


    const Camera* cam = renderer->getCamera();

    const HqMatrix<float,3> &mvmat = obj->getModelViewMatrix(cam);
    const HqMatrix<float,3> &pmat = obj->getProjectionMatrix(cam);

    SqMatrix<float,3>        nmat = mvmat.getRotationMatrix();

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

      // Get Material Data
      const Material &m = obj->getMaterial();
      _prog.uniform( "u_mat_amb", m.getAmb() );
      _prog.uniform( "u_mat_dif", m.getDif() );
      _prog.uniform( "u_mat_spc", m.getSpc() );
      _prog.uniform( "u_mat_shi", m.getShininess() );

      GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );
      GL::AttributeLocation normal_loc = _prog.getAttributeLocation( "in_normal" );

      _vbo.bind();
      _vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE,  sizeof(GL::GLVertexNormal), 0x0 );
      _vbo.enable( normal_loc, 3, GL_FLOAT, GL_TRUE, sizeof(GL::GLVertexNormal), reinterpret_cast<const GLvoid*>(sizeof(GL::GLVertex)) );

      draw();

      _vbo.disable( vert_loc );
      _vbo.disable( normal_loc );
      _vbo.unbind();

    } _prog.unbind();
  }

  template <typename T>
  inline
  void TriangleFacetsDefaultVisualizer<T>::replot(const TriangleFacets<T> *tf)  {

    TriangleFacetsVisualizer<T>::fillStandardVBO( _vbo, tf );
    TriangleFacetsVisualizer<T>::fillStandardIBO( _ibo, tf );

    _no_elements = tf->getNoTriangles() * 3;
  }

  template <typename T>
  inline
  void TriangleFacetsDefaultVisualizer<T>::draw() const {

    _ibo.bind();
    _ibo.drawElements( GL_TRIANGLES, _no_elements, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0x0) );
    _ibo.unbind();
  }

  template <typename T>
  void TriangleFacetsDefaultVisualizer<T>::initShader() {

    const std::string prog_name = "triangle_facets_prog";

    if(_prog.acquire(prog_name)) return;

    std::string vs_str =
      GMlib::GL::OpenGLManager::glslDefHeaderVersionSource() +

      "uniform mat4 u_mvmat, u_mvpmat;\n"
      "\n"
      "in vec4 in_vertex;\n"
      "in vec4 in_normal;\n"
      "in vec2 in_tex;\n"
      "\n"
      "out vec4 gl_Position;\n"
      "\n"
      "smooth out vec3 ex_pos;\n"
      "smooth out vec3 ex_normal;\n"
      "\n"
      "void main() {\n"
      "\n"
      "  // Transform the normal to view space\n"
      "  mat3 nmat = inverse( transpose( mat3( u_mvmat ) ) );\n"
      "  ex_normal = nmat * vec3(in_normal);\n"
      "\n"
      "  // Transform position into view space;\n"
      "  vec4 v_pos = u_mvmat * in_vertex;\n"
      "  ex_pos = v_pos.xyz * v_pos.w;\n"
      "\n"
      "  // Compute vertex position\n"
      "  gl_Position = u_mvpmat * in_vertex;\n"
      "}\n"
      ;

    std::string fs_str =
      GMlib::GL::OpenGLManager::glslDefHeaderVersionSource() +
      GMlib::GL::OpenGLManager::glslFnComputeBlinnPhongLightingSource() +

      "uniform mat4      u_mvmat;\n"
      "uniform mat3      u_nmat;\n"
      "\n"
      "uniform vec4      u_mat_amb;\n"
      "uniform vec4      u_mat_dif;\n"
      "uniform vec4      u_mat_spc;\n"
      "uniform float     u_mat_shi;\n"
      "\n"
      "smooth in vec3    ex_pos;\n"
      "smooth in vec3    ex_normal;\n"
      "\n"
      "out vec4 gl_FragColor;\n"
      "\n"
      "void main() {\n"
      "\n"
      "  vec3 normal = normalize( u_nmat * ex_normal );\n"
      "\n"
      "  Material mat;\n"
      "  mat.ambient   = u_mat_amb;\n"
      "  mat.diffuse   = u_mat_dif;\n"
      "  mat.specular  = u_mat_spc;\n"
      "  mat.shininess = u_mat_shi;\n"
      "\n"
      "  gl_FragColor = computeBlinnPhongLighting( mat, ex_pos, normal );\n"
      "}\n"
      ;

    bool compile_ok, link_ok;

    GMlib::GL::VertexShader vshader;
    vshader.create("phong_tf_vs");
    vshader.setPersistent(true);
    vshader.setSource(vs_str);
    compile_ok = vshader.compile();
    assert(compile_ok);

    GMlib::GL::FragmentShader fshader;
    fshader.create("phong_tf_fs");
    fshader.setPersistent(true);
    fshader.setSource(fs_str);
    compile_ok = fshader.compile();
    if( !compile_ok ) {
      std::cout << "Src:" << std::endl << fshader.getSource() << std::endl << std::endl;
      std::cout << "Error: " << fshader.getCompilerLog() << std::endl;
    }
    assert(compile_ok);

    _prog.create(prog_name);
    _prog.setPersistent(true);
    _prog.attachShader( vshader );
    _prog.attachShader( fshader );
    link_ok = _prog.link();
    assert(link_ok);

  }

  template <typename T>
  inline
  void TriangleFacetsDefaultVisualizer<T>::renderGeometry(const SceneObject* obj, const Renderer *renderer, const Color &color ) const {

    _color_prog.bind(); {
        _color_prog.uniform("u_color", color);
        _color_prog.uniform( "u_mvpmat", obj->getModelViewProjectionMatrix(renderer->getCamera()) );
        GL::AttributeLocation vertice_loc = _color_prog.getAttributeLocation( "in_vertex" );

        _vbo.bind();
        _vbo.enable( vertice_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexNormal), reinterpret_cast<const GLvoid*>(0x0) );

        draw();

        _ibo.unbind();
        _vbo.disable( vertice_loc );
        _vbo.unbind();
      } _color_prog.unbind();
  }

} // END namespace GMlib
