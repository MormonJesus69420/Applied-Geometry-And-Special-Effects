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
PSubTriangle<T>::PSubTriangle( PSurf<T,3>* s, Vector< Point<T,2>,3> p ) {

  _s = s;
  _q = p;
}

template <typename T>
PSubTriangle<T>::PSubTriangle( const PSubTriangle <T>& copy ) : PTriangle<T,3>(copy) {

  _s      = copy._s;
  _q      = copy._q;
}


template <typename T>
PSubTriangle<T>::~PSubTriangle () {}



template <typename T>
inline
void PSubTriangle<T>::edit( SceneObject* d ) {
  //    cout << "Should I be here, and trying to edit SubTriangle in this way, via child : ";
  //    cout << d->getIdentity() << "\n";
}

template <typename T>
inline
void PSubTriangle<T>::edit( int selector ) {

  //    cout << "Should I be here, and trying to edit SubTriangle in this way, via selector <<"<<selector<<"\n";
  if( this->_parent )
    this->_parent->edit( this );

  this->replot();
}


template <typename T>
inline
void PSubTriangle<T>::eval( T u, T v, T w, int d ) const {

  if(d==0)         this->_p.setDim(1);
  else if(d==1)    this->_p.setDim(4);
  else if(d==2)    this->_p.setDim(10);
  else             this->_p.setDim(20); // d==3

  Point<T,2> h = u * _q[0] + v * _q[1] + w * _q[2];
  // Note that that the derivatives: h_u = _q[0],   h_v = _q[1],  h_w = _q[2]
  // and that all second derivatives, for example h_uu = 0 etc.

  DMatrix<Vector<T,3> > p = _s->evaluateParent(h,d);

  this->_p[0] = p[0][0];

  if(d>0)
  {            //      S_u       S_v
    this->_p[1] = dS(p[1][0], p[0][1], _q[0]); // dS(h_u)
    this->_p[2] = dS(p[1][0], p[0][1], _q[1]); // dS(h_v)
    this->_p[3] = dS(p[1][0], p[0][1], _q[2]); // dS(h_w)

    if (d>1)
    {
      Vector<T,3> s1 = dS(p[2][0], p[1][1], _q[0]);
      Vector<T,3> s2 = dS(p[1][1], p[0][2], _q[0]);
      this->_p[4] = dS(s1, s2, _q[0]);     // [dS_u(h_u),dS_v(h_u)](h_u) : S_uu
      this->_p[5] = dS(s1, s2, _q[1]);     // [dS_u(h_u),dS_v(h_u)](h_v) : S_uv
      this->_p[6] = dS(s1, s2, _q[2]);     // [dS_u(h_u),dS_v(h_u)](h_w) : S_uw

      s1 = dS(p[2][0], p[1][1], _q[1]);
      s2 = dS(p[1][1], p[0][2], _q[1]);
      this->_p[7] = dS(s1, s2, _q[1]);     // [dS_u(h_v),dS_v(h_v)](h_v) : S_vv
      this->_p[8] = dS(s1, s2, _q[2]);     // [dS_u(h_v),dS_v(h_v)](h_w) : S_vw
      this->_p[9] = dS2(p[2][0], p[1][1], p[0][2], _q[2]);// [dS_u(h_w),dS_v(h_w)](h_w) : S_ww
    }
    if (d>2)
    {
      Vector<T,3> s1 = dS(p[3][0], p[2][1], _q[0]); // u
      Vector<T,3> s2 = dS(p[2][1], p[1][2], _q[0]);
      Vector<T,3> s3 = dS(p[1][2], p[0][3], _q[0]);
      Vector<T,3> s4 = dS(s1, s2, _q[0]);           // u
      Vector<T,3> s5 = dS(s2, s3, _q[0]);
      this->_p[10] = dS(s4, s5, _q[0]);             // u    : S_uuu
      this->_p[11] = dS(s4, s5, _q[1]);             // v    : S_uuv
      this->_p[12] = dS(s4, s5, _q[2]);             // w    : S_uuw

      Vector<T,3> s6 =  dS(s1, s2, _q[1]);          // v
      Vector<T,3> s7 =  dS(s2, s3, _q[1]);
      this->_p[13] = dS(s6, s7, _q[1]);             // v    : S_uvv
      this->_p[14] = dS(s6, s7, _q[2]);             // w    : S_uvw
      this->_p[15] = dS2(s1, s2, s3, _q[2]);        // w    : S_uww

      Vector<T,3> s10 = dS(p[3][0], p[2][1], _q[1]);// v
      Vector<T,3> s11 = dS(p[2][1], p[1][2], _q[1]);
      Vector<T,3> s12 = dS(p[1][2], p[0][3], _q[1]);

      Vector<T,3> s13= dS(s10, s11, _q[1]);         // v
      Vector<T,3> s14= dS(s11, s12, _q[1]);
      this->_p[16] = dS(s13, s14, _q[1]);           // v    : S_vvv
      this->_p[17] = dS(s13, s14, _q[2]);           // w    : S_vvw
      this->_p[18] = dS2(s10, s11, s12, _q[2]);     // w    : S_vww

      Vector<T,3> s17 = dS(p[3][0], p[2][1], _q[2]);// w
      Vector<T,3> s18 = dS(p[2][1], p[1][2], _q[2]);
      Vector<T,3> s19 = dS(p[1][2], p[0][3], _q[2]);
      this->_p[19] = dS2(s17, s18, s19, _q[2]);     // w    : S_www
    }
  }
}



template <typename T>
inline
void PSubTriangle<T>::setBp( const Vector< Point<T,2>,3>& bp ) {

  _q = bp;
}

template <typename T>
inline
void PSubTriangle<T>::updateCoeffs( const Vector<T,3>& d ) {

  //Vector<GLfloat,3> diff(_matrix.inverse2()*d);
  this->translateParent(-d);
  //   cout << "Updated SubTriangle coeffs: " << d << "\n";
}


template <typename T>
Vector<Point<T,3>,3> PSubTriangle<T>::getPoints(){

  Vector<Point<T,3>,3> a;

  for (int i=0; i < 3; i++)
  {
    DMatrix<Vector<T,3> > m = _s->evaluateGlobal(_q[i] , 0);
    a[i] = m[0][0];
  }
  return a;
}


template <typename T>
inline
Vector<Point<T,2>,3>  PSubTriangle<T>::getParPoints(){

  return _q;
}


template <typename T>
inline
Vector<T,3> PSubTriangle<T>::dS(const Vector<T,3>& Su, const Vector<T,3>& Sv, const Point<T,2>& p) const
{
    return Su*p(0) + Sv*p(1);
}

template <typename T>
inline
Vector<T,3> PSubTriangle<T>::dS2(const Vector<T,3>& suu, const Vector<T,3>& suv, const Vector<T,3>& svv, const Point<T,2>& p) const
{
    return suu*(p(0)*p(0))+ suv*(2*p(0)*p(1)) + svv*(p(1)*p(1));
}


} // END namespace GMlib
