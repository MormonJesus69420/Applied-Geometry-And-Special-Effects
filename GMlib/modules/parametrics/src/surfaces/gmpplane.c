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
  PPlane<T>::PPlane( const Point<T,3>& p, const Vector<T,3>& u, const Vector<T,3>& v ) {

    init();
    _pt = p;
    _u  = u;
    _v  = v;

    _n  = u^v;
  }


  template <typename T>
  inline
  PPlane<T>::PPlane( const PPlane<T>& copy ) : PSurf<T,3>( copy ) {

    init();
    _pt   = copy._pt;
    _u    = copy._u;
    _v    = copy._v;
    _n    = copy._n;
  }


  template <typename T>
  PPlane<T>::~PPlane() {}


  //*****************************************
  //            Local functons             **
  //*****************************************

  template <typename T>
  inline
  const UnitVector<T,3>& PPlane<T>::getNormal() const {

    return _n;
  }


  template <typename T>
  inline
  const Vector<T,3>& PPlane<T>::getU() const {
    return _u;
  }


  template <typename T>
  inline
  const Vector<T,3>& PPlane<T>::getV() const {
    return _v;
  }


  template <typename T>
  inline
  void PPlane<T>::setP( const Point<T,3>& p ) {
    _pt = p;
  }

  template <typename T>
  inline
  void PPlane<T>::setU( const Vector<T,3>& u ) {
    _u = u;
  }


  template <typename T>
  inline
  void PPlane<T>::setV( const Vector<T,3>& v ) {
    _v = v;
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PPlane<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PPlane<T>::isClosedV() const {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PPlane<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    this->_p[0][0] = _pt + u*_u + v*_v ;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      // 1st
      if(d1)            this->_p[1][0] = _u; // S_u
      if(d2)            this->_p[0][1] = _v; // S_v
      if(d1>1 && d2>1)  this->_p[1][1] = Vector<T,3>(T(0)); // S_uv

      // 2nd
      if(d1>1)          this->_p[2][0] = Vector<T,3>(T(0)); // S_uu
      if(d2>1)          this->_p[0][2] = Vector<T,3>(T(0)); // S_vv
      if(d1>1 && d2)    this->_p[2][1] = Vector<T,3>(T(0)); // S_uuv
      if(d1   && d2>1)  this->_p[1][2] = Vector<T,3>(T(0)); // S_uvv
      if(d1>1 && d2>1)  this->_p[2][2] = Vector<T,3>(T(0)); // S_uuvv

      // 3rd
      if(d1>2)          this->_p[3][0] = Vector<T,3>(T(0)); // S_uuu
      if(d2>2)          this->_p[0][3] = Vector<T,3>(T(0)); // S_vvv
      if(d1>2 && d2)    this->_p[3][1] = Vector<T,3>(T(0)); // S_uuuv
      if(d1   && d2>2)  this->_p[1][3] = Vector<T,3>(T(0)); // S_uvvv
      if(d1>2 && d2>1)  this->_p[3][2] = Vector<T,3>(T(0)); // S_uuuvv
      if(d1>1 && d2>2)  this->_p[2][3] = Vector<T,3>(T(0)); // S_uuvvv
      if(d1>2 && d2>2)  this->_p[3][3] = Vector<T,3>(T(0)); // S_uuuvvv
    }
  }


  template <typename T>
  T PPlane<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PPlane<T>::getEndPU() const	{
    return T(1);
  }

  template <typename T>
  T PPlane<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PPlane<T>::getEndPV() const	{
    return T(1);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PPlane<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


}
