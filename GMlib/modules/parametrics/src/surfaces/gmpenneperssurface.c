
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
  PEnnepersSurface<T>::PEnnepersSurface() {

    init();
  }


  template <typename T>
  inline
  PEnnepersSurface<T>::PEnnepersSurface( const PEnnepersSurface<T>& copy ) : PSurf<T,3>( copy ) {

    init();
  }



  template <typename T>
  PEnnepersSurface<T>::~PEnnepersSurface() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PEnnepersSurface<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PEnnepersSurface<T>::isClosedV() const {
    return false;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PEnnepersSurface<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T a = T(3.3333333e-01);

    this->_p[0][0][0] =  -a*(u*u*u) + (v*v)*u + u;
    this->_p[0][0][1] =  -v - v*(u*u) + a*(v*v*v);
    this->_p[0][0][2] =  (u*u) - (v*v);


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                      //u
        this->_p[1][0][0] =	-T(3)*a*u*u + v*v + T(1);
        this->_p[1][0][1] =	-T(2)*v*u;
        this->_p[1][0][2] =	 T(2)*u;
      }
      if(d1>1) {                    //uu
        this->_p[2][0][0] =	-T(6)*a*u;
        this->_p[2][0][1] =	-T(2)*v;
        this->_p[2][0][2] =	 T(2);
      }
      if(d2) {                      //v
        this->_p[0][1][0] =	 T(2)*v*u;
        this->_p[0][1][1] =	-T(1) - u*u + T(3)*a*v*v;
        this->_p[0][1][2] =	-T(2)*v;
      }
      if(d2>1) {                    //vv
        this->_p[0][2][0] =  T(2)*u;
        this->_p[0][2][1] =  T(6)*a*v;
        this->_p[0][2][2] =	-T(2);
      }
      if(d1 && d2) {                //uv
        this->_p[1][1][0] =	 T(2)*v;
        this->_p[1][1][1] =	-T(2)*u;
        this->_p[1][1][2] =	 T(0);
      }
      if(d1>1 && d2) {              //uuv
        this->_p[2][1][0] =	 T(0);
        this->_p[2][1][1] =	-T(2);
        this->_p[2][1][2] =	 T(0);
      }
      if(d1 && d2>1) {              //uvv
        this->_p[1][2][0] =	T(2);
        this->_p[1][2][1] =	T(0);
        this->_p[1][2][2] =	T(0);
      }
      if(d1>1 && d2>1) {            //uuvv
        this->_p[2][2][0] =	T(0);
        this->_p[2][2][1] =	T(0);
        this->_p[2][2][2] =	T(0);
      }
    }
  }



  template <typename T>
  T PEnnepersSurface<T>::getStartPU() const {
    return -T(1.5);
  }


  template <typename T>
  T PEnnepersSurface<T>::getEndPU() const {
    return T(1.5);
  }


  template <typename T>
  T PEnnepersSurface<T>::getStartPV() const {
    return -T(1.5);
  }


  template <typename T>
  T PEnnepersSurface<T>::getEndPV() const {
    return T(1.5);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PEnnepersSurface<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
