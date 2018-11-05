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
#include <scene/gmscene.h>
#include <scene/camera/gmcamera.h>
#include <scene/light/gmlight.h>
#include <scene/utils/gmmaterial.h>
#include <opengl/gmopengl.h>

//// stl
//#include <set>
//#include <string>
//#include <cstring>

namespace GMlib {

template <typename T, int n>
PSurfVisualizer<T,n>::PSurfVisualizer() {}


template <typename T, int n>
PSurfVisualizer<T,n>::PSurfVisualizer(const PSurfVisualizer<T,n>& copy)
  : Visualizer(copy)  {}


template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillMap(GL::Texture& map, const DMatrix<DMatrix<Vector<T,n> > > &p, int d1, int d2, bool closed_u, bool closed_v) {

  int m1 = closed_u ? p.getDim1()-1 : p.getDim1();
  int m2 = closed_v ? p.getDim2()-1 : p.getDim2();

  DVector< Vector<float,3> > tex_data(m1*m2);
  Vector<float,3> *ptr = tex_data.getPtr();
  for( int j = 0; j < m1; ++j ) {
    for( int i = 0; i < m2; ++i ) {

      *ptr++ = p(i)(j)(d1)(d2);
    }
  }

  // Create Normal map texture and set texture parameters
  map.texImage2D( 0, GL_RGB16F, m2, m1, 0, GL_RGB, GL_FLOAT, tex_data.getPtr()->getPtr() );
  map.texParameteri( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  map.texParameteri( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  if( closed_v )  map.texParameterf(GL_TEXTURE_WRAP_S, GL_REPEAT);
  else            map.texParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  if( closed_u )  map.texParameterf(GL_TEXTURE_WRAP_T, GL_REPEAT);
  else            map.texParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillNMap( GL::Texture& nmap, const DMatrix< Vector<float, 3> >& ns, bool closed_u, bool closed_v) {

    int m1 = closed_u ? ns.getDim1()-1 : ns.getDim1();
    int m2 = closed_v ? ns.getDim2()-1 : ns.getDim2();
//  int m1 = ns.getDim1();
//  int m2 = ns.getDim2();

////   Fill data
//  DVector< GL::GLNormal > tex_data(m1 * m2);
//  for( int i = 0, k=0; i < m1; ++i )
//      for( int j = 0; j < m2; ++j )
//      {

//          tex_data[k++] = GL::GLNormal{ns(i)(j)(0), ns(i)(j)(1), ns(i)(j)(2)};;
//      }

//  // Create Normal map texture and set texture parameters
//  nmap.texImage2D( 0, GL_RGB16F, m2, m1, 0, GL_RGB, GL_FLOAT, reinterpret_cast<float*>(tex_data.getPtr()) );


  // Quicker upload method (coords must be picked as swizzled "ts" and not "st" in the shader
  nmap.texImage2D( 0, GL_RGB16F, m2, m1, 0, GL_RGB, GL_FLOAT, 0x0 ); // this can be optimized by remembereing the "current" nmap dimensions
  for( int i = 0; i < m1; ++i )
    nmap.texSubImage2D( 0, 0, i, m2, 1, GL_RGB, GL_FLOAT, reinterpret_cast<float*>(ns(i).getPtr()) );

  // set texture parameters for the nmap
  nmap.texParameteri( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  nmap.texParameteri( GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  if( closed_v )  nmap.texParameterf(GL_TEXTURE_WRAP_S, GL_REPEAT);
  else            nmap.texParameterf(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  if( closed_u )  nmap.texParameterf(GL_TEXTURE_WRAP_T, GL_REPEAT);
  else            nmap.texParameterf(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillStandardIBO( GLuint ibo_id, int m1, int m2 ) {

  const int no_indices = m1 * m2;

  int index = 0;
  DVector<GLushort> indices(no_indices);
  GLushort* indice_ptr = indices.getPtr();
  for( int i = 0; i < m1; i++ )
    for( int j = 0; j < m2; j++ )
      *indice_ptr++ = index++;

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_id );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, no_indices * sizeof(GLushort), indices.getPtr(), GL_STATIC_DRAW );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0x0 );
}


template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillStandardVBO(GL::VertexBufferObject &vbo,
                                       const DMatrix<DMatrix<Vector<T,n> > > &p) {

  GLsizeiptr no_vertices = p.getDim1() * p.getDim2() * sizeof(GL::GLVertexTex2D);

  vbo.bufferData( no_vertices, 0x0, GL_STATIC_DRAW );
  GL::GLVertexTex2D *ptr = vbo.mapBuffer<GL::GLVertexTex2D>();
  for( int i = 0; i < p.getDim1(); i++ ) {
    float s = i/float(p.getDim1()-1);
    for( int j = 0; j < p.getDim2(); j++, ptr++ ) {
      // vertex position
      ptr->x = p(i)(j)(0)(0)(0);
      ptr->y = p(i)(j)(0)(0)(1);
      ptr->z = p(i)(j)(0)(0)(2);
      // tex coords
      ptr->s = s;
      ptr->t = j/float(p.getDim2()-1);
    }
  }
  vbo.unmapBuffer();
}



template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillStandardVBO(GL::VertexBufferObject &vbo,
                                       const DVector<DVector<Vector<T,n> > >& p) {

  int nv = p(0).getDim();
  for(int i=1; i< p.getDim(); i++)  nv += p(i).getDim();
  GLsizeiptr no_vertices = nv*sizeof(GL::GLVertexTex2D);

  vbo.bufferData( no_vertices, 0x0, GL_STATIC_DRAW );
  GL::GLVertexTex2D *ptr = vbo.mapBuffer<GL::GLVertexTex2D>();
  for( int i = 0; i < p.getDim(); i++ ) {
    float s = i/float(p.getDim()-1);
    for( int j = 0; j < p(i).getDim(); j++, ptr++ ) {
      // vertex position
      ptr->x = p(i)(j)(0);
      ptr->y = p(i)(j)(1);
      ptr->z = p(i)(j)(2);
      // tex coords
      ptr->s = s;
      ptr->t = j/float(p(i).getDim()-1);
    }
  }
  vbo.unmapBuffer();
}



template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillTriangleStripIBO(GL::IndexBufferObject& ibo, int m1, int m2,
                                                GLuint& no_strips, GLuint& no_strip_indices, GLsizei& strip_size) {

  const int no_indices = (m1-1) * m2 * 2;
  DVector<GLuint> indices(no_indices);

  for( int i = 0; i < m1-1; i++ ) {
    const int i0    = i*m2;
    const int i1    = (i+1)*m2;
    const int idx_i = i0 * 2;

    for( int j = 0; j < m2; j++ ) {
      const int idx_j  = idx_i + (j*2);
      indices[idx_j]   = i0 + j;
      indices[idx_j+1] = i1 + j;
    }
  }

  ibo.bufferData( no_indices * sizeof(GLuint), indices.getPtr(), GL_STATIC_DRAW );

  compTriangleStripProperties( m1, m2, no_strips, no_strip_indices, strip_size );
}



template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillTriangleStripNormalVBO( GLuint vbo_id, DMatrix< Vector<float,3> >& normals ) {

  int no_normals = (normals.getDim1()-1) * normals.getDim2() * 2;

  glBindBuffer( GL_ARRAY_BUFFER, vbo_id );
  glBufferData( GL_ARRAY_BUFFER, no_normals * 3 * sizeof(float), 0x0, GL_DYNAMIC_DRAW );

  float *ptr = static_cast<float*>(glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY ));

  if( ptr ) {
    for( int i = 0; i < normals.getDim1()-1; i++ ) {
      const int idx_i = i * normals.getDim2() * 2;
      for( int j = 0; j < normals.getDim2(); j++ ) {
        // Normals
        const int idx_j = (idx_i + (j*2)) * 3;
        const UnitVector<float,3> n1 = normals[i  ][j];
        const UnitVector<float,3> n2 = normals[i+1][j];
        for( int k = 0; k < 3; k++ ) {
          const int idx_k = idx_j + k;
          ptr[idx_k]   = n1(k);
          ptr[idx_k+3] = n2(k);
        }
      }
    }
  }

  glUnmapBuffer( GL_ARRAY_BUFFER );
  glBindBuffer( GL_ARRAY_BUFFER, 0x0 );
}



template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillTriangleStripTexVBO( GLuint vbo_id, int m1, int m2 ) {

  int no_tex = (m1-1) * m2 * 2;

  glBindBuffer( GL_ARRAY_BUFFER, vbo_id );
  glBufferData( GL_ARRAY_BUFFER, no_tex * 2 * 2 * sizeof(float), 0x0, GL_STATIC_DRAW );

  float *ptr = static_cast<float*>(glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY ));
  if( ptr ) {

    for( int i = 0; i < m1-1; i++ ) {

      const int idx_i = i * m2 * 2;
      for( int j = 0; j < m2; j++ ) {

        // Texture Coords
        const int idx_k = (idx_i + (j*2)) * 2;
        ptr[idx_k]   = i / float( m1 - 1 ); // s1
        ptr[idx_k+1] = j / float( m2 - 1 ); // t1
        ptr[idx_k+2] = (i+1) / float( m1 - 1 ); // s2
        ptr[idx_k+3] = j / float( m2 - 1 ); // t2
      }
    }
  }

  glUnmapBuffer( GL_ARRAY_BUFFER );
  glBindBuffer( GL_ARRAY_BUFFER, 0x0 );
}



template <typename T, int n>
inline
void PSurfVisualizer<T,n>::fillTriangleStripVBO( GLuint vbo_id, DMatrix< DMatrix< Vector<T,n> > >& p, int d1, int d2 ) {

  int no_dp;
  int no_strips;
  int no_verts_per_strips;
  PSurfVisualizer<T,n>::getTriangleStripDataInfo( p, no_dp, no_strips, no_verts_per_strips );

  glBindBuffer( GL_ARRAY_BUFFER, vbo_id );
  glBufferData( GL_ARRAY_BUFFER, no_dp * 3 * sizeof(float), 0x0,  GL_DYNAMIC_DRAW );

  float *ptr = static_cast<float*>(glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY ));
  if( ptr ) {

    for( int i = 0; i < p.getDim1()-1; i++ ) {
      const int idx_i = i * p.getDim2() * 2;
      for( int j = 0; j < p.getDim2(); j++ ) {
        // Populate data
        const int idx_j = (idx_i + (j*2)) * 3;
        for( int k = 0; k < 3; k++ ) {
          const int idx_k = idx_j + k;
          ptr[idx_k]   = p[ i   ][j][d1][d2][k];
          ptr[idx_k+3] = p[ i+1 ][j][d1][d2][k];
        }
      }
    }
  }

  glUnmapBuffer( GL_ARRAY_BUFFER );
  glBindBuffer( GL_ARRAY_BUFFER, 0x0 );
}



template <typename T, int n>
inline
void PSurfVisualizer<T,n>::compTriangleStripProperties(int m1, int m2, GLuint &no_strips, GLuint &no_strip_indices, GLsizei &strip_size) {

  no_strips = m1 - 1;
  no_strip_indices = m2 * 2;
  strip_size = no_strip_indices * sizeof(GLuint);
}



template <typename T, int n>
inline
void PSurfVisualizer<T,n>::getTriangleStripDataInfo( const DMatrix< DMatrix< Vector<T,n> > >& p, int& no_dp, int& no_strips, int& no_verts_per_strips ) {

  no_dp = (p.getDim1()-1) * p.getDim2() * 2;
  no_strips = p.getDim1()-1;
  no_verts_per_strips = p.getDim2()*2;
}



template <typename T, int n>
void PSurfVisualizer<T,n>::replot(
  const DMatrix< DMatrix< Vector<T,n> > >& /*p*/,
  const DMatrix< Vector<float,3> >& /*normals*/,
  int /*m1*/, int /*m2*/, int /*d1*/, int /*d2*/,
  bool /*closed_u*/, bool /*closed_v*/
) {}



template <typename T, int n>
void PSurfVisualizer<T,n>::replot(
  const DVector< DVector< Vector<T, n> > >& /*p*/,
  const DMatrix< Vector<float,3> >& /*normals*/,
  int /*m*/, bool /*closed_u*/, bool /*closed_v*/
) {}



} // END namespace GMlib






