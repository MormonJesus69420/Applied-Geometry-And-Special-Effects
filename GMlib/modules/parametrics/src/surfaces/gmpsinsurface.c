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
  PSinSurface<T>::PSinSurface( T radius ) {

    init();

    _r = radius;
  }


  template <typename T>
  inline
  PSinSurface<T>::PSinSurface( const PSinSurface<T>& copy ) : PSurf<T,3>( copy ) {

    init();

    _r = copy._r;
  }



  template <typename T>
  PSinSurface<T>::~PSinSurface() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PSinSurface<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PSinSurface<T>::isClosedV() const {
    return false;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PSinSurface<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T su  = _r*sin(u);
    T sv  = _r*sin(v);
    T suv = _r*sin(v+u);

    //	S(u,v)
    this->_p[0][0][0] =	su;
    this->_p[0][0][1] =	sv;
    this->_p[0][0][2] =	suv;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {
        T cu  = _r*cos(u);
        T cv  = _r*cos(v);
        T cuv = _r*cos(v+u);

      if(d1)   {                        //u
        this->_p[1][0][0] =	cu;
        this->_p[1][0][1] =	0.0;
        this->_p[1][0][2] =	cuv;
      }
      if(d1>1) {                        //uu
        this->_p[2][0][0] =	-su;
        this->_p[2][0][1] =	0.0;
        this->_p[2][0][2] =	-suv;
      }
      if(d2)   {                        //v
        this->_p[0][1][0] =	0.0;
        this->_p[0][1][1] =	cv;
        this->_p[0][1][2] =	cuv;
      }
      if(d2>1) {                        //vv
        this->_p[0][2][0] =	0.0;
        this->_p[0][2][1] =	-sv;
        this->_p[0][2][2] =	-suv;
      }
      if(d1 && d2) {                    //uv
        this->_p[1][1][0] =	0.0;
        this->_p[1][1][1] =	0.0;
        this->_p[1][1][2] =	-suv;
      }
      if(d1>1 && d2) {                  //uuv
        this->_p[2][1][0] =	0.0;
        this->_p[2][1][1] =	0.0;
        this->_p[2][1][2] =	-cuv;
      }
      if(d1 && d2>1) {                  //uvv
        this->_p[1][2][0] =	0.0;
        this->_p[1][2][1] =	0.0;
        this->_p[1][2][2] =	-cuv;
      }
      if(d1>1 && d2>1) {                //uuvv
        this->_p[2][2][0] =	0.0;
        this->_p[2][2][1] =	0.0;
        this->_p[2][2][2] =	suv;
      }
    }
  }



  template <typename T>
  T PSinSurface<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PSinSurface<T>::getEndPU() const {
    return T(2 * M_PI);
  }


  template <typename T>
  T PSinSurface<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PSinSurface<T>::getEndPV() const {
    return T(2 * M_PI);
  }


  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PSinSurface<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
