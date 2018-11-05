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
  PApple<T>::PApple( T radius ) {
    init();
    _r = radius;
  }


  template <typename T>
  inline
  PApple<T>::PApple( const PApple<T>& copy ) : PSurf<T,3>( copy ) {
    init();
    _r = copy._r;
  }


  template <typename T>
  PApple<T>::~PApple() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************

  template <typename T>
  bool PApple<T>::isClosedU() const {
    return true;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PApple<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T cu = cos(u);
    T su = sin(u);
    T cv = cos(v);
    T sv = sin(v);
    T a  = 1 - M_PI * v / 10;
    T v1 = 3.8 * cv;
    T v2 = 3.8 * sv;


    this->_p[0][0][0] =	 _r * ( cu * ( 4 + v1 ) );
    this->_p[0][0][1] =	 _r * ( su * ( 4 + v1 ) );
    this->_p[0][0][2] =	 _r * ((cv+sv-1)*(1+sv)*log(a) + 7.5*sv);

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      T c2v = cos(2*v);
      T s2v = sin(2*v);
      T b = M_PI/10;
      T c = log(M_E)*b;
      T d = (1/a)*c;
      T e = c*b*(a*a);
      T la = log(a);

      if(d1){                   // du
        this->_p[1][0][0] = -su * (4 + v1);
        this->_p[1][0][1] =  cu * (4 + v1);
        this->_p[1][0][2] = T(0);
      }
      if(d1>1){                 //duu
        this->_p[2][0][0] = -cu * (4 + v1);
        this->_p[2][0][1] = -su * (4 + v1);
        this->_p[2][0][2] =	T(0);
      }
      if(d2){                   //dv
        this->_p[0][1][0] = -cu * v2;
        this->_p[0][1][1] = -su * v2;
        this->_p[0][1][2] = -cv*d - sv*la + 0.5*(2*c2v*la - s2v*d + d + 2*s2v*la + c2v*d) + 7.5*cv;
      }
      if(d2>1){                 //dvv
        this->_p[0][2][0] = -cu * v1;
        this->_p[0][2][1] = -su * v1;
        this->_p[0][2][2] = 2*sv*d - cv*e - cv*la - 2*c2v*d + 0.5*e*(1+c2v-s2v) + 2*la*(c2v-s2v) - 2*s2v*d - 7.5*sv;
      }
      if(d1 && d2){             //duv/dvu
        this->_p[1][1][0] =  su * v2;
        this->_p[1][1][1] = -cu * v2;
        this->_p[1][1][2] = T(0);
      }
      if(d1 && d2>1){           //duvv
        this->_p[1][2][0] =  su * v1;
        this->_p[1][2][1] = -cu * v1;
        this->_p[1][2][2] = T(0);
      }
      if(d1>1 && d2){           //duuv
        this->_p[2][1][0] = cu * v2;
        this->_p[2][1][1] = su * v2;
        this->_p[2][1][2] = T(0);
      }
      if(d1>1 && d2>1){         //duuvv
        this->_p[2][2][0] = cu * v1;
        this->_p[2][2][1] = su * v1;
        this->_p[2][2][2] = T(0);
      }
    }
  }


  template <typename T>
  T PApple<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PApple<T>::getEndPU() const {
    return T(M_2PI);
  }


  template <typename T>
  T PApple<T>::getStartPV() const {
    return -T(M_PI);
  }


  template <typename T>
  T PApple<T>::getEndPV() const {
    return T(M_PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PApple<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
