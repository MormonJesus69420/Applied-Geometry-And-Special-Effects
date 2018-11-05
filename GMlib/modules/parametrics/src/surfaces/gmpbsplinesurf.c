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

// gmlib
#include <core/containers/gmdmatrix.h>
#include <scene/selector/gmselector.h>

namespace GMlib {


  //*****************************************
  // Constructors and destructor           **
  //*****************************************

  template <typename T>
  inline
  PBSplineSurf<T>::PBSplineSurf( const DMatrix< Vector<T,3> >& c, const DVector<T>& u,  const DVector<T>& v, int du, int dv) {

      init();

      // Set Control Points
      _c = c;

      // Set order
      _ku = u.getDim()-_c.getDim1();
      _kv = v.getDim()-_c.getDim2();

      // Set knot-vector in u-direction
      if(_ku == 1) {  // closed in u-direction
          _du = du;
          initKnot( _u, _cu, _ku, u, _c.getDim1(), _du);
      }
      else {
          _u = u;
          _du = _ku-1;
          _cu = false;
      }

      // Set knot-vector in v-direction
      if(_kv == 1) {  // closed in v-direction
          if(dv==0)   _dv = du;
          else        _dv = dv;
          initKnot( _v, _cv, _kv, v, _c.getDim2(), _dv);
      }
      else {
          _v = v;
          _dv = _kv-1;
          _cv = false;
      }
  }


  template <typename T>
  inline
  PBSplineSurf<T>::PBSplineSurf( const PBSplineSurf<T>& copy ) : PSurf<T,3>( copy ) {

      init();

      _c = copy._c;
      _u = copy._u;
      _v = copy._v;
      _ku = copy._ku;
      _kv = copy._kv;
      _du = copy._du;
      _dv = copy._dv;
      _cu = copy._cu;
      _cv = copy._cv;
  }


  template <typename T>
  PBSplineSurf<T>::~PBSplineSurf() {

      if(_sgv) delete _sgv;
  }


  //*********************************
  //**   Public local functons     **
  //*********************************

  template <typename T>
  inline
  DMatrix< Vector<T,3> >& PBSplineSurf<T>::getControlPoints() const {
      return _c;
  }


  template <typename T>
  inline
  int PBSplineSurf<T>::getDegreeU() const {
      return _du;
  }


  template <typename T>
  inline
  int PBSplineSurf<T>::getDegreeV() const {
      return _dv;
  }


  template <typename T>
  bool PBSplineSurf<T>::isSelectorsVisible() const {
      return _selectors;
  }



  template <typename T>
  void PBSplineSurf<T>::setClosed( bool closed_u, bool closed_v, T du, T dv ) {

      bool changed = false;

      if(_cu != closed_u) {
          DVector<T> nu = _u;
          if(closed_u) {
              if(du==T(0)) du = (_u[_u.getDim()-_ku]-_u[_du])/(_u.getDim() - 2*_ku + 1);
              initKnot( _u, _cu, _ku, nu, _c.getDim1(), _du, du);
          } else
              initKnot2(_u, _cu, nu, _c.getDim1(), _du);
          changed = true;
      }

      if(_cv != closed_v){
          DVector<T> nv = _v;
          if(closed_v) {
              if(dv==T(0)) dv = (_v[_v.getDim()-_kv]-_v[_dv])/(_v.getDim() - 2*_kv + 1);
              initKnot( _v, _cv, _kv, nv, _c.getDim2(), _dv, dv);
          } else
              initKnot2(_v, _cv, nv, _c.getDim2(), _dv);
          changed = true;
      }

      if(changed && _selectors) {
          hideSelectors();
          showSelectors(_selector_radius, _grid, _selector_color, _grid_color);
      }
  }



  template <typename T>
  inline
  void PBSplineSurf<T>::setPartitionCritere(int pcu, int pcv) {
      _pcu = pcu;
      _pcv = pcv;
  }



  template <typename T>
  inline
  void PBSplineSurf<T>::setControlPoints( const DMatrix< Vector<T,3> >& cp ) {

      if( _c.getDim1() == cp.getDim1() && _c.getDim2() == cp.getDim2() )
          _c = cp;
      else
          std::cerr << "Can not change the control point because the dimentions are wrong!";
  }



  template <typename T>
  inline
  void PBSplineSurf<T>::updateCoeffs( const Vector<T,3>& d ) {

      if( _c_moved ) {
          HqMatrix<T,3> invmat = this->_matrix;
          invmat.invertOrthoNormal();

          Vector<T,3> diff = invmat*d;
          for( int i = 0; i < _c.getDim1(); i++ ) {
              for( int j = 0; j < _c.getDim2(); j++ ) {
                  _c[i][j] += diff;
                  _s[i][j]->translateParent( diff );
              }
          }
          this->translateParent( -d );
      }
  }



  template <typename T>
  inline
  void PBSplineSurf<T>::enablePartitionVisualizer(int u, int v) {
      _part_viz = true;
      setPartitionCritere(u, v);
}



  //********************************************************
  // Overrided (public) virtual functons from SceneObject **
  //********************************************************

  template <typename T>
  void PBSplineSurf<T>::edit( int selector_id, const Vector<T,3>& dp  ) {

    _c_moved = true;
      if( this->_parent ) this->_parent->edit( this );
      _pos_change.push_back(EditSet(selector_id, dp));
      this->setEditDone();
    _c_moved = false;
  }



//  template <typename T>
//  inline
//  void PBSplineSurf<T>::insertVisualizer( Visualizer *visualizer ) {

//    SceneObject::insertVisualizer( visualizer );

//    PSurfVisualizer<T,3> *visu = dynamic_cast<PSurfVisualizer<T,3>*>( visualizer );
//    if( !visu )
//      return;

//    for(int i=0; i < _visu.getDim1(); i++)
//        for(int j=0; j < _visu.getDim2(); j++) {
//            _visu[i][j].vis += visu;
//        }
//  }




  template <typename T>
  void PBSplineSurf<T>::replot( ) const{

          int d1 = this->_no_der_u;
          int d2 = this->_no_der_v;

          DMatrix< DMatrix< Vector<T,3> > >   p;
          DMatrix< Vector<float,3> >          normals;
          Sphere<T,3>                         s;

          for(int i=0; i<_vpu.getDim(); i++)
              for(int j=0; j<_vpv.getDim(); j++) {

                  // Sample Positions and related Derivatives
                  resample(p, _ru[i], _rv[j], _vpu[i].m, _vpv[j].m, d1, d2 );

                  // Compute normals at the sample points
                  this->resampleNormals( p, normals );

                  this->uppdateSurroundingSphere(s, p);

                  // Replot Visaulizers
                  for( int k = 0; k < _visu[i][j].vis.getSize(); k++ )
                      _visu[i][j].vis[k]->replot( p, normals, _vpu[i].m, _vpv[j].m, d1, d2, false, false );
              }
          Parametrics<T,2,3>::setSurroundingSphere(s);
  }




  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************

  template <typename T>
  void PBSplineSurf<T>::replot( int m1, int m2, int d1, int d2 ) {

      if(_part_viz) {
          // Make new pre-samples if necessary
          if( m1 != this->_no_sam_u && m1 > 1) {
              this->_no_sam_u = m1;
              preSample(1, m1);
          }
          if( m2 != this->_no_sam_v && m2 > 1){
              this->_no_sam_v = m2;
              preSample(2, m2);
          }
          // Correct derivatives ?
          if( d1 < 1 )    d1 = this->_no_der_u;
          else            this->_no_der_u = d1;
          if( d2 < 1 )    d2 = this->_no_der_v;
          else            this->_no_der_v = d2;

          DMatrix< DMatrix< Vector<T,3> > >   p;
          DMatrix< Vector<float,3> >          normals;
          Sphere<T,3>                         s;

          for(int i=0; i<_vpu.getDim(); i++)
              for(int j=0; j<_vpv.getDim(); j++) {

                  // Sample Positions and related Derivatives
                  resample(p, _ru[i], _rv[j], _vpu[i].m, _vpv[j].m, d1, d2 );

                  // Compute normals at the sample points
                  this->resampleNormals( p, normals );

                  this->uppdateSurroundingSphere(s, p);

                  // Replot Visaulizers
                  for( int k = 0; k < _visu[i][j].vis.getSize(); k++ )
                      _visu[i][j].vis[k]->replot( p, normals, _vpu[i].m, _vpv[j].m, d1, d2, false, false );
              }
          Parametrics<T,2,3>::setSurroundingSphere(s);
      } else
          PSurf<T,3>::replot( m1, m2, d1, d2 );
  }


  template <typename T>
  bool PBSplineSurf<T>::isClosedU() const {
      return _cu;
  }


  template <typename T>
  bool PBSplineSurf<T>::isClosedV() const {
      return _cv;
  }


  template <typename T>
  void PBSplineSurf<T>::showSelectors( T rad, bool grid, const Color& selector_color, const Color& grid_color ) {

      if( _selectors ) return;

      _s.setDim( _c.getDim1(), _c.getDim2() );

      for( int i = 0; i < _c.getDim1(); i++ )
          for( int j = 0; j < _c.getDim2(); j++ ) {
              Selector<T,3> *sel = new Selector<T,3>( _c[i][j], _map2(i, j), this, rad, selector_color );
              this->insert( sel );
              _s[i][j] = sel;
          }

      if( grid ) {
          if(!_sgv) _sgv = new SelectorGridVisualizer<T>;
          _sgv->setSelectors( _c, _cu, _cv );
          _sgv->setColor( grid_color );
          this->insertVisualizer( _sgv );
      }

      _selector_radius = rad;
      _selector_color  = selector_color;
      _grid_color      = grid_color;
      _grid            = grid;
      _selectors       = true;
  }


  template <typename T>
  void PBSplineSurf<T>::hideSelectors() {

      if( !_selectors ) return;

      // Remove Selector Grid Visualizer
      if(_sgv){
        this->removeVisualizer( _sgv );
        _sgv->reset();
      }

      // Remove Selectors
      for( int i = 0; i < _s.getDim1(); i++ ) {
          for( int j = 0; j < _s.getDim2(); j++ ) {
              this->remove( _s[i][j] );
              delete _s[i][j];
          }
      }
      _selectors = false;
  }



  /*! void PBSplineSurf<T>::toggleSelectors()
   *  To toggle the selectors and selector grid.
   */
  template <typename T>
  void PBSplineSurf<T>::toggleSelectors() {

    if(_selectors)  hideSelectors();
    else            showSelectors();
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************


  template <typename T>
  void PBSplineSurf<T>::eval( T u, T v, int du, int dv, bool lu, bool lv ) const {

      DMatrix<T>   bu, bv;
      std::vector<int> ind_i(_ku), ind_j(_kv);

      int i = EvaluatorStatic<T>::evaluateBSp( bu, u, _u, _du, lu) - _du;
      int j = EvaluatorStatic<T>::evaluateBSp( bv, v, _v, _dv, lv) - _dv;

      makeIndex(ind_i, i, _ku, _c.getDim1());
      makeIndex(ind_j, j, _kv, _c.getDim2());

      multEval( this->_p, bu, bv, ind_i, ind_j, du, dv);
  }


  template <typename T>
  T PBSplineSurf<T>::getEndPU() const {
      return _u(_u.getDim()-_ku);
  }


  template <typename T>
  T PBSplineSurf<T>::getEndPV() const {
      return _v(_v.getDim()-_kv);
  }


  template <typename T>
  T PBSplineSurf<T>::getStartPU() const {
      return _u(_du);
  }


  template <typename T>
  inline
  T PBSplineSurf<T>::getStartPV() const {
      return _v(_dv);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************


  template <typename T>
  inline
  void PBSplineSurf<T>::init() {

      this->_type_id = GM_SO_TYPE_SURFACE_BSPLINE;

      _selectors    = false;
      _c_moved      = false;

      _part_viz       = false;
      _partitioned[0] = false;
      _partitioned[1] = false;
      _pcu = 1;
      _pcv = 1;

      _ru.setDim(1);
      _ru.setDim(1);

      _sgv =  0x0;
  }




  //***************************************************
  // Overrided (private) virtual functons from PSurf **
  //***************************************************


  // Sampling for pre-evaluation, chose direction
  //**********************************************
  template <typename T>
  void  PBSplineSurf<T>::preSample( int dir, int m ) {
      if(_part_viz) {
          if( dir==1 ) {
              if(!_partitioned[0]) {
                  _partitioned[0] = true;
                  makePartition( _vpu, _u, _ku, _pcu, m );
              }
              _ru.setDim(_vpu.getDim());
              for(int i=0; i<_ru.getDim(); i++)
                  preSample( _ru[i], _u, _vpu[i].m, _du, _c.getDim1(), _u[_vpu[i].is], _u[_vpu[i].ie] );
          }
          if( dir==2 ) {
              if(!_partitioned[1]) {
                  _partitioned[1] = true;
                  makePartition( _vpv, _v, _kv, _pcv, m );
              }
              _rv.setDim(_vpv.getDim());
              for(int i=0; i<_rv.getDim(); i++)
                  preSample( _rv[i], _v, _vpv[i].m, _dv, _c.getDim2(), _v[_vpv[i].is], _v[_vpv[i].ie] );
          }
      }
      else {
          if( dir==1 )
              preSample( _ru[0], _u, m, _du, _c.getDim1(), _u[_du], _u[_u.getDim()-_ku] );
          if( dir==2 )
              preSample( _rv[0], _v, m, _dv, _c.getDim2(), _v[_dv], _v[_v.getDim()-_kv] );
      }
  }



  template <typename T>
  void PBSplineSurf<T>::resample( DMatrix< DMatrix< Vector<T,3> > >& p, int m1, int m2, int d1, int d2, T s_u, T s_v, T e_u, T e_v ) const {

      p.setDim(m1, m2);

      for(int i=0; i<m1; i++)
          for(int j=0; j<m2; j++)
              multEval( p[i][j], _ru[0][i].m, _rv[0][j].m, _ru[0][i].ind, _rv[0][j].ind, d1, d2 );
  }



  template <typename T>
  void PBSplineSurf<T>::resample( DMatrix< DMatrix< Vector<T,3> > >& p, const DVector<PreMat<T>>& bu, const DVector<PreMat<T>>& bv, int m1, int m2, int d1, int d2 ) const {

      p.setDim(m1, m2);

      for(int i=0; i<m1; i++)
          for(int j=0; j<m2; j++)
              multEval( p[i][j], bu(i).m, bv(j).m, bu(i).ind, bv(j).ind, d1, d2);
  }



  //***************************************
  //**   Local (private) help functons   **
  //***************************************

  template <typename T>
  inline
  void PBSplineSurf<T>::makeIndex( std::vector<int>& ind, int i, int k, int n) const{

      if(i+k > n){
          int j, s = n-i;
          for (j=0; j < s; j++)
              ind[j] = i++;
          for ( ; j < k; j++)
              ind[j] = j-s;
      }
      else
          for(int j=0; j<k; j++)
              ind[j]= i+j;
  }



  template <typename T>
  inline
  void PBSplineSurf<T>::multEval(DMatrix<Vector<T,3>>& p, const DMatrix<T>& bu, const DMatrix<T>& bv, const std::vector<int>& ii, const std::vector<int>& ij, int du, int dv) const {

      // Set Dimensions
      p.setDim(du+1,dv+1);

      // We manually do these two operations!
      //    bv.transpose();
      //    p = bu * (c^bv);
      // The reason why we do it manually is that we only copmpute the du first lines of bu and the dv first lines of bv

      //    c = _c^bvT
      DMatrix<Vector<T,3>> c(_ku, dv+1);

      for(int i=0; i< _ku; i++)
          for(int j=0; j<=dv; j++) {
              c[i][j] = _c(ii[i])(ij[0])*bv(j)(0);
              for(int k=1; k<_kv; k++)
                  c[i][j] += _c(ii[i])(ij[k])*bv(j)(k);
          }
      //    p = bu * c
      for(int i=0; i<=dv; i++)
          for(int j=0; j<=du; j++) {
              p[i][j] = bu(i)(0)*c[0][j];
              for(int k=1; k<_ku; k++)
                  p[i][j] += bu(i)(k)*c[k][j];
          }
  }



  template <typename T>
  inline
  void PBSplineSurf<T>::initKnot( DVector<T>& t, bool& c, int& k, const DVector<T>& g, int n, int d, T dt) {

      k = d+1;
      t.setDim(n+d+k);
      if(dt > T(0)) {
          for(int i = d; i < n+1; i++)                  t[i] = g(i);
          for(int i = n+1; i < n+k; i++)                t[i] = t[i-1] + dt;
      } else
          for(int i = d; i < n+k; i++)                  t[i] = g(i-d);
      for(int i = d-1, j = n+d; i >= 0; i--,j--)        t[i] = t[i+1] - (t[j]-t[j-1]);
      for(int i = n+k, j = k; i < t.getDim(); i++,j++)  t[i] = t[i-1] + (t[j]-t[j-1]);
      c = true;
  }


  template <typename T>
  inline
  void PBSplineSurf<T>::initKnot2( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d) {

      t.setDim(g.getDim()-d);
      for(int i = d; i <= n; i++)            t[i] = g(i-d);
      for(int i = d-1; i >= 0; i--)          t[i] = t[i+1];
      for(int i = n+1; i < t.getDim(); i++)  t[i] = t[i-1];
      c = false;
  }


  // pre-evaluation of basis fuctions, the B-spline-Hermite matrix, independent of direction
  //*******************************************************
  template <typename T>
  inline
  void PBSplineSurf<T>::preSample( DVector< PreMat<T> >& p, const DVector<T>& t, int m, int d, int n, T start, T end ) {

      const T dt = ( end - start ) / T(m-1); // dt is the step in parameter values
      p.setDim(m);      // p is a vector of  Bernstein-Hermite matrises at the sample points

      // Compute the Bernstein-Hermite matrix
      for( int j = 0; j < m-1; j++ ) {
          int i = EvaluatorStatic<T>::evaluateBSp( p[j].m, start+j*dt, t, d, false );// - d;
          p[j].ind.init( i, d+1, n);
      }
      int i = EvaluatorStatic<T>::evaluateBSp( p[m-1].m, end, t, d, true );// - d;
      p[m-1].ind.init( i, d+1, n);
  }



  template <typename T>
  inline
  void  PBSplineSurf<T>::makePartition( DVector<VisuPar>& vp, const DVector<T>& t, int k, int dis, int m ) {

      VisPart<T> pu( t, k, dis );
      SampNr<T>  su( t, pu, m );
      vp.setDim(su.size());
      for(int i=0; i<vp.getDim(); i++) {
          vp[i].m  = su[i];
          vp[i].is = pu[2*i];
          vp[i].ie = pu[2*i+1];
      }

      // Inserting vizualizers
      if(_partitioned[0] && _partitioned[1]) {
          _visu.setDim(_vpu.getDim(), _vpv.getDim());
          for(int i=0; i<_visu.getDim1(); i++)
              for(int j=0; j<_visu.getDim2(); j++) {
                  PSurfVisualizer<T,3>* vis = new PSurfDefaultVisualizer<T,3>();
                  _visu[i][j].vis += vis;
                  this->insertVisualizer(vis);
                  _visu[i][j].s_u = Vector<T,2>(_u[_vpu[i].is], _u[_vpu[i].ie]);
                  _visu[i][j].s_v = Vector<T,2>(_v[_vpv[j].is], _v[_vpv[j].ie]);
              }
      }
  }



  template <typename T>
  inline
  Vector<int,2> PBSplineSurf<T>::_map1(int i) const {

      Vector<int,2> j;
      int n = _c.getDim2();
      j[0] = i/n;
      j[1] = i%n;
      return j;
  }


  template <typename T>
  inline
  int PBSplineSurf<T>::_map2(int i, int j) const {

      int n = _c.getDim2();
      return i*n+j;
  }



} // END namespace GMlib


