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




#ifndef GM_PARAMETRICS_CURVES_PSURFCURVE_H
#define GM_PARAMETRICS_CURVES_PSURFCURVE_H


// GMlib includes+
#include "../gmpcurve.h"
#include "../gmpsurf.h"


namespace GMlib {


  template <typename T>
  class PSurfCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(PSurfCurve)
  public:
    PSurfCurve( PSurf<T,3>* s, const Point<T,2>& p1,  const Point<T,2>& p2);
    PSurfCurve( PSurf<T,3>* s, const Point<T,2>& p1,  const Point<T,2>& p2,
                               const Vector<T,2>& v1, const Vector<T,2>& v2);
    PSurfCurve( const PSurfCurve<T>& copy );

    virtual ~PSurfCurve();


    void                togglePlot();
     void               resample( DVector< DVector< Vector<T, 3> > >& p,
                                      int m, int d, T start, T end ) override;


    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool                isClosed() const;


  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void                eval(T t, int d, bool l) const override;
    T                   getStartP() const override;
    T                   getEndP()   const override;

    // Protected data for the curve
    PSurf<T,3>*         _s;
    Point<T,2>          _p1;
    Point<T,2>          _p2;

    bool                _der_curve;
    Vector<T,2>         _v1;
    Vector<T,2>         _v2;

    Vector<T,2>         _dv;

    bool                _plot;

    // Protected help functions
    void	            eval1( T t, int d) const;
    void	            eval2( T t, int d) const;

  }; // END class PSurfCurve

} // END namepace GMlib


// Include PSurfCurve class function implementations
#include "gmpsurfcurve.c"


#endif // GM_PARAMETRICS_CURVES_PSURFCURVE_H

