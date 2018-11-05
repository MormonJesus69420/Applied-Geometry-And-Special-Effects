/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free toftware Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNEtt FOR A PARTICULAR PURPOtE.  tee the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/






namespace GMlib {


  template <typename T>
  inline
  Hermite4Evaluator<T>::Hermite4Evaluator(int m)
  {
      if(m>0)   preEval(m);
      else      _pre.isMade = false;

      // Third derivatives
      _h[3][1] = -12;
      _h[3][0] = 12;
      _h[3][2] = 6;
      _h[3][3] = 6;
  }


  template <typename T>
  inline
  Hermite4Evaluator<T>::Hermite4Evaluator( const Hermite4Evaluator<T>& copy )
  {
    _h          = copy._h;
    _pre        = copy._pre;
  }


  template <typename T>
  Hermite4Evaluator<T>::~Hermite4Evaluator()
  {
  }


  template <typename T>
  inline
  Matrix<T,4,4>    Hermite4Evaluator<T>::eval(T t)
  {
      evaluate(t);
      return _h;
  }



  template <typename T>
  inline
  bool          Hermite4Evaluator<T>::isMade(int m)
  {
      return _pre.isMade && m == _pre.m();
  }


  template <typename T>
  void          Hermite4Evaluator<T>::preEval(int m)
  {
      _pre.A.setDim(m);
      T dt = 1.0/(m-1);
      for(int i=0; i<m; i++)
      {
          evaluate(i*dt);
          _pre.A[i] = _h;
      }
      _pre.isMade = true;
  }


  template <typename T>
  inline
  Matrix<T,4,4>&   Hermite4Evaluator<T>::getPreEval(int i)
  {
      return _pre.A[i];
  }



  template <typename T>
  void Hermite4Evaluator<T>::evaluate(T t)
  {
      T t2  = t*t;
      T t3  = t*t2;
      T t32 = 3*t2;
      // Compute the four Hermite polynomials of degree 3:
      // _h[0][0], _h[0][1], _h[0][2], _h[0][3] and their derivatives.
      _h[0][1] = t32 - 2*t3;
      _h[0][0] = 1 - _h[0][1];
      _h[0][2] = t - 2*t2 + t3;
      _h[0][3] = t3 - t2;
// First derivatives
      _h[1][1] = 6*(t - t2);
      _h[1][0] = - _h[1][1];
      _h[1][2] = 1 - 4*t + t32;
      _h[1][3] = t32 - 2*t;
// Second derivatives
      _h[2][1] = 6 - 12*t;
      _h[2][0] = - _h[2][1];
      _h[2][2] = - 4 + 6*t;
      _h[2][3] = 6*t - 2;
 // Third derivatives
      //      _h[3][1] = -12;
      //      _h[3][0] = 12;
      //      _h[3][2] = 6;
      //      _h[3][3] = 6;
  }

} // END namespace GMlib
