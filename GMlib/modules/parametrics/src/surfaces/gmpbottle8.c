
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
  PBottle8<T>::PBottle8( T radius ) {
    init();
    _r = radius;
  }


  template <typename T>
  inline
  PBottle8<T>::PBottle8( const PBottle8<T>& copy ) : PSurf<T,3>( copy ) {
    init();
    _r = copy._r;
  }


  template <typename T>
  PBottle8<T>::~PBottle8() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PBottle8<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PBottle8<T>::isClosedV() const {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PBottle8<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

      T cu  = cos(u);
      T su  = sin(u);
      T cv  = cos(v);
      T sv  = sin(v);
      T c2v = cos(T(2)*v);
      T s2v = sin(T(2)*v);
      T c5u = cos(T(.5)*u);
      T s5u = sin(T(.5)*u);
      T c5  = c5u*s2v;
      T s5  = s5u*s2v;

    this->_p[0][0][0] =	(_r+c5u*sv-s5)*cu;
    this->_p[0][0][1] =	(_r+c5u*sv-s5)*su;
    this->_p[0][0][2] =	s5u*sv+c5;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                  //u
        this->_p[1][0][0] =	-T(.5)*cu*s5u*sv - T(.5)*cu*c5 - su*_r - su*c5u*sv + su*s5;
        this->_p[1][0][1] =	-T(.5)*su*s5u*sv - T(.5)*su*c5 + cu*_r + cu*c5u*sv - cu*s5;
        this->_p[1][0][2] =	-T(.5)*(-c5u*sv+s5);
      }
      if(d1>1) {                //uu
        this->_p[2][0][0] =	-T(.25)*cu*c5u*sv + T(.25)*cu*s5 + su*s5u*sv + su*c5 - cu*_r - cu*c5u*sv + cu*s5;
        this->_p[2][0][1] =	-T(.25)*su*c5u*sv + T(.25)*su*s5 - cu*s5u*sv - cu*c5 - su*_r - su*c5u*sv + su*s5;
        this->_p[2][0][2] =	-T(.25)*(s5u*sv+c5);
      }
      if(d2) {                  //v
        this->_p[0][1][0] =	(c5u*cv - s5u*c2v*T(2))*cu;
        this->_p[0][1][1] =	(c5u*cv - s5u*c2v*T(2))*su;
        this->_p[0][1][2] =	s5u*cv + T(2)*c5u*c2v;
      }
      if(d2>1) {                //vv
        this->_p[0][2][0] =	(-c5u*sv + T(4))*s5*cu;
        this->_p[0][2][1] =	(-c5u*sv + T(4))*s5*su;
        this->_p[0][2][2] =	-s5u*sv - T(4)*c5;
      }
      if(d1 && d2) {            //uv
        this->_p[1][1][0] = -T(.5)*cu*s5u*cv - cu*c5u*c2v - su*c5u*cv + T(2)*su*s5u*c2v;
        this->_p[1][1][1] =	-T(.5)*su*s5u*cv - su*c5u*c2v + cu*c5u*cv - T(2)*cu*s5u*c2v;
        this->_p[1][1][2] =	-T(.5)*(-c5u*cv + T(2)*s5u*c2v);
      }
      if(d1>1 && d2) {          //uuv
        this->_p[2][1][0] =	-T(.25)*cu*c5u*cv + T(.5)*cu*s5u*c2v + su*s5u*cv + T(2)*su*c5u*c2v - cu*c5u*cv + T(2)*cu*s5u*c2v;
        this->_p[2][1][1] =	-T(.25)*su*c5u*cv + T(.5)*su*s5u*c2v - cu*s5u*cv - T(2)*cu*c5u*c2v - su*c5u*cv + T(2)*su*s5u*c2v;
        this->_p[2][1][2] =	-T(.25)*(s5u*cv + T(2)*c5u*c2v);
      }
      if(d1 && d2>1) {          //uvv
        this->_p[1][2][0] =	cu*s5u*T(.5)*sv + cu*c5*T(2) + su*c5u*sv - su*s5*T(4);
        this->_p[1][2][1] =	su*s5u*T(.5)*sv + su*c5*T(2) - cu*c5u*sv + cu*s5*T(4);
        this->_p[1][2][2] =	T(.5)*(-c5u*sv + T(4)*s5);
      }
      if(d1>1 && d2>1) {        //uuvv
        this->_p[2][2][0] =	T(.25)*cu*c5u*sv - cu*s5 - su*s5u*sv - T(4)*su*c5 + cu*c5u*sv - T(4)*cu*s5;
        this->_p[2][2][1] =	T(.25)*su*c5u*sv - su*s5 + cu*s5u*sv + T(4)*cu*c5 + su*c5u*sv - T(4)*su*s5;
        this->_p[2][2][2] =	T(.25)*(s5u*sv + T(4)*c5);
      }
    }
  }


  template <typename T>
  T PBottle8<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PBottle8<T>::getEndPU() const {
    return T(M_2PI);
  }


  template <typename T>
  T PBottle8<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PBottle8<T>::getEndPV() const {
    return T(M_2PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PBottle8<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
