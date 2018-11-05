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
  PTorus<T>::PTorus( T wheelrad, T tuberad1, T tuberad2 ) {

    init();
    _a = wheelrad;
    _b = tuberad1;
    _c = tuberad2;
    this->_dm = GM_DERIVATION_EXPLICIT;
  }


  template <typename T>
  inline
  PTorus<T>::PTorus( const PTorus<T>& copy ) : PSurf<T,3>(copy) {

    init();
    _a = copy._a;
    _b = copy._b;
    _c = copy._b;
  }


  template <typename T>
  PTorus<T>::~PTorus() {}


  //*****************************************
  //            Local functons             **
  //*****************************************

  template <typename T>
  inline
  const T& PTorus<T>::getTubeRadius1() const {
    return _b;
  }

  template <typename T>
  inline
  const T& PTorus<T>::getTubeRadius2() const {
    return _c;
  }

  template <typename T>
  inline
  const T& PTorus<T>::getWheelRadius() const {
    return _a;
  }

  template <typename T>
  inline
  void PTorus<T>::setTubeRadius1( const T& radius ) {
    _b = radius;
  }

  template <typename T>
  inline
  void PTorus<T>::setTubeRadius2( const T& radius ) {
    _c = radius;
  }

  template <typename T>
  inline
  void PTorus<T>::setWheelRadius( const T& radius ) {
    _a = radius;
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PTorus<T>::isClosedU() const {
    return true;
  }


  template <typename T>
  bool PTorus<T>::isClosedV() const {
    return true;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PTorus<T>::eval(T u, T v, int d1,int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim(d1+1,d2+1);

    T su = sin(u);
    T sv = sin(v);
    T cu = cos(u);
    T cv = cos(v);
    T bcva = _b*cv+_a;
    T cucv = _b*cu*cv;
    T cusv = _b*cu*sv;
    T sucv = _b*su*cv;
    T susv = _b*su*sv;
    sv *= _c;
    cv *= _c;
    cu *= bcva;
    su *= bcva;

    //S
    this->_p[0][0][0] =	   cu;
    this->_p[0][0][1] =    su;
    this->_p[0][0][2] =    sv;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                  //Su
        this->_p[1][0][0] =	-su;
        this->_p[1][0][1] =	 cu;
        this->_p[1][0][2] =	 T(0);
      }
      if(d1>1) {                //Suu
        this->_p[2][0][0] =  -cu;
        this->_p[2][0][1] =  -su;
        this->_p[2][0][2] =   T(0);
      }
      if(d2) {                  //Sv
        this->_p[0][1][0] =  -cusv;
        this->_p[0][1][1] =  -susv;
        this->_p[0][1][2] =   cv;
      }
      if(d2>1) {                //Svv
        this->_p[0][2][0] =  -cucv;
        this->_p[0][2][1] =  -sucv;
        this->_p[0][2][2] =  -sv;
      }
      if(d1 && d2) {            //Suv
        this->_p[1][1][0] =   susv;
        this->_p[1][1][1] =  -cusv;
        this->_p[1][1][2] =   T(0);
      }
      if(d1>1 && d2) {          //Suuv
        this->_p[2][1][0] =   cusv;
        this->_p[2][1][1] =   susv;
        this->_p[2][1][2] =   T(0);
      }
      if(d1 && d2>1) {          //Suvv
        this->_p[1][2][0] =   sucv;
        this->_p[1][2][1] =  -cucv;
        this->_p[1][2][2] =   T(0);
      }
      if(d1>1 && d2>1) {        //Suuvv
        this->_p[2][2][0] =   cucv;
        this->_p[2][2][1] =   sucv;
        this->_p[2][2][2] =   T(0);
      }
      if(d1>2 && d2) {          //Suuuv
        this->_p[3][1][0] =  -susv;
        this->_p[3][1][1] =   cusv;
        this->_p[3][1][2] =   T(0);
      }
      if(d1>2 && d2>1) {        //Suuuvv
        this->_p[3][2][0] =  -sucv;
        this->_p[3][2][1] =   cucv;
        this->_p[3][2][2] =   T(0);
      }
      if(d1>2 && d2>1) {        //Suuuvvv
        this->_p[3][3][0] =   susv;
        this->_p[3][3][1] =  -cusv;
        this->_p[3][3][2] =   T(0);
      }
      if(d1 && d2>2) {          //Suvvv
        this->_p[1][3][0] =  -susv;
        this->_p[1][3][1] =   cusv;
        this->_p[1][3][2] =   T(0);
      }
      if(d1>1 && d2>2) {        //Suuvvv
        this->_p[2][3][0] =  -cusv;
        this->_p[2][3][1] =  -susv;
        this->_p[2][3][2] =   T(0);
      }
      if(d1>2) {                //Suuu
        this->_p[3][0][0] =   su;
        this->_p[3][0][1] =  -cu;
        this->_p[3][0][2] =   T(0);
      }
      if(d2>2) {                //Svvv
        this->_p[0][3][0] =   cusv;
        this->_p[0][3][1] =   susv;
        this->_p[0][3][2] =  -cv;
      }
    }
  }


  template <typename T>
  T PTorus<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PTorus<T>::getEndPU() const {
    return T(2*M_PI);
  }


  template <typename T>
  T PTorus<T>::getStartPV() const {
    return T(0);
  }

  template <typename T>
  T PTorus<T>::getEndPV() const {
    return T(2*M_PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PTorus<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib

