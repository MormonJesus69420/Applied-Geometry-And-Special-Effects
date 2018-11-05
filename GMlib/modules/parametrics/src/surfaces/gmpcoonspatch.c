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
  PCoonsPatch<T>::PCoonsPatch( PSurf<T,3>* s1, PSurf<T,3>* s2, PSurf<T,3>* s3 ) {

    init();
    _s1 = s1;
    _s2 = s2;
    _s3 = s3;
  }


  template <typename T>
  inline
  PCoonsPatch<T>::PCoonsPatch( const PCoonsPatch<T>& copy ) : PSurf<T,3>( copy ) {

      init();
      _s1 = copy._s1;
      _s2 = copy._s2;
      _s3 = copy._s3;
  }


  template <typename T>
  PCoonsPatch<T>::~PCoonsPatch() {}


  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PCoonsPatch<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PCoonsPatch<T>::isClosedV() const {
    return false;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PCoonsPatch<T>::eval(T u, T v, int d1,int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p=_s1->evaluate(u, v, d1, d2) + _s2->evaluate(u, v, d1, d2) - _s3->evaluate(u, v, d1, d2);
  }



  template <typename T>
  T PCoonsPatch<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PCoonsPatch<T>::getEndPU() const {
    return T(1);
  }


  template <typename T>
  T PCoonsPatch<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PCoonsPatch<T>::getEndPV() const {
    return T(1);
  }


  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PCoonsPatch<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib

