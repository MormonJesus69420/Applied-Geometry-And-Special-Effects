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

/*! PLine<T>::PLine( const Point<T,3>& p, const Vector<T,3>& v  )
 *  Default constructor, to create a line starting in p and ending in p+v.
 *
 *  \param[in] p      The start point
 *  \param[in] v      A vector describing the line
 */
  template <typename T>
  inline
  PLine<T>::PLine( const Point<T,3>& p, const Vector<T,3>& v ) : PCurve<T,3>(20, 0, 7), _pt(p), _v(v) {}



  /*! PLine<T>::PLine( const Point<T,3>& p, const Vector<T,3>& v  )
   *  Default constructor, to create a line starting in p1 and ending in p2.
   *
   *  \param[in] p1      The start point
   *  \param[in] p2      The end point
   */
  template <typename T>
  inline
  PLine<T>::PLine( const Point<T,3>& p1, const Point<T,3>& p2 ) : PCurve<T,3>(20, 0, 7), _pt(p1), _v(p2-p1) {}



  /*! PLine<T>::PLine(const PLine<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve (line)
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  inline
  PLine<T>::PLine( const PLine<T>& copy ) : PCurve<T,3>(copy) {}



  /*! PLine<T>::~PLine()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T>
  PLine<T>::~PLine() {}




  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************

  /*! bool PLine<T>::isClosed() const
   *  To tell that this curve (line) is always open.
   *
   */
  template <typename T>
  bool PLine<T>::isClosed() const {
    return false;
  }



  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  /*! void PCircle<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  7 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (dummy) because left and right are always equal
   */
  template <typename T>
  void PLine<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );

    this->_p[0] = _pt + t * _v;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {
      if( d )     this->_p[1] = _v;
      if( d > 1 ) this->_p[2] = Vector<T,3>(T(0));
      if( d > 2 ) this->_p[3] = Vector<T,3>(T(0));
      if( d > 3 ) this->_p[4] = Vector<T,3>(T(0));
      if( d > 4 ) this->_p[5] = Vector<T,3>(T(0));
      if( d > 5 ) this->_p[6] = Vector<T,3>(T(0));
      if( d > 6 ) this->_p[7] = Vector<T,3>(T(0));
    }
  }



  /*! T PLine<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *  (the start parameter value = 0).
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PLine<T>::getStartP() const {
    return T(0);
  }



  /*! T PLine<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *  (the end parameter value = 1).
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PLine<T>::getEndP() const {
    return T(1);
  }


} // END namespace GMlib
