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





// include for syntax highlighting
#include "gmpsurfderivativesvisualizer.h"

namespace GMlib {

  template <typename T, int n>
  PSurfDerivativesVisualizer<T,n>::PSurfDerivativesVisualizer() {

    _prog.acquire("color");
    _vbo.create();

    _color = GMcolor::green();
    _u = 1;
    _v = 0;
    _mode = GM_SURF_DERIVATIVESVISUALIZER_RELATIVE;
    _size = 1.0;

    _no_elements = 0;
  }

  template <typename T, int n>
  PSurfDerivativesVisualizer<T,n>::~PSurfDerivativesVisualizer() {}

  template <typename T, int n>
  inline
  void PSurfDerivativesVisualizer<T,n>::render(const SceneObject *obj, const DefaultRenderer *renderer) const {

    const HqMatrix<float,3> &mvpmat = obj->getModelViewProjectionMatrix(renderer->getCamera());

    _prog.bind(); {

      _prog.uniform( "u_mvpmat", mvpmat );
      _prog.uniform( "u_color", _color );

      GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );

      // Bind & draw
      _vbo.bind();
      _vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, 0, static_cast<const GLvoid*>(0x0) );

      GL_CHECK(glDrawArrays( GL_LINES, 0, _no_elements ));

      _vbo.disable( vert_loc );
      _vbo.unbind();

    } _prog.unbind();

  }

  template <typename T, int n>
  const Color& PSurfDerivativesVisualizer<T,n>::getColor() const {

    return _color;
  }

  template <typename T, int n>
  int PSurfDerivativesVisualizer<T,n>::getDerivativeU() const {

    return _u;
  }

  template <typename T, int n>
  int PSurfDerivativesVisualizer<T,n>::getDerivativeV() const {

    return _v;
  }

  template <typename T, int n>
  GM_SURF_DERIVATIVESVISUALIZER_SIZE PSurfDerivativesVisualizer<T,n>::getMode() const {

    return _mode;
  }

  template <typename T, int n>
  double PSurfDerivativesVisualizer<T,n>::getSize() const {

    return _size;
  }

  template <typename T, int n>
  inline
  void PSurfDerivativesVisualizer<T,n>::replot(
      const DMatrix< DMatrix< Vector<T, n> > >& p,
      const DMatrix< Vector<T,3> >& normals,
      int m1, int m2, int d1, int d2,
      bool closed_u, bool closed_v
  ) {
    GM_UNUSED(normals)
    GM_UNUSED(m1) GM_UNUSED(m2)
    GM_UNUSED(d1) GM_UNUSED(d2)
    GM_UNUSED(closed_u) GM_UNUSED(closed_v)


    assert( p(0)(0).getDim1() >= (_u+1) && p(0)(0).getDim2() >= (_v+1) );


    int no_derivatives = p.getDim1() * p.getDim2();
    _no_elements = no_derivatives * 2;

    _vbo.bufferData(no_derivatives * 2 * 3 * sizeof(GLfloat), 0x0, GL_DYNAMIC_DRAW);

    Vector<GLfloat,3>* ptr = _vbo.mapBuffer< Vector<GLfloat,3> >();
    if( ptr ) {

      switch( _mode ) {
      case GM_SURF_DERIVATIVESVISUALIZER_RELATIVE: {

          for( int i = 0; i < p.getDim1(); i++ )
            for( int j = 0; j < p.getDim2(); j++ ) {

              const Point<T,3> &pos = p(i)(j)(0)(0);
              const Vector<T,3> &v = p(i)(j)(_u)(_v) * _size;

              *(ptr++) = pos;
              *(ptr++) = pos + v;
            }
        }
        break;

      case GM_SURF_DERIVATIVESVISUALIZER_ABSOLUTE: {

          for( int i = 0; i < p.getDim1(); i++ )
            for( int j = 0; j < p.getDim2(); j++ ) {

              const Point<T,3> &pos = p(i)(j)(0)(0);
              const Vector<T,3> &v = p(i)(j)(_u)(_v).getNormalized() * _size;
              *(ptr++) = pos;
              *(ptr++) = pos + v;
            }
        }
        break;

      case GM_SURF_DERIVATIVESVISUALIZER_NORMALIZED: {

          for( int i = 0; i < p.getDim1(); i++ )
            for( int j = 0; j < p.getDim2(); j++ ) {

              const Point<T,3> &pos = p(i)(j)(0)(0);
              const UnitVector<T,3> &v = p(i)(j)(_u)(_v);
              *(ptr++) = pos;
              *(ptr++) = pos + v;
            }
        }
        break;
      }
    }

    _vbo.unmapBuffer();
  }

  template <typename T, int n>
  void PSurfDerivativesVisualizer<T,n>::setColor( const Color& color ) {

    _color = color;
  }

  template <typename T, int n>
  void PSurfDerivativesVisualizer<T,n>::setDerivatives( int u, int v ) {

    if( u < 1 && v < 1 )
      return;

    if( u < 0 || v < 0 )
      return;

    _u = u;
    _v = v;
  }

  template <typename T, int n>
  void PSurfDerivativesVisualizer<T,n>::setMode( GM_SURF_DERIVATIVESVISUALIZER_SIZE mode ) {

    _mode = mode;
  }

  template <typename T, int n>
  void PSurfDerivativesVisualizer<T,n>::setSize( double size ) {

    _size = size;
  }


} // END namespace GMlib
