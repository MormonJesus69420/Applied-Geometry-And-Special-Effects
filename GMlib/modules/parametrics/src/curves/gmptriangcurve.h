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




#ifndef GM_PARAMETRICS_CURVES_PTRIANGCURVE_H
#define GM_PARAMETRICS_CURVES_PTRIANGCURVE_H


// GMlib includes+
#include "../gmpcurve.h"
#include "../gmptriangle.h"
#include "../evaluators/gmerbsevaluator.h"


namespace GMlib {


  template <typename T>
  class PTriangCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(PTriangCurve)
  public:
    PTriangCurve( PTriangle<T,3>* s, const Point<T,3>& p1,  const Point<T,3>& p2);
    PTriangCurve( PTriangle<T,3>* s, const Point<T,3>& p1,  const Point<T,3>& p2,
                                     const Vector<T,3>& v1, const Vector<T,3>& v2);
    PTriangCurve( const PTriangCurve<T>& copy );

    virtual ~PTriangCurve();

    bool          isClosed() const override;

    void          togglePlot();

    void  resample( DVector< DVector< Vector<T, 3> > >& p, int m, int d, T start, T end ) override;

    DVector<DVector<Vector<T,3> > >& getSample3(int m);

  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void                eval(T t, int d, bool l) const override;
    T                   getStartP() const override;
    T                   getEndP()   const override;

    PTriangle<T,3>*             _s;

    Point<T,3>                  _p1;
    Point<T,3>                  _p2;

    bool                        _der_curve;
    Vector<T,3>                 _v1;
    Vector<T,3>                 _v2;

    bool                        _plot;

    mutable
    ERBSEvaluator<long double>  _basis;

    private:

    void	     eval1( T t, int d) const;
    void	     eval2( T t, int d) const;
    void	     eval12( T t);  // d=1
    void	     eval123( T t); // d=2

    Vector<T,3>  dS(Vector<T,3>& Su, Vector<T,3>& Sv, Vector<T,3>& Sw, Vector<T,3>& h) const;

  }; // END class PTriangCurve

} // END namepace GMlib


// Include PSurfCurve class function implementations
#include "gmptriangcurve.c"


#endif // GM_PARAMETRICS_CURVES_PTRIANGCURVE_H

