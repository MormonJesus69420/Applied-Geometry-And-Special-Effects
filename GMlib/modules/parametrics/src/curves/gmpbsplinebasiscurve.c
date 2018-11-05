/**********************************************************************************
**
** Copyright (C) 1994 - 2016 University of Tromsø - The Arctic University of Norway
** Contact: GMlib Online Portal at https://source.uit.no/gmlib/gmlib/wikis/home
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



#include "../evaluators/gmevaluatorstatic.h"


namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************


/*! PBSplineBasisCurve<T>::PBSplineBasisCurve( const DVector<T>& t )
 *  Default constructor, to make a B-spline basis function
 *  The polynomial degree is desided by the knot vector d = t.size()-2
 *
 *  \param[in] t   The knot vector with size k+1 (order+1) or (degree+2)
 */
  template <typename T>
  inline
  PBSplineBasisCurve<T>::PBSplineBasisCurve( const DVector<T>& t ) : PCurve<T,3>(20,0,4) {

    // k is the order and we expand the knot vector with (k-1) knots at start and (k-1) knots at the end
    // which are only dummy, but we need them to be able to use standard B-splines tools.
    int k = t.getDim()-1;
    _t.setDim(3*k-1);

    for(int i=0; i<k-1; i++)
      _t[i] = t(0)- T(k-i-1);
    for(int i=k-1; i < 2*k; i++)
      _t[i] = t(i-k+1);
    for(int i=2*k; i<_t.getDim(); i++)
      _t[i] = t(k)+T(i-2*k+1);
  }



  /*! PBSplineBasisCurve<T>::PBSplineBasisCurve(const PBSplineBasisCurve<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve (b-spline)
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  inline
  PBSplineBasisCurve<T>::PBSplineBasisCurve( const PBSplineBasisCurve<T>& copy ) : PCurve<T,3>( copy ) {
      _t = copy._t;
  }



  /*! PBSplineBasisCurve<T>::~PBSplineBasisCurve()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T>
  PBSplineBasisCurve<T>::~PBSplineBasisCurve() {}



  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************

  /*! bool PBSplineBasisCurve<T>::isClosed() const
   *  To tell that this curve (b-spline) is not closed.
   */
  template <typename T>
  bool PBSplineBasisCurve<T>::isClosed() const {
    return false;
  }



  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  /*! void PBSplineBasisCurve<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  4 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute (max is the polynomial degree)
   *  \param  l[in]  Evaluating from left or right, important if multiple knots
   */
  template <typename T>
  void PBSplineBasisCurve<T>::eval( T t, int d, bool l ) const {

    this->_p.setDim( d + 1 );

    DMatrix<T> B;
    int k = (_t.getDim()+1)/3;
    int i = 2*(k-1) - EvaluatorStatic<T>::evaluateBSp( B, t, _t, k-1, l);

    this->_p[0][0] = B[0][i];
    this->_p[0][1] = t;
    this->_p[0][2] = 0;

    if(d>0)
    {
      this->_p[1][0] = B[1][i];
      this->_p[1][1] = 1;
      this->_p[1][2] = 0;
      if(d>1)
      {
        this->_p[2][0] = B[2][i];
        this->_p[2][1] = 0;
        this->_p[2][2] = 0;
        if(d>2)
        {
          this->_p[3][0] = B[3][i];
          this->_p[3][1] = 0;
          this->_p[3][2] = 0;
          if(d>3)
          {
            this->_p[4][0] = B[4][i];
            this->_p[4][1] = 0;
            this->_p[4][2] = 0;
          }
        }
      }
    }
  }



  /*! T PBSplineBasisCurve<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PBSplineBasisCurve<T>::getStartP() const {
    int k = (_t.getDim()+1)/3;
    return _t(k-1);
  }



  /*! T PBSplineBasisCurve<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PBSplineBasisCurve<T>::getEndP() const {
    int k = (_t.getDim()+1)/3;
    return _t(_t.getDim()-k);
  }


} // END namespace GMlib

