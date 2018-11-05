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





namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************

/*! PLogSpiral<T>::PLogSpiral( T a, T b, T c, T s, T e )
 *  Default constructor
 *  Making an ordinary logaritmic spiral curve
   *
   *  \param[in]  a   (default = 1)   The radius of the start point
   *  \param[in]  b   (default = 0.1) How fast the radius increases
   *  \param[in]  c   (default = 0)   How steep the spiral rises
   *  \param[in]  s   (default = 0)   The start parameter value is "s*Pi"
   *  \param[in]  e   (default = 12 ) The end parameter value is "e*Pi"
 */
  template <typename T>
  inline
  PLogSpiral<T>::PLogSpiral( T a, T b, T c, T s, T e ):PCurve<T,3>(20,0,3) {
    _a = a;
    _b = b;
    _c = c;

    _s = s;
    _e = e;
  }



  /*! PLogSpiral<T>::PLogSpiral(const PLogSpiral<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve (spiral)
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  inline
  PLogSpiral<T>::PLogSpiral( const PLogSpiral<T>& copy ) : PCurve<T,3>(copy) {
      _a = copy._a;
      _b = copy._b;
      _c = copy._c;

      _s = copy._s;
      _e = copy._e;
  }



  /*! PLogSpiral<T>::~PLogSpiral()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T>
  PLogSpiral<T>::~PLogSpiral() {}


  //**************************************
  //        Public local functons       **
  //**************************************


  /*! T PLogSpiral<T>::getA() const
   *  Give you the radius of the start point of the spiral
   *
   *  \return  The radius of the start point
   */
  template <typename T>
  inline
  T PLogSpiral<T>::getA() const {
    return _a;
  }



  /*! T PLogSpiral<T>::getB() const
   *  Give you how fast the radius increases
   *
   *  \return  How fast the radius increases
   */
  template <typename T>
  inline
  T PLogSpiral<T>::getB() const {
    return _b;
  }



  /*! T PLogSpiral<T>::getC() const
   *  Give you how steep the spiral rises
   *
   *  \return  How steep the spiral rises
   */
  template <typename T>
  inline
  T PLogSpiral<T>::getC() const {
    return _c;
  }



  /*! void PLogSpiral<T>::setA( T a )
   *  Will change the radius of the start point
   *
   *  \param[in] a   (default = 1) The new radius of the start point
   */
  template <typename T>
  inline
  void PLogSpiral<T>::setA( T a ) {
      _a = a;
  }



  /*! void PLogSpiral<T>::setB( T b )
   *  Will change how fast the radius increases
   *
   *  \param[in] b   (default = 0.1) How fast the radius increases
   */
  template <typename T>
  inline
  void PLogSpiral<T>::setB( T b ) {
      _b = b;
  }



  /*! void PLogSpiral<T>::setC( T c )
   *  Will change how steep the spiral rises
   *
   *  \param[in] c   (default = 0) How steep the spiral rises
   */
  template <typename T>
  inline
  void PLogSpiral<T>::setC( T c ) {
      _c = c;
  }



  /*! void PLogSpiral<T>::setC( T c )
   *  Will change the radius of the start point
   *
   *  \param[in]  s   (default = 0)   The start parameter value is "s*Pi"
   *  \param[in]  e   (default = 12 ) The end parameter value is "e*Pi"
   */
  template <typename T>
  inline
  void PLogSpiral<T>::setSE( T s, T e ) {
      _s = s;
      _e = e;
  }


  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  /*! void PLogSpiral<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  3 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (dummy) because left and right are always equal
   */
  template <typename T>
  void PLogSpiral<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );

    const T ct = cos(t);
    const T st = sin(t);
    const T ex = _a*exp(_b*t);

    this->_p[0][0] = ex*ct;
    this->_p[0][1] = ex*st;
    this->_p[0][2] = _c*t;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {
      if( d > 0 ) {
        this->_p[1][0] = ex*(_b*ct-st);
        this->_p[1][1] = ex*(_b*st+ct);
        this->_p[1][2] =  _c;
      }
      if( d > 1 ) {
          this->_p[2][0] = ex*((_b*_b-1)*ct - 2*_b*st);
          this->_p[2][1] = ex*((_b*_b-1)*st + 2*_b*ct);
          this->_p[2][2] = T(0);
      }
      if( d > 2 ) {
          this->_p[3][0] = ex*((1-3*_b*_b)*st + (_b*_b*_b-3*_b)*ct);
          this->_p[3][1] = ex*((3*_b*_b-1)*ct + (_b*_b*_b-3*_b)*st);
          this->_p[3][2] = T(0);
      }
    }
  }



  /*! T PLogSpiral<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PLogSpiral<T>::getStartP() const {
    return T(_s * M_PI);
  }



  /*! T PLogSpiral<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PLogSpiral<T>::getEndP()const {
    return T(_e * M_PI );
  }


} // END namespace GMlib
