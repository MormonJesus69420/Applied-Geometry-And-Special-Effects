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




// local
#include "visualizers/gmptriangledefaultvisualizer.h"



namespace GMlib {

  template <typename T, int n>
  PTriangle<T,n>::PTriangle( int samples ) {

    _d      = -1;
    _no_sam = samples;
    _init();
    setEval(0);

    _default_visualizer = nullptr;
  }

  template <typename T, int n>
  PTriangle<T,n>::PTriangle( const PTriangle<T,n>& copy ) {

    _p   = copy._p;
    _n   = copy._n;
    _u   = copy._u;
    _v   = copy._v;
    _d   = copy._d;
    _all = copy._all;

    for( int i = 0; i < 4; ++i ) _pt[i] = copy._pt[i];

    _default_d    = copy._default_d;

    _default_visualizer = nullptr;
  }

  template <typename T, int n>
  PTriangle<T,n>::~PTriangle() {

    enableDefaultVisualizer( false );
    if( _default_visualizer )
      delete _default_visualizer;
  }

  template <typename T, int n>
  inline
  void PTriangle<T,n>::_eval( T u, T v, int d ) const
  {
    if(!(u == _u && v == _v && d <= _d))
    {
      _u = u;
      _v = v;
      _d = d;
      eval(u, v, 1-u-v, d);
    }
  }


  template <typename T, int n>
  inline
  int PTriangle<T,n>::_sum( int i ) const {

    int r = 0;
    for(; i>0; i--) r += i;
    return r;
  }

  template <typename T, int n>
  void PTriangle<T,n>::enableDefaultVisualizer( bool enable ) {

    if( !enable )
      removeVisualizer( _default_visualizer );
    else {

      if( !_default_visualizer )
        _default_visualizer = new PTriangleDefaultVisualizer<T,n>();

      insertVisualizer( _default_visualizer );
    }
  }

  template <typename T, int n>
  inline
  const PTriangleVisualizer<T, n>* PTriangle<T,n>::getDefaultVisualizer() const {

    return _default_visualizer;
  }

  template <typename T, int n>
  inline
  const DVector<Vector<T,n> >& PTriangle<T,n>::evaluateGlobal( T u, T v, int d  ) const
  {
    static DVector<Vector<T,n> > p;

    _eval(u,v, d);
    p.setDim( _p.getDim() );

    p[0] = this->_present * static_cast< Point<T,n> >(_p[0]);

    for( int j = 1; j < p.getDim(); j++ )
      p[j] = this->_present * _p[j]; //  They are Vector<T,n> as default

    return p;
  }


  template <typename T, int n>
  inline
  const DVector<Vector<T,n> >& PTriangle<T,n>::evaluate( T u, T v, int d ) const
  {
    _eval(u, v, d);
    return _p;
  }


  template <typename T, int n>
  inline
  const DVector<Vector<T,n> >& PTriangle<T,n>::evaluateParent( T u, T v, int d ) const
  {
    static DVector<Vector<T,n> > p;

    _eval(u, v, d);
    p.setDim( _p.getDim() );

    p[0] = this->_matrix * static_cast< Point<T,n> >(_p[0]);

    for( int j = 1; j < p.getDim(); j++ )
      p[j] = this->_matrix * _p[j]; //  They are Vector<T,n> as default

    return p;
  }


  template <typename T, int n>
  void PTriangle<T,n>::_fuForm( T u, T v, T& E, T& F, T& G, T& e, T& f, T& g) const
  {
    _eval( u, v, 2);
    Vector<T,n>     du = _p[2]-_p[1];
    Vector<T,n>     dv = _p[3]-_p[1];
    UnitVector<T,n> N  = du^dv;

    Vector<T,n>     duu = _p[6] - 2*_p[9] + _p[4];
    Vector<T,n>     duv = getDerUV(u,v);
    Vector<T,n>     dvv = _p[5] - 2*_p[8] + _p[4];

    E = du*du;
    F = du*dv;
    G = dv*dv;
    e = N*duu;
    f = N*duv;
    g = N*dvv;
  }



  template <typename T, int n>
  T PTriangle<T,n>::getCurvatureGauss( T u, T v ) const
  {
     T E, F, G, e, f, g;

     _fuForm( u, v, E, F, G, e, f, g);

     return (e*g - f*f)/(E*G-F*F);
  }


  template <typename T, int n>
  T PTriangle<T,n>::getCurvatureMean( T u, T v ) const
  {
     T E, F, G, e, f, g;

     _fuForm( u, v, E, F, G, e, f, g);

     return 0.5*(e*G-2*(f*F)+g*E)/(E*G-F*F);
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerU( T u, T v ) const
  {
    _eval(u,v,1);
    return _p[1];
  }

  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerV( T u, T v ) const
  {
    _eval(u,v,1);
    return _p[2];
  }

  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerW( T u, T v ) const
  {
    _eval(u,v,1);
    return _p[3];
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerUU( T u, T v ) const {

    _eval(u,v,2);
    return _p[4];
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerUV( T u, T v ) const {

    _eval(u,v,2);
    return _p[5];
  }

  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerUW( T u, T v ) const {

    _eval(u,v,2);
    return _p[6];
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerVV( T u, T v ) const {

    _eval(u,v,2);
    return _p[7];
  }

  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerVW( T u, T v ) const {

    _eval(u,v,2);
    return _p[8];
  }

  template <typename T, int n>
  inline
  const Vector<T,n>& PTriangle<T,n>::getDerWW( T u, T v ) const {

    _eval(u,v,2);
    return _p[9];
  }



//  template <typename T, int n>
//  std::string PTriangle<T,n>::getIdentity() const {

//    return "PTriangle";
//  }


  template <typename T, int n>
  const Vector<T,n>& PTriangle<T,n>::getNormal( T u, T v ) const {

    _eval(u, v, 1);
    _n = Vector<T,n>(_p[2]-_p[1])^(_p[3]-_p[1]);
    return _n;
  }


  template <typename T, int n>
  UnitVector<T,n> PTriangle<T,n>::getUnitNormal( T u, T v ) const {

    return getNormal( u, v );
  }

  template <typename T, int n>
  inline
  void PTriangle<T,n>::insertVisualizer( Visualizer *visualizer ) {

    Parametrics<T,2,n>::insertVisualizer( visualizer );

    PTriangleVisualizer<T,n> *visu = dynamic_cast<PTriangleVisualizer<T,n>*>( visualizer );

    if( !visu )
      return;

    if( _ptriangle_visualizers.exist( visu ) )
      return;

    _ptriangle_visualizers += visu;


  }

  template <typename T, int n>
  inline
  void PTriangle<T,n>::removeVisualizer( Visualizer *visualizer ) {

    PTriangleVisualizer<T,n> *visu = dynamic_cast<PTriangleVisualizer<T,n>*>( visualizer );
    if( visu )
      _ptriangle_visualizers.remove( visu );

    Parametrics<T,2,n>::removeVisualizer( visualizer );
  }


  template <typename T, int n>
  bool PTriangle<T,n>::isClosestPoint( const Point<T,n>& q, T& u, T& v ) const {

    T a11,a12,a21,a22,b1,b2;
    T du,dv,det;

    HqMatrix<float,3> invmat = this->_present;
    invmat.invertOrthoNormal();
    Point<T,n> p = (invmat * q).template toType<T>();  // Egentlig _present

    for(int i=0; i<20;i++) {

      _eval( u, v, 1 );

      Vector<T,n> d = p - _p[0];

      a11 = d*(_p[7]-2*_p[5]+_p[4])-(_p[2]-_p[1])*(_p[2]-_p[1]);
      a12 = d*(_p[8]-_p[6]-_p[5]+_p[4])-(_p[2]-_p[1])*(_p[3]-_p[1]);
      a21 = a12;
      a22 = d*(_p[9]-2*_p[6]+_p[4])-(_p[3]-_p[1])*(_p[3]-_p[1]);

      b1  = -(d*(_p[2]-_p[1]));
      b2  = -(d*(_p[3]-_p[1]));

      det = a11*a22-a12*a21;
      du  = (b1*a22-a12*b2)/det;
      dv  = (a11*b2-b1*a21)/det;
      if(fabs(du)< 1e-6 && fabs(dv) < 1e-6) break;
      u += du;
      v += dv;
    }
    return true;
  }

  template <typename T, int n>
  inline
  void PTriangle<T,n>::replot( int m, int d )
  {
    if( m < 2 )        m = _no_sam;
    else         _no_sam = m;

    // Sample Positions and related Derivatives
    DVector< DVector< Vector<T,n> > > p;
    resample( p, m, d );

    setSurroundingSphere( p );

    // Replot Visaulizers
    for( int i = 0; i < this->_ptriangle_visualizers.getSize(); i++ )
      this->_ptriangle_visualizers[i]->replot( p, m);
  }


  template <typename T, int n>
  inline
  void PTriangle<T,n>::setTriangNr(bool all, int nr)
  {
      _all  = all;
      _t_nr = nr;
  }


  template <typename T, int n>
  inline
  void PTriangle<T,n>::resample( DVector< DVector < Vector<T,n> > >& p, int m, int d )
  {
    if(_all) resample1(p, m, d);
    else
    {
        if (_t_nr == 1) resample2(p, m, 1, 2);
        if (_t_nr == 2) resample2(p, m, 2, 3);
        if (_t_nr == 3) resample2(p, m, 3, 1);
    }
  }


  template <typename T, int n>
  inline
  void PTriangle<T,n>::resample1( DVector< DVector < Vector<T,n> > >& p, int m, int d )
  {
    T u,v,du = T(1)/(m-1);
    p.setDim(_sum(m));
    int i,j,k;
    for(k=0,i=0; i<m; i++) {
      for( j=0; j<=i; j++ )
      {
        v = j*du;
        u = (i-j)*du;
        eval(u, v, 1-u-v, d);
        p[k++] = _p;
      }
    }
  }


  template <typename T, int n>
  inline
  void PTriangle<T,n>::resample2( DVector< DVector < Vector<T,n> > >& p, int m, int a, int b)
  {
    T u,v,w,du = T(1)/(m-1);
    p.setDim(_sum(m));
    int i,j,k;
    for(k=0,i=0; i<m; i++) {
      for( j=0; j<=i; j++ )
      {
        v = j*du;
        u = i*du;
        w = 1 - u;
        u -= v;
        Point<T,n> pr = u*_pt[0] + v*_pt[a] + w*_pt[b];
        eval(pr, 1);
        p[k++] = _p;
      }
    }
  }


  template <typename T, int n>
  inline
  void PTriangle<T,n>::_init() {
    T t= 1/3.0;
    _all = true;
    _pt[0] = Point<T,n>(t, t, t);
    _pt[1] = Point<T,n>(T(1), T(0), T(0));
    _pt[2] = Point<T,n>(T(0), T(1), T(0));
    _pt[3] = Point<T,n>(T(0), T(0), T(1));
  }


  template <typename T, int n>
  inline
  void PTriangle<T,n>::setEval( int d ) {

    _default_d = d;
  }


  template <typename T, int n>
  inline
  void PTriangle<T,n>::setSurroundingSphere( const DVector<DVector<Vector<T,n> > >& p ) {

    Sphere<float,3>  s( p(0)(0) );

    s += p( p.getDim()-1)(0);
    for( int i = 1; i < p.getDim() - 1; i++ )
      s += p(i)(0);

    Parametrics<T,2,n>::setSurroundingSphere(s.template toType<float>());
  }


  template <typename T, int n>
  inline
  Parametrics<T,2,n>* PTriangle<T,n>::split( T t, int uv ) {

    return 0x0;
  }

  template <typename T, int n>
  void PTriangle<T,n>::toggleDefaultVisualizer() {

    if( !_ptriangle_visualizers.exist( _default_visualizer ) )
      enableDefaultVisualizer( true );
    else
      enableDefaultVisualizer( false );
  }

  template <typename T, int n>
  inline
  void PTriangle<T,n>::updateCoeffs( const Vector<T,n>& /*d*/ ) {}


  template <typename T, int n>
  inline
  const Point<T,n>& PTriangle<T,n>::operator()( T u, T v ) {

    _eval(u,v);
    return static_cast< Point<T,n> >( _p[0][0] );
  }

  template <typename T, int n>
  inline
  void PTriangle<T,n>::eval(const Point<T,3> &p, int d) const { eval(p(0),p(1),p(2),d); }

  template <typename T, int n>
  inline
  const DVector<Vector<T,n> >&  PTriangle<T,n>::evaluateGlobal(const Point<T,3> & p, int d) const
  {
      return  evaluateGlobal(p(0), p(1), d);
  }

  template <typename T, int n>
  inline
  const DVector<Vector<T,n> >&  PTriangle<T,n>::evaluate(const Point<T,3> & p, int d) const
  {
      return  evaluate(p(0), p(1), d);
  }

  template <typename T, int n>
  inline
  const DVector<Vector<T,n> >&  PTriangle<T,n>::evaluateParent(const Point<T,3> & p, int d) const
  {
      return  evaluateParent(p(0), p(1), d);
  }

  template <typename T, int n>
  inline
  const Point<T,n>&             PTriangle<T,n>::operator()(const Point<T,3> & p)
  {
      return  operator()(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerU(const Point<T,3> & p) const
  {
      return  getDerU(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerV(const Point<T,3> & p) const
  {
      return  getDerV(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerW(const Point<T,3> & p) const
  {
      return  getDerW(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerUU(const Point<T,3> & p) const
  {
      return  getDerUU(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerUV(const Point<T,3> & p) const
  {
      return  getDerUV(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerUW(const Point<T,3> & p) const
  {
      return  getDerUW(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerVV(const Point<T,3> & p) const
  {
      return  getDerVV(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerVW(const Point<T,3> & p) const
  {
      return  getDerVW(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDerWW(const Point<T,3> & p) const
  {
      return  getDerWW(p(0), p(1));
  }

  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getNormal(const Point<T,3> & p) const
  {
      return  getNormal(p(0), p(1));
  }

  template <typename T, int n>
  inline
  UnitVector<T,n>               PTriangle<T,n>::getUnitNormal(const Point<T,3> & p) const
  {
      return  getUnitNormal(p(0), p(1));
  }

  template <typename T, int n>
  inline
  T                             PTriangle<T,n>::getCurvatureGauss(const Point<T,3> & p) const
  {
      return  getCurvatureGauss(p(0), p(1));
  }

  template <typename T, int n>
  inline
  T                             PTriangle<T,n>::getCurvatureMean(const Point<T,3> & p) const
  {
      return  getCurvatureMean(p(0), p(1));
  }


  template <typename T, int n>
  inline
  const Vector<T,n>&            PTriangle<T,n>::getDer_d(const Point<T,3> & p, const Vector<T,3> & d) const
  {
      eval(p, 1);
      return d[0]*_p[1] + d[1]*_p[2] + d[2]*_p[3];
  }


  template <typename T, int n>
  Vector<Point<T,n>,3>          PTriangle<T,n>::getPoints() const
  {
      Vector<Point<T,n>,3> a;

      return a;
  }

} // END namespace GMlib
