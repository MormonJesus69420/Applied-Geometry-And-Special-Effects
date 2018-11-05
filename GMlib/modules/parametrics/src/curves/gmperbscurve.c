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




#include "gmperbscurve.h"

#include "../evaluators/gmevaluatorstatic.h"

#include "gmparc.h"
#include "gmpbeziercurve.h"
#include "gmpsubcurve.h"


namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************


  template <typename T>
  inline
  PERBSCurve<T>::PERBSCurve(){

    init(false);
  }



  /*! PERBSCurve<T>::PERBSCurve( PCurve<T,3>* g, int n, bool cu_len)
   *  \brief Constructor copy curve g, using SubCurves
   *
   *  Making an ordinary blending-spline curve (ERBS)
   *
   *  \param g      The original curve to copy
   *  \param n      The number of local curves to use
   *  \param cu_len Whether we use uniform sampling (false), or curve length sampling (true)
   */
  template <typename T>
  inline
  PERBSCurve<T>::PERBSCurve(PCurve<T,3>* g, int n, bool cu_len) {

    _type_local = SUB_CURVE;
    g->setDerived(this);
    _origin     = g;
    init(g->isClosed());

    if(cu_len)
        generateCuLenKnotVector(g, n, isClosed());
    else
        generateKnotVector(g, n, isClosed());

    // Make local curves.
    _c.setDim(n);
    for( int i = 0; i < n; i++ ) {
       _c[i] = new PSubCurve<T>(g,  _t[i], _t[i+2], _t[i+1]);
       _c[i]->setNumber(i);
       insertLocal(_c[i]);
    }
  }




  /*! PERBSCurve<T>::PERBSCurve(PCurve<T,3>* g, int n, int d, bool cu_len)
   *  \brief Constructor copy curve g, using BezierCurves
   *
   *  Making an ordinary blending-spline curve (ERBS)
   *
   *  \param g      The original curve to copy
   *  \param n      The number of local curves to use
   *  \param n      The polynomial degree of the Bezier curves
   *  \param cu_len Whether we use uniform sampling (false), or curve length sampling (true)
   */
  template <typename T>
  inline
  PERBSCurve<T>::PERBSCurve(PCurve<T,3>* g, int n, int d, bool cu_len) {

    _type_local = BEZIER_CURVE;
    _origin     = g;
    init(g->isClosed());

    if(cu_len)
        generateCuLenKnotVector(g, n, isClosed());
    else
        generateKnotVector(g, n, isClosed());

    // Make local curves.
    _c.setDim(n);
    for( int i = 0; i < n; i++ )
      insertLocal(_c[i] = new PBezierCurve<T>(g->evaluateParent(_t[i+1], d), _t[i], _t[i+1], _t[i+2]));
  }



  /*! PERBSCurve<T>::PERBSCurve(int n, PCurve<T,3>* g, bool cu_len)
   *  \brief Constructor copy curve g, using ArcCurves
   *
   *  Making an ordinary blending-spline curve (ERBS)
   *
   *  \param n      The number of local curves to use
   *  \param g      The original curve to copy
   *  \param cu_len Whether we use uniform sampling (false), or curve length sampling (true)
   */
  template <typename T>
  inline
  PERBSCurve<T>::PERBSCurve(int n, PCurve<T,3>* g, bool cu_len) {

      _type_local = ARC_CURVE;
      _origin     = g;
      init(g->isClosed());

    if(cu_len)
        generateCuLenKnotVector(g, n, isClosed());
    else
        generateKnotVector(g, n, isClosed());

    // Make local curves.
    _c.setDim(n);
    for( int i = 0; i < n; i++ )
      insertLocal(_c[i] = new PArc<T>(g->evaluateParent(_t[i+1], 2), _t[i], _t[i+1], _t[i+2]));
  }



  /*! PERBSCurve<T>::PERBSCurve(int n, PCurve<T,3>* g, bool cu_len)
   *  \brief The copy constructor.
   *
   *  Making an ordinary blending-spline curve (ERBS)
   *
   *  \param copy   The PERBSCurve to copy
   */
  template <typename T>
  inline
  PERBSCurve<T>::PERBSCurve( const PERBSCurve<T>& copy ) : PCurve<T,3>( copy ) {

      init(copy._cl);
      _origin = copy._origin;

      // Copy the knot vector
      _t = copy._t;

      // sync local patches
      const DVector<PCurve<T,3>*> &c = copy._c;
      _c.setDim( c.getDim() );

      // Find and save the reference to the local patches
      for( int i = 0; i < c.getDim(); ++i ) {
          SceneObject* local_p =  dynamic_cast<SceneObject*>(c[i]);
          for( int j = 0; j < this->getChildren().getSize(); ++i ) {
              SceneObject *child = this->getChildren()[j];
              if( local_p == child->_copy_of) {
                  _c[i] = dynamic_cast<PCurve<T,3>*>(child);
                  break;
              }
          }
      }
  }





  template <typename T>
  PERBSCurve<T>::~PERBSCurve() {

    for( int i = 0; i < _c.getDim(); i++ ) {
      SceneObject::remove( _c[i] );
      delete _c[i];
    }

    if( _evaluator )
      delete _evaluator;
  }




  //*********************************
  //**   Public local functons     **
  //*********************************



  /*! void PERBSCurve<T>::setPartitionCriterion(int pct)
   *  \brief To set criterion for partitions of the curve
   *
   *  To set a criterion for how to split the curve into several partitions
   *  This can ensure discontinuities at different levels are displayed
   *
   *  Note that you have to call:
   *            sample( int m, int d ) after changing the criterion.
   *
   *  \param  pct  partition criterie (valid values are 0 and 1.
   *          pct = 0 - split where we have a discontinue function (multiplisity = order = 2)
   *          pct = 1 - split in every single knot
   */
  template <typename T>
  inline
  void PERBSCurve<T>::setPartitionCriterion(int pct) {
      _pct = pct;
  }





  template <typename T>
  inline
  void PERBSCurve<T>::edit( SceneObject *obj )
  {
      if(obj->getName() == _origin->getName()) {
          int m = this->getNumSamples();
          int d = this->getNumDerivatives();
          if(_origin->isClosed() != this->isClosed()) { // fra lukket til åpen etc.
              _origin->sample(m, d);
              _cl = _origin->isClosed();
              generateKnotVector(_origin, _c.getDim(), isClosed());
              // Uppdate local curves.
              for( int i = 0; i < _c.getDim(); i++ )
                  _c[i]->openClosedChanged(_t[i], _t[i+1], _t[i+2]);
          }
          sample(m, d);
          for(int j=0; j<_c.getDim(); j++)
              _c[j]->edit(obj);
      }
      else {
          int i = obj->getNumber();
          if(i >= 0) {
              for(uint j=0; j<_local_change.size(); j++)
                  if(i == _local_change[j]) return;
              _local_change.push_back(i);
              this->setEditDone();
          }
          if( this->_parent && !(obj->getName() == _origin->getName()))
              this->_parent->edit( this );
      }
  }





  // This function is not meant for public use

  /*! void PERBSCurve<T>::replot()
   *  \brief Not for public use
   *
   *  To replot after object editing or shape changing else,
   *  therefor, this function is not meant for public use
   */
  template <typename T>
  void PERBSCurve<T>::replot() const {

      updatSamples();
      PCurve<T,3>::replot();
  }








  template <typename T>
  void PERBSCurve<T>::eval( T t, int d, bool left ) const
  {
    int k = EvaluatorStatic<T>::knotIndex(_t, t, 1, left);

    IndexBsp ii( k, 2, _c.getDim());

    // Evaluating first Local Curve @ (t-_t[k-1])/(_t[k+1]-_t[k-1])
    DVector<Vector<T,3>> c0 = _c[ii[0]]->evaluateParent(t, d);

    // If t == _t[k], the sample is at the knot, set the values to the values of the first local curve.
    if(std::abs(t - _t[k]) < 1e-5) { this->_p = c0; return; }

    // Evaluating second Local Curve @ (t-_t[k])/(_t[k+2]-_t[k)
    this->_p = _c[ii[1]]->evaluateParent(t - (ii[0]<ii[1] ? T(0):this->getParDelta()), d);

    // Blend c0 and c1
    Vector<T,3>& B = getB(t, k, d);
    compBlend( d, B, c0, this->_p );
  }



  template <typename T>
  inline
  void PERBSCurve<T>::compBlend(int d, const Vector<T,3>& B, DVector<Vector<T,3>>& c0, DVector<Vector<T,3>>& c1) const {

    c0 -= c1;
    DVector<T> a(d+1);
    for( int i = 0; i <= d; i++ )
    {
      // Compute the pascal triangle numbers
      a[i] = 1;
      for( int j = i-1; j > 0; j-- )
        a[j] += a[j-1];
      // Compute the sample position data
      for( int j = 0; j <= i; j++ )
        c1[i] += (a[j] * B(j)) * c0[i-j];
    }
  }



  template <typename T>
  inline
  Vector<T,3>& PERBSCurve<T>::getB(T t, int k, int d) const {

    static Vector<T,3> B;

    _evaluator->set( _t[k], _t[k+1] - _t[k] );
    B[0] = 1 - (*_evaluator)(t);
    switch(d) {
      case 2: B[2] = - _evaluator->getDer2();
      case 1: B[1] = - _evaluator->getDer1();
    }
    return B;
  }



  template <typename T>
  inline
  DVector< PCurve<T,3>* >& PERBSCurve<T>::getLocalCurves() {
    return _c;
  }



  template <typename T>
  inline
  const DVector< PCurve<T,3>* >& PERBSCurve<T>::getLocalCurves() const {
    return _c;
  }



  template <typename T>
  inline
  int PERBSCurve<T>::getNoLocalCurves() const {
    return _c.getDim();
  }



  template <typename T>
  T PERBSCurve<T>::getStartP() const {
    return _t(1);
  }



  template <typename T>
  T PERBSCurve<T>::getEndP() const {
    return _t(_t.getDim()-2);
  }



  template <typename T>
  inline
  void PERBSCurve<T>::hideLocalCurves() {

    for( int i = 0; i < _c.getDim(); i++ )
      _c[i]->setVisible( false, -1 );
  }



  template <typename T>
  inline
  void PERBSCurve<T>::showLocalCurves() {

    for( int i = 0; i < _c.getDim(); i++ ) {
      if( !_c[i]->getDefaultVisualizer() ) {
        _c[i]->enableDefaultVisualizer();
        _c[i]->sample(10,0);
      }
      _c[i]->setVisible( true, -1 );
    }
  }



  template <typename T>
  inline
  void PERBSCurve<T>::insertLocal( PCurve<T,3>* local ) {

    local->toggleDefaultVisualizer();
    local->sample( 30,0 );
    static Color cl= GMcolor::blue();
    local->setColor( cl );
    cl = cl.getInterpolatedHSV( 0.2, GMcolor::yellow() );
    local->setVisible( true );
    local->setCollapsed( true );
    this->insert( local );
  }


  template <typename T>
  inline
  bool PERBSCurve<T>::isClosed() const {

    return _cl;
  }


  template <typename T>
  inline
  bool PERBSCurve<T>::isLocalCurvesVisible() const {

    for( int i = 0; i < _c.getDim(); i++ )
      if( _c(i)->isVisible() ) return true;
    return false;
  }






  template <typename T>
  inline
  void PERBSCurve<T>::toggleLocalCurves() {

    for( int i = 0; i < _c.getDim(); i++ )
      _c[i]->toggleVisible();
  }






  template <typename T>
  inline
  void PERBSCurve<T>::generateCuLenKnotVector( PCurve<T,3>* g, int n, bool closed ) {

      const T  sp = g->getParStart();
      const T  ep = g->getParEnd();

      if(closed) {
          const T  dt = (ep-sp)/n;
          _t.setDim(n+3);
          for(int i=0; i<n+3; i++ )
              _t[i] = sp + (i-1) * dt;
      }
      else {
          const T  dt = (ep-sp)/(n-1);
          _t.setDim(n+2);
          _t[0] = _t[1] = sp;             // Set the start knots
          for(int i = 2; i < n; i++)
              _t[i] = sp + (i-1)*dt;      // Set the "step"-knots
          _t[n] = _t[n+1] = ep;           // Set the end knots
      }
      _cl = closed;
  }



  template <typename T>
  inline
  void PERBSCurve<T>::generateKnotVector( PCurve<T,3>* g, int n, bool closed ) {

    const T  sp = g->getParStart();
    const T  ep = g->getParEnd();

    if(closed) {
        const T  dt = (ep-sp)/n;
        _t.setDim(n+3);
        for(int i=0; i<n+3; i++ )
            _t[i] = sp + (i-1) * dt;
    }
    else {
        const T  dt = (ep-sp)/(n-1);
        _t.setDim(n+2);           
        _t[0] = _t[1] = sp;             // Set the start knots
        for(int i = 2; i < n; i++)
            _t[i] = sp + (i-1)*dt;      // Set the "step"-knots
        _t[n] = _t[n+1] = ep;           // Set the end knots
    }
    _cl = closed;
}





  template <typename T>
  inline
  void PERBSCurve<T>::splitKnot(int tk) {

    // Return if knot does not exist
    if( tk >= _c.getDim() )
      return;

    // expand knot vector
    _t.insert( tk+1, _t(tk+1) );

    // expand local curves
    PCurve<T,3> *split_curve = static_cast<PCurve<T,3>*>(_c(tk)->makeCopy());
    _c.insert( tk, split_curve );
    insertLocal( split_curve );

    /*!
     *  Scale domain of splited curves
     *
     *  \todo Does this scaling handle multiple knot splits to the "same" knot ?
     */
    split_curve->setDomain( T(0), T(0.5) );
    _c(tk)->setDomain( T(0.5), T(1) );
  }





  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  /*! void PERBSCurve<T>::sample( int m, int d )
   *  To sample and plot the curve.
   *
   *  \param  m  The number of samples to make
   *  \param  d  The number of derivatives to compute
   */
  template <typename T>
  void PERBSCurve<T>::sample(int m, int d) {

    this->_checkSampleVal( m, d );

    for(uint i=1; i < this->_visu.size(); i++)
       this->cleanVisualizers(i);

      // Make partitions, the parameter values and pre-evaluated b-functions for each partition
    makePartition( m );
    prepareSampling(d);
    preSample(d);
    this->prepareVisualizers();
    this->setEditDone();
  }





  /*! void  PERBSCurve<T>::preSample( int d )
   *  \brief Private, not for public use
   *
   *  Compute all sample points for all partitions
   *
   *  \param  d   The number of derivatives to compute
   */
  template <typename T>
  void  PERBSCurve<T>::preSample( int d ) {

      // For all partisions,
      // prepare the array of (start, end) index of sample points influented by a local curve
      _cp_index.resize(_c.getDim());
      for(uint i=0; i < _cp_index.size(); i++) {
          _cp_index[i].setDim(_pre_basis.size());
          for(int j=0; j < _cp_index[i].getDim(); j++)
              _cp_index[i][j] = {std::numeric_limits<int>::max(),-1};
      }

      for(int i=0; i<_c.getDim(); i++)                      // For each local curve
          _c[i]->updateMat();

      // for hver partisjon -
      for(uint i=0; i<_pre_basis.size(); i++) {                     // For each partition
          this->_visu[i].sur_sphere.reset();                        // Reset surounding sphere
          this->_visu[i].sample_val.resize(_pre_basis[i].size());   // Set number of sample points
          for(uint j=0; j<_pre_basis[i].size(); j++) {              // for hvert sample-punkt i partisjonen
              multEval( this->_visu[i].sample_val[j], _pre_basis[i][j].B, _pre_basis[i][j].ind, j, i);
              this->_visu[i].sur_sphere += this->_visu[i].sample_val[j][0];
              for(uint k=0; k<2; k++ ) {
                  int i_p = _pre_basis[i][j].ind[k];
                  if(int(j) < _cp_index[i_p][i][0]) _cp_index[i_p][i][0] = j;
                  if(int(j) > _cp_index[i_p][i][1]) _cp_index[i_p][i][1] = j;
              }
          }
      }
  }




  /*! void  PERBSCurve<T>::updatSamples() const
   *  \brief Private, not for public use
   *
   *  Update affected sample points for all partitions when some local curves has mover or rotated ...
   */
  template <typename T>
  void  PERBSCurve<T>::updatSamples() const {
      if(_local_change.size() > 0) {
          int k = _local_change.back();
          std::vector<CpIndex> ind;
          ind.resize(_pre_basis.size());
          for(uint i=0; i<ind.size(); i++) // for all partisions
            if(_cp_index[k][i][1]>=0)
                ind[i].push_back(_cp_index[k][i]);
          _c[k]->updateMat();
          _local_change.pop_back();
          while(!_local_change.empty()) { // for all changed curves
              k = _local_change.back();
              CpIndex lc_ind = _cp_index[k];
              for(uint i=0; i<ind.size(); i++){ // for all partisions
                  if(_cp_index[k][i][1]>=0) {
                      if(ind[i].getDim()==0)
                          ind[i].push_back(_cp_index[k][i]);
                      else {
                          int r = ind[i].getDim();
                          for(int j=0; j<r; j++) { //for all interval i partision
                              int s = isIntersecting(ind[i][j], lc_ind[i]);
                              if(s==0) ind[i].push_back(lc_ind[i]);
                              else if(s==2) ind[i][j] = lc_ind[i];
                              else if(s==3) ind[i][j][0] = lc_ind[i][0];
                              else if(s==4) ind[i][j][1] = lc_ind[i][1];
                          }
                      }
                  }
              }
              _c[k]->updateMat();
              _local_change.pop_back();
          }
          for(uint i=0; i<this->_visu.size(); i++) {  // for all partisions
              for(int j=0; j<ind[i].getDim(); j++){
                  for(int k = ind[i][j][0]; k <= ind[i][j][1]; k++){
                      multEval( this->_visu[i].sample_val[k], _pre_basis[i][k].B, _pre_basis[i][k].ind, k, i);
                  }
              }
          }
      }
  }






  /*!  void  makePartition( std::vector<Partition2<T>>& partition, const DVector<T>& t, int k, int n, int dis, int m, bool close = false )
   *  To make a partitioning and preevaluate basis functions of the sample points of a spline.
   *  We split the curve in partition based on continuity criteria, (continuity C^_pct)
   *  For each partition we find the parameter values for all sample points and put it into this->_visu[i][j],
   *      where i is the index of the partition and j is the index of the sample points in partition i.
   *  For each partition and sample point we also compute the B-spline Hermite matrix and put it into _pre_basis
   *      together with a vector of indices, _pre_basis[i][j].ind, of the control points that is conected to each colomn of the matrix.
   *
   *  \param[in]  m          the initial sugestion of the total number of sample points
   */
    template <typename T>
    inline
    void PERBSCurve<T>::makePartition( int m ) const {

        if(_type_local == SUB_CURVE) {
            this->_visu.resize((_origin->getSampler())->size());
            _pre_basis.resize((_origin->getSampler())->size());
            for(uint i=0; i<this->_visu.size(); i++) {
                this->_visu[i] = _origin->getSampleValues(i);
                _pre_basis[i].resize(this->_visu[i].size());

                for(uint j=0; j<_pre_basis[i].size()-1; j++) {
                    int k = EvaluatorStatic<T>::knotIndex(_t, this->_visu[i][j], 1, false);
                    IndexBsp ii( k, 2, _c.getDim());
                    _pre_basis[i][j].B = getB(this->_visu[i][j],k,2);
                    _pre_basis[i][j].ind[0] = ii[0];
                    _pre_basis[i][j].ind[1] = ii[1];
                }
                int k = EvaluatorStatic<T>::knotIndex(_t, this->_visu[i][_pre_basis[i].size()-1], 1, true);
                IndexBsp ii( k, 2, _c.getDim());
                _pre_basis[i][_pre_basis[i].size()-1].B = getB(this->_visu[i][_pre_basis[i].size()-1],k,2);
                _pre_basis[i][_pre_basis[i].size()-1].ind[0] = ii[0];
                _pre_basis[i][_pre_basis[i].size()-1].ind[1] = ii[1];
            }
        }
        else {
            VisPart<T> pu( _t, 2, _pct );
            SampNr<T>  su( _t, pu, m );
            this->_visu.resize(su.size());
            _pre_basis.resize(su.size());

            for(uint i=0; i<this->_visu.size(); i++) {
                computeUniformParamVal(this->_visu[i], su[i], _t[pu[2*i]], _t[pu[2*i+1]]);
                _pre_basis[i].resize(su[i]);

                for(int j=0; j<su[i]-1; j++) {
                    int k = EvaluatorStatic<T>::knotIndex(_t, this->_visu[i][j], 1, false);
                    IndexBsp ii( k, 2, _c.getDim());
                    _pre_basis[i][j].B = getB(this->_visu[i][j],k,2);
                    _pre_basis[i][j].ind[0] = ii[0];
                    _pre_basis[i][j].ind[1] = ii[1];
                }
                int k = EvaluatorStatic<T>::knotIndex(_t, this->_visu[i][su[i]-1], 1, true);
                IndexBsp ii( k, 2, _c.getDim());
                _pre_basis[i][su[i]-1].B = getB(this->_visu[i][su[i]-1],k,2);
                _pre_basis[i][su[i]-1].ind[0] = ii[0];
                _pre_basis[i][su[i]-1].ind[1] = ii[1];
            }
        }
    }





    template <typename T>
    inline
    void PERBSCurve<T>::prepareSampling( int d ) const {

       if(_type_local == SUB_CURVE) {
//         _origin->preSample(this->_visu);
//         for(int i=0,s=0,e=0; i < _c.getDim(); i++) {
//             for(int i=0; i< _t)
//             _c[i]->
//         }
       }
    }


    /*! void PERBSCurve<T>::multEval(DVector<Vector<T,3>>& p, const Vector<T,3>& B, const Vector<int,2>& ii, int j, int i) const
     *  Private, not for public use
     *  Partial matrix vector multiplication,
     *  only the d+1 first rows of the matrix,
     *  and the part of the vector _c (the control points) with the indices in int-vector ii.
     *  p = m * _c
     *
     *  \param[out]  p   Return value - The position and d derivatives
     *  \param[in]   p   The B-function value including derivatives
     *  \param[in]   ii  The index vector (size 2) to be used in _c
     *  \param[in]   j   The number of sample to compute
     *  \param[in]   i   The number of partition
     */
    template <typename T>
    inline
    void PERBSCurve<T>::multEval(DVector<Vector<T,3>>& p, const Vector<T,3>& B, const Vector<int,2>& ii, int j, int i) const {

        p = _c[ii[1]]->evaluateParent(j,i);
        compBlend( 1, B, _c[ii[0]]->evaluateParent(j,i), p );
    }





    template <typename T>
    inline
    int  PERBSCurve<T>::isIntersecting(const Vector<int,2>& a, const Vector<int,2>& b) const {

        if(b[1] < (a[0]-1) || b[0] > (a[1]+1))    return 0;
        if(a[0] <= b[0] && a[1] >= b[1])          return 1;
        if(b[0] <= a[0] && b[1] >= a[1])          return 2;
        if(b[0] < a[0])                           return 3;
        if(b[1] > a[1])                           return 4;
        else return -1;
    }





    //***************************************
    //**   Local (private) help functons   **
    //***************************************

  /*! void  PERBSCurve<T>::init(bool closed)
   *  \brief Private, not for public use
   *
   *  Default initiation for the constructors
   *
   *  \param[in]  closed   whether the curve is closed or not
   */
  template <typename T>
  void PERBSCurve<T>::init(bool closed) {

    this->_type_id = GM_SO_TYPE_CURVE_ERBS;
    _cl            = closed;
    _pct           = 0;
    _evaluator     = new ERBSEvaluator<long double>();
  }



} // END namespace GMlib


