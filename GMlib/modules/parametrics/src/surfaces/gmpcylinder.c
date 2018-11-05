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
  PCylinder<T>::PCylinder( T rx, T ry, T h ) {
    init();
    setConstants( rx, ry, h );
  }


  template <typename T>
  inline
  PCylinder<T>::PCylinder( const PCylinder<T>& copy ) : PSurf<T,3>(copy) {
    init();
    _rx = copy._rx;
    _ry = copy._ry;
    _h  = copy._h;
  }



  template <typename T>
  PCylinder<T>::~PCylinder() {}



  //*****************************************
  //            Local functons             **
  //*****************************************

  template <typename T>
  inline
  T PCylinder<T>::getHeight() const {
    return _h;
  }


  template <typename T>
  inline
  T PCylinder<T>::getRadiusX() const {
    return _rx;
  }


  template <typename T>
  inline
  T PCylinder<T>::getRadiusY() const {
    return _ry;
  }


  template <typename T>
  inline
  void PCylinder<T>::setConstants( T rx, T ry, T h ) {
    _rx = rx;
    _ry = ry;
    _h = h;
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PCylinder<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PCylinder<T>::isClosedV() const {
    return true;
  }




  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PCylinder<T>::eval( T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    this->_p[0][0][0] =		_rx * sin( v );
    this->_p[0][0][1] =		_ry * cos( v );
    this->_p[0][0][2] =		_h * u;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                  //u
        this->_p[1][0][0] =	T(0);
        this->_p[1][0][1] =	T(0);
        this->_p[1][0][2] =	_h;
      }
      if(d1>1) {                //uu
        this->_p[2][0][0] =	T(0);
        this->_p[2][0][1] =	T(0);
        this->_p[2][0][2] =	T(0);
      }
      if(d2) {                  //v
        this->_p[0][1][0] = _ry * cos(v);
        this->_p[0][1][1] = -_rx *sin(v);
        this->_p[0][1][2] =	T(0);
      }
      if(d2>1) {                //vv
        this->_p[0][2][0] = -_ry * sin(v);
        this->_p[0][2][1] =	-_rx * cos(v);
        this->_p[0][2][2] =	T(0);
      }
      if(d1 && d2) {            //uv
        this->_p[1][1][0] =	T(0);
        this->_p[1][1][1] =	T(0);
        this->_p[1][1][2] =	T(0);
      }
      if(d1>1 && d2) {          //uuv
        this->_p[2][1][0] = T(0);
        this->_p[2][1][1] = T(0);
        this->_p[2][1][2] = T(0);
      }
      if(d1 && d2>1) {          //uvv
        this->_p[1][2][0] =	T(0);
        this->_p[1][2][1] =	T(0);
        this->_p[1][2][2] =	T(0);
      }
      if(d1>1 && d2>1) {        //uuvv
        this->_p[2][2][0] =	T(0);
        this->_p[2][2][1] =	T(0);
        this->_p[2][2][2] =	T(0);
      }
    }
  }


  template <typename T>
  T PCylinder<T>::getStartPU() const {
    return T( -0.5 );
  }


  template <typename T>
  T PCylinder<T>::getEndPU() const {
    return T( 0.5 );
  }


  template <typename T>
  T PCylinder<T>::getStartPV() const {
    return T( -1.5 * M_PI );
  }


  template <typename T>
  T PCylinder<T>::getEndPV() const {
    return T( 0.5 * M_PI );
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PCylinder<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }



} // END namespace GMlib

