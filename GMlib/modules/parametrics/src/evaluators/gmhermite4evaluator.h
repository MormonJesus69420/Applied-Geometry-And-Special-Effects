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




#ifndef GM_PARAMETRICS_EVALUATORS_HERMITE4EVALUATOR_H
#define GM_PARAMETRICS_EVALUATORS_HERMITE4EVALUATOR_H


// GMlib includes
#include "gmerbsevaluator.h"


namespace GMlib {


  template <typename T>
  class Hermite4Evaluator
  {
  private:
    class PreEval
    {
    public:
        DVector<Matrix<T,4,4> > A;
        bool                    isMade;

        PreEval()           {isMade=false;}
        int         m()     {return A.getDim();}
    };

  public:
    Hermite4Evaluator( int m = 0);
    Hermite4Evaluator( const Hermite4Evaluator<T>& copy );
    virtual ~Hermite4Evaluator();


    Matrix<T,4,4>             eval(T t);

    void                      preEval(int m);

    Matrix<T,4,4>&            getPreEval(int i);
    bool                      isMade(int m);

  protected:
    void                      evaluate( T t);

    Matrix<T,4,4>             _h;
    PreEval                   _pre;
  }; // END class Hermite4Evaluator

} // END namepace GMlib

// Include Hermite4Evaluator class function implementations
#include "gmhermite4evaluator.c"


#endif // GM_PARAMETRICS_EVALUATORS_HERMITE4EVALUATOR_H



