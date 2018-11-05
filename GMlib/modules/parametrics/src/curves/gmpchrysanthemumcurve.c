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



/*! PChrysanthemumCurve<T>::PChrysanthemumCurve( T radius, T scale, T trans )
 *  Default constructor,
 *  To make a Chrysanthemum curve determined by radius and curvature
 *  The curve start in origin and follows the x-axis
 *  It lies in the xy-plane
 *  \param[in] radius      The size of the curve
 *  \param[in] scale       shape of radial z-value
 *  \param[in] trans       translation of radial curvature
 */
  template <typename T>
  PChrysanthemumCurve<T>::PChrysanthemumCurve( T radius, T scale, T trans ) : PCurve<T,3>(0, 0, 0) {

    this->_dm = GM_DERIVATION_DD;
    _r     = radius;
    _scale = scale;
    _trans = trans;
  }



  /*! PChrysanthemumCurve<T>::PChrysanthemumCurve(const PChrysanthemumCurve<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  PChrysanthemumCurve<T>::PChrysanthemumCurve( const PChrysanthemumCurve<T>& copy ) : PCurve<T,3>( copy ) {
  }



  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************

  /*! bool PChrysanthemumCurve<T>::isClosed() const
   *  To tell that this curve is always closed.
   */
  template <typename T>
  bool PChrysanthemumCurve<T>::isClosed() const {
    return true;
  }



  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  /*! void PChrysanthemumCurve<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  0 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (dummy) because left and right are always equal
   */
  template <typename T>
  void PChrysanthemumCurve<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d+1 );

    const double p4 = sin(17*t/3);
    const double p8 = sin(2*cos(3*t) - 28*t);
    const double r  = 5*(1+sin(11*t/5)) - 4*pow(p4,4) * pow(p8,8);

    this->_p[0][0] = _r * T(r * cos(t));
    this->_p[0][1] = _r * T(r * sin(t));
    this->_p[0][2] = _r * T((r/20 + _trans) * sin(_scale * r));
  }



  /*! T PChrysanthemumCurve<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PChrysanthemumCurve<T>::getStartP() const {
    return T(0);
  }



  /*! T PChrysanthemumCurve<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PChrysanthemumCurve<T>::getEndP() const {
    return T( 21 * M_PI );
  }


} // END namespace GMlib

