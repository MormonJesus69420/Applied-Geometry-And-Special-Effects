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



#include "gmpcurvevisualizer.h"

// gmlib
#include <opengl/gmopengl.h>

namespace GMlib {

  template <typename T, int n>
  PCurveVisualizer<T,n>::PCurveVisualizer(): _p(nullptr) {}

  template <typename T, int n>
  PCurveVisualizer<T,n>::PCurveVisualizer(std::vector<DVector<Vector<T,3>>>& p): _p(&p) {}

  template <typename T, int n>
  PCurveVisualizer<T,n>::PCurveVisualizer(const PCurveVisualizer<T,n>& copy): Visualizer(copy), _p(copy._p) {}

  template <typename T, int n>
  PCurveVisualizer<T,n>::~PCurveVisualizer() {}

  template <typename T, int n>
  void PCurveVisualizer<T,n>::fillStandardVBO( GL::VertexBufferObject &vbo,
                                               const std::vector< DVector< Vector<T, n>>>& p,
                                               int d,
                                               bool scale,
                                               const Vector<T,n>& s) {

    GLsizeiptr buffer_size = p.size() * sizeof(GL::GLVertex);

    vbo.bufferData( buffer_size, 0x0, GL_STATIC_DRAW );

    GL::GLVertex *ptr = vbo.mapBuffer<GL::GLVertex>();
    if( ptr ) {
        if(scale) {
            for( unsigned int i = 0; i < p.size(); i++ ) {
                ptr->x = (GLfloat)(s(0) * p[i](d)(0));
                ptr->y = (GLfloat)(s(1) * p[i](d)(1));
                ptr->z = (GLfloat)(s(2) * p[i](d)(2));
                ptr++;
            }
        }
        else
        {
            for( unsigned int i = 0; i < p.size(); i++ ) {
                ptr->x = (GLfloat)p[i](d)(0);
                ptr->y = (GLfloat)p[i](d)(1);
                ptr->z = (GLfloat)p[i](d)(2);
                ptr++;
            }
        }
    }
    vbo.unmapBuffer();
  }

  template <typename T, int n>
  void PCurveVisualizer<T,n>::replot( const std::vector< DVector< Vector<T, n> > >& /*p*/,
                                      int /*m*/, int /*d*/, bool /*closed*/ ) {}

} // END namespace GMlib
