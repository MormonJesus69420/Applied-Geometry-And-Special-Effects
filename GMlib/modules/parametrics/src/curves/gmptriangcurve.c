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




#include "../evaluators/gmevaluatorstatic.h"

namespace GMlib {


template <typename T>
inline
PTriangCurve<T>::PTriangCurve(  PTriangle<T,3>* s,  const Point<T,3>& p1,   const Point<T,3>& p2 )
{
  _der_curve = false;
  _plot = false;
  _s  = s;
  _p1 = p1;
  _p2 = p2;
}


template <typename T>
inline
PTriangCurve<T>::PTriangCurve(  PTriangle<T,3>* s,  const Point<T,3>& p1,   const Point<T,3>& p2,
                                const Vector<T,3>& v1,  const Vector<T,3>& v2 )
{
  _der_curve = true;
  _plot = false;

  _s  = s;
  _p1 = p1;
  _p2 = p2;
  _v1 = v1;
  _v2 = v2;
}


template <typename T>
inline
PTriangCurve<T>::PTriangCurve( const PTriangCurve<T>& copy ) : PCurve<T,3>( copy ) {

  _der_curve = copy._der_curve;
  _plot = false;

  _s  = copy._s;
  _p1 = copy._p1;
  _p2 = copy._p2;
  _v1 = copy._v1;
  _v2 = copy._v2;
}


template <typename T>
PTriangCurve<T>::~PTriangCurve() {}



template <typename T>
inline
void PTriangCurve<T>::eval( T t, int d, bool /*l*/ ) const
{
  if (_der_curve)
    eval2(t,d);
  else
    eval1(t,d);
}




template <typename T>
inline
void PTriangCurve<T>::eval1( T t, int d ) const
{
  this->_p.setDim( d + 1 );

  Vector<T,3> dh = _p2 - _p1;
  Vector<T,3>  h = _p1 + t*dh;

  DVector<Vector<T,3> > m = _s->evaluateParent(h, d);

  this->_p[0] = m[0];
  if(d>0)
  {
    this->_p[1] = dS(m[1], m[2], m[3], dh);

    if(d>1)
    {
      Vector<T,3> v1 = dS(m[4], m[5], m[6], dh);
      Vector<T,3> v2 = dS(m[5], m[7], m[8], dh);
      Vector<T,3> v3 = dS(m[6], m[8], m[9], dh);
      this->_p[2]    = dS(v1, v2, v3, dh);
    }
  }
}



template <typename T>
inline
void PTriangCurve<T>::eval2( T t, int d ) const
{
  this->_p.setDim( d + 1 );
  double tt = _basis(t);

  Vector<T,3> dh  = _p2 - _p1;
  Vector<T,3> h   = _p1 + t*dh;
  Vector<T,3> v   = _v1 + tt*(_v2 - _v1);

  DVector<Vector<T,3> > m = _s->evaluateParent(h, d+1);

  this->_p[0] = dS(m[1], m[2], m[3], v);
  if(d>0)
  {
    Vector<T,3> dv = _basis.getDer1()*(_v2 - _v1);
    Vector<T,3> v1 = dS(m[4], m[5], m[6], dh);
    Vector<T,3> v2 = dS(m[5], m[7], m[8], dh);
    Vector<T,3> v3 = dS(m[6], m[8], m[9], dh);
    this->_p[1] = dS(v1, v2, v3, v) + dS(m[1], m[2], m[3], dv);

    if(d>1)
    {
      Vector<T,3> d2v = _basis.getDer2()*(_v2 - _v1);
      Vector<T,3> v0 = dS(v1, v2, v3, dv);  // d(dS(dh))(dv)
      Vector<T,3> v4,v5,v6;
      //  [S_uuu  S_uuv  S_uuw]
      v1 = dS(m[10], m[11], m[12], dh);
      //  [S_uuv  S_uvv  S_uvw]
      v2 = dS(m[11], m[13], m[14], dh);
      //  [S_uuw  S_uvw  S_uww]
      v3 = dS(m[12], m[14], m[15], dh);
      //  [S_uvv  S_vvv  S_vvw]
      v4 = dS(m[13], m[16], m[17], dh);
      //  [S_uvw  S_vvw  S_vww]
      v5 = dS(m[14], m[17], m[18], dh);
      //  [S_uww  S_vww  S_www]
      v6 = dS(m[15], m[18], m[19], dh);

      Vector<T,3> d1 = dS(v1, v2, v3, dh);  // d(dS_u(dh))(v)
      Vector<T,3> d2 = dS(v2, v4, v5, dh);  // d(dS_v(dh))(v)
      Vector<T,3> d3 = dS(v3, v5, v6, dh);  // d(dS_w(dh))(v)
      Vector<T,3> d4 = dS(d1, d2, d3, v) + 2*v0 + dS(m[1], m[2], m[3], d2v);

      this->_p[2] = d4;
    }
  }
}



template <typename T>
inline
void PTriangCurve<T>::eval123( T t)
{
  this->_p.setDim(6);
  double tt = _basis(t);
  double dt = _basis.getDer1();
  double d2t= _basis.getDer2();

  Vector<T,3> dh = _p2 - _p1;
  Vector<T,3> h  = _p1 + t*dh;
  Vector<T,3> v  = _v1 + tt*(_v2 - _v1);
  Vector<T,3> dv = dt*(_v2 - _v1);
  Vector<T,3> d2v = d2t*(_v2 - _v1);

  DVector<Vector<T,3> > m = _s->evaluateParent(h, 3);

  Vector<T,3> v1 = dS(m[4], m[5], m[6], dh); // uu, uv, uw
  Vector<T,3> v2 = dS(m[5], m[7], m[8], dh); // uv, vv, vw
  Vector<T,3> v3 = dS(m[6], m[8], m[9], dh); // uw, vw, ww
  Vector<T,3> v0 = dS(v1, v2, v3, dv);         // d(dS(dh))(dv)
  Vector<T,3> v4,v5,v6;

  this->_p[0] = m[0];
  this->_p[2] = dS(m[1], m[2], m[3], dh);
  this->_p[4] = dS(v1, v2, v3, dh);

  this->_p[1] = dS(m[1], m[2], m[3], v);
  this->_p[3] = dS(v1, v2, v3, v) + dS(m[1], m[2], m[3], dv);
  //  [S_uuu  S_uuv  S_uuw]
  v1 = dS(m[10], m[11], m[12], dh);
  //  [S_uuv  S_uvv  S_uvw]
  v2 = dS(m[11], m[13], m[14], dh);
  //  [S_uuw  S_uvw  S_uww]
  v3 = dS(m[12], m[14], m[15], dh);
  //  [S_uvv  S_vvv  S_vvw]
  v4 = dS(m[13], m[16], m[17], dh);
  //  [S_uvw  S_vvw  S_vww]
  v5 = dS(m[14], m[17], m[18], dh);
  //  [S_uww  S_vww  S_www]
  v6 = dS(m[15], m[18], m[19], dh);
  Vector<T,3> d1 = dS(v1, v2, v3, dh);  // d(dS_u(dh))(dh)
  Vector<T,3> d2 = dS(v2, v4, v5, dh);  // d(dS_v(dh))(dh)
  Vector<T,3> d3 = dS(v3, v5, v6, dh);  // d(dS_w(dh))(dh)

  this->_p[5] = dS(d1, d2, d3, v) + 2*v0 + dS(m[1], m[2], m[3], d2v);
}



template <typename T>
inline
void PTriangCurve<T>::eval12( T t)
{
  this->_p.setDim(4);
  double tt = _basis(t);

  Vector<T,3> dh = _p2 - _p1;
  Vector<T,3> h  = _p1 + t*dh;
  Vector<T,3> v  = _v1 + tt*(_v2 - _v1);
  Vector<T,3> dv = _basis.getDer1()*(_v2 - _v1);

  DVector<Vector<T,3> > m = _s->evaluateParent(h, 2);

  Vector<T,3> v1 = dS(m[4], m[5], m[6], dh);
  Vector<T,3> v2 = dS(m[5], m[7], m[8], dh);
  Vector<T,3> v3 = dS(m[6], m[8], m[9], dh);

  this->_p[0] = m[0];
  this->_p[2] = dS(m[1], m[2], m[3], dh);

  this->_p[1] = dS(m[1], m[2], m[3], v);
  this->_p[3] = dS(v1, v2, v3, v) + dS(m[1], m[2], m[3], dv);
}




template <typename T>
T PTriangCurve<T>::getStartP() const
{
  return T(0);
}


template <typename T>
T PTriangCurve<T>::getEndP() const
{
  return T(1);
}


template <typename T>
inline
bool PTriangCurve<T>::isClosed() const
{
  return false;
}


template <typename T>
inline
void PTriangCurve<T>::togglePlot()
{
  _plot = !_plot;
}



template <typename T>
void PTriangCurve<T>::resample( DVector< DVector< Vector<T, 3> > >& p, int m, int d, T start, T end )
{
  if (_der_curve && _plot)
  {
    T du = (end-start)/(m-1);
    p.setDim(m);

    for( int i = 0; i < m - 1; i++ )
    {
      p[i].setDim(d+2);
      eval1(start + i * du, 0);
      p[i][0] = this->_p[0];
      eval2(start + i * du, d);
      for(int j=0; j<=d;j++)
        p[i][j+1] = this->_p[j];
    }
    p[m-1].setDim(d+2);
    eval1(end, 0);
    p[m-1][0] = this->_p[0];
    eval2(end, d);
    for(int j=0; j<=d;j++)
      p[m-1][j+1] = this->_p[j];

    switch( this->_dm )
    {
    case GM_DERIVATION_EXPLICIT:
      // Do nothing, evaluator algorithms for explicite calculation of derivatives
      // should be defined in the eval( ... ) function enclosed by
      // if( this->_derivation_method == this->EXPLICIT ) { ... eval algorithms for derivatives ... }
      break;

    case GM_DERIVATION_DD:
    default:
      //  _evalDerDD( p, d, du );
      break;
    };
  }
  else
    PCurve<T,3>::resample(  p, m, d, start, end );
}




template <typename T>
DVector<DVector<Vector<T,3> > >& PTriangCurve<T>::getSample3(int m)
{
  static DVector<DVector<Vector<T,3> > > mat;

  T du = T(1)/(m-1);
  mat.setDim(m);

  for(int i=0; i<m; i++)
  {
    eval123(i*du);
    mat[i] = this->_p;
  }
  return mat;
}


template <typename T>
inline
Vector<T,3> PTriangCurve<T>::dS(Vector<T,3>& Su, Vector<T,3>& Sv, Vector<T,3>& Sw, Vector<T,3>& h) const
{
  return Su*h[0] + Sv*h[1] + Sw*h[2];
}

} // END namespace GMlib

