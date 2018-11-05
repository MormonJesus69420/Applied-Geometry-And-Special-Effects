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





#include "../evaluators/gmevaluatorstatic.h"

// gmlib
#include <scene/visualizers/gmselectorgridvisualizer.h>

namespace GMlib {


  template <typename T>
  PBezierTriangle<T>::PBezierTriangle( const DVector< Vector<T,3> >& c ) {

    _c = c;
//    for( int i = 0; i < _c.getDim(); i++ )
//      _c[i] -= c(0);
//    this->translateGlobal(c(0));

    _selectors = false;
    _c_moved   = false;
    _sgv       = new SelectorGridVisualizer<T>;
  }

  template <typename T>
  PBezierTriangle<T>::PBezierTriangle( const PBezierTriangle<T>& copy ) : PTriangle<T,3>( copy ) {

    _c = copy._c;

    _selectors = false;
    _c_moved   = false;
    _sgv       = new SelectorGridVisualizer<T>;
  }

  template <typename T>
  PBezierTriangle<T>::~PBezierTriangle() {

    delete _sgv;
  }

  template <typename T>
  inline
  void PBezierTriangle<T>::edit( int /*selector*/ ) {

    _c_moved = true;

      if( this->_parent )
        this->_parent->edit( this );

      this->replot();
    _sgv->update();

    _c_moved = false;
  }

  template <typename T>
  inline
  void PBezierTriangle<T>::eval( T u, T v, T w, int d ) const {

    this->_p.setDim(4);

    if( _c.getDim() == 3 ) {
      this->_p[0] = _c[0]*u + _c[1]*v + _c[2]*w;
      this->_p[1] = _c[0];
      this->_p[2] = _c[1];
      this->_p[3] = _c[2];
    }
    else if( _c.getDim() == 6 ) {
      this->_p[0] = _c[0]*(u*u) + _c[1]*(2*u*v) + _c[2]*(2*u*w)
                    + _c[3]*(v*v) + _c[4]*(2*v*w) + _c[5]*(w*w);
      this->_p[1] = _c[0]*(2*u) + _c[1]*(2*v) + _c[2]*(2*w);
      this->_p[2] = _c[1]*(2*u) + _c[3]*(2*v) + _c[4]*(2*w);
      this->_p[3] = _c[2]*(2*u) + _c[4]*(2*v) + _c[5]*(2*w);
    }
    else if( _c.getDim() == 10 ) {
      this->_p[0] = _c[0]*(u*u*u) + _c[1]*(3*u*u*v) + _c[2]*(3*u*u*w) +
                    _c[3]*(3*u*v*v) + _c[4]*(6*u*v*w) + _c[5]*(3*u*w*w) +
                    _c[6]*(v*v*v) + _c[7]*(3*v*v*w) + _c[8]*(3*v*w*w) + _c[9]*(w*w*w);
      this->_p[1] = _c[0]*(3*u*u) + _c[1]*(6*u*v) + _c[2]*(6*u*w) + _c[3]*(3*v*v) + _c[4]*(6*v*w) + _c[5]*(3*w*w);
      this->_p[2] = _c[1]*(3*u*u) + _c[3]*(6*u*v) + _c[4]*(6*u*w) + _c[6]*(3*v*v) + _c[7]*(6*v*w) + _c[8]*(3*w*w);
      this->_p[3] = _c[2]*(3*u*u) + _c[4]*(6*u*v) + _c[5]*(6*u*w) + _c[7]*(3*v*v) + _c[8]*(6*v*w) + _c[9]*(3*w*w);
    }
  }

  template <typename T>
  inline
  DVector< Vector<T,3> > PBezierTriangle<T>::getControlPoints() {

    return _c;
  }

  template <typename T>
  inline
  void PBezierTriangle<T>::hideSelectors() {

    if( !_selectors )
      return;

    // Remove Selector Grid Visualizer
    this->removeVisualizer( _sgv );
    _sgv->reset();

    // Remove selectors
    for( int i = 0; i < _s.getDim(); i++ ) {
      this->remove( _s[i] );
      delete _s[i];
    }

    _selectors = false;
  }

  template <typename T>
  inline
  bool PBezierTriangle<T>::isSelectorsVisible() const {

    return _selectors;
  }

  template <typename T>
  inline
  void PBezierTriangle<T>::setControlPoints( const DVector< Vector<T,3> >& c ) {

    _c = c;
  }

  template <typename T>
  inline
  void PBezierTriangle<T>::showSelectors( bool grid, Color selector_color, Color grid_color ) {

    if( _selectors )
      return;

    _s.setDim( _c.getDim() );
    for( int i = 0, k = 0; i < _s.getDim(); i++ ) {
      Selector<T,3> *sel;
      if( i == 0 )
        sel = new Selector<T,3>( _c[i], k++, this, T(0.2), GMcolor::red() );
      else
        sel = new Selector<T,3>( _c[i], k++, this, T(0.2), selector_color );

      this->insert( sel );
      _s[i] = sel;
    }

    if( grid ) {
      _sgv->setSelectors( _c, 1 );
      this->insertVisualizer( _sgv );
    }

    _selectors = true;
  }



  template <typename T>
  inline
  void PBezierTriangle<T>::updateCoeffs( const Vector<T,3>& d ) {

    if( _c_moved ) {

      HqMatrix<T,3> invmat = this->_matrix;
      invmat.invertOrthoNormal();

      Vector<T,3> diff = invmat * d;
      for( int i = 0; i < _c.getDim(); i++ ) {

        _c[i] += diff;
        _s[i]->translateParent( diff.template toType<float>() );
      }

      this->translateParent( -d.template toType<float>(), false );

      _sgv->update();
    }
  }



} // END namespace GMlib









