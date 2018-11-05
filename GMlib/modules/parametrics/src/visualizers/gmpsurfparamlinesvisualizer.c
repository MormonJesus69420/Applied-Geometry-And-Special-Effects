
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

#include "gmpsurfparamlinesvisualizer.h"

#include "../gmpsurf.h"

// gmlib
#include <opengl/gmopengl.h>
#include <scene/gmscene.h>
#include <scene/camera/gmcamera.h>
#include <scene/light/gmlight.h>
#include <scene/utils/gmmaterial.h>

// stl
#include <set>
#include <string>

namespace GMlib {

template <typename T, int n>
PSurfParamLinesVisualizer<T,n>::PSurfParamLinesVisualizer()
  : _no_strips(0), _no_strip_indices(0), _strip_size(0),
    _mat(GMmaterial::blackRubber()) {

  initShaderProgram();

  _vbo.create();
  _ibo.create();
  _nmap.create(GL_TEXTURE_2D);
  _ptex_u.create(GL_TEXTURE_2D);
  _ptex_v.create(GL_TEXTURE_2D);
}

template <typename T, int n>
PSurfParamLinesVisualizer<T,n>::PSurfParamLinesVisualizer(const PSurfParamLinesVisualizer<T,n>& copy)
  : _no_strips(0), _no_strip_indices(0), _strip_size(0),
    _mat(copy._mat) {

  initShaderProgram();

  _vbo.create();
  _ibo.create();
  _nmap.create(GL_TEXTURE_2D);
  _ptex_u.create(GL_TEXTURE_2D);
  _ptex_v.create(GL_TEXTURE_2D);
}

template <typename T, int n>
void PSurfParamLinesVisualizer<T,n>::render( const SceneObject* obj, const DefaultRenderer * renderer) const {

  const Camera* cam = renderer->getCamera();

  const HqMatrix<float,3> &mvmat = obj->getModelViewMatrix(cam);
  const HqMatrix<float,3> &pmat = obj->getProjectionMatrix(cam);

  this->glSetDisplayMode();

  _prog.bind(); {

    // Model view and projection matrices
    _prog.uniform( "u_mvmat", mvmat );
    _prog.uniform( "u_mvpmat", pmat * mvmat );

    // Lights
    _prog.bindBufferBase( "DirectionalLights",  renderer->getDirectionalLightUBO(), 0 );
    _prog.bindBufferBase( "PointLights",        renderer->getPointLightUBO(), 1 );
    _prog.bindBufferBase( "SpotLights",         renderer->getSpotLightUBO(), 2 );

    // Base material
    _prog.uniform( "u_mat_amb", obj->getMaterial().getAmb() );
    _prog.uniform( "u_mat_dif", obj->getMaterial().getDif() );
    _prog.uniform( "u_mat_spc", obj->getMaterial().getSpc() );
    _prog.uniform( "u_mat_shi", obj->getMaterial().getShininess() );

    // Line Material
    _prog.uniform( "u_mat_line_amb", _mat.getAmb() );
    _prog.uniform( "u_mat_line_dif", _mat.getDif() );
    _prog.uniform( "u_mat_line_spc", _mat.getSpc() );
    _prog.uniform( "u_mat_line_shi", _mat.getShininess() );

    // Normal map
    _prog.uniform( "u_nmap", _nmap, (GLenum)GL_TEXTURE0, 0 );
    _prog.uniform( "u_ptex_u", _ptex_u, (GLenum)GL_TEXTURE1, 1 );
    _prog.uniform( "u_ptex_v", _ptex_v, (GLenum)GL_TEXTURE2, 2 );

    // Get vertex and texture attrib locations
    GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );
    GL::AttributeLocation tex_loc = _prog.getAttributeLocation( "in_tex" );

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
inline
void PSurfParamLinesVisualizer<T,n>::draw() const {

  _ibo.bind();
  for( unsigned int i = 0; i < _no_strips; ++i )
    _ibo.drawElements( GL_TRIANGLE_STRIP, _no_strip_indices, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(i * _strip_size) );
  _ibo.unbind();
}


template <typename T, int n>
void PSurfParamLinesVisualizer<T,n>::replot(
  const DMatrix< DMatrix< Vector<T, n> > >& p,
  const DMatrix< Vector<T, 3> >& normals,
  int /*m1*/, int /*m2*/, int /*d1*/, int /*d2*/,
  bool closed_u, bool closed_v
) {

  PSurfVisualizer<T,n>::fillStandardVBO( _vbo, p );
  PSurfVisualizer<T,n>::fillTriangleStripIBO( _ibo, p.getDim1(), p.getDim2(), _no_strips, _no_strip_indices, _strip_size );
  PSurfVisualizer<T,n>::fillNMap( _nmap, normals, closed_u, closed_v );

  generatePTex( 10, 10, 3, 3, closed_u, closed_v );
}

/*!
 *
 *  \param[in] m1 Nuber of U lines
 *  \param[in] m2 Nuber of V lines
 *  \param[in] w1 U line spacing [0,n]
 *  \param[in] w2 V line spacing [0,n]
 *  \param[in] closed_u Closed in U
 *  \param[in] closed_v Closed in V
 */
template <typename T, int n>
void PSurfParamLinesVisualizer<T,n>::generatePTex(int m1, int m2, int s1, int s2, bool closed_u, bool closed_v) {

  DVector<float> tex_data_u, tex_data_v;

  if( m1 < 2 || m2 < 2 || s1 ==0 || s2 == 0 ) {

    tex_data_u = DVector<float>(1,0.0f);
    tex_data_v = DVector<float>(1,0.0f);
  }
  else {

    int m1_t, m2_t;
    m1_t = (m1-1)*(s1+1) + 1;
    m2_t = (m2-1)*(s2+1) + 1;

    tex_data_u = DVector<float>(m1_t,0.0f);
    tex_data_v = DVector<float>(m2_t,0.0f);

    for( int i = 0; i < m1_t; i += (s1+1) )  tex_data_u[i] = 1.0f;
    for( int i = 0; i < m2_t; i += (s2+1) )  tex_data_v[i] = 1.0f;
  }

  // Create U line tex
  _ptex_u.texImage2D( 0, GL_R8, tex_data_u.getDim(), 1, 0, GL_RED, GL_FLOAT, tex_data_u.getPtr() );
  _ptex_u.texParameteri( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  _ptex_u.texParameteri( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  if( closed_u )  _ptex_u.texParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  else            _ptex_u.texParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  if( closed_v )  _ptex_v.texParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  else            _ptex_v.texParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Create V line tex
  _ptex_v.texImage2D( 0, GL_R8, 1, tex_data_v.getDim(), 0, GL_RED, GL_FLOAT, tex_data_v.getPtr() );
  _ptex_v.texParameteri( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  _ptex_v.texParameteri( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  if( closed_u )  _ptex_u.texParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  else            _ptex_u.texParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  if( closed_v )  _ptex_v.texParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  else            _ptex_v.texParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

template <typename T, int n>
void PSurfParamLinesVisualizer<T,n>::initShaderProgram() {

  const std::string prog_name    = "psurf_paramlines_prog";
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
      "uniform sampler2D u_ptex_u;\n"
      "uniform sampler2D u_ptex_v;\n"
      "uniform mat4      u_mvmat;\n"
      "\n"
      "uniform vec4      u_mat_amb;\n"
      "uniform vec4      u_mat_dif;\n"
      "uniform vec4      u_mat_spc;\n"
      "uniform float     u_mat_shi;\n"
      "\n"
      "uniform vec4      u_mat_line_amb;\n"
      "uniform vec4      u_mat_line_dif;\n"
      "uniform vec4      u_mat_line_spc;\n"
      "uniform float     u_mat_line_shi;\n"
      "\n"
      "smooth in vec3    ex_pos;\n"
      "smooth in vec2    ex_tex;\n"
      "\n"
      "out vec4 gl_FragColor;\n"
      "\n"
      "void main() {\n"
      "\n"
      "  mat3 nmat = inverse( transpose( mat3( u_mvmat ) ) );\n"
      "  vec3 nmap_normal = texture( u_nmap, ex_tex.st).xyz;\n"
      "  vec3 normal = normalize( nmat * nmap_normal );\n"
      "\n"
      "  float pf_u = texture( u_ptex_u, ex_tex.st ).r;\n"
      "  float pf_v = texture( u_ptex_v, ex_tex.st ).r;\n"
      "\n"
      "  Material mat;\n"
      "\n"
      "  if( max(pf_u,pf_v) < 0.5 ) {\n"
      "    mat.ambient   = u_mat_amb;\n"
      "    mat.diffuse   = u_mat_dif;\n"
      "    mat.specular  = u_mat_spc;\n"
      "    mat.shininess = u_mat_shi;\n"
      "  }\n"
      "  else {\n"
      "    mat.ambient   = u_mat_line_amb;\n"
      "    mat.diffuse   = u_mat_line_dif;\n"
      "    mat.specular  = u_mat_line_spc;\n"
      "    mat.shininess = u_mat_line_shi;\n"
      "  }\n"
      "\n"
      "  vec4 light_color = vec4(0.0);\n"
      "\n"
      "  gl_FragColor = computeBlinnPhongLighting( mat, ex_pos, normal );\n"
      "\n"
      "}\n"
      ;

  bool compile_ok, link_ok;

  GL::VertexShader vshader;
  vshader.create("psurf_paramlines_vs");
  vshader.setPersistent(true);
  vshader.setSource(vs_src);
  compile_ok = vshader.compile();
  assert(compile_ok);

  GL::FragmentShader fshader;
  fshader.create("psurf_paramlines_fs");
  fshader.setPersistent(true);
  fshader.setSource(fs_src);
  compile_ok = fshader.compile();
  assert(compile_ok);

  _prog.create(prog_name);
  _prog.setPersistent(true);
  _prog.attachShader(vshader);
  _prog.attachShader(fshader);
  link_ok = _prog.link();
  assert(link_ok);
}

} // END namespace GMlib






