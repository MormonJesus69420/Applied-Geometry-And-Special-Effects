
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
  PDiniSurface<T>::PDiniSurface( T radius, T dist_turns ) {
    init();
    _r1 = radius;
    _r2 = dist_turns;
  }

  template <typename T>
  inline
  PDiniSurface<T>::PDiniSurface( const PDiniSurface<T>& copy ) : PSurf<T,3>( copy ) {
    init();
    _r1 = copy._r1;
    _r2 = copy._r2;
  }

  template <typename T>
  PDiniSurface<T>::~PDiniSurface() {}


  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PDiniSurface<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PDiniSurface<T>::isClosedV() const {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PDiniSurface<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );


    this->_p[0][0][0] =  	_r1*cos(u)*sin(v);
    this->_p[0][0][1] =  	_r1*sin(u)*sin(v);
    this->_p[0][0][2] =  	_r1*(cos(v)+log(tan((T(0.5)*v))))+_r2*u;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {              //u
        this->_p[1][0][0] =	-_r1*sin(u)*sin(v);
        this->_p[1][0][1] =	_r1*cos(u)*sin(v);
        this->_p[1][0][2] =	_r2;
      }
      if(d1>1) {            //uu
        this->_p[2][0][0] =	-_r1*cos(u)*sin(v);
        this->_p[2][0][1] =	-_r1*sin(u)*sin(v);
        this->_p[2][0][2] =	T(0);
      }
      if(d2) {              //v
        this->_p[0][1][0] =	_r1*cos(u)*cos(v);
        this->_p[0][1][1] =	_r1*sin(u)*cos(v);
        this->_p[0][1][2] =	_r1*(-sin(v)+(1+tan(T(.5)*v)*tan(T(.5)*v))*T(.5)/tan(T(.5)*v));
      }
      if(d2>1) {            //vv
        this->_p[0][2][0] =	-_r1*cos(u)*sin(v);
        this->_p[0][2][1] =	-_r1*sin(u)*sin(v);
        this->_p[0][2][2] =	-0.25*_r1*(-4.0*cos(v)*pow(cos(T(0.5)*v),T(2.0))+4.0*cos(v)*pow(cos(T(0.5)*v),T(4.0))+1.0
                -2.0*pow(cos(T(0.5)*v),T(2.0)))/pow(cos(T(0.5)*v),T(2.0))/(-1.0+pow(cos(T(0.5)*v),T(2.0)));
      }
      if(d1 && d2) {        //uv
        this->_p[1][1][0] =	-_r1*sin(u)*cos(v);
        this->_p[1][1][1] =	_r1*cos(u)*cos(v);
        this->_p[1][1][2] =	T(0);
      }
      if(d1>1 && d2) {      //uuv
        this->_p[2][1][0] =	-_r1*cos(u)*cos(v);
        this->_p[2][1][1] =	-_r1*sin(u)*cos(v);
        this->_p[2][1][2] =	T(0);
      }
      if(d1 && d2>1) {      //uvv
        this->_p[1][2][0] =	_r1*sin(u)*sin(v);
        this->_p[1][2][1] =	-_r1*cos(u)*sin(v);
        this->_p[1][2][2] =	T(0);
      }
      if(d1>1 && d2>1) {    //uuvv
        this->_p[2][2][0] =	_r1*cos(u)*sin(v);
        this->_p[2][2][1] =	_r1*sin(u)*sin(v);
        this->_p[2][2][2] =	T(0);
      }
    }
  }


  template <typename T>
  T PDiniSurface<T>::getStartPU() const {
    return T(0);
  }

  template <typename T>
  T PDiniSurface<T>::getEndPU() const {
    return T(4 * M_PI);
  }

  template <typename T>
  T PDiniSurface<T>::getStartPV() const {
    return (0.001);
  }

  template <typename T>
  T PDiniSurface<T>::getEndPV() const {
    return T(2);
  }





  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PDiniSurface<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }



} // END namespace GMlib
