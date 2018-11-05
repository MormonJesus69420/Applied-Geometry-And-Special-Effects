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
  PAsteroidalSphere<T>::PAsteroidalSphere( T sx, T sy, T sz ) {

    init();
    setConstants( sx, sy, sz );
  }


  template <typename T>
  PAsteroidalSphere<T>::PAsteroidalSphere( const PAsteroidalSphere<T>& copy ) : PSurf<T,3>( copy ) {

    _a = copy._a;
    _b = copy._b;
    _c = copy._c;
  }


  //*****************************************
  //            Local functons             **
  //*****************************************

  template <typename T>
  void PAsteroidalSphere<T>::setConstants( T sx, T sy, T sz ) {

    _a = sx;
    _b = sy;
    _c = sz;
  }


  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PAsteroidalSphere<T>::isClosedU() const {
    return true;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PAsteroidalSphere<T>::eval( T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T cu = cos(u);
    T cv = cos(v);
    T su = sin(u);
    T sv = sin(v);


    this->_p[0][0][0] =	T(pow(_a * cu*cv, T(3)));
    this->_p[0][0][1] =	T(pow(_b * su*cv, T(3)));
    this->_p[0][0][2] =	T(pow(_c * sv, T(3)));

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                      //u
        this->_p[1][0][0] =	-T(3)*_a*_a*_a*cu*cu*cv*cv*cv*su;
        this->_p[1][0][1] =	-T(3)*_b*_b*_b*(-1.0+cu*cu)*cv*cv*cv*cu;
        this->_p[1][0][2] =	T(0);
      }
      if(d1>1) {                    //uu
        this->_p[2][0][0] = -T(3)*_a*_a*_a*cu*cv*cv*cv*(T(3)*cu*cu - T(2));
        this->_p[2][0][1] =	 T(3)*_b*_b*_b*su*cv*cv*cv*(T(3)*cu*cu - T(1));
        this->_p[2][0][2] =	 T(0);
      }
      if(d2) {                      //v
        this->_p[0][1][0] =	-T(3)*_a*_a*_a*cu*cu*cu*cv*cv*sv;
        this->_p[0][1][1] =	-T(3)*_b*_b*_b*cu*cu*su*cv*cv*sv;
        this->_p[0][1][2] =	 T(3)*_c*_c*_c*cv*cv*cv;
      }
      if(d2>1) {                    //vv
        this->_p[0][2][0] =	-T(3)*_a*_a*_a*cu*cu*cu*cv*(T(3)*cv*cv - T(2));
        this->_p[0][2][1] =	 T(3)*_b*_b*_b*(cu*cu - T(1))*su*cv*(T(3)*cv*cv - T(2));
        this->_p[0][2][2] =	 T(3)*_c*_c*_c*sv*(T(3)*cv*cv - T(1));
      }
      if(d1 && d2) {                //uv
        this->_p[1][1][0] = T(9)*_a*_a*_a*cu*cu*cv*cv*su*sv;
        this->_p[1][1][1] =	T(9)*_b*_b*_b*(cu*cu - T(1))*cv*cv*cu*sv;
        this->_p[1][1][2] =	T(0);
      }
      if(d1>1 && d2) {              //uuv
        this->_p[2][1][0] =	 T(9)*_a*_a*_a*cu*cv*cv*sv*(T(3)*cu*cu - T(2));
        this->_p[2][1][1] =	-T(9)*_b*_b*_b*su*cv*cv*sv*(T(3)*cu*cu - T(1));
        this->_p[2][1][2] =	T(0);
      }
      if(d1 && d2>1) {              //uvv
        this->_p[1][2][0] =	T(9)*_a*_a*_a*cu*cu*cv*su*(T(3)*cv*cv - T(2));
        this->_p[1][2][1] =	T(9)*_b*_b*_b*(cu*cu - T(1))*cv*cu*(T(3)*cv*cv - T(2));
        this->_p[1][2][2] =	T(0);
      }
      if(d1>1 && d2>1) {            //uuvv
        this->_p[2][2][0] =	 T(9)*_a*_a*_a*cu*cv*(T(4) - T(6)*cv*cv - T(6)*cu*cu + T(9)*cu*cu*cv*cv);
        this->_p[2][2][1] =	-T(9)*_b*_b*_b*su*cv*(T(9)*cu*cu*cv*cv - T(6)*cu*cu + T(2) - T(3)*cv*cv);
        this->_p[2][2][2] =	 T(0);
      }
    }
  }



  template <typename T>
  T PAsteroidalSphere<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PAsteroidalSphere<T>::getEndPU() const {
    return T( M_2PI );
  }


  template <typename T>
  T PAsteroidalSphere<T>::getStartPV() const {
    return -T( M_PI_2 );
  }


  template <typename T>
  T PAsteroidalSphere<T>::getEndPV() const {
    return T( M_PI_2 );
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PAsteroidalSphere<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }



} // END namespace GMlib
