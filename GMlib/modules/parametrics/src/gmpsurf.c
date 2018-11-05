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





// gmlib
#include "visualizers/gmpsurfdefaultvisualizer.h"

#include <core/utils/gmdivideddifferences.h>


// stl
#include <cmath>
#include <sstream>
#include <iomanip>

namespace GMlib {

    //*************************************
    //***  Constructors and destructors  **
    //*************************************

  template <typename T, int n>
  inline
  PSurf<T,n>::PSurf( int s1, int s2 ) {

    _no_sam_u                       = s1;
    _no_sam_v                       = s2;
    _no_der_u                       = 1;
    _no_der_v                       = 1;
    _d1                             = -1;
    _d2                             = -1;
    _tr_u                           = T(0);
    _sc_u                           = T(1);
    _tr_v                           = T(0);
    _sc_v                           = T(1);
    _resample                       = false;

    setNoDer( 2 );

    _default_visualizer = 0x0;
  }


  template <typename T, int n>
  inline
  PSurf<T,n>::PSurf( const PSurf<T,n>& copy ) : Parametrics<T,2,n>( copy ) {

    _p            = copy._p;
    _u            = copy._u;
    _v            = copy._v;
    _d1           = copy._d1;
    _d2           = copy._d2;
    _tr_u         = copy._tr_u;
    _sc_u         = copy._sc_u;
    _tr_v         = copy._tr_v;
    _sc_v         = copy._sc_v;
    _sam_p_u      = copy._sam_p_u;
    _sam_p_v      = copy._sam_p_v;
    _no_sam_p_u   = copy._no_sam_p_u;
    _no_sam_p_v   = copy._no_sam_p_v;
    _default_d    = copy._default_d;

    _no_sam_u     = copy._no_sam_u;
    _no_sam_v     = copy._no_sam_v;
    _no_der_u     = copy._no_sam_u;
    _no_der_v     = copy._no_sam_v;

    _resample     = false;

    _default_visualizer = 0x0;
  }

  template <typename T, int n>
  PSurf<T,n>::~PSurf() {

    enableDefaultVisualizer( false );
    if( _default_visualizer )
      delete _default_visualizer;
  }







  //******************************************************
  //      public evaluate functons for PSurf           **
  //******************************************************



  template <typename T, int n>
  inline
  DMatrix<Vector<T,n>>& PSurf<T,n>::evaluate( T u, T v, int d1, int d2 ) const {

    _eval(u, v, d1, d2);
    return _p;
  }



  template <typename T, int n>
  inline
  DMatrix<Vector<T,n>>& PSurf<T,n>::evaluateParent( T u, T v, int d1, int d2 ) const {

    _eval(u,v,d1,d2);
    p.setDim( _p.getDim1(), _p.getDim2() );
    _mat = this->_matrix;

    p[0][0] = _mat * _p[0][0].toPoint();
    for( int j = 1; j < p.getDim2(); j++ )
      p[0][j] = _mat * _p[0][j];
    for( int i = 1; i < p.getDim1(); i++ )
      for( int j = 0; j < p.getDim2(); j++ )
        p[i][j] = _mat * _p[i][j];

    return p;
  }




  template <typename T, int n>
  inline
  DMatrix<Vector<T,n>>& PSurf<T,n>::evaluateGlobal( T u, T v, int d1, int d2 ) const {

    _eval(u,v,d1,d2);
    p.setDim( _p.getDim1(), _p.getDim2() );
    _mat = this->_present;

    p[0][0] = _mat * _p[0][0].toPoint();
    for( int j = 1; j < p.getDim2(); j++ )
      p[0][j] = _mat * _p[0][j];
    for( int i = 1; i < p.getDim1(); i++ )
      for( int j = 0; j < p.getDim2(); j++ )
        p[i][j] = _mat * _p[i][j];

    return p;
  }




  template <typename T, int n>
  inline
  DVector<Vector<T,n>>& PSurf<T,n>::evaluateD( T u, T v, int d ) const {
    // Here we are copy  the matrix diagonally into a vector
    static DVector<Vector<T,n> > p;

    _eval(u, v, d, d);
    p.setDim((d*d+3*d+2)/2);

    for(int i = 0, k=0; i <= d; i++)
      for(int j = 0; j <= i; j++)
        p[k++] = _p[i-j][j];

    return p;
  }




  template <typename T, int n>
  inline
  DMatrix<Vector<T,n>>& PSurf<T,n>::evaluate( const Point<T,2>& p, const Point<int,2>& d ) const {

    _eval( p(0), p(1), d(0), d(1) );
    return _p;
  }



  template <typename T, int n>
  inline
  DMatrix<Vector<T,n>>& PSurf<T,n>::evaluateParent( const Point<T,2>& p, const Point<int,2>& d ) const {

    return evaluateParent( p(0), p(1), d(0), d(1) );
  }



  template <typename T, int n>
  inline
  DMatrix<Vector<T,n>>& PSurf<T,n>::evaluateGlobal( const Point<T,2>& p, const Point<int,2>& d ) const {

    return evaluateGlobal( p(0), p(1), d(0), d(1) );
  }


  template <typename T, int n>
  inline
  DVector<Vector<T,n>>& PSurf<T,n>::evaluateD( const Point<T,2>& p, int d ) const {

    return evaluateD( p(0), p(1), d );
  }



  template <typename T, int n>
  inline
  DMatrix<Vector<T,n>>& PSurf<T,n>::evaluate( int i, int j ) const {

    return _pre_val[i][j];
  }



  template <typename T, int n>
  inline
  DMatrix<Vector<T,n>>& PSurf<T,n>::evaluateParent(  int i, int j  ) const {

    static DMatrix<Vector<T,n> > p;
    DMatrix<Vector<T,n>>& q = _pre_val[i][j];
    int k1 = q.getDim1();
    int k2 = q.getDim2();
    p.setDim(k1, k2);

    p[0][0] = _mat * q[0][0].toPoint();
    for( int j = 1; j < k2; j++ )
      p[0][j] = _mat * q[0][j];
    for( int i = 1; i < k1; i++ )
      for( int j = 0; j < k2; j++ )
        p[i][j] = _mat * q[i][j];

    return p;
  }




  //******************************************************
  //      public closest point functions                **
  //******************************************************


  template <typename T, int n>
  inline
  void PSurf<T,n>::estimateClpPar( const Point<T,n>& p, T& u, T& v, int m ) const {

    T su = getParStartU();
    T sv = getParStartV();
    T du = getParDeltaU()/(m-1);
    T dv = getParDeltaV()/(m-1);

    Vector<T,n> q = getPosition(su, sv);
    T min = (p-q).getLength();
    u = su; v = sv;

    for(int i=0; i<m; i++) {
      for(int j=0; j<m; j++) {
        if(!(i==0 && j==0)) {
          q = getPosition(su+i*du, sv+j*dv);
          T mn = (p-q).getLength();
          if (mn < min) {
            min = mn;
            u = su + i*du;
            v = sv + j*dv;
          }
        }
      }
    }
  }




  template <typename T, int n>
  bool PSurf<T,n>::getClosestPoint( const Point<T,n>& q, T& u, T& v, double eps, int max_iterations ) const {

    T a11, a12, a21, a22, b1, b2;
    T du, dv, det;

    HqMatrix<T,n> invmat = this->_present;
    invmat.invertOrthoNormal();
    Point<T,n> p = invmat * q;

    for(int i = 0; i < max_iterations; i++ ) {

      DMatrix< Vector<T,n>>& r = evaluate( u, v, 2, 2 );
      Vector<T,n> d = p-r[0][0];

      a11 =       d*r[2][0] - r[1][0] * r[1][0];
      a21 = a12 = d*r[1][1] - r[1][0] * r[0][1];
      a22 =       d*r[0][2] - r[0][1] * r[0][1];

      b1  = -(d*r[1][0]);
      b2  = -(d*r[0][1]);
      det = a11*a22 - a12*a21;

      du  = (b1*a22 - a12*b2) / det;
      dv  = (a11*b2 - b1*a21) / det;
      u   += du;
      v   += dv;

      if(std::abs(du) < eps && std::abs(dv) < eps)
        return true;
    }
    return false;
  }



  template <typename T, int n>
  inline
  bool PSurf<T,n>::getClosestPoint( const Point<T,n>& q, Point<T,2>& uv, double eps, int max_iterations ) const {
    return getClosestPoint(q, uv[0], uv[1], eps, max_iterations);
  }





  //**************************************************
  //      public curvature functions                **
  //**************************************************


  template <typename T, int n>
  T PSurf<T,n>::getCurvatureGauss( T u, T v ) const {

    T E, F, G, e, f, g;
    _computeEFGefg( u, v, E, F, G, e, f, g );

    return (e*g - f*f) / (E*G - F*F);
  }


  template <typename T, int n>
  T PSurf<T,n>::getCurvatureMean( T u, T v ) const {

      T E, F, G, e, f, g;
      _computeEFGefg( u, v, E, F, G, e, f, g );

    return 0.5 * (e*G - 2 * (f*F) + g*E) / (E*G - F*F);
  }


  template <typename T, int n>
  T PSurf<T,n>::getCurvaturePrincipalMax( T u, T v ) const {

    T K = getCurvatureGauss( u, v );
    T H = getCurvatureMean( u, v );

    return H + sqrt( H*H - K );
  }


  template <typename T, int n>
  T PSurf<T,n>::getCurvaturePrincipalMin( T u, T v ) const {

    T K = getCurvatureGauss( u, v );
    T H = getCurvatureMean( u, v );

    return H - sqrt( H*H - K );
  }



  //***********************************************************
  //   To see the number of derivatives in pre-evaluation
  //***********************************************************


  template <typename T, int n>
  inline
  int PSurf<T,n>::getDerivativesU() const {

    return _no_der_u;
  }


  template <typename T, int n>
  inline
  int PSurf<T,n>::getDerivativesV() const {

    return _no_der_v;
  }



  //****************************************************************
  //   To get the position or given derivatives at a given (u,v)
  //****************************************************************


  template <typename T, int n>
  inline
  const Point<T,n>& PSurf<T,n>::operator () ( T u, T v ) const {

    _eval(u, v, _default_d, _default_d);
    return _p[0][0].toPoint();
  }


  template <typename T, int n>
  inline
  const Point<T,n>& PSurf<T,n>::getPosition( T u, T v ) const {

    _eval(u, v, 0, 0);
    return _p[0][0];
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PSurf<T,n>::getDerU( T u, T v ) const {

    _eval(u, v, 1, 0);
    return _p(1)(0);
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PSurf<T,n>::getDerUU( T u, T v ) const {

    _eval(u, v, 2, 0);
    return _p(2)(0);
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PSurf<T,n>::getDerUV( T u, T v ) const {

    _eval(u, v, 2, 2);
    return _p(1)(1);
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PSurf<T,n>::getDerV( T u, T v ) const {

    _eval(u, v, 0, 1);
    return _p(0)(1);
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PSurf<T,n>::getDerVV( T u, T v ) const {

    _eval(u, v, 0, 2);
    return _p(0)(2);
  }


  template <typename T, int n>
  inline
  const Vector<T,n>& PSurf<T,n>::getNormal() const {

    return _n = _p(1)(0)^_p(0)(1);
  }



  //***********************************************************
  //   To get the desired domain of the surface
  //***********************************************************


  template <typename T, int n>
  inline
  T PSurf<T,n>::getParStartU() const {

    return getStartPU() + _tr_u;
  }


  template <typename T, int n>
  inline
  T PSurf<T,n>::getParEndU() const {

    return getParStartU() + getParDeltaU();
  }


  template <typename T, int n>
  inline
  T PSurf<T,n>::getParDeltaU() const {

    return _sc_u * (getEndPU() - getStartPU());
  }


  template <typename T, int n>
  inline
  T PSurf<T,n>::getParStartV() const {

    return getStartPV() + _tr_v;
  }


  template <typename T, int n>
  inline
  T PSurf<T,n>::getParEndV() const {

    return getParStartV() + getParDeltaV();
  }


  template <typename T, int n>
  inline
  T PSurf<T,n>::getParDeltaV() const {

    return _sc_v * (getEndPV() - getStartPV());
  }




  //***********************************************************
  //   To get the sample data
  //***********************************************************


  template <typename T, int n>
  inline
  int PSurf<T,n>::getNumSamIntPU() const {

    return _no_sam_p_u.getDim();
  }


  template <typename T, int n>
  inline
  int PSurf<T,n>::getNumSamIntPV() const {

    return _no_sam_p_v.getDim();
  }


  template <typename T, int n>
  inline
  int PSurf<T,n>::getSamPU( int i ) const {

    return _no_sam_p_u(i);
  }


  template <typename T, int n>
  inline
  int PSurf<T,n>::getSamPV( int i ) const {

    return _no_sam_p_v(i);
  }


  template <typename T, int n>
  inline
  int PSurf<T,n>::getSamplesU() const {

    return _no_sam_u;
  }


  template <typename T, int n>
  inline
  int PSurf<T,n>::getSamplesV() const {

    return _no_sam_v;
  }



  //******************************************
  // Virtual functons for surface properies **
  //******************************************

  template <typename T, int n>
  bool PSurf<T,n>::isClosedU() const {
    return false;
  }


  template <typename T, int n>
  bool PSurf<T,n>::isClosedV() const {
    return false;
  }



  //*******************************************************
  //***  Virtual functons for pre-samling  and plotting  **
  //*******************************************************

  template <typename T, int n>
  void PSurf<T,n>::preSample( int /*m1*/, int /*m2*/, int /*d1*/, int /*d2*/,
                                T /*s_u*/, T /*s_v*/, T /*e_u*/, T /*e_v*/ ) {}


  template <typename T, int n>
  void PSurf<T,n>::preSample( int /*dir*/, int /*m*/ ) {}



  template <typename T, int n>
  void PSurf<T,n>::replot( int m1, int m2, int d1, int d2 ) {

    if( m1 != _no_sam_u && m1 > 1) {
        _no_sam_u = m1;
        preSample(1, m1);
    }
    else m1 = _no_sam_u;

    if( m2 != _no_sam_v && m2 > 1) {
        _no_sam_v = m2;
        preSample(2, m2);
    }
    else m2 = _no_sam_v;

    // Correct derivatives
    if( d1 < 1 )    d1 = _no_der_u;
    else            _no_der_u = d1;
    if( d2 < 1 )    d2 = _no_der_v;
    else            _no_der_v = d2;

    // Sample Positions and related Derivatives
    DMatrix< DMatrix< Vector<T,n> > > p;
    resample( p, m1, m2, d1, d2, getStartPU(), getStartPV(), getEndPU(), getEndPV() );

    // Compute normals at the sample points
    DMatrix< Vector<float,n> > normals;
    resampleNormals( p, normals );

    // Set The Surrounding Sphere
    setSurroundingSphere( p );

    // Replot Visaulizers
    for( int i = 0; i < this->_psurf_visualizers.getSize(); i++ )
      this->_psurf_visualizers[i]->replot( p, normals, m1, m2, d1, d2, isClosedU(), isClosedV() );
  }



  template <typename T, int n>
  void PSurf<T,n>::replot() const {

      // Sample Positions and related Derivatives
      DMatrix<DMatrix<Vector<T,n>>> p;
      resample( p, _no_sam_u, _no_sam_v, _no_der_u, _no_der_v, getStartPU(), getStartPV(), getEndPU(), getEndPV() );

      // Compute normals at the sample points
      DMatrix< Vector<float,n> > normals;
      resampleNormals( p, normals );

      // Set The Surrounding Sphere
      setSurroundingSphere( p );

      // Replot Visaulizers
      for( int i = 0; i < this->_psurf_visualizers.getSize(); i++ )
        this->_psurf_visualizers[i]->replot( p, normals, _no_sam_u, _no_sam_v, _no_der_u, _no_der_v, isClosedU(), isClosedV() );
  }





  //*******************************************************
  //***  functons to handle visualizers to the surface   **
  //*******************************************************

  template <typename T, int n>
  void PSurf<T,n>::enableDefaultVisualizer( bool enable ) {

    if( !enable )
      removeVisualizer( _default_visualizer );
    else {

      if( !_default_visualizer )
        _default_visualizer = new PSurfDefaultVisualizer<T,n>();

      insertVisualizer( _default_visualizer );
    }
  }




  template <typename T, int n>
  PSurfVisualizer<T, n> *PSurf<T,n>::getDefaultVisualizer() const {

    return _default_visualizer;
  }



  template <typename T, int n>
  void PSurf<T,n>::toggleDefaultVisualizer() {

    if( !_psurf_visualizers.exist( _default_visualizer ) )
      enableDefaultVisualizer( true );
    else
      enableDefaultVisualizer( false );
  }



  template <typename T, int n>
  inline
  void PSurf<T,n>::insertVisualizer( Visualizer *visualizer ) {

    Parametrics<T,2,n>::insertVisualizer( visualizer );

    PSurfVisualizer<T,n> *visu = dynamic_cast<PSurfVisualizer<T,n>*>( visualizer );
    if( !visu )
      return;

    if( _psurf_visualizers.exist( visu ) )
      return;

    _psurf_visualizers += visu;
  }



  template <typename T, int n>
  inline
  void PSurf<T,n>::removeVisualizer( Visualizer *visualizer ) {

    PSurfVisualizer<T,n> *visu = dynamic_cast<PSurfVisualizer<T,n>*>( visualizer );
    if( visu )
      _psurf_visualizers.remove( visu );

    Parametrics<T,2,n>::removeVisualizer( visu );
  }




  template <typename T, int n>
  void PSurf<T,n>::resample( DMatrix< DMatrix < Vector<T,n> > >& p,
                                    int m1, int m2, int d1, int d2, T s_u, T s_v, T e_u, T e_v ) const {
    _resample = true;

    T du = (e_u-s_u)/(m1-1);
    T dv = (e_v-s_v)/(m2-1);

    p.setDim(m1, m2);

    for(int i=0; i<m1-1; i++) {
      _ind[0]=i;
      T u = s_u + i*du;
      for(int j=0;j<m2-1;j++) {
        _ind[1]=j;
        eval(u, s_v + j*dv, d1, d2, true, true );
        p[i][j] = _p;
      }
      _ind[1]=m2-1;
      eval(u, e_v, d1, d2, true, false);
      p[i][m2-1] = _p;
    }

    _ind[0]=m1-1;
    for(int j=0;j<m2-1;j++) {
      _ind[1]=j;
      eval(e_u, s_v + j*dv, d1, d2, false, true);
      p[m1-1][j] = _p;
    }
    _ind[1]=m2-1;
    eval(e_u, e_v, d1, d2, false, false);
    p[m1-1][m2-1] = _p;

    switch( this->_dm ) {
      case GM_DERIVATION_EXPLICIT:
        // Do nothing, evaluator algorithms for explicite calculation of derivatives
        // should be defined in the eval( ... ) function enclosed by
        // if( this->_derivation_method == this->EXPLICIT ) { ... eval algorithms for derivatives ... }
        break;
      case GM_DERIVATION_DD:
        DD::compute2D(p,double(du),double(dv),isClosedU(),isClosedV(),d1,d2);
        break;
    }

    _resample = false;
  }


  template <typename T, int n>
  inline
  void PSurf<T,n>::resample( DMatrix<DMatrix <DMatrix <Vector<T,n> > > >& a,
                                                int m1, int m2, int d1, int d2 ) {

    resample( a, m1, m2, d1, d2, getStartPU(), getStartPV(), getEndPU(), getEndPV() );
  }


  template <typename T, int n>
  void PSurf<T,n>::resampleNormals( const DMatrix<DMatrix<Vector<T,n> > > &p, DMatrix<Vector<float,3> > &normals ) const {

    normals.setDim( p.getDim1(), p.getDim2() );

    for( int i = 0; i < p.getDim1(); i++ )
      for( int j = 0; j < p.getDim2(); j++ ){
        normals[i][j] = p(i)(j)(1)(0) ^ p(i)(j)(0)(1);
        normals[i][j].normalize();
      }
  }


  template <typename T, int n>
  inline
  void PSurf<T,n>::setDomainU( T start, T end ) {

    _sc_u  = (end - start) / (getEndPU() - getStartPU());
    _tr_u  = start - getStartPU();
  }


  template <typename T, int n>
  inline
  void PSurf<T,n>::setDomainV( T start, T end ) {

    _sc_v  = (end - start) / (getEndPV() - getStartPV());
    _tr_v  = start - getStartPV();
  }


  template <typename T, int n>
  inline
  void PSurf<T,n>::setDomainUScale( T sc ) {

    _sc_u = sc;
  }


  template <typename T, int n>
  inline
  void PSurf<T,n>::setDomainVScale( T sc ) {

    _sc_v = sc;
  }


  template <typename T, int n>
  inline
  void PSurf<T,n>::setDomainUTrans( T tr ) {

    _tr_u = tr;
  }


  template <typename T, int n>
  inline
  void PSurf<T,n>::setDomainVTrans( T tr ) {

    _tr_v = tr;
  }



  template <typename T, int n>
  inline
  void PSurf<T,n>::setNoDer( int d ) {

     _default_d  = d;
  }


  template <typename T, int n>
  void PSurf<T,n>::setSurroundingSphere( const DMatrix< DMatrix< Vector<T,n> > >& p ) const {
    Sphere<T,n>  s;
    uppdateSurroundingSphere(s, p);
    Parametrics<T,2,n>::setSurroundingSphere(s);
  }


  template <typename T, int n>
  inline
  void PSurf<T,n>::uppdateSurroundingSphere( Sphere<T,n>& s, const DMatrix< DMatrix< Vector<T,n> > >& p ) const {
      s += p(0)(0)(0)(0).toPoint();
      s += p( p.getDim1()-1 )( p.getDim2()-1 )(0)(0).toPoint();
      s += p( p.getDim1()/2 )( p.getDim2()/2 )(0)(0).toPoint();
      s += p( p.getDim1()-1 )( 0             )(0)(0).toPoint();
      s += p( 0             )( p.getDim2()-1 )(0)(0).toPoint();
      s += p( p.getDim1()-1 )( p.getDim2()/2 )(0)(0).toPoint();
      s += p( p.getDim1()/2 )( p.getDim2()-1 )(0)(0).toPoint();
      s += p( 0             )( p.getDim2()/2 )(0)(0).toPoint();
      s += p( p.getDim1()/2 )( 0             )(0)(0).toPoint();
  }


  template <typename T, int n>
  inline
  T PSurf<T,n>::shiftU( T u ) const {

    return getStartPU() + _sc_u * ( u - getParStartU() );
  }


  template <typename T, int n>
  inline
  T PSurf<T,n>::shiftV( T v ) const {

    return getStartPV() + _sc_v * ( v - getParStartV() );
  }


  template <typename T, int n>
  Parametrics<T,2,n>* PSurf<T,n>::split( T /*t*/, int /*uv*/ ) {

    return 0;
  }



  template <typename T, int n>
  inline
  void PSurf<T,n>::_eval( T u, T v, int d1, int d2 ) const {

    if( !(d1 <= _d1 && d2 <=_d2 && GMutils::compValueF(u,_u) && GMutils::compValueF(v,_v) ) ) {

      _u  = u;
      _v  = v;
      _d1 = d1;
      _d2 = d2;

      eval( shiftU(u), shiftV(v), d1, d2 );
    }
  }



  template <typename T, int n>
  inline
  void PSurf<T,n>::_computeEFGefg( T u, T v, T& E, T& F, T& G, T& e, T& f, T& g ) const {
      _eval(u,v,2,2);
      UnitVector<T,n>  N   = _p[1][0]^_p[0][1];
      Vector<T,n>      du  = _p[1][0];
      Vector<T,n>      dv  = _p[0][1];
      Vector<T,n>      duu = _p[2][0];
      Vector<T,n>      duv = _p[1][1];
      Vector<T,n>      dvv = _p[0][2];
      E = du * du;
      F = du * dv;
      G = dv * dv;
      e = N  * duu;
      f = N  * duv;
      g = N  * dvv;
  }




} // END namespace GMlib
