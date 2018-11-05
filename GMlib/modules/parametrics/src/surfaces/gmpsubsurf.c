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
  PSubSurf<T>::PSubSurf( PSurf<T,3>* s, T su, T eu, T sv, T ev )
  {
    init();
    set(s, su, eu, (su+eu)/2, sv, ev, (sv+ev)/2);
    // Set local coordinate system
    DMatrix<Vector<T,3> > tr = _s->evaluateParent(_q, 0);
    _trans = tr[0][0];
    this->translateParent(_trans);
  }


  template <typename T>
  inline
  PSubSurf<T>::PSubSurf( PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v )
  {
    init();
    set(s, su, eu, u, sv, ev, v);
    // Set local coordinate system
    DMatrix<Vector<T,3> > tr = _s->evaluateParent(_q, 0);
    _trans = tr[0][0];
    this->translateParent( _trans );
  }


  template <typename T>
  inline
  PSubSurf<T>::PSubSurf( PSurf<T,3>* s, const Point<T,2>& p1, const Point<T,2>& p2, const Point<T,2>& p3, const Point<T,2>& p4)
  {
    init();
    set(s, p1, p2, p3, p4, (p1+p2+p3+p4)/4);
    // Set local coordinate system
    DMatrix<Vector<T,3> > tr = _s->evaluateParent(_q, 0);
    _trans = tr[0][0];
    this->translateParent( _trans );
  }


  template <typename T>
  inline
  PSubSurf<T>::PSubSurf( PSurf<T,3>* s, const Point<T,2>& p1, const Point<T,2>& p2, const Point<T,2>& p3, const Point<T,2>& p4, const Point<T,2>& p)
  {
    init();
    set(s, p1, p2, p3, p4, p);
    // Set local coordinate system
    DMatrix<Vector<T,3> > tr = _s->evaluateParent(_q, 0);
    _trans = tr[0][0];
    this->translateParent( _trans );
  }


  template <typename T>
  inline
  PSubSurf<T>::PSubSurf( const PSubSurf<T>& copy ) : PSurf<T,3>( copy )
  {
    init();
    set(copy._s, copy._p1, copy._a + copy._p1, copy._b + copy._p1,
        copy._a + copy._b + copy._c + copy._p1, copy._q);
    // Set local coordinate system
    _trans = copy._trans;
  }


  template <typename T>
  PSubSurf<T>::~PSubSurf() {}


  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PSubSurf<T>::isClosedU() const
  {
    return false;
  }

  template <typename T>
  bool PSubSurf<T>::isClosedV() const
  {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PSubSurf<T>::eval( T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/) const {

    this->_p.setDim(3,3);

    DMatrix< Vector<T,3> > p = _s->evaluateParent(S(u,v), d1+1);
    this->_p[0][0] = p[0][0] - _trans;

    if(d1 || d2)
    {
      Vector<T,2> bu  = Su(u,v);
      Vector<T,2> bv  = Sv(u,v);
      Matrix<T,3,2> S1;
      S1.setCol(p[1][0], 0);
      S1.setCol(p[0][1], 1);
      if(d1) this->_p[1][0] = S1*bu;        // Q_u
      if(d2) this->_p[0][1] = S1*bv;        // Q_v
//*************************************************
      if(d1>1 || d2>1)
      {
        Vector<T,2> buv = Suv(u,v);

        Matrix<T,3,2> S2;
        // S2 = dS_u = [S_uu  S_uv]
        S2.setCol(p[2][0], 0);
        S2.setCol(p[1][1], 1);
        Vector<T,3> a1 = S2*bu;
        Vector<T,3> b1 = S2*bv;

        // S2 = dS_v = [S_uv  S_vv]
        S2.setCol(p[1][1], 0);
        S2.setCol(p[0][2], 1);
        Vector<T,3> a2 = S2*bu;
        Vector<T,3> b2 = S2*bv;

        // S2 = [dS_u*bu  dS_v*bu]
        S2.setCol(a1, 0);
        S2.setCol(a2, 1);
        this->_p[2][0] = S2*bu;             // Q_uu
        this->_p[1][1] = S2*bv +S1*buv;     // Q_uv
        Vector<T,3> s2buv = S2*buv;

        // S2 = [dS_u*bv  dS_v*bv]
        S2.setCol(b1, 0);
        S2.setCol(b2, 1);
        this->_p[0][2] = S2*bv;             // Q_vv
        Vector<T,3> s22buv = S2*buv;

//*************************************************
        Matrix<T,3,2> S3;
        S3.setCol(p[3][0], 0);
        S3.setCol(p[2][1], 1);
        a1 = S3*bu;

        S3.setCol(p[2][1], 0);
        S3.setCol(p[1][2], 1);
        a2 = S3*bu;

        S3.setCol(p[1][2], 0);
        S3.setCol(p[0][3], 1);
        Vector<T,3> a3 = S3*bu;

        S3.setCol(a1, 0);
        S3.setCol(a2, 1);
        a1 = S3*bu;
        b1 = S3*bv;

        S3.setCol(a2, 0);
        S3.setCol(a3, 1);
        a2 = S3*bu;
        b2 = S3*bv;

        S3.setCol(a1, 0);
        S3.setCol(a2, 1);
        this->_p[2][1] = S3*bv + s2buv;    // Q_uuv

        //Vector<T,3> s3buv = 2*(S3*buv);

        S3.setCol(b1, 0);
        S3.setCol(b2, 1);
        this->_p[1][2] = S3*bv + s22buv;    // Q_uvv

        Vector<T,3> NNL(0,0,0);
        Matrix<T,3,2> S4;
        S4.setCol(NNL, 0);
        S4.setCol(p[3][1], 1);
        a1 = S4*bu;
        S4.setCol(p[3][1], 0);
        S4.setCol(p[2][2], 1);
        a2 = S4*bu;
        S4.setCol(p[2][2], 0);
        S4.setCol(p[1][3], 1);
        a3 = S4*bu;
        S4.setCol(p[1][3], 0);
        S4.setCol(NNL, 1);
        Vector<T,3> a4 = S4*bu;

        S4.setCol(a1, 0);
        S4.setCol(a2, 1);
        a1 = S4*bu;
        S4.setCol(a2, 0);
        S4.setCol(a3, 1);
        a2 = S4*bu;
        S4.setCol(a3, 0);
        S4.setCol(a4, 1);
        a3 = S4*bu;

        S4.setCol(a1, 0);
        S4.setCol(a2, 1);
        a1 = S4*bv;
        S4.setCol(a2, 0);
        S4.setCol(a3, 1);
        a2 = S4*bv;

        S4.setCol(a1, 0);
        S4.setCol(a2, 1);
        this->_p[2][2] = S4*bv;             // Q_uuvv
      }
    }
//    std::cout << this->_p << std::endl;

  }


  template <typename T>
  T PSubSurf<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PSubSurf<T>::getEndPU() const {
    return T(1);
  }


  template <typename T>
  T PSubSurf<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PSubSurf<T>::getEndPV() const {
    return T(1);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PSubSurf<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


  //*****************************************
  //       Private help functions          **
  //*****************************************

  template <typename T>
  inline
  Vector<T,2> PSubSurf<T>::S(T u, T v) const {
    return _p1 + u*_a + v*_b + (u*v)*_c;
  }

  template <typename T>
  inline
  Vector<T,2> PSubSurf<T>::Su(T u, T v) const{
    return _a + v*_c;
  }

  template <typename T>
  inline
  Vector<T,2> PSubSurf<T>::Sv(T u, T v) const{
    return _b + u*_c;
  }

  template <typename T>
  inline
  Vector<T,2> PSubSurf<T>::Suv(T u, T v) const{
    return _c;
  }



  template <typename T>
  inline
  void PSubSurf<T>::set(PSurf<T,3>* s, T su, T eu, T u, T sv, T ev, T v) {
    _s  = s;
    _p1 = Point<T,2>(su,sv);
    _a  = Point<T,2>(eu,sv)-_p1;
    _b  = Point<T,2>(su,ev)-_p1;
    _c  = Point<T,2>(0,0);
    _q  = Point<T,2>(u,v);
  }


  template <typename T>
  inline
  void PSubSurf<T>::set(PSurf<T,3>* s, const Point<T,2>& p1, const Point<T,2>& p2, const Point<T,2>& p3, const Point<T,2>& p4, const Point<T,2>& p) {
    _s  = s;
    _p1 = p1;
    _a  = p2-p1;
    _b  = p3-p1;
    _c  = p4-p3-p2+p1;
    _q  = p;
  }



} // END namespace GMlib

