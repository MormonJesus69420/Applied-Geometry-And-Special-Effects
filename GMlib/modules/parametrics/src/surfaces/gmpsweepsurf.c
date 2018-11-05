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
  PSweepSurf<T>::PSweepSurf( PCurve<T,3>* profile, PCurve<T,3>* spine, bool RMF) {

    init();
    _profile = profile;
    _spine   = spine;
    _RMF     = RMF;
    _spv     = spine->evaluate(T(0),0)[0];
    makeOmega(300,0);
  }


  template <typename T>
  inline
  PSweepSurf<T>::PSweepSurf( const PSweepSurf<T>& copy ) : PSurf<T,3>( copy ) {

      init();
      _profile = copy._profile;
      _spine   = copy._spine;
      _RMF     = copy._RMF;
  }


  template <typename T>
  PSweepSurf<T>::~PSweepSurf() {}


  //*****************************************
  //            Local functons             **
  //*****************************************

  template <typename T>
  inline
  void PSweepSurf<T>::makeOmega(int n, T omega_0) {
      T start = _spine->getParStart();
      T delta = _spine->getParDelta()/n;
      T del_2 = delta/2;

      DVector<Vector<T,3> > h     = _spine->evaluate(start,3);
      Vector<T,3>           h12   = h[1]^h[2];
      T                     h1_2  = h[1]*h[1];
      T                     h12_2 = h12*h12;
      T                     tau   = (h12*h[3])/h12_2;
      T                     prv   = omega_0;

      _omega.setDim(n);
      _omega[0] = prv;

      for(int i=1; i<=n; i++)
      {
          h         = _spine->evaluate(start + delta*i,3);
          h12       = h[1]^h[2];
          h1_2      = h[1]*h[1];
          h12_2     = h12*h12;
          tau       = (h12*h[3])/h12_2;
          T k       = tau*sqrt(h1_2);
          _omega[i] = _omega[i-1] - del_2*(k+prv);
          prv       = k;
      }
  }


  template <typename T>
  inline
  T PSweepSurf<T>::getOmega(T t) const {
      int   n  = _omega.getDim()-1;
      T     d  = _spine->getParDelta();
      T     u  = t - _spine->getParStart();
      int   i  = n*u/d;
      T     r  = (n*u/d) - T(i);

      return _omega(i) + r*(_omega(i+1) - _omega(i));
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PSweepSurf<T>::isClosedU() const {
    return  _profile->isClosed();
  }


  template <typename T>
  bool PSweepSurf<T>::isClosedV() const {
    return _spine->isClosed();
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PSweepSurf<T>::eval(T u, T v, int d1,int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim(d1+1,d2+1);
    int i;

    DVector<Vector<T,3> > h = _spine->evaluate(v,3);

    Vector<T,3> h12 = h[1]^h[2];

    T h1_2 = h[1]*h[1];
    T h1_1 = sqrt(h1_2);
    T h12_2 = h12*h12;
    T h12_1 = sqrt(h12_2);

    T kappa = h12_1/(h1_2*h1_1);
    T tau   = (h12*h[3])/h12_2;

    Vector<T,3> _T = h[1]/h1_1;
    Vector<T,3> _B = h12/h12_1;
    Vector<T,3> _N = _B^_T;

    Matrix<T,3,3> Av;
    Matrix<T,3,3> dAv;

    for(i=0; i<3; i++) Av[i][0] = _T[i];
    for(i=0; i<3; i++) Av[i][1] = _N[i];
    for(i=0; i<3; i++) Av[i][2] = _B[i];

    Vector<T,3> _dT = (h1_1*kappa)*_N;
    Vector<T,3> _dN = (h1_1*tau)*_B - (h1_1*kappa)*_T;
    Vector<T,3> _dB = (-h1_1*tau)*_N;

    for(i=0; i<3; i++) dAv[i][0] = _dT[i];
    for(i=0; i<3; i++) dAv[i][1] = _dN[i];
    for(i=0; i<3; i++) dAv[i][2] = _dB[i];

    if(_RMF)
    {
        T omega = getOmega(v);
        Matrix<T,3,3> m;
        m[0][0] = 1;  m[0][1] = 0;           m[0][2] = 0;
        m[1][0] = 0;  m[1][1] = cos(omega);  m[1][2] = -sin(omega);
        m[2][0] = 0;  m[2][1] = sin(omega);  m[2][2] =  cos(omega);

        T th =  -tau*h1_1;
        Matrix<T,3,3> dm;
        dm[0] = Vector<T,3>(0.0);
        dm[1] = -th*m[2];
        dm[2] =  th*m[1];

        dAv = dAv*m + Av*dm;
        Av  = Av*m;
    }

    DVector<Vector<T,3> > g = _profile->evaluate(u,d1);

    this->_p[0][0] = h[0] + Av*(g[0] - _spv);       //S

    if( this->_dm == GM_DERIVATION_EXPLICIT)
    {
      if(d1)
        this->_p[1][0] = Av*g[1];                   //Su
      if(d2)
        this->_p[0][1] = h[1] + dAv*(g[0] - _spv);  //Sv
    }
  }


  template <typename T>
  T PSweepSurf<T>::getStartPU() const {
    return _profile->getParStart();
  }


  template <typename T>
  T PSweepSurf<T>::getEndPU() const {
    return _profile->getParEnd();
  }


  template <typename T>
  T PSweepSurf<T>::getStartPV() const {
    return _spine->getParStart();
  }


  template <typename T>
  T PSweepSurf<T>::getEndPV() const {
    return _spine->getParEnd();
  }


  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PSweepSurf<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }




}

