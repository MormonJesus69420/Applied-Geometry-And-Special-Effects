/*****************************************************************************
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
*****************************************************************************/




#include "gmselectorgridvisualizer.h"

#include "../selector/gmselector.h"

namespace GMlib {


  template <typename T>
  SelectorGridVisualizer<T>::SelectorGridVisualizer() : _no_indices(0), _line_width(2.0f) {

    _prog.acquire("color");
    _vbo.create();
    _ibo.create();
    _color = GMcolor::lightGreen();
    _scaled = false;
  }


  template <typename T>
  inline
  void SelectorGridVisualizer<T>::setColor( const Color& col ) {
      _color = col;
  }


  template <typename T>
  void SelectorGridVisualizer<T>::_fillVBO() {

      // Fill the vertice buffer
      DVector<GL::GLVertex> dp( _c.getDim() );
      if(_scaled) {
          for( int i = 0; i < dp.getDim(); i++ ) {
              dp[i].x = _scale[0]*((*_c[i])(0));
              dp[i].y = _scale[1]*((*_c[i])(1));
              dp[i].z = _scale[2]*((*_c[i])(2));
          }
      }
      else {
          for( int i = 0; i < dp.getDim(); i++ ) {
              dp[i].x = (*_c[i])(0);
              dp[i].y = (*_c[i])(1);
              dp[i].z = (*_c[i])(2);
          }
      }
      const GLsizeiptr data_size = _c.getDim() * sizeof(GL::GLVertex);
      _vbo.bufferData( data_size, dp.getPtr(), GL_DYNAMIC_DRAW );
  }

  template <typename T>
  inline
  void SelectorGridVisualizer<T>::render(const SceneObject *obj, const DefaultRenderer *renderer) const {

    const HqMatrix<float,3> &mvpmat = obj->getModelViewProjectionMatrix(renderer->getCamera());

    GL_CHECK(::glLineWidth(_line_width));
    _prog.bind(); {

      _prog.uniform( "u_mvpmat", mvpmat );

      _prog.uniform( "u_color", _color) ;
      _prog.uniform( "u_selected", false );

      GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );

      _vbo.bind();
      _vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0x0) );

      _ibo.bind();
      glDrawElements( GL_LINES, _no_indices, GL_UNSIGNED_SHORT, (const GLvoid*)0x0 );
      _ibo.unbind();

      _vbo.disable( vert_loc );
      _vbo.unbind();

    } _prog.unbind();
  }

  template <typename T>
  void SelectorGridVisualizer<T>::reset() {

    _no_indices = 0;
  }

  template <typename T>
  void SelectorGridVisualizer<T>::setLineWidth(float line_width) {

    _line_width = line_width;
  }


  template <typename T>
  void SelectorGridVisualizer<T>::setScale(const Vector<T,3>& s) {

    _scale = s;
    _scaled = true;
  }


  template <typename T>
  void SelectorGridVisualizer<T>::setSelectors( DVector< Point<T,3> >& c, int tp, bool cl) {

    storeSelectorValuePointers(c);

    // Fill VBO
    _fillVBO();

    // Define grid lines
    switch(tp){
      case 0: _makeLines(cl);
              break;
      case 1: _makeTriangs();
              break;
      default: ;
    }
  }

  template <typename T>
  void SelectorGridVisualizer<T>::setSelectors( DVector< Vector<T,3> >& c, int tp, bool cl) {

    storeSelectorValuePointers(c);

    // Fill VBO
    _fillVBO();

    // Define grid lines
    switch(tp){
      case 0: _makeLines(cl);
              break;
      case 1: _makeTriangs();
              break;
      default: ;
    }
  }


  template <typename T>
  inline
  void SelectorGridVisualizer<T>::_makeLines(bool cl)     // Fill IBO
  {
    // Create the indice buffer
    int n = 2*(_c.getDim()-1);
    _no_indices = n + (cl? 2 : 0);
    std::vector<GLushort> indices(_no_indices);
    for(int i=0, j=0; i < n; i+=2, j++) {
      indices[i]   = j;
      indices[i+1] = j + 1;
    }
    if(cl) {
        indices[n]   = 0;
        indices[n+1] = _c.getDim() - 1;
    }

    _ibo.bufferData( _no_indices * sizeof(GLushort), indices.data(), GL_DYNAMIC_DRAW );
  }


  template <typename T>
  inline
  void SelectorGridVisualizer<T>::_makeTriangs()     // Fill IBO
  {
    // Create the indice buffer
    int m = (sqrt(8*_c.getDim()+1)-1)/2;
    DVector<GLushort> indices(_no_indices = 3*(m*(m-1)));
    for( int k=0, i=0; i < m-1; i++) {
      int o1 = (i*(i+1))/2;
      int o2 = ((i+1)*(i+2))/2;
      for( int j = 0, l = o2-o1; j < l; j++ ) {
        indices[k++] = o1+j; indices[k++] = o2+j;
        indices[k++] = o1+j; indices[k++] = o2+j+1;
        indices[k++] = o2+j; indices[k++] = o2+j+1;
      }
    }

    _ibo.bufferData( _no_indices * sizeof(GLushort), indices.getPtr(), GL_DYNAMIC_DRAW );
  }

  template <typename T>
  void SelectorGridVisualizer<T>::_makeQuads(int m1, int m2, bool cu, bool cv) {
      // Fill IBO

      int n1 = m1 - (cu?0:1);
      int n2 = m2 - (cv?0:1);
      _no_indices = (m1 * n2 + n1 * m2) * 2;
      DVector<GLushort> indices(_no_indices);     // Create the indice buffer
      GLushort *iptr = indices.getPtr();

      // "Lines" in v dir
      for( int i = 0; i < m1; i++ ) {
          for( int j = 0; j < m2-1; j++ ) {
              *iptr++ = i * m2 + j;
              *iptr++ = i * m2 + j + 1;
          }
          if(cv) {
              *iptr++ = i * m2 + m2-1;
              *iptr++ = i * m2;
          }
      }

      // "Lines" in u dir
      for( int j = 0; j < m2; j++ ) {
          for( int i = 0; i < m1-1; i++ ) {
              *iptr++ = i * m2 + j;
              *iptr++ = (i+1) * m2 + j;
          }
          if(cu) {
              *iptr++ = (m1-1)*m2+j;
              *iptr++ = j;
          }
      }

      _ibo.bufferData( _no_indices * sizeof(GLushort), indices.getPtr(), GL_DYNAMIC_DRAW );
  }

  template <typename T>
  void SelectorGridVisualizer<T>::setSelectors( DMatrix< Point<T,3> >& c, bool cu, bool cv ) {

    storeSelectorValuePointers(c);

    // Fill VBO
    _fillVBO();

    _makeQuads(c.getDim1(), c.getDim2(), cu, cv);
  }

  template <typename T>
  void SelectorGridVisualizer<T>::setSelectors( DMatrix< Vector<T,3> >& c, bool cu, bool cv ) {

    storeSelectorValuePointers(c);

    // Fill VBO
    _fillVBO();

    _makeQuads(c.getDim1(), c.getDim2(), cu, cv);
  }

  template <typename T>
  void SelectorGridVisualizer<T>::update() {

    GL::GLVertex *ptr = _vbo.mapBuffer<GL::GLVertex>();

    if(_scaled) {
        for( int i = 0; i < _c.getDim(); ++i ) {
            ptr[i].x = _scale[0]*((*_c[i])(0));
            ptr[i].y = _scale[1]*((*_c[i])(1));
            ptr[i].z = _scale[2]*((*_c[i])(2));
        }
    }
    else {
        for( int i = 0; i < _c.getDim(); ++i ) {
            ptr[i].x = (*_c[i])(0);
            ptr[i].y = (*_c[i])(1);
            ptr[i].z = (*_c[i])(2);
        }
    }
    _vbo.unmapBuffer();
  }

  template <typename T>
  void SelectorGridVisualizer<T>::storeSelectorValuePointers( DVector<Point<T,3> >& c ) {

    // Order the selectors in the local Dvector structure
    _c.setDim( c.getDim() );
    for( int i = 0; i < c.getDim(); i++ )
      _c[i] = &c[i];
  }

  template <typename T>
  void SelectorGridVisualizer<T>::storeSelectorValuePointers( DVector<Vector<T,3> >& c ) {

    // Order the selectors in the local Dvector structure
    _c.setDim( c.getDim() );
    for( int i = 0; i < c.getDim(); i++ )
      _c[i] = reinterpret_cast<Point<T,3>*>(&c[i]);
  }

  template <typename T>
  void SelectorGridVisualizer<T>::storeSelectorValuePointers( DMatrix<Point<T,3> >& c ) {

    // Order the selectors in a DVector structure
    _c.setDim( c.getDim1() * c.getDim2() );
    for( int k = 0, i = 0; i < c.getDim1(); i++ )
      for( int j = 0; j < c.getDim2(); j++ )
        _c[k++] = &c[i][j];
  }

  template <typename T>
  void SelectorGridVisualizer<T>::storeSelectorValuePointers( DMatrix<Vector<T,3> >& c ) {

    // Order the selectors in a DVector structure
    _c.setDim( c.getDim1() * c.getDim2() );
    for( int k = 0, i = 0; i < c.getDim1(); i++ )
      for( int j = 0; j < c.getDim2(); j++ )
        _c[k++] = reinterpret_cast<Point<T,3>*>(&c[i][j]);
  }

} // END namespace GMlib
