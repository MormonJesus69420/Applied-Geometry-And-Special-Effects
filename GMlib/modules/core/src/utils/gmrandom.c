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

 template <typename T>
 inline
 Random<T>::Random() {

  _high  = T(1);
  _low   = T(0);
 }


  template <typename T>
  inline
  Random<T>::Random( T vLow, T vHigh ) {

    _high = vHigh;
    _low  = vLow;
   }


  template <typename T>
  inline
  T Random<T>::get() {

    T difference = _high - _low;
    T value = T(difference * double(rand()) / double(RAND_MAX));
    return (_low + value);

    //return floor(((float)rand( ) /(float)RAND_MAX) * (_high - _low) + _low);
  }


  template <typename T>
  inline
  void Random<T>::set( T vLow, T vHigh ) {

    _high = vHigh;
    _low  = vLow;
  }


  template <typename T>
  inline
  void Random<T>::setSeed( unsigned int s ) {

    srand(s);
  }

} // END namespace GMlib
