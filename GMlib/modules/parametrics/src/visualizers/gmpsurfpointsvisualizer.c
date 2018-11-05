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






// syntax highlighting
#include "gmpsurfpointsvisualizer.h"


namespace GMlib {

  template <typename T, int n>
  PSurfPointsVisualizer<T,n>::PSurfPointsVisualizer() : _size(1.0f), _color(GMcolor::blueViolet()), _no_points(0) {

    _prog.acquire("color");
    _vbo.create();
  }

  template <typename T, int n>
  PSurfPointsVisualizer<T,n>::PSurfPointsVisualizer(const PSurfPointsVisualizer<T,n>& copy)
    : PSurfVisualizer<T,n>(copy), _size(1.0f), _color(GMcolor::blueViolet()), _no_points(0) {

    _prog.acquire("color");
    _vbo.create();
  }

  template <typename T, int n>
  inline
  void PSurfPointsVisualizer<T,n>::render(const SceneObject *obj, const DefaultRenderer* renderer) const {

    const HqMatrix<float,3> &mvpmat = obj->getModelViewProjectionMatrix(renderer->getCamera());

    _prog.bind(); {

      _prog.uniform( "u_mvpmat", mvpmat );
      _prog.uniform( "u_color", _color );

      GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );

      _vbo.bind();
      _vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, 0, static_cast<const GLvoid*>(0x0) );

      // Draw
      GL_CHECK(::glDrawArrays( GL_POINTS, 0, _no_points));

      _vbo.disable( vert_loc );
      _vbo.unbind();

    } _prog.unbind();

  }

  template <typename T, int n>
  const Color& PSurfPointsVisualizer<T,n>::getColor() const {

    return _color;
  }

  template <typename T, int n>
  float PSurfPointsVisualizer<T,n>::getSize() const {

    return _size;
  }

  template <typename T, int n>
  inline
  void PSurfPointsVisualizer<T,n>::replot( const DMatrix< DMatrix< Vector<T, n> > >& p,
                                          const DMatrix< Vector<T, n> >& normals,
                                          int m1, int m2, int d1, int d2,
                                          bool closed_u, bool closed_v ) {

    _no_points = p.getDim1() * p.getDim2();

    _vbo.bufferData( _no_points * sizeof(GL::GLVertex), 0x0, GL_STATIC_DRAW );


    GL::GLVertex *vtx = _vbo.mapBuffer<GL::GLVertex>();
    if( vtx ) {

      for( int i = 0; i < p.getDim1(); i++ ) {
        for( int j = 0; j < p.getDim2(); j++ ) {
          const Point<T,3> &pos = p(i)(j)(0)(0);
          (*vtx).x = pos(0);
          (*vtx).y = pos(1);
          (*vtx).z = pos(2);
          vtx++;
        }
      }

      _vbo.unmapBuffer();
    }
  }

  template <typename T, int n>
  void PSurfPointsVisualizer<T,n>::setColor( const Color& color ) {

    _color = color;
  }

  template <typename T, int n>
  void PSurfPointsVisualizer<T,n>::setSize( float size ) {

    _size = size;
  }

} // END namespace GMlib
