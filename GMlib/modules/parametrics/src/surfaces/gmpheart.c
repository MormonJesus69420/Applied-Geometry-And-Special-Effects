
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

#include <cmath>



namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************

  template <typename T>
  inline
  PHeart<T>::PHeart() {

    init();
  }


  template <typename T>
  inline
  PHeart<T>::PHeart( const PHeart<T>& copy ) : PSurf<T,3>( copy ) {

    init();
  }


  template <typename T>
  PHeart<T>::~PHeart() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PHeart<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PHeart<T>::isClosedV() const {
    return false;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PHeart<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );


    this->_p[0][0][0] =	(T(4)*std::sqrt((T)(T(1) - u*u))*std::pow( (T)std::sin((T)std::abs(v)) , (T)std::abs(v)))*std::sin(v);
    this->_p[0][0][1] =	(T(4)*std::sqrt((T)(T(1) - u*u))*std::pow( (T)std::sin((T)std::abs(v)) , (T)std::abs(v)))*std::cos(v);
    this->_p[0][0][2] =	u;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                      //u
        this->_p[1][0][0] =	-T(4)*std::pow(std::sin(std::abs(v)),std::abs(v))*std::sin(v)*u/(std::sqrt(T(1)-u*u));
        this->_p[1][0][1] =	-T(4)*std::pow(std::sin(std::abs(v)),std::abs(v))*std::cos(v)*u/(std::sqrt(T(1)-u*u));
        this->_p[1][0][2] =	T(1);
      }
      if(d1>1) {                    //uu
        this->_p[2][0][0] =	T(0);
        this->_p[2][0][1] =	T(0);
        this->_p[2][0][2] =	T(0);
      }
      if(d2) {                      //v
        this->_p[0][1][0] =	T(4)*std::sqrt(T(1)-u*u)*std::pow(std::sin(std::abs(v)),std::abs(v))*(/*abs(1,v)*/log(std::sin(std::abs(v)))+std::abs(v)*std::cos(std::abs(v))
                /*abs(1,v)*//std::sin(std::abs(v)))*std::sin(v)+T(4)*std::sqrt(T(1)-u*u)*std::pow(std::sin(std::abs(v)),std::abs(v))*std::cos(v);
        this->_p[0][1][1] =	T(4)*std::sqrt(T(1)-u*u)*std::pow(std::sin(std::abs(v)),std::abs(v))*(/*abs(1,v)*/log(std::sin(std::abs(v)))+std::abs(v)*std::cos(std::abs(v))
                /*abs(1,v)*//(std::abs(v)))*std::cos(v)-T(4)*std::sqrt(T(1)-u*u)*std::pow(std::sin(std::abs(v)),std::abs(v))*std::sin(v);
        this->_p[0][1][2] =	T(0);
      }
      if(d2>1) {                    //vv
        this->_p[0][2][0] =	T(0);
        this->_p[0][2][1] =	T(0);
        this->_p[0][2][2] =	T(0);
      }
      if(d1 && d2) {                //uv
        this->_p[1][1][0] =	T(0);
        this->_p[1][1][1] =	T(0);
        this->_p[1][1][2] =	T(0);
      }
      if(d1>1 && d2) {              //uuv
        this->_p[2][1][0] =	T(0);
        this->_p[2][1][1] =	T(0);
        this->_p[2][1][2] =	T(0);
      }
      if(d1 && d2>1) {              //uvv
        this->_p[1][2][0] =	T(0);
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
  T PHeart<T>::getStartPU() const {
    return -T(0.98);
  }


  template <typename T>
  T PHeart<T>::getEndPU() const {
    return T(0.98);
  }


  template <typename T>
  T PHeart<T>::getStartPV() const {
    return -T(M_PI+0.0001);
  }


  template <typename T>
  T PHeart<T>::getEndPV() const {
    return T(M_PI-0.0001);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PHeart<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
