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

  template <typename T>
  inline
  PCircularSurface<T>::PCircularSurface( T radius ) {
    init();
    _radius = radius;
  }


  template <typename T>
  inline
  PCircularSurface<T>::PCircularSurface( const PCircularSurface<T>& copy ) : PSurf<T,3>( copy ) {
    init();
  }


  template <typename T>
  PCircularSurface<T>::~PCircularSurface() {}



  //*****************************************
  //            Local functons             **
  //*****************************************

  template <typename T>
  inline
  T PCircularSurface<T>::getRadius() const {
    return _radius;
  }


  template <typename T>
  inline
  void PCircularSurface<T>::setRadius( T radius ) {

    _radius = radius;
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PCircularSurface<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PCircularSurface<T>::isClosedV() const {
    return true;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PCircularSurface<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    this->_p[0][0][0] = u * cos( v );
    this->_p[0][0][1] = u * sin( v );
    this->_p[0][0][2] = T(0);


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {
        this->_p[1][0][0] = cos(v);         // S_u
        this->_p[1][0][1] = sin(v);
        this->_p[1][0][2] =  T(0);
      }
      if(d2) {
        this->_p[0][1][0] = u * -sin(v);	// S_v
        this->_p[0][1][1] = u *  cos(v);
        this->_p[0][1][2] = T(0);
      }
      if(d1 && d2) {
        this->_p[1][1][0] = -sin(v);        // S_uv
        this->_p[1][1][1] =  cos(v);
        this->_p[1][1][2] =  T(0);
      }
      if(d1>1) {
        this->_p[2][0][0] =  T(0);          // S_uu
        this->_p[2][0][1] =  T(0);
        this->_p[2][0][2] =  T(0);
      }
      if(d2>1) {
        this->_p[0][2][0] = u * -cos(v);	// S_vv
        this->_p[0][2][1] = u * -sin(v);
        this->_p[0][2][2] =  T(0);
      }
      if(d1>1 && d2) {
        this->_p[2][1][0] =  T(0);          // S_uuv
        this->_p[2][1][1] =  T(0);
        this->_p[2][1][2] =  T(0);
      }
      if(d1 && d2>1) {
        this->_p[1][2][0] = -cos(v);        // S_uvv
        this->_p[1][2][1] = -sin(v);
        this->_p[1][2][2] =  T(0);
      }
      if(d1>1 && d2>1) {
        this->_p[2][2][0] =  T(0);          // S_uuvv
        this->_p[2][2][1] =  T(0);
        this->_p[2][2][2] =  T(0);

      }
    }
  }


  template <typename T>
  T PCircularSurface<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PCircularSurface<T>::getEndPU() const {
    return _radius;
  }


  template <typename T>
  T PCircularSurface<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PCircularSurface<T>::getEndPV() const {
    return T(M_2PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PCircularSurface<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }

} // END namespace GMlib
