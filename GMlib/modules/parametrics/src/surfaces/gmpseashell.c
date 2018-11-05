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
  PSeashell<T>::PSeashell() {

    init();
  }

  template <typename T>
  inline
  PSeashell<T>::PSeashell( const PSeashell<T>& copy ) : PSurf<T,3>( copy ) {

    init();
  }

  template <typename T>
  PSeashell<T>::~PSeashell() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PSeashell<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PSeashell<T>::isClosedV() const {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PSeashell<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T a = 0.51449576;
    T b = 0.085749293;
    T v1 = 0.1*v;
    T su = sin(u);
    T cu = cos(u);
    T sv = sin(v);
    T cv = cos(v);
    T cucv = cu*cv;
    T cusv = cu*sv;
    T sucv = su*cv;
    T susv = su*sv;

    //S
    this->_p[0][0][0] =	    cv+v1*(cucv+a*susv);
    this->_p[0][0][1] =     sv+v1*(cusv-a*sucv);
    this->_p[0][0][2] =     (b*su+0.6)*v;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {                  //Su
        this->_p[1][0][0] =  v1*(-sucv+a*cusv);
        this->_p[1][0][1] =  v1*(-susv-a*cucv);
        this->_p[1][0][2] =  b*v*cu;
      }
      if(d2) {                  //Sv
        this->_p[0][1][0] =  v1*(a*sucv-cusv)+0.1*(cucv+a*susv)-sv;
        this->_p[0][1][1] =  v1*(a*susv+cucv)+0.1*(cusv-a*sucv)+cv;
        this->_p[0][1][2] =  b*su+0.6;
      }
      if(d1>1) {                //Suu
        this->_p[2][0][0] =  v1*(-cucv-a*susv);
        this->_p[2][0][1] =  v1*(-cusv+a*sucv);
        this->_p[2][0][2] = -b*v*su;
      }
      if(d1 && d2) {            //Suv
        this->_p[1][1][0] =  v1*(a*cucv+susv)-0.1*(sucv+a*cusv);
        this->_p[1][1][1] =  v1*(a*cusv-sucv)-0.1*(susv+a*cucv);
        this->_p[1][1][2] =  b*cu;
      }
      if(d2>1) {                //Svv
        this->_p[0][2][0] =  0.2*(a*sucv-cusv)+v1*(-cucv-a*susv)-cv;
        this->_p[0][2][1] =  0.2*(a*susv+cucv)+v1*(a*sucv-cusv)-sv;
        this->_p[0][2][2] =  0.0;
      }
      if(d1>2) {                //Suuu
        this->_p[3][0][0] =  v1*(sucv-a*cusv);
        this->_p[3][0][1] =  v1*(susv+a*cucv);
        this->_p[3][0][2] = -b*v*cu;
      }
      if(d1>1 && d2) {          //Suuv
        this->_p[2][1][0] =  v1*(cusv-a*sucv)-0.1*(cucv-a*susv);
        this->_p[2][1][1] =  v1*(-cucv-a*susv)-0.1*(cusv+a*sucv);
        this->_p[2][1][2] = -b*su;
      }
      if(d1 && d2>1) {          //Suvv
        this->_p[1][2][0] =  0.2*(a*cucv+susv)+v1*(sucv-a*cusv);
        this->_p[1][2][1] =  0.2*(a*cusv-sucv)+v1*(susv+a*cucv);
        this->_p[1][2][2] =  0;
      }
      if(d2>2) {                //Svvv
        this->_p[0][3][0] =  0.3*(-a*susv-cucv)+v1*(cusv-a*sucv)+sv;
        this->_p[0][3][1] =  0.3*(a*sucv-cusv)+v1*(-a*susv-cucv)-cv;
        this->_p[0][3][2] =  0;
      }
      if(d1>2 && d2) {          //Suuuv
        this->_p[3][1][0] =  v1*(-susv-a*cucv)-0.1*(-sucv-a*cusv);
        this->_p[3][1][1] =  v1*(sucv-a*cusv)-0.1*(-susv+a*cucv);
        this->_p[3][1][2] =  -b*cu;
      }
      if(d1>1 && d2>1) {        //Suuvv
        this->_p[2][2][0] =  0.2*(cusv-a*sucv)+v1*(cucv+a*susv);
        this->_p[2][2][1] =  0.2*(-a*susv-cucv)+v1*(-a*sucv+cusv);
        this->_p[2][2][2] =  0;
      }
      if(d1>2 && d2>1) {        //Suuuvv
        this->_p[3][2][0] =  0.2*(-susv-a*cucv)+v1*(-sucv+a*cusv);
        this->_p[3][2][1] =  0.2*(-a*cusv+sucv)+v1*(-a*cucv-susv);
        this->_p[3][2][2] =  0;
      }
      if(d1 && d2>2) {          //Suvvv
        this->_p[1][3][0] =  0.3*(-a*cusv+sucv)+v1*(-susv-a*cucv);
        this->_p[1][3][1] =  0.3*(a*cucv+susv)+v1*(-a*cusv+sucv);
        this->_p[1][3][2] =  0;
      }
      if(d1>1 && d2>2) {        //Suuvvv
        this->_p[2][3][0] =  0.3*(a*susv+cucv)+v1*(-cusv+a*sucv);
        this->_p[2][3][1] =  0.3*(-a*sucv+cusv)+v1*(a*susv+cucv);
        this->_p[2][3][2] =  0;
      }
      if(d1>2 && d2>2) {        //Suuuvvv
        this->_p[3][3][0] =  0.3*(a*cusv-sucv)+v1*(susv+a*cucv);
        this->_p[3][3][1] =  0.3*(-a*cucv-susv)+v1*(a*cusv-sucv);
        this->_p[3][3][2] =  0;
      }
    }
  }


  template <typename T>
  T PSeashell<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PSeashell<T>::getEndPU() const {
    return T(2*M_PI);
  }


  template <typename T>
  T PSeashell<T>::getStartPV() const {
    return T(M_PI*0.25);
  }


  template <typename T>
  T PSeashell<T>::getEndPV() const {
    return T(5*M_PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PSeashell<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
