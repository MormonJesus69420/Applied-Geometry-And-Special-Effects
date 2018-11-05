
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
  PCone<T>::PCone( T radius, T height ) {

    init();
    _r = radius;
    _h = height;
  }


  template <typename T>
  inline
  PCone<T>::PCone( const PCone<T>& copy ) : PSurf<T,3>( copy ) {

    init();
    _r = copy._r;
    _h = copy._h;
  }


  template <typename T>
  PCone<T>::~PCone() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PCone<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PCone<T>::isClosedV() const {
    return false;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PCone<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );


    this->_p[0][0][0] =	(_h-u)*_r*cos(v)/_h;
    this->_p[0][0][1] =	(_h-u)*_r*sin(v)/_h;
    this->_p[0][0][2] =	u;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                      //u
        this->_p[1][0][0] =	-_r*cos(v)/_h;
        this->_p[1][0][1] =	-_r*sin(v)/_h;
        this->_p[1][0][2] =	T(1);
      }
      if(d1>1) {                    //uu
        this->_p[2][0][0] =	T(0);
        this->_p[2][0][1] =	T(0);
        this->_p[2][0][2] =	T(0);
      }
      if(d2) {                      //v
        this->_p[0][1][0] =	-(_h-u)*_r*sin(v)/_h;
        this->_p[0][1][1] =	(_h-u)*_r*cos(v)/_h;
        this->_p[0][1][2] =	T(0);
      }
      if(d2>1) {                    //vv
        this->_p[0][2][0] =	-(_h-u)*_r*cos(v)/_h;
        this->_p[0][2][1] =	-(_h-u)*_r*sin(v)/_h;
        this->_p[0][2][2] =	T(0);
      }
      if(d1>1 && d2>1) {            //uv
        this->_p[1][1][0] =	_r*sin(v)/_h;
        this->_p[1][1][1] =	-_r*cos(v)/_h;
        this->_p[1][1][2] =	T(0);
      }
      if(d1>2) {                    //uuv
        this->_p[2][1][0] =	T(0);
        this->_p[2][1][1] =	T(0);
        this->_p[2][1][2] =	T(0);
      }
      if(d2>2) {                    //uvv
        this->_p[1][2][0] =	_r*cos(v)/_h;
        this->_p[1][2][1] =	_r*sin(v)/_h;
        this->_p[1][2][2] =	T(0);
      }
      if(d1>2 && d2>2) {            //uuvv
        this->_p[2][2][0] =	T(0);
        this->_p[2][2][1] =	T(0);
        this->_p[2][2][2] =	T(0);
      }
    }
  }


  template <typename T>
  T PCone<T>::getStartPU() const {
    return -_h;
  }


  template <typename T>
  T PCone<T>::getEndPU() const {
    return _h;
  }


  template <typename T>
  T PCone<T>::getStartPV() const {
    return -T(0.5*M_PI);
  }


  template <typename T>
  T PCone<T>::getEndPV() const {
    return T(1.5*M_PI);
  }




  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PCone<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
