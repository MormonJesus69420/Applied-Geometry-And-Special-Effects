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




#ifndef GM_PARAMETRICS_CURVES_PTCURVE_H
#define GM_PARAMETRICS_CURVES_PTCURVE_H

#include "../gmpcurve.h"

// gmlib
#include <core/types/gmangle.h>


namespace GMlib {


  template <typename T>
  class PTCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(PTCurve)
  public:
    PTCurve( PCurve<T,3>* pcA, PCurve<T,3>* pcB );
    PTCurve( const PTCurve<T>& ptc );

    bool             isClosed() const;
    void             setAngle( T a );
    void             setPA( PCurve<T,3>* pcA );
    void             setPB( PCurve<T,3>* pcB );


  protected:
    // Virtual functiions from PCurve, which have to be implemented locally
    void             eval( T t, int d, bool l = true ) {}
    T                getStartP() { return T(0); }
    T                getEndP() { return T(1); }



    void             localSimulate( double dt );
    void             resample( DVector< DVector< Vector<T,3> > >& p, int m, int d, T start, T end );


  private:
    PCurve<T,3>                         *_pcA;
    PCurve<T,3>                         *_pcB;

    T                                   _angle;
    DVector<DVector<Vector<T,3> > >     _pA;
    DVector<DVector<Vector<T,3> > >     _pB;
    bool                                _resampleA;
    bool                                _resampleB;
    Point<T,6>                          _tPoints;

    void                                _compute( DVector< DVector< Vector<T,3> > >& p );
    void                                _merge_PA_and_PB( int i );
    void                                _resample( PCurve<T,3>* obj, DVector<DVector< Vector<T,3> > >& p, int m, int d );
    Point<T,3>                          _rotate_and_project();

  }; // END class PTCurve

} // END namespace GMlib


// Include PTCurve class function implementations
#include "gmptcurve.c"

#endif // GM_PARAMETRICS_CURVES_PTCURVE_H

