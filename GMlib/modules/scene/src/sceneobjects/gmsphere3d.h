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



#ifndef GM_SCENE_SCENEOBJECTS_SPHERE3D_H
#define GM_SCENE_SCENEOBJECTS_SPHERE3D_H


#include "../utils/gmmaterial.h"

// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmarray.h>
#include <opengl/gmopengl.h>
#include <opengl/gmprogram.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>


namespace GMlib {


  /*! \class Sphere3D
   *  \brief Pending Documentation
   *
   *  3D Sphere
   */
  class Sphere3D : public Sphere<float,3> {
  public:
    Sphere3D(float r=1.0, int m1=7, int m2=5);
    Sphere3D(const Sphere<float,3>& s, int m1=7, int m2=5);
    Sphere3D( const Sphere3D& copy );

    void        render( const HqMatrix<float,3>& modelview_projection,
                        const Color& color ) const;
//    void        render( const HqMatrix<float,3>& modelview,
//                        const HqMatrix<float,3>& projection,
//                        const Material& material ) const;

    void        select(const GL::Program& prog) const;

    void        replot(int m1, int m2);

  private:
    int                       _top_bot_verts;
    int                       _mid_strips;
    int                       _mid_strips_verts;
    
    GL::Program               _color_prog;
    GL::Program               _shade_prog;
    GL::Program               _select_prog;
    GL::VertexBufferObject    _vbo_v;
    GL::VertexBufferObject    _vbo_n;

    int                       _m1;
    int                       _m2;

  }; // END class Sphere3D








  /*! Sphere3D::Sphere3D(float r, int m1, int m2)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Sphere3D::Sphere3D(float r, int m1, int m2) : Sphere<float,3>(Point<float,3>(float(0)),r) {

    _shade_prog.acquire("blinn_phong");
    _color_prog.acquire("color");
    _vbo_v.create();
    _vbo_n.create();
    replot(m1,m2);
  }


  /*! Sphere3D::Sphere3D(const Sphere<float,3>& s, int m1, int m2)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Sphere3D::Sphere3D(const Sphere<float,3>& s, int m1, int m2) : Sphere<float,3>(s) {

    _shade_prog.acquire("blinn_phong");
    _color_prog.acquire("color");
    _vbo_v.create();
    _vbo_n.create();
    replot(m1,m2);
  }

  inline
  Sphere3D::Sphere3D( const Sphere3D& copy ): Sphere<float,3>( copy ) {

    _shade_prog.acquire("blinn_phong");
    _color_prog.acquire("color");
    _vbo_v.create();
    _vbo_n.create();
    replot(copy._m1,copy._m2);
  }


  /*! void Sphere3D::display()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Sphere3D::render( const HqMatrix<float,3>& modelview_projection, const Color& color ) const {

    _color_prog.bind(); {

      // Model view and projection matrices
      _color_prog.uniform( "u_mvpmat", modelview_projection );
      _color_prog.uniform( "u_color",  color );

      GL::AttributeLocation vert_loc = _color_prog.getAttributeLocation( "in_vertex" );

      _vbo_v.bind();
      _vbo_v.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0x0) );

      // Draw top and bottom caps
      for( int i = 0; i < 2; i++ )
        glDrawArrays( GL_TRIANGLE_FAN, i * _top_bot_verts, _top_bot_verts );

      // Draw body strips
      for( int i = 0; i < _mid_strips; i++ )
        glDrawArrays( GL_TRIANGLE_STRIP, _top_bot_verts*2 + i*_mid_strips_verts, _mid_strips_verts );
 
      _vbo_v.disable(vert_loc);
      _vbo_v.unbind();

    } _color_prog.unbind();
    
  }

//  /*! void Sphere3D::display()
//   *  \brief Pending Documentation
//   *
//   *  Pending Documentation
//   */
//  inline
//  void Sphere3D::render( const HqMatrix<float,3>& modelview, const HqMatrix<float,3>& projection, const Material& material  ) const {

//    _shade_prog.bind(); {

//      // Model view and projection matrices
//      _shade_prog.uniform( "u_mvmat",  modelview );
//      _shade_prog.uniform( "u_mvpmat", modelview * projection );

//      // Lights
//      _shade_prog.uniformBlockBinding( "Lights", _lights_ubo, 0 );

//      // Get Material Data
//      _shade_prog.uniform( "u_mat_amb",  material.getAmb() );
//      _shade_prog.uniform( "u_mat_dif",  material.getDif() );
//      _shade_prog.uniform( "u_mat_spc",  material.getSpc() );
//      _shade_prog.uniform( "u_mat_shin", material.getShininess() );

//      GL::AttributeLocation vert_loc = _shade_prog.getAttributeLocation( "in_vertex" );
//      GL::AttributeLocation normal_loc = _shade_prog.getAttributeLocation( "in_normal" );

//      _vbo_v.bind();
//      _vbo_v.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0x0) );

//      _vbo_n.bind();
//      _vbo_n.enable( normal_loc, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0x0) );

//      // Draw top and bottom caps
//      for( int i = 0; i < 2; i++ )
//        glDrawArrays( GL_TRIANGLE_FAN, i * _top_bot_verts, _top_bot_verts );

//      // Draw body strips
//      for( int i = 0; i < _mid_strips; i++ )
//        glDrawArrays( GL_TRIANGLE_STRIP, _top_bot_verts*2 + i*_mid_strips_verts, _mid_strips_verts );

//      _vbo_v.bind();
//      _vbo_v.disable(vert_loc);
//      _vbo_n.bind();
//      _vbo_n.disable(normal_loc);
//      _vbo_n.unbind();

//    } _shade_prog.unbind();
//  }

  inline
  void Sphere3D::select( const GL::Program& prog ) const {


    // Model view and projection matrices

    GL::AttributeLocation vert_loc = prog.getAttributeLocation( "in_vertex" );

    _vbo_v.bind();
    _vbo_v.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0x0) );

//    std::cout << "Selthe Sphere3D in color mode; top/bot verts: " << _top_bot_verts << "; mid strips: " << _mid_strips << std::endl;
    // Draw top and bottom caps
    for( int i = 0; i < 2; i++ )
      glDrawArrays( GL_TRIANGLE_FAN, i * _top_bot_verts, _top_bot_verts );

    // Draw body strips
    for( int i = 0; i < _mid_strips; i++ )
      glDrawArrays( GL_TRIANGLE_STRIP, _top_bot_verts*2 + i*_mid_strips_verts, _mid_strips_verts );

    _vbo_v.disable(vert_loc);
    _vbo_v.unbind();

  }

  /*! void Sphere3D::replot(int m1, int m2)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Sphere3D::replot(int m1, int m2) {

    // Check lower replot boundaries
    if( m1 > 1 )
      _m1 = m1;

    if( m2 > 1 )
      _m2 = m2;


    // Set some vars
    const float x = _pos[0];
    const float y = _pos[1];
    const float z = _pos[2];
    const float r = _value;
    _top_bot_verts = _m2+2;
    _mid_strips = (_m1-2);
    _mid_strips_verts = (_m2+1) * 2;


    // Map buffers and allocate data on the GPU
    const unsigned int verts = _top_bot_verts * 2 + _mid_strips * _mid_strips_verts;

    _vbo_v.bufferData( verts * 3 * sizeof(float), 0x0, GL_STATIC_DRAW );
    _vbo_n.bufferData( verts * 3 * sizeof(float), 0x0, GL_STATIC_DRAW );
    float *ptr_v = _vbo_v.mapBuffer<float>();
    float *ptr_n = _vbo_n.mapBuffer<float>();


    // Compute stride in the spheres u and v parametric direction.
    const float du = M_PI / _m1;
    const float dv = M_2PI / _m2;

    // Compute top and bottom triangle fans for the sphere
    {
      float u;
      float su, cu, ru;

      const Arrow<float,3> top = Arrow<float,3>(_pos+Point<float,3>(0,0,r), Vector<float,3>(0,0,1));
      *ptr_v++ = top.getPos()(0);
      *ptr_v++ = top.getPos()(1);
      *ptr_v++ = top.getPos()(2);
      *ptr_n++ = top.getDir()(0);
      *ptr_n++ = top.getDir()(1);
      *ptr_n++ = top.getDir()(2);

      u = M_PI_2 - du;

      su = sin(u);
      cu = cos(u);
      ru = r * cu;

      float v = 0;
      for( int i = 0; i < _m2+1; i++, v += dv ) {

        const float sv = sin(v);
        const float cv = cos(v);

        *ptr_v++ = x + ru*cv;
        *ptr_v++ = y + ru*sv;
        *ptr_v++ = z + r*su;
        *ptr_n++ = cu*cv;
        *ptr_n++ = cu*sv;
        *ptr_n++ = su;
      }


      const Arrow<float,3> bottom = Arrow<float,3>(_pos+Point<float,3>(0,0,-r),Vector<float,3>(0,0,-1));
      *ptr_v++ = bottom.getPos()(0);
      *ptr_v++ = bottom.getPos()(1);
      *ptr_v++ = bottom.getPos()(2);
      *ptr_n++ = bottom.getDir()(0);
      *ptr_n++ = bottom.getDir()(1);
      *ptr_n++ = bottom.getDir()(2);

      u = M_PI_2 - du * (_m1-1);
      su = sin(u);
      cu = cos(u);
      ru = r * cu;

      v = 0;
      for( int i = _m2; i >= 0; i--, v += dv ) {

        const float sv = sin(v);
        const float cv = cos(v);

        *ptr_v++ = x + ru*cv;
        *ptr_v++ = y + ru*sv;
        *ptr_v++ = z + r*su;
        *ptr_n++ = cu*cv;
        *ptr_n++ = cu*sv;
        *ptr_n++ = su;
      }
    }


    // Compute body triangle strips on the sphere
    for( int i = 0; i < _m1-2; i++ ) {

      const float u1 = M_PI_2 - du * (i+1);
      const float u2 = M_PI_2 - du * (i+2);

      const float su1 = sin(u1);
      const float cu1 = cos(u1);
      const float ru1 = r * cu1;

      const float su2 = sin(u2);
      const float cu2 = cos(u2);
      const float ru2 = r * cu2;

      for( int j = 0; j < _m2+1; j++ ) {

        const float v = j * dv;
        const float sv = sin(v);
        const float cv = cos(v);

        *ptr_v++ = x + ru1*cv;
        *ptr_v++ = y + ru1*sv;
        *ptr_v++ = z + r*su1;
        *ptr_n++ = cu1*cv;
        *ptr_n++ = cu1*sv;
        *ptr_n++ = su1;

        *ptr_v++ = x + ru2*cv;
        *ptr_v++ = y + ru2*sv;
        *ptr_v++ = z + r*su2;
        *ptr_n++ = cu2*cv;
        *ptr_n++ = cu2*sv;
        *ptr_n++ = su2;
      }
    }


    // Unmap GPU buffers
    _vbo_v.unmapBuffer();
    _vbo_n.unmapBuffer();
  }


} // END namespace GMlib


#endif // GM_SCENE_SCENEOBJECTS_SPHERE3D_H
