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




#ifndef GM_PARAMETRICS_EVALUATORS_BASISEVALUATOR_H
#define GM_PARAMETRICS_EVALUATORS_BASISEVALUATOR_H


// gmlib
#include <core/containers/gmdmatrix.h>
#include <core/containers/gmdvector.h>

// stl
#include <cmath>



namespace GMlib {



  template <typename T>
  class BasisEvaluator {
  public:
    BasisEvaluator( int m = 1024 );
    virtual ~BasisEvaluator();

    T               getDer( int i ) const;
    T               getDer1() const;
    T               getDer1( T t );
    T               getDer2() const;
    T               getDer2( T t );
    T               getDt() const;
    int             getResolution() const;
    T               getScale() const;
    T               getScale1() const;
    T               getScale2() const;

    void            set( T t = 0, T dt = 1 );

    T               operator () ( T t );

  protected:
    int             _m;

    DMatrix<T>      _a;
    DVector<T>      _b;

    int             _local;         //  j
    T               _local_dt;        //  dt
    T               _dt;              // _dt

    T               _tk;
    T               _dtk;

    int             _der;

    T               _scale;
    T               _scale1;
    T               _scale2;

    virtual T       getF2( T t ) = 0;
    virtual T       getIntegral( T a, T b, T sum, T eps );
    virtual T       getPhi( T t ) = 0;

    virtual void    init( int m );
    virtual void    interpolate( int i, double p0, double p1, double f0, double f1 );

  private:
    virtual void    _prepare( T t );


  }; // END class BasisEvaluator

} // END namespace GMlib


// Include BasisEvaluator class function implementations
#include "gmbasisevaluator.c"


#endif // GM_PARAMETRICS_EVALUATORS_BASISEVALUATOR_H
