
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
  PBohemianDome<T>::PBohemianDome( T radius, T domewidth, T height ) {
    init();
    _r = radius;
    _w = domewidth;
    _h = height;
  }


  template <typename T>
  inline
  PBohemianDome<T>::PBohemianDome( const PBohemianDome<T>& copy ) : PSurf<T,3>( copy ) {
    init();
    _r = copy._r;
    _w = copy._w;
    _h = copy._h;
  }


  template <typename T>
  PBohemianDome<T>::~PBohemianDome() {}


  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PBohemianDome<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PBohemianDome<T>::isClosedV() const {
    return false;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PBohemianDome<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T cu  = cos(u);
    T cv  = cos(v);
    T su  = sin(u);
    T sv  = sin(v);

    this->_p[0][0][0] =	_r*cu;
    this->_p[0][0][1] =	_r*su+_w*cv;
    this->_p[0][0][2] =	_h*sv;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {              //u
        this->_p[1][0][0] =	-_r*su;
        this->_p[1][0][1] =	_r*cu;
        this->_p[1][0][2] =	T(0);
      }
      if(d1>1) {            //uu
        this->_p[2][0][0] =	-_r*cu;
        this->_p[2][0][1] =	-_r*su;
        this->_p[2][0][2] =	T(0);
      }
      if(d2) {              //v
        this->_p[0][1][0] =	T(0);
        this->_p[0][1][1] =	-_w*sv;
        this->_p[0][1][2] =	 _h*cv;
      }
      if(d2>1) {            //vv
        this->_p[0][2][0] =	T(0);
        this->_p[0][2][1] =	-_w*cv;
        this->_p[0][2][2] =	-_h*sv;
      }
      if(d1 && d2) {        //uv
        this->_p[1][1][0] =	T(0);
        this->_p[1][1][1] =	T(0);
        this->_p[1][1][2] =	T(0);
      }
      if(d1>1 && d2) {      //uuv
        this->_p[2][1][0] =	T(0);
        this->_p[2][1][1] =	T(0);
        this->_p[2][1][2] =	T(0);
      }
      if(d1 && d2>1) {      //uvv
        this->_p[1][2][0] =	T(0);
        this->_p[1][2][1] =	T(0);
        this->_p[1][2][2] =	T(0);
      }
      if(d1>1 && d2>1) {    //uuvv
        this->_p[2][2][0] =	T(0);
        this->_p[2][2][1] =	T(0);
        this->_p[2][2][2] =	T(0);
      }
    }
  }


  template <typename T>
  T PBohemianDome<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PBohemianDome<T>::getEndPU() const {
    return T(M_2PI);
  }


  template <typename T>
  T PBohemianDome<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PBohemianDome<T>::getEndPV() const {
    return T(M_2PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PBohemianDome<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
