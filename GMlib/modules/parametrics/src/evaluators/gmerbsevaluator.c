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


  //**************************************
  //            constructor             **
  //**************************************


  template <typename T>
  ERBSEvaluator<T>::ERBSEvaluator( int m, T alpha, T beta, T gamma, T lambda ) {
    texParameters( alpha, beta, gamma, lambda );

    this->init(m);
    this->set(0, 1);
  }


  //**************************************
  //      get intrinsic parameters      **
  //**************************************


  template <typename T>
  inline
  T ERBSEvaluator<T>::getAlpha() const {
    return _alpha;
  }


  template <typename T>
  inline
  T ERBSEvaluator<T>::getBeta() const {
    return _beta;
  }


  template <typename T>
  inline
  T ERBSEvaluator<T>::getGamma() const {
    return _gamma;
  }


  template <typename T>
  inline
  T ERBSEvaluator<T>::getLambda() const {
    return _lambda;
  }


  //**************************************
  //      set intrinsic parameters      **
  //**************************************


  template <typename T>
  inline
  void ERBSEvaluator<T>::setAlpha( T alpha ) {
    _alpha = alpha;
  }


  template <typename T>
  inline
  void ERBSEvaluator<T>::setBeta( T beta ) {
    _beta   = beta;
  }


  template <typename T>
  inline
  void ERBSEvaluator<T>::setGamma( T gamma ) {
    _gamma  = gamma;
  }


  template <typename T>
  inline
  void ERBSEvaluator<T>::setLambda( T lambda ) {
    _lambda = lambda;
  }


  template <typename T>
  inline
  void ERBSEvaluator<T>::texParameters( T alpha, T beta, T gamma, T lambda ) {
    _alpha  = alpha;
    _beta   = beta;
    _gamma  = gamma;
    _lambda = lambda;
  }



  //**************************************
  //   static variable and functions    **
  //**************************************


  template <typename T>
  ERBSEvaluator<T> *ERBSEvaluator<T>::_s_instance = 0x0;



  template <typename T>
  inline
  ERBSEvaluator<T>* ERBSEvaluator<T>::getInstance() {
    if( !_s_instance )
      _s_instance = new ERBSEvaluator<T>();

    return _s_instance;
  }


  //**************************************
  //            lokal functions         **
  //**************************************


  template <typename T>
  inline
  T ERBSEvaluator<T>::getF2( T t ) {
    if( t == _lambda ||  t < 2.3e-308 || t == 1 )
      return T(0);

    T h = (t-1/(1+_gamma)) * std::fabs(t-_lambda)/(t * (1-t));
    if( t < _lambda )
      h -= T(1);
    else
      h += T(1);
    h *= -this->getScale()*_alpha*_beta*(1+_gamma)/std::pow(t*std::pow(1-t, _gamma), _alpha);

    if( (1+_gamma)*_alpha < 1 ) {

      const T g = std::pow(std::fabs(t-_lambda), 1-(1+_gamma)*_alpha)/h;
      if( g < 2.3e-308 )
        return 0.0;
      else
        return 1 / g;
    }
    else if( (1+_gamma)*_alpha > 1 )
      return	h * std::pow(std::fabs(t-_lambda), (1+_gamma)*_alpha-1);
    else
      return	h;
  }



  template <typename T>
  inline
  T ERBSEvaluator<T>::getPhi( T t ) {
    const T d = std::pow( t*std::pow( 1-t, _gamma ), _alpha );
    if( d < 2.3e-308 )
      return T(0);
    else
      return std::exp(
        -_beta * std::pow( std::fabs(t-_lambda),
        _alpha * (1+_gamma) ) / d
      );
  }



} // END namespace GMlib
