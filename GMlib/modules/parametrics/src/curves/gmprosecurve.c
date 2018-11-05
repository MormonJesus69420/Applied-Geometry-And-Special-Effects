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




namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************

/*! PRoseCurve<T>::PRoseCurve( T radius )
 *  Default constructor, to make a rose curve with the radius = radius.
 *
 *  \param[in] radius    (default 5)   The radius of the rose curve
 *  \param[in] rize      (default 0.1) How the rose curve rize
 */
  template <typename T>
  inline
  PRoseCurve<T>::PRoseCurve( T radius, T rize ) : PCurve<T,3>(20, 0, 2) {
    _r    = radius;
    _rize = rize;
  }



  /*! PRoseCurve<T>::PRoseCurve(const PRoseCurve<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  inline
  PRoseCurve<T>::PRoseCurve( const PRoseCurve<T>& copy ) : PCurve<T,3>( copy ) {
      _r    = copy._r;
      _rize = copy._rize;
  }



  /*! PRoseCurve<T>::~PRoseCurve()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T>
  PRoseCurve<T>::~PRoseCurve() {}



  //**************************************
  //        Public local functons       **
  //**************************************

  /*! T PRoseCurve<T>::getRize() const
   *  Give you the rize of the rose
   *
   *  \return  The rize of the roze
   */
  template <typename T>
  inline
  T PRoseCurve<T>::getRize() const {
    return _rize;
  }


  /*! void PRoseCurve<T>::setRize( T rize )
   *  Will change the rize of the rose
   *
   *  \param[in] rize   (default 0.1) The new rize of the rose
   */
  template <typename T>
  inline
  void PRoseCurve<T>::setRize( T rize ) {
      _rize = rize;
  }



  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************


  /*! bool PRoseCurve<T>::isClosed() const
   *  To tell that this curve is closed.
   */
  template <typename T>
  bool PRoseCurve<T>::isClosed() const {
    return true;
  }




  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************


  /*! void PRoseCurve<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  2 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (dummy) because left and right are always equal
   */
  template <typename T>
  void PRoseCurve<T>::eval( T t, int d, bool /*l*/ ) const {

      this->_p.setDim(d + 1);

      this->_p[0][0] = _r * T(cos(t)*cos(1.75*t));
      this->_p[0][1] = _r * T(sin(t)*cos(1.75*t));
      this->_p[0][2] = _rize*(this->_p[0][0]*this->_p[0][0] + this->_p[0][1]*this->_p[0][1]);

      if( this->_dm == GM_DERIVATION_EXPLICIT ) {
          if(d > 0) {
              this->_p[1][0] = _r * T(-1.75*cos(t) * sin(1.75*t) - sin(t)*cos(1.75*t));
              this->_p[1][1] = _r * T(-1.75*sin(t) * sin(1.75*t) + cos(t)*cos(1.75*t));
              this->_p[1][2] = 2*_rize*(this->_p[0][0]*this->_p[1][0] + this->_p[0][1]*this->_p[1][1]);
              if(d > 1) {
                  this->_p[2][0]= _r * T( 3.5*sin(t)*sin(1.75*t) - 4.0625*cos(t)*cos(1.75*t));
                  this->_p[2][1]= _r * T(-3.5*cos(t)*sin(1.75*t) - 4.0625*sin(t)*cos(1.75*t));
                  this->_p[2][2]= 2*_rize*(this->_p[1][0]*this->_p[1][0] + this->_p[0][0]*this->_p[2][0]+
                                           this->_p[1][1]*this->_p[1][1] + this->_p[0][1]*this->_p[2][1]);
              }
          }
      }
  }



  /*! T PRoseCurve<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *  (the start parameter value = 0).
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PRoseCurve<T>::getStartP() const {
    return T(0);
  }



  /*! T PRoseCurve<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *  (the end parameter value = 8*Pi).
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PRoseCurve<T>::getEndP() const {
    return T( 8 * M_PI );
  }


}

