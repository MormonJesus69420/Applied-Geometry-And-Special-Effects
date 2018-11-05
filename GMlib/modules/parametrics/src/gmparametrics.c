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


  template <typename T, int m, int n>
  inline
  Parametrics<T,m,n>::Parametrics() {

    _dm = GM_DERIVATION_EXPLICIT;
    _initSoType();
  }



  template <typename T, int m, int n>
  inline
  Parametrics<T,m,n>::Parametrics( const Parametrics<T,m,n>& copy )
    : SceneObject( copy ), _dm{copy._dm}, _A{copy._A} {}



  template <typename T, int m, int n>
  inline
  Parametrics<T,m,n>::~Parametrics() {}



  template <typename T, int m, int n>
  void Parametrics<T,m,n>::_initSoType() {

    switch( m ) {
      case GM_POINT:
        this->_type_id = GM_SO_TYPE_POINT;
        break;
      case GM_CURVE:
        this->_type_id = GM_SO_TYPE_CURVE;
        break;
      case GM_SURFACE:
        this->_type_id = GM_SO_TYPE_SURFACE;
        break;
      case GM_VOLUME:
        this->_type_id = GM_SO_TYPE_VOLUME;
        break;
      case GM_FLOW:
        this->_type_id = GM_SO_TYPE_FLOW;
        break;
    }
  }



  template <typename T, int m, int n>
  void Parametrics<T,m,n>::setDerivationMethod( GM_DERIVATION_METHOD method ) {

    _dm = method;
  }




} // END namespace GMlib
