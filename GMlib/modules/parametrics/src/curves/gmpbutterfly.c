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


/*! PButterfly<T>::PButterfly( T size )
 *  Default constructor, to make a butterfly curve with size = size.
 *
 *  \param[in] size      The size of the butterfly curve
 */
  template <typename T>
  inline
  PButterfly<T>::PButterfly( T size ) : PCurve<T,3>(0, 0, 2), _size(size), _flaps(0.5)
  {
      _sim_boundary = 0.7;
      _sim_speed    = 1;
  }




  /*! PButterfly<T>::PButterfly(const PButterfly<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve (butterfly)
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  inline
  PButterfly<T>::PButterfly( const PButterfly<T>& copy ) : PCurve<T,3>(copy) {}




  /*! PButterfly<T>::~PButterfly()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T>
  PButterfly<T>::~PButterfly() {}




  //**************************************
  //        Public local functons       **
  //**************************************


  /*! void PButterfly<T>::setSize( T size )
   *  Will change the size of the butterfly curve
   *
   *  \param[in] size  The new size of the butterfly curve
   */
  template <typename T>
  inline
  void PButterfly<T>::setSize( T size ) {
      _size = size;
  }




  /*! T PButterfly<T>::getSize() const
   *  Give you the size of the butterfly curbe
   *
   *  \return  The size of the butterfly curve
   */
  template <typename T>
  inline
  T PButterfly<T>::getSize() const {
      return _size;
  }




  /*! void PButterfly<T>::setFlaps( T flaps )
   *  Will change the flaps (the hight of the wings) of the butterfly curve
   *
   *  \param[in] flaps  The new flaps (the hight of the wings) of the butterfly curve
   */
  template <typename T>
  inline
  void PButterfly<T>::setFlaps( T flaps ) {
      _flaps = flaps;
  }



  /*! T PButterfly<T>::getFlaps() const
   *  Give you the flaps (the hight of the wings) of the butterfly curve
   *
   *  \return  The flaps (the hight of the wings) of the butterfly curve
   */
  template <typename T>
  inline
  T PButterfly<T>::getFlaps() const {
      return _flaps;
  }



  /*! void PButterfly<T>::setSimSpeed( T sim_speed )
   *  Will change the speed of the simulation of the butterfly curve
   *
   *  \param[in] sim_speed  The new speed of the simulatione
   */
  template <typename T>
  inline
  void PButterfly<T>::setSimSpeed( T sim_speed ) {
      _sim_speed = sim_speed;
  }



  /*! T PButterfly<T>::getSimSpeed() const
   *  Give you the the speed of the simulation of the butterfly curve
   *
   *  \return  The speed of the simulation
   */
  template <typename T>
  inline
  T PButterfly<T>:: getSimSpeed() const {
      return _sim_speed;
  }



  /*! void PButterfly<T>::setSimBoundary( T sim_boundary )
   *  Will change the boundary of the simulation of the butterfly curve
   *
   *  \param[in] sim_boundary  The new boundary of the simulatione
   */
  template <typename T>
  inline
  void PButterfly<T>::setSimBoundary( T sim_boundary ) {
      _sim_boundary = sim_boundary;
  }



  /*! T PButterfly<T>::getSimBoundary() const
   *  Give you the the boundary of the simulation of the butterfly curve
   *
   *  \return  The boundary of the simulatione
   */
  template <typename T>
  inline
  T PButterfly<T>::getSimBoundary() const {
      return _sim_boundary;
  }


  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************




  /*! bool PButterfly<T>::isClosed() const
   *  To tell that this curve (butterfly) is closed.
   */
  template <typename T>
  bool PButterfly<T>::isClosed() const {
    return true;
  }



  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  /*! void PButterfly<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  2 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (dummy) because left and right are always equal
   */
  template <typename T>
  void PButterfly<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );

    const double ct   = cos(t);
    const double st   = sin(t);
    const double st12 = sin(t/12);
    const double a    = exp(ct) - 2*cos(4*t) - pow(st12, 5.0);

    this->_p[0][0] = _size * T(ct * a);
    this->_p[0][1] = _size * T(st * a);
    this->_p[0][2] =  fabs(this->_p[0][1])*_flaps;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {
        if( d > 0 ) { // The first derivative
            const double ct12 = cos(t/12);
            const double a1 = -exp(ct)*st + 8*sin(4*t) - (5.0/12)*pow(st12,4.0)*ct12;
            this->_p[1][0]  = _size * T( -st*a + ct*a1 );
            this->_p[1][1]  = _size * T(  ct*a + st*a1 );
            if(this->_p[0][1] > 0)
                this->_p[1][2]  = this->_p[1][1]*_flaps;
            else
                this->_p[1][2]  = -this->_p[1][1]*_flaps;
            if( d > 1 ) { // The second derivative
                const double a2 = exp(ct)*st*st - exp(ct) * ct + 8.0*4.0*cos(4*t)
                                  - (5.0/12)*((1.0/3)* pow(st12,3.0 )*pow(ct12,2.0)
                                  - (1.0/12)* pow(st12,5.0));
                this->_p[2][0]  = _size * T( -ct*a - st*a1 - st*a1 + ct*a2 );
                this->_p[2][1]  = _size * T( -st*a + ct*a1 + ct*a1 + st*a2 );
                if(this->_p[0][1] > 0)
                    this->_p[2][2]  = this->_p[2][1]*_flaps;
                else
                    this->_p[2][2]  = -this->_p[2][1]*_flaps;
            }
        }
    }
  }



  /*! T PButterfly<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *  (the start parameter value = 0).
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PButterfly<T>::getStartP() const {
    return T(0);
  }



  /*! T PButterfly<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *  (the end parameter value = 24*Pi).
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PButterfly<T>::getEndP() const {
    return T( M_PI * 24.0 );
  }



  //***********************************************************
  // Overrided (protected) virtual functons from SceneObject **
  //***********************************************************

  /*! T PButterfly<T>::localSimulate( double dt )
   *  Virtual from SceneObject, implemented because we want a simulation
   *  This function causes the butterfly to move the wings up and down
   *  (the end parameter value = 24*Pi).
   *
   *  \param[in] dt Time in seconds since last call to this function
   */
  template <typename T>
  void PButterfly<T>::localSimulate( double dt ) {
    static bool up = false;

    if(up)  _flaps += _sim_speed*dt;
    else    _flaps -= _sim_speed*dt;

    if(_flaps <= -_sim_boundary || _flaps >= _sim_boundary)   up = !up;

    this->resample();
    this->setEditDone();
  }

} // END namespace GMlib
