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
  PMoebiusStrip<T>::PMoebiusStrip( T radius, T width ) {

    init();

    _r = radius;
    _w = width;
  }


  template <typename T>
  inline
  PMoebiusStrip<T>::PMoebiusStrip( const PMoebiusStrip<T>& copy ) : PSurf<T,3>( copy ) {

    init();

    _r = copy._r;
    _w = copy._w;
  }



  template <typename T>
  PMoebiusStrip<T>::~PMoebiusStrip() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PMoebiusStrip<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PMoebiusStrip<T>::isClosedV() const {
    return false;
  }




  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PMoebiusStrip<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T sv = sin(v);
    T cv = cos(v);
    T s5v = sin(T(.5)*v);
    T c5v = cos(T(.5)*v);
    T wu  = _w*u;

    this->_p[0][0][0] =	(_r + wu*c5v)*cv;
    this->_p[0][0][1] =	(_r + wu*c5v)*sv;
    this->_p[0][0][2] =	wu*s5v;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                                      //u
        this->_p[1][0][0] =	_w*c5v*cv;
        this->_p[1][0][1] =	_w*c5v*sv;
        this->_p[1][0][2] =	_w*s5v;
      }
      if(d1>1) {                                     //uu
        this->_p[2][0][0] =	T(0);
        this->_p[2][0][1] =	T(0);
        this->_p[2][0][2] =	T(0);
      }
      if(d2) {                                       //v
        this->_p[0][1][0] =	-T(.5)*wu*s5v*cv - sv*_r - sv*wu*c5v;
        this->_p[0][1][1] =	-T(.5)*wu*s5v*sv + cv*_r + cv*wu*c5v;
        this->_p[0][1][2] =	 T(.5)*wu*c5v;
      }
      if(d2>1) {                                    //vv
        this->_p[0][2][0] =	-T(.25)*wu*c5v*cv + wu*s5v*sv - cv*_r - cv*wu*c5v;
        this->_p[0][2][1] =	-T(.25)*wu*c5v*sv - wu*s5v*cv - sv*_r - sv*wu*c5v;
        this->_p[0][2][2] =	-T(.25)*wu*s5v;
      }
      if(d1 && d2) {                                //uv
        this->_p[1][1][0] =	-_w*(s5v*T(.5)*cv + c5v*sv);
        this->_p[1][1][1] =	-_w*(s5v*T(.5)*sv - c5v*cv);
        this->_p[1][1][2] =	 _w*c5v*T(.5);
      }
      if(d1>1 && d2) {                              //uuv
        this->_p[2][1][0] =	T(0);
        this->_p[2][1][1] =	T(0);
        this->_p[2][1][2] =	T(0);
      }
      if(d1 && d2>1) {                              //uvv
        this->_p[1][2][0] =	-_w*(c5v*T(.25)*cv - s5v*sv + c5v*cv);
        this->_p[1][2][1] =	-_w*(c5v*T(.25)*sv + s5v*cv + c5v*sv) ;
        this->_p[1][2][2] =	-_w*s5v*T(.25);
      }
      if(d1>1 && d2>1) {                            //uuvv
        this->_p[2][2][0] =	T(0);
        this->_p[2][2][1] =	T(0);
        this->_p[2][2][2] =	T(0);
      }
    }
  }



  template <typename T>
  T PMoebiusStrip<T>::getStartPU() const {
    return -T(1);
  }


  template <typename T>
  T PMoebiusStrip<T>::getEndPU() const {
    return T(1);
  }


  template <typename T>
  T PMoebiusStrip<T>::getStartPV() const {
    return -T(0.5*M_PI);
  }


  template <typename T>
  T PMoebiusStrip<T>::getEndPV() const {
    return T(1.5*M_PI);
  }


  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PMoebiusStrip<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
