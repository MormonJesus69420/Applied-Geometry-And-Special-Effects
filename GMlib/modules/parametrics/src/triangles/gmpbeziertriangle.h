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





#ifndef GM_PARAMETRICS_TRIANGLES_PBEZIERTRIANGLE_H
#define GM_PARAMETRICS_TRIANGLES_PBEZIERTRIANGLE_H



#include "../gmptriangle.h"

// gmlib



namespace GMlib {

template <typename T, int n>
class Selector;

template <typename T>
class SelectorGridVisualizer;


  template <typename T>
  class PBezierTriangle : public PTriangle<T,3> {
    GM_SCENEOBJECT(PBezierTriangle)
  public:
    PBezierTriangle( const DVector< Vector<T,3> >& c );
    PBezierTriangle( const PBezierTriangle& copy );
    ~PBezierTriangle();

    DVector< Vector<T,3> >      getControlPoints();
    void                        edit( int selector ) override;
    virtual void                hideSelectors();
    bool                        isSelectorsVisible() const;
    void                        setControlPoints( const DVector< Vector<T,3> >& c );
    virtual void                showSelectors( bool grid, Color selector_color = GMcolor::darkBlue(), Color grid_color = GMcolor::lightGreen() );
    void                        updateCoeffs( const Vector<T,3>& d ) override;

  protected:
    DVector< Vector<T,3> >      _c;

    bool                        _selectors;
    SelectorGridVisualizer<T>  *_sgv;
    DVector< Selector<T,3>* >   _s;
    bool                        _c_moved;

    /* implemented from PTriangle */
    void                        eval( T u, T v, T w, int d ) const override;

  }; // END class PBezierTriangle



} // END namespace GMlib




// Include PBezierTriangle class function implementations
#include "gmpbeziertriangle.c"


#endif // GM_PARAMETRICS_TRIANGLES_PBEZIERTRIANGLE_H
