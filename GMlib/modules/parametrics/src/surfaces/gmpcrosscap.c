
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
  PCrossCap<T>::PCrossCap( T radius ) {

    init();
    _r = radius;
  }


  template <typename T>
  inline
  PCrossCap<T>::PCrossCap( const PCrossCap<T>& copy ) : PSurf<T,3>( copy ) {

    init();
    _r = copy._r;
  }


  template <typename T>
  PCrossCap<T>::~PCrossCap() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PCrossCap<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PCrossCap<T>::isClosedV() const {
    return false;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PCrossCap<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T a = _r*_r*cos(u)*sin(u)*sin(v);
    T b = _r*_r*sin(2*v)*cos(u)*cos(u);
    T c = _r*_r*cos(2*v)*cos(u)*cos(u);

    this->_p[0][0][0] =	a;
    this->_p[0][0][1] =	b;
    this->_p[0][0][2] =	c;

    T d, e, f, g;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) //u
      {
        d = 2*_r*_r*sin(2*v)*cos(u)*sin(u);
        e = 2*_r*_r*cos(2*v)*cos(u)*sin(u);
        this->_p[1][0][0] =	-_r*_r*sin(u)*sin(u)*sin(v) + _r*_r*cos(u)*cos(u)*sin(v);
        this->_p[1][0][1] =	-d;
        this->_p[1][0][2] =	-e;
      }
      if(d1>1)//uu
      {
        f = 2*_r*_r*sin(2*v)*sin(u)*sin(u);
        g = 2*_r*_r*cos(2*v)*sin(u)*sin(u);
        this->_p[2][0][0] =	-4*a;
        this->_p[2][0][1] =	 f - 2*b;
        this->_p[2][0][2] =	 g - 2*c;
      }
      if(d2) //v
      {
        this->_p[0][1][0] =	 _r*_r*cos(u)*sin(u)*cos(v);
        this->_p[0][1][1] =	 2*c;
        this->_p[0][1][2] =	-2*b;
      }
      if(d2>1) //vv
      {
        this->_p[0][2][0] =	  -a;
        this->_p[0][2][1] =	-4*b;
        this->_p[0][2][2] =	-4*c;
      }
      if(d1 && d2) //uv
      {
        this->_p[1][1][0] =	 -_r*_r*sin(u)*sin(u)*cos(v) + _r*_r*cos(u)*cos(u)*cos(v);
        this->_p[1][1][1] =	 -2*e;
        this->_p[1][1][2] =	  2*d;
      }
      if(d1>1 && d2)//uuv
      {
        this->_p[2][1][0] =	-4*this->_p[0][1][0];
        this->_p[2][1][1] =	 2*g - 4*c;
        this->_p[2][1][2] =	-2*f + 4*b;
      }
      if(d1 && d2>1) //uvv
      {
        this->_p[1][2][0] =	 -this->_p[1][0][0];
        this->_p[1][2][1] =	 4*d;
        this->_p[1][2][2] =	 4*e;
      }
      if(d1>1 && d2>1) //uuvv
      {
        this->_p[2][2][0] =	 4*a;
        this->_p[2][2][1] =	-4*f + 8*b;
        this->_p[2][2][2] =	-4*g + 8*c;
      }
    }
  }



  template <typename T>
  T PCrossCap<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PCrossCap<T>::getEndPU() const {
    return T(/*2 */ M_PI);
  }


  template <typename T>
  T PCrossCap<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PCrossCap<T>::getEndPV() const {
    return T(/*2 */ M_PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PCrossCap<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
