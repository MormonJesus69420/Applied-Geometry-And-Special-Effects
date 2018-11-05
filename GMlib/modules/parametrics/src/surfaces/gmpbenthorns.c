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
  PBentHorns<T>::PBentHorns() {
    init();
  }


  template <typename T>
  inline
  PBentHorns<T>::PBentHorns( const PBentHorns<T>& copy ) : PSurf<T,3>( copy ) {
    init();
  }



  template <typename T>
  PBentHorns<T>::~PBentHorns() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PBentHorns<T>::isClosedU() const {
    return true;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PBentHorns<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T cu	 = cos(u);
    T cv	 = cos(v);
    T su	 = sin(u);
    T sv	 = sin(v);
    T cv1	 = cv - 1;
    T cu2pm3 = cos(u - M_2PI_3);
    T cu2pp3 = cos(u + M_2PI_3);
    T su2pm3 = sin(u - M_2PI_3);
    T su2pp3 = sin(u + M_2PI_3);
    T v3s	 = (v-3*sv)/3;
    T v3c	 = (3*cv-1)/3;

    this->_p[0][0][0] = (2+cu)*v3s;
    this->_p[0][0][1] = (2+cu2pm3)*cv1;
    this->_p[0][0][2] = (2+cu2pp3)*cv1;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {				//Su
        this->_p[1][0][0] = -su*v3s;
        this->_p[1][0][1] = -su2pm3*cv1;
        this->_p[1][0][2] = -su2pp3*cv1;
      }
      if(d1>1) {			//Suu
        this->_p[2][0][0] = -cu*v3s;
        this->_p[2][0][1] = -cu2pm3*cv1;
        this->_p[2][0][2] = -cu2pp3*cv1;
      }
      if(d2) {				//Sv
        this->_p[0][1][0] = -(2+cu)*v3c;
        this->_p[0][1][1] = -(2+cu2pm3)*sv;
        this->_p[0][1][2] = -(2+cu2pp3)*sv;
      }
      if(d2>1) {			//Svv
        this->_p[0][2][0] =	(2+cu)*sv;
        this->_p[0][2][1] = -(2+cu2pm3)*cv;
        this->_p[0][2][2] = -(2+cu2pp3)*cv;
      }
      if(d1 && d2) {		//Suv
        this->_p[1][1][0] =	su*v3c;
        this->_p[1][1][1] =	su2pm3*sv;
        this->_p[1][1][2] =	su2pp3*sv;
      }
      if(d1>1 && d2) {		//Suuv
        this->_p[2][1][0] =	cu*v3c;
        this->_p[2][1][1] =	cu2pm3*sv;
        this->_p[2][1][2] =	cu2pp3*sv;
      }
      if(d1 && d2>1) {		//Suvv
        this->_p[1][2][0] = -su*sv;
        this->_p[1][2][1] =	su2pm3*cv;
        this->_p[1][2][2] =	su2pp3*cv;
      }
      if(d1>1 && d2>1) {	//Suuvv
        this->_p[2][2][0] = -cu*sv;
        this->_p[2][2][1] =	cu2pm3*cv;
        this->_p[2][2][2] =	cu2pp3*cv;
      }
      if(d1>2 && d2) {		//Suuuv
        this->_p[3][1][0] = -su*v3c;
        this->_p[3][1][1] = -su2pm3*sv;
        this->_p[3][1][2] = -su2pp3*sv;
      }
      if(d1>2 && d2>1) {	//Suuuvv
        this->_p[3][2][0] =  su*sv;
        this->_p[3][2][1] = -su2pm3*cv;
        this->_p[3][2][2] = -su2pp3*cv;
      }
      if(d1>2 && d2>2) {	//Suuuvvv
        this->_p[3][3][0] =  su*cv;
        this->_p[3][3][1] =  su2pm3*sv;
        this->_p[3][3][2] =  su2pp3*sv;
      }
      if(d1 && d2>2) {		//Suvvv
        this->_p[1][3][0] = -su*cv;
        this->_p[1][3][1] = -su2pm3*sv;
        this->_p[1][3][2] = -su2pp3*sv;
      }
      if(d1>1 && d2>2) {	//Suuvvv
        this->_p[2][3][0] = -cu*cv;
        this->_p[2][3][1] = -cu2pm3*sv;
        this->_p[2][3][2] = -cu2pp3*sv;
      }
      if(d1>2) {			//Suuu
        this->_p[3][0][0] =  su*v3s;
        this->_p[3][0][1] =  su2pm3*cv1;
        this->_p[3][0][2] =  su2pp3*cv1;
      }
      if(d2>2) {			//Svvv
        this->_p[0][3][0] =	(2+cu)*cv;
        this->_p[0][3][1] = (2+cu2pm3)*sv;
        this->_p[0][3][2] = (2+cu2pp3)*sv;
      }
    }
  }


  template <typename T>
  T PBentHorns<T>::getStartPU() const {
    return -T(M_PI);
  }


  template <typename T>
  T PBentHorns<T>::getEndPU() const {
    return T(M_PI);
  }


  template <typename T>
  T PBentHorns<T>::getStartPV() const {
    return -T(2*M_PI);
  }


  template <typename T>
  T PBentHorns<T>::getEndPV() const {
    return T(2*M_PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PBentHorns<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }



} // END namespace GMlib
