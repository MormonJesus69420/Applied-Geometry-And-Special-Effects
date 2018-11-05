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




#ifndef GM_PARAMETRICS_EVALUATORS_BFBSEVALUATOR_H
#define GM_PARAMETRICS_EVALUATORS_BFBSEVALUATOR_H



#include "gmbasisevaluator.h"


namespace GMlib {


  template <typename T>
  class BFBSEvaluator : public BasisEvaluator<T> {
  public:
    BFBSEvaluator( int m = 1024, int ik = 3, int ikp1 = 3 );

    void      setIk( int ik );
    void      setIkp1( int ikp1 );
    void      texParameters( int ik, int ikp1 );


  protected:
    int       _ik;
    int       _ikp1;

    int       getFact( int m );
    T         getF2( T t );
    T         getPhi( T t );

  }; // END class BFBSEvaluator

} // END namespace GMlib


// Include BFBSEvaluator class function implementations
#include "gmbfbsevaluator.c"


#endif // GM_PARAMETRICS_EVALUATORS_BFBSEVALUATOR_H
