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
  PApple2<T>::PApple2( T radius ) {
    init();
    _r = radius;
  }


  template <typename T>
  inline
  PApple2<T>::PApple2( const PApple2<T>& copy ) : PSurf<T,3>( copy ) {
    init();
    _r = copy._r;
  }


  template <typename T>
  PApple2<T>::~PApple2() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  inline
  bool PApple2<T>::isClosedU() const {
    return true;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PApple2<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T cu = cos(u);
    T su = sin(u);
    T cv = cos(v);
    T sv = sin(v);

    this->_p[0][0][0] =	 2*_r*cu*(1+cv)*sv;
    this->_p[0][0][1] =	 2*_r*su*(1+cv)*sv;
    this->_p[0][0][2] =	-2*_r*cv*(1+cv);

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      // To prevent singularities at top and bottom
      if(GMutils::compValueF(sv,T(0))) {
        if(GMutils::compValueF(cv,-T(1)))
           cv += T(1e-4);
        sv += T(1e-6);
      }

      if(d1) {                          //u
        this->_p[1][0][0] = -2*_r*su*(1+cv)*sv;
        this->_p[1][0][1] =  2*_r*cu*(1+cv)*sv;
        this->_p[1][0][2] =  T(0);
      }
      if(d1>1) {                        //uu
        this->_p[2][0][0] =	-2*_r*cu*(1+cv)*sv;
        this->_p[2][0][1] =	-2*_r*su*(1+cv)*sv;
        this->_p[2][0][2] =	 T(0);
      }
      if(d2) {                          //v
        this->_p[0][1][0] =	 2*_r*cu*(2*cv-1)*(cv+1);
        this->_p[0][1][1] =	 2*_r*su*(2*cv-1)*(cv+1);
        this->_p[0][1][2] =	 2*_r*sv*(1+2*cv);
      }
      if(d2>1) {                        //vv
        this->_p[0][2][0] =	-2*_r*cu*sv*(1+4*cv);
        this->_p[0][2][1] =	-2*_r*su*sv*(1+4*cv);
        this->_p[0][2][2] =	 2*_r*(cv+2*(cv*cv-sv*sv));
      }
      if(d1 && d2) {                    //uv
        this->_p[1][1][0] =	-2*_r*su*(2*cv*(cv+1)-1);
        this->_p[1][1][1] =	 2*_r*cu*(2*cv*(cv+1)-1);
        this->_p[1][1][2] =	 T(0);
      }
      if(d1>1 && d2) {                  //uuv
        this->_p[2][1][0] =	-2*_r*cu*(2*cv*(cv+1)-1);
        this->_p[2][1][1] =	-2*_r*su*(2*cv*(cv+1)-1);
        this->_p[2][1][2] =	 T(0);
      }
      if(d1 && d2>1) {                  //uvv
        this->_p[1][2][0] =	 2*_r*su*sv*(4*cv+2);
        this->_p[1][2][1] =	-2*_r*cu*sv*(4*cv+2);
        this->_p[1][2][2] =	 T(0);
      }
      if(d1>1 && d2>1) {                //uuvv

        this->_p[2][2][0] =	 2*_r*cu*sv*(4*cv+2);
        this->_p[2][2][1] =	 2*_r*su*sv*(4*cv+2);
        this->_p[2][2][2] =	 T(0);
      }
    }
  }



  template <typename T>
  T PApple2<T>::getStartPU() const {
      return -T(M_PI);
  }


  template <typename T>
  T PApple2<T>::getEndPU() const {
      return T(M_PI);
  }


  template <typename T>
  T PApple2<T>::getStartPV() const {
      return T(0);
  }


  template <typename T>
  T PApple2<T>::getEndPV() const {
      return T(M_PI);
  }




  //*****************************************
  //     Local (protected) functons        **
  //*****************************************


  template <typename T>
  void PApple2<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
