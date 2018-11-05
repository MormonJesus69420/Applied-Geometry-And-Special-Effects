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




#ifndef GM_PARAMETRICS_EVALUATORS_BASISTRANGLEERBS_H
#define GM_PARAMETRICS_EVALUATORS_BASISTRANGLEERBS_H


#include "gmbasisevaluator.h"


namespace GMlib {


enum BasisTriangleType
{
    ERBS_RATIONAL,
    BFBS_RATIONAL,
    POLY_RATIONAL,
    ERBS,
    BFBS_2,
    BFBS_3
};

  template <typename T>
  class BasisTriangleERBS {

  public:
    BasisTriangleERBS( BasisTriangleType t);
    BasisTriangleERBS( const BasisTriangleERBS <T>& copy );
    virtual ~BasisTriangleERBS ();

    Vector<Vector<T,3>,20>&   eval( T u, T v, T w );

    void reset( BasisTriangleType t);

  private:
    BasisTriangleType            _type;
    Vector<Vector<T,3>,20>       _A;

    BasisEvaluator<long double>* _b;

    void    getB( DVector<T>& B, T t, int d );
    void    computeB1( T u, T v, T w );
    void    computeB2( T u, T v, T w );
    void    computeB3( T u, T v, T w );
    void    computeB4( T u, T v, T w );

    T       B4_a( T u, T u2, T vw3);
    T       B4_b( T uuw, T vw, T u);
    T       B4_c( T u3, T vw, T u);
    T       B4_d( T u, T vw);

  }; // END class BasisTriangleERBS

} // END namespace GMlib



// Include BasisTriangleERBS  class function implementations
#include "gmbasistriangleerbs.c"



#endif // GM_PARAMETRICS_EVALUATORS_BASISTRANGLEERBS_H
