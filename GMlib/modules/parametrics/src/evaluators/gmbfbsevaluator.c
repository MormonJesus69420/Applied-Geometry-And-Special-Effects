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
  BFBSEvaluator<T>::BFBSEvaluator( int m, int ik, int ikp1 ) {

    texParameters( ik, ikp1 );

    this->init( m );
    this->set( 0, 1 );
  }

  template <typename T>
  int BFBSEvaluator<T>::getFact( int m ) {

    int prod = 1;
    for( int i=1; i <= m; i++ )
      prod *= i;
    return prod;
  }

  template <typename T>
  T BFBSEvaluator<T>::getF2( T t ) {

    if( t < 2.3e-308 || t == 1.0 )			// check for underflow or legal
      return 0.0;

    T d = t * (1-t);
    if( d < 2.3e-308 )
      return 0.0;
    else {

      T h = -this->getScale() * ( _ik * (1-t) - _ikp1 * t ) / d;
      return h * getPhi(t);
    }
  }

  template <typename T>
  T BFBSEvaluator<T>::getPhi( T t ) {

    T c1 = getFact(_ik) * getFact(_ikp1);			// the denominator

    if(c1 < 2.3e-308 || c1 == 0)			// check for underflow
      return 0.0;
    else
      return std::pow(t,_ik) * std::pow( 1-t, _ikp1 ) * getFact(_ik+_ikp1) / c1;
  }

  template <typename T>
  void BFBSEvaluator<T>::setIk( int ik ) {

    _ik = ik;
  }

  template <typename T>
  void BFBSEvaluator<T>::setIkp1( int ikp1 ) {

    _ikp1 = ikp1;
  }

  template <typename T>
  void BFBSEvaluator<T>::texParameters( int ik, int ikp1 ) {

    _ik = ik;
    _ikp1 = ikp1;
  }


} // END namespace GMlib
