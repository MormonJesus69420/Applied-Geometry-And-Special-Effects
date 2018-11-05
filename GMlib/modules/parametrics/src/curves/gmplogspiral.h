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




#ifndef GM_PARAMETRICS_CURVES_PLOGSPIRAL_H
#define GM_PARAMETRICS_CURVES_PLOGSPIRAL_H


#include "../gmpcurve.h"


namespace GMlib {


  template <typename T>
  class PLogSpiral : public PCurve<T,3> {
    GM_SCENEOBJECT(PLogSpiral)

  public:
    PLogSpiral( T a = T(1), T b = T(0.1), T c = T(0), T s = T(0), T e = T(12) );
    PLogSpiral( const PLogSpiral<T>& copy );
    virtual ~PLogSpiral();

    // Public local functions
    T               getA() const;
    T               getB() const;
    T               getC() const;
    void            setA( T radius = T(1) );
    void            setB( T radius = T(0.1) );
    void            setC( T radius = T(0) );
    void            setSE( T s = T(0), T e = T(12) );

  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void                eval(T t, int d, bool l) const override;
    T                   getStartP() const override;
    T                   getEndP()   const override;


    // Protected data for the curve
    T               _a, _b, _c;
    T               _s, _e;

  }; // END class PLogSpiral

} // END namepace GMlib

// Include PLogSpiral class function implementations
#include "gmplogspiral.c"


#endif // GM_PARAMETRICS_CURVES_PLOGSPIRAL_H
