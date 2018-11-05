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




#ifndef GM_PARAMETRICS_EVALUATORS_ERBSEVALUATOR_H
#define GM_PARAMETRICS_EVALUATORS_ERBSEVALUATOR_H



// GMlib includes
#include "gmbasisevaluator.h"


namespace GMlib {


  template <typename T>
  class ERBSEvaluator : public BasisEvaluator<T> {
  public:
    ERBSEvaluator( int m = 1024, T alpha = T(1), T beta = T(1), T gamma = T(1), T lambda = T(0.5) );

    T             getAlpha() const;
    T             getBeta() const;
    T             getGamma() const;
    T             getLambda() const;
    void          setAlpha( T alpha );
    void          setBeta( T beta );
    void          setGamma( T gamma );
    void          setLambda( T lambda );
    void          texParameters( T alpha = T(1), T beta = T(1), T gamma = T(1), T lambda = T(0.5) );

    static ERBSEvaluator<T>*    getInstance();

  protected:
    T             _alpha;
    T             _beta;
    T             _gamma;
    T             _lambda;

    T             getF2( T t );
    T             getPhi( T t );

  private:
    static ERBSEvaluator<T>    *_s_instance;

  }; // END class ERBSEvaluator





} // END namespace GMlib


// Include ERBSEvaluator class function implementations
#include "gmerbsevaluator.c"







#endif // GM_PARAMETRICS_EVALUATORS_ERBSEVALUATOR_H
