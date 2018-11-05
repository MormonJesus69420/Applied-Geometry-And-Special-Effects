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




#ifndef GM_PARAMETRICS_EVALUATORS_EVALUATORSTATIC_H
#define GM_PARAMETRICS_EVALUATORS_EVALUATORSTATIC_H


// gmlib
#include <core/containers/gmdmatrix.h>

namespace GMlib {


  template <typename T>
  class EvaluatorStatic {
  public:
    static void evaluateBhp( DMatrix<T>& mat, int degree, T t, T scale = 1 );
    static int  evaluateBSp( DMatrix<T>& mat, T t, const DVector<T>& tv, int d, bool left = true, T scale = 1  );
    static void evaluateBSp2( DMatrix<T>& mat, T t, const DVector<T>& tv, int d, int i, T scale = 1 );

    static void evaluateH3d( DMatrix<T>& mat, int d, T t );
    static void evaluateH5d( DMatrix<T>& mat, int d, T t );

    static int  knotIndex( const DVector<T>& tv, T t, int d, bool left=false );
    static T    knotInterval( const DVector<T>& tv, int d, int n );

  private:
    static T getW( const DVector<T>& tv, T t, int i, int d );
    static T delta( const DVector<T>& tv, int i, int d,  T scale = 1 );
  };



} // end namespace GMlib



// Include function implementations
#include "gmevaluatorstatic.c"


#endif // GM_PARAMETRICS_EVALUATORS_EVALUATORSTATIC_H
