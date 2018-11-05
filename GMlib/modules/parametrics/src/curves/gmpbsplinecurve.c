/**********************************************************************************
**
** Copyright (C) 1994 - 2016 University of Tromsø - The Arctic University of Norway
** Contact: GMlib Online Portal at https://source.uit.no/gmlib/gmlib/wikis/home
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modifyit under
** the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
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

/*! PBSplineCurve<T>::PBSplineCurve(const DVector< Vector<T,3> >& c, const DVector<T>& t, int d)
 *  Default constructor taking controll points, knot vector and optional a degree if the curve is closed
 *
 *  NB!! If the curve is open, then the number of knots is the number of controll points + the polynomial degree + 1.
 *  NB!! If the curve is closed, then the number of knots must be the number of controll points + 1.
 *    Note that usually the number of control points must be greater than the polynomial degree,
 *    but if a curve is closed then the number of control points may be smaller.
 *
 *  \param[in] c The controll points
 *  \param[in] t The knot vector
 *  \param[in] d The polynomial degree (Optional - must only be used if the curve is closed)
 */
template <typename T>
  inline
  PBSplineCurve<T>::PBSplineCurve(const DVector< Vector<T,3> >& c, const DVector<T>& t, int d):PCurve<T,3>(0,0,10) {

    init(d);
    _t = t;
    _c = c;

    _k = _t.getDim() - _c.getDim();
    if(_k == 1) {     // close
        _k = _d+1;
        DVector<T> u = _t;
        spesKnotClose( _t, _cl, u, _c.getDim(), _d);
    }
    else              // open
        _d = _k-1;
  }




  /*! PBSplineCurve<T>::PBSplineCurve(const DVector< Vector<T, 3> >& c, int d, bool closed)
   *  Default constructor taking controll points, a degree and a boolean about the curve is closed or open.
   *  The knot vector is generated automatically
   *    Note that usually the number of control points must be greater than the polynomial degree,
   *    but if a curve is closed then the number of control points may be smaller.
   *    So if the number of control points is less or equal the polynomial degree then the curve is
   *    set to closed independent of the value of the parameter "closed".
   *
   *  \param[in] c      The controll points
   *  \param[in] d      The polynomial degree
   *  \param[in] closed (default false) The curve is closed or open
   */
  template <typename T>
  inline
  PBSplineCurve<T>::PBSplineCurve(const DVector< Vector<T, 3> >& c, int d, bool closed):PCurve<T,3>(0,0,10) {

    if(c.getDim() <= d) closed = true;

    init(d);
    _c = c;
    generateKnotVector(closed);
  }




  /*! PBSplineCurve<T>::PBSplineCurve(const PCurve<T,3>& cp, int d, int n, int m)
   *  A constructor to aproximate a curve using least square
   *
   *  \param[in] cp The curve to try to copy
   *  \param[in] d  The polynomial degree of the new B-spline curve
   *  \param[in] n  The number of control points in the new B-spline
   *  \param[in] m  The number of points to use in least square computation
   */
  template <typename T>
  inline
  PBSplineCurve<T>::PBSplineCurve(const PCurve<T,3>& cp, int d, int n, int m):PCurve<T,3>(0,0,10) {

      init(d);
      _cl = cp.isClosed();
      _c.setDim(n);
      generateKnotVector(cp.getParStart(), cp.getParEnd(), _cl);
      std::vector<T> sample;
      computeUniformParamVal(sample, (_cl ? (m+1) : m), cp.getParStart(), cp.getParEnd());
      if(_cl) sample.pop_back();

      DVector<Vector<T, 3> >  p(m);
      for(int i=0;i<m; i++)
          p[i] = cp(sample[i]);
      DMatrix<T> A(m,n);
      makeLeastSquareMatrix(A, sample, d, n);
      DMatrix<T> AT = A;
      AT.transpose();
      DMatrix<T> AA = AT*A;
      AA.invert();
      DVector<Vector<T,3>> bb = AT*p;
      _c = AA*bb;
  }




  /*! PBSplineCurve<T>::PBSplineCurve(const PBSplineCurve<T>& copy )
   *  A copy constructor
   *  Making a copy of a B-spline curve
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  inline
  PBSplineCurve<T>::PBSplineCurve(const PBSplineCurve<T>& copy ) : PCurve<T,3>(copy) {

      init(copy._d);
      _c = copy._c;
      _t = copy._t;
  }




  /*! PBSplineCurve<T>::~PBSplineCurve()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T>
  PBSplineCurve<T>::~PBSplineCurve() {

      this->cleanVisualizers(0);
      hideSelectors();
      if(_sgv) delete _sgv;
  }



  //*********************************
  //**   Public local functons     **
  //*********************************


  /*! void PBSplineCurve<T>::setDegree( int d )
   *  Will change the polynomial degree if possible
   *  Note that this is not degree raising, we only make a new knot vector
   *  based on the old one, and we keep the control points unchanged
   *
   *  \param[in] d      The polynomial degree
   */
  template <typename T> // NB! - må/bør testes
  void PBSplineCurve<T>::setDegree( int d ) {

    if( (!isClosed() && d >= _c.getDim()) || d == _d)
      return;

    // Update knot vector !!
    if(isClosed())
        updateKnotClosed(d);
    else
        updateKnotOpen(d);
  }



  /*! int PBSplineCurve<T>::getDegree() const
   *  Give you the polynomial degree of the curve
   *
   *  \return       The polynomial degree
   */
  template <typename T>
  inline
  int PBSplineCurve<T>::getDegree() const {
    return _d;
  }




  /*! void PBSplineCurve<T>::setClosed( bool closed, T dt)
   *  \brief To make the curve closed
   *
   *  To make a closed curve, the knot vector is also automatical changed
   *  If you skip the parameter dt then it will be generated.
   *  If you give a value to the parameter dt it will affect the speed on the curve over the joints between the end and start.
   *
   *  Note that you have to call:
   *            sample( int m, int d ) after calling this function.
   *
   *  \param[in]  closed     The polynomial degree
   *  \param[in]  dt         To give a value between the knots at start and end (optional)
   */
  template <typename T>
  void PBSplineCurve<T>::setClosed( bool closed, T dt) {

      if(_cl != closed) {
          int n = _c.getDim();
          DVector<T> nt = _t;
          if(closed) {
              if(dt==T(0)) dt = EvaluatorStatic<T>::knotInterval(nt, _d, n);
              transKnotClose(_t, _cl, nt, n, _d, dt);
          } else
              transKnot(_t, _cl, nt, n, _d);

          if( this->_derived ) this->_derived->edit( this );
      }
  }



  /*! void PBSplineCurve<T>::setControlPoints( const DVector< Vector<T,3> >& c, bool gen_t )
   *  \brief To change the controll points of the curve
   *
   *  To replace the controll points with a new set
   *  If you skip the parameter gen_t then a new knot vector will be generated.
   *  if get_t is set to false then you have to make a new knot vector
   *  by either calling setKnotVector() or generateKnotVector()
   *
   *  Note that you have to call:
   *            sample( int m, int d ) after calling this function.
   *
   *  \param[in]  c       The new controll points
   */
  template <typename T>
  void PBSplineCurve<T>::setControlPoints( const DVector< Vector<T,3> >& c ) {

      if( _c.getDim() != c.getDim() )
          if(!isClosed() && _k > c.getDim() ) { // Adjust degree if to high.
              _k = _c.getDim();
              _d = _k-1;
          }
      _c = c;
  }



  /*! void PBSplineCurve<T>::setKnotVector( const DVector< T >& t, bool closed )
   *  \brief To set a new knot vector of the curve
   *
   *  To replace the knot vector with a new.
   *  If you skip the parameter closed then the curve will be open.
   *  if closed is set to true then you get a closed curve
   *
   *  Note that you have to call:
   *            sample( int m, int d ) after calling this function.
   *
   *  \param[in]  t       The new knot vector
   *  \param[in]  closed  (default false) closed or open curve
   */
  template <typename T>
  void PBSplineCurve<T>::setKnotVector( const DVector< T >& t, bool closed ) {

      if(closed) {
          if(t.getDim() != (_c.getDim() + _k + _d)) {
              std::cerr << " Invalid size of closed knot vector" << std::endl;
              return;
          }
      } else  if(t.getDim() != (_c.getDim() + _k)) {
          std::cerr << " Invalid size of open knot vector" << std::endl;
          return;
      }

      for( int i = 1; i < t.getDim(); i++ )
          if( t(i) < t(i-1) ) {
              std::cerr << " Invalid knot vector, not increasing (monoton) " << std::endl;
              return;
          }

      _t = t;
      _cl = closed;
  }



  /*! void PBSplineCurve<T>::setPartitionCriterion(int pct)
   *  \brief To set criterion for partitions of the curve
   *
   *  To set a criterion for how to split the curve into several partitions
   *  This can ensure discontinuities at different levels are displayed
   *
   *  Note that you have to call:
   *            sample( int m, int d ) after changing the criterion.
   *
   *  \param[in]  pct  partition criterie
   *              pct = 0 - split where we have a discontinue function
   *              pct = 1 - split where we have a discontinue 1st derivative
   *              pct = 2 - split where we have a discontinue 2nd derivative ...
   *              pct   must not be bigger than the polynomial degree !!!
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::setPartitionCriterion(int pct) {
      _pct = pct;
  }



  /*! const DVector< Vector<T,3> >& PBSplineCurve<T>::getControlPoints() const
   *  To get the control points
   *
   *  \return    The control points
   */
  template <typename T>
  inline
  const DVector<Vector<T,3>>& PBSplineCurve<T>::getControlPoints() const {
    return _c;
  }




  /*! const DVector< Vector<T,3> >& PBSplineCurve<T>::getKnotVector() const
   *  To get the knot vector
   *
   *  \return The knot vector
   */
  template <typename T>
  inline
  const DVector<T>& PBSplineCurve<T>::getKnotVector() const {
    return _t;
  }




  //********************************************************
  // Overrided (public) virtual functons from SceneObject **
  //********************************************************


  // This function is not meant for public use

  /*! void PBSplineCurve<T>::edit( int selector_id, Vector<T,3> dp )
   *  Not for public use
   *  To take action when a selector has been moved
   *  therefor, this function is not meant for public use
   *
   *  \param[in] selector_id   The identity of the selected object
   *  \param[in] dp            The translation vector that the object has been moved
   */
  template <typename T>
  void PBSplineCurve<T>::edit( int selector_id, const Vector<T,3>& dp ) {

    _c_moved = true;
       if( this->_parent ) this->_parent->edit( this );
       if( this->_derived ) this->_derived->edit( this );
       _pos_change.push_back(EditSet(selector_id, dp));
       this->setEditDone();
    _c_moved = false;
  }




  // This function is not meant for public use
  /*! void PBSplineCurve<T>::replot()
   *  Not for public use
   *  To replot after object editing or shape changing else,
   *  therefor, this function is not meant for public use
   */
  template <typename T>
  void PBSplineCurve<T>::replot() const {

      updatSamples();
      PCurve<T,3>::replot();
  }




  //**************************************************
  // Overrided (public) virtual functons from PCurve **
  //**************************************************


  /*! void PBSplineCurve<T>::sample( int m, int d )
   *  To sample and plot the curve.
   *
   *  \param[in] m  The number of sample to make
   *  \param[in] d  The number of derivatives to compute
   */
  template <typename T>
  void PBSplineCurve<T>::sample( int m, int d ) {

      this->_checkSampleVal( m, d );

      for(unsigned int i=1; i < this->_visu.size(); i++)
          this->cleanVisualizers(i);
      makePartition( m );
      preSample(d);
      this->prepareVisualizers();
      this->setEditDone();
  }



  /*! bool PBSplineCurve<T>::isClosed() const
   *  To see if the curve is closed or not.
   */
  template <typename T>
  bool PBSplineCurve<T>::isClosed() const {
    return _cl;
  }



  /*! void PBSplineCurve<T>::showSelectors( T rad, bool grid, const Color& selector_color, const Color& grid_color )
   *  To generate the selecors and optionally the grid.
   *  The frace (optional, d) below means that if you skip the parameter,
   *  the default value is d.
   *
   *  \param[in]  rad - (optional, 1) Radius of the selectors
   *  \param[in]  grid - (optional, true)  Do we plot the grid also?
   *  \param[in]  selector_color (optional, darkBlue) Color of the selectors
   *  \param[in]  grid_color (optional, lightGreen) Color of the grid
   */
  template <typename T>
  void PBSplineCurve<T>::showSelectors( T rad, bool grid, const Color& selector_color, const Color& grid_color ) {

      if( !_selectors ) {
          _s.resize( _c.getDim() );

          for( int i = 0; i < _c.getDim(); i++ )
              if(this->isScaled())
                  this->insert( _s[i] = new Selector<T,3>( _c[i], i, this, this->_scale.getScale(), rad, selector_color ) );
              else
                  this->insert( _s[i] = new Selector<T,3>( _c[i], i, this, rad, selector_color ) );
          _selectors       = true;
      }
      _selector_radius = rad;
      _selector_color  = selector_color;

      if( grid ) {
          if(!_sgv) _sgv = new SelectorGridVisualizer<T>;
          _sgv->setSelectors( _c, 0, _cl);
          _sgv->setColor( grid_color );
          SceneObject::insertVisualizer( _sgv );
          this->setEditDone();
      }
      _grid_color      = grid_color;
      _grid            = grid;
  }



  /*! void PBSplineCurve<T>::hideSelectors()
   *  To hide and delete selectors and hide and reset the selector grid
   */
  template <typename T>
  void PBSplineCurve<T>::hideSelectors() {

      // Remove Selectors
      if( _selectors ) {
          for( unsigned int i = 0; i < _s.size(); i++ ) {
              this->remove( _s[i] );
              delete _s[i];
          }
          _s.clear();
          _selectors = false;
      }

      // Hide Selector Grid
      if(_sgv) {
        SceneObject::removeVisualizer( _sgv );
        _sgv->reset();
      }
  }





  /*! void PBSplineCurve<T>::toggleSelectors()
   *  To toggle the selectors and selector grid.
   */
  template <typename T>
  void PBSplineCurve<T>::toggleSelectors() {

    if(_selectors)  hideSelectors();
    else            showSelectors(_selector_radius, _grid, _selector_color, _grid_color);
  }




  /*! void PBSplineCurve<T>::toggleClose()
   *  To toggle open curve - closed curve
   */
  template <typename T>
  void PBSplineCurve<T>::toggleClose() {

      if(!isClosed() && _k >_c.getDim()) return;

      if(isClosed())  setClosed( false );
      else            setClosed( true );

      this->cleanVisualizers();
      sample( this->_visu.no_sample, this->_visu.no_derivatives );

      if(_selectors) {
          hideSelectors();
          showSelectors(_selector_radius, _grid, _selector_color, _grid_color);
      }
      this->setEditDone();
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************


  /*! void PBSplineCurve<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (default true) To evauate from left or from right
   */
  template <typename T>
  void PBSplineCurve<T>::eval( T t, int d, bool l ) const {

      // Make the B-spline Hermite matrix
      DMatrix<T> bsp;
      int idx = EvaluatorStatic<T>::evaluateBSp( bsp, t, _t, _d);
      IndexBsp ind(idx, _k, _c.getDim());
      multEval(this->_p, bsp, ind, d);
  }



  /*! T PBSplineCurve<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PBSplineCurve<T>::getStartP() const {
    return _t(_d);
  }



  /*! T PCircle<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PBSplineCurve<T>::getEndP() const {
    return _t(_t.getDim()-_k);
  }




  //*****************************************
  //     Local (protected) help functons   **
  //*****************************************


  /*! void PBSplineCurve<T>::transKnot( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d)
   *  Private, not for public use
   *  To make an open knot vector from an old "closed" knot vector
   *
   *  \param[out]  t  Return value - The knot vector to maken
   *  \param[out]  c  Return value - set curve to open (return false)
   *  \param[in]   g  Old knot vector to be used in the computation
   *  \param[in]   n  The number of control points
   *  \param[in]   d  The polynomial degree
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::transKnot( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d) {

      if(_c.getDim() <= d ) {
          std::cerr << " There is too few control points for this polynomial degree" << std::endl;
          return;
      }
      t.setDim(g.getDim()-d);
      for(int i = d; i <= n; i++)            t[i] = g(i);
      for(int i = d-1; i >= 0; i--)          t[i] = t[i+1];
      for(int i = n+1; i < t.getDim(); i++)  t[i] = t[i-1];
      c = false;
  }




  /*! void PBSplineCurve<T>::transKnotClose( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d, T dt)
   *  Private, not for public use
   *  To make a closed knot vector from an old knot vector
   *  If dt is 0, then we use
   *
   *  \param[out]  t  Return value - The knot vector to maken
   *  \param[out]  c  Return value - set curve to closed (return true)
   *  \param[in]   g  Old knot vector to be used in the computation
   *  \param[in]   n  The number of control points
   *  \param[in]   d  The polynomial degree
   *  \param[in]   dt (default 0) The interval between the knots at ends
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::transKnotClose( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d, T dt) {

      int k = d+1;
      t.setDim(n+k+d);
      if(dt > T(0)) {
          for(int i = d; i < n+1; i++)              t[i] = g(i);
          for(int i = n+1; i < n+k; i++)            t[i] = t[i-1] + dt;
      } else
          for(int i = d; i < n+k; i++)              t[i] = g(i);

      for(int i=d-1, j=n+d; i >= 0; i--,j--)        t[i] = t[i+1] - (t[j]-t[j-1]);
      for(int i=n+k, j=k; i < t.getDim(); i++,j++)  t[i] = t[i-1] + (t[j]-t[j-1]);
      c = true;
  }




  /*! void PBSplineCurve<T>::spesKnotClose( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d)
   *  Private, not for public use
   *  To make a closed knot vector from a knot vector that describes the distance between each intervall
   *
   *  \param[out]  t  Return value - The knot vector to maken
   *  \param[out]  c  Return value - set curve to closed (return true)
   *  \param[in]   g  The vector describing intervalls (size -- number of control points + 1)
   *  \param[in]   n  The number of control points
   *  \param[in]   d  The polynomial degree
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::spesKnotClose( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d) {

      t.setDim(n+d+d+1);
      for(int i=d; i <= n+d; i++)                      t[i] = g(i-d);
      for(int i=d-1, j=n+d; i >= 0; i--,j--)           t[i] = t[i+1] - (t[j]-t[j-1]);
      for(int i=n+d+1, j=d+1; i < t.getDim(); i++,j++) t[i] = t[i-1] + (t[j]-t[j-1]);
      c = true;
  }





  /*! void PBSplineCurve<T>::updateKnotClosed(int d)
   *  Private, not for public use
   *  To update a open knot vector from an old knot vector when polynomial degree is changed
   *
   *  \param[in]  d  The new polynomial degree
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::updateKnotClosed(int d) {

      int           n = _c.getDim();
      DVector<T>    t = _t;

      _t.setDim(n+d+d+1);
      int dd = d - _d;
      if(dd<0)
          for(int i=0; i < _t.getDim(); i++)               _t[i] = t[i-dd];
      else {
          for(int i = dd; i < t.getDim()+dd; i++)          _t[i] =  t[i-dd];
          for(int i = dd-1, j = t.getDim()+dd; i >= 0; i--, j++) {
              _t[i] = _t[i+1] - (_t[n+i+1] - _t[n+i]);
              _t[j] = _t[j-1] + (_t[j-n] - _t[j-n-1]);
          }
      }
      _d = d;
      _k = d+1;
  }





  /*! void PBSplineCurve<T>::updateKnotOpen(int d)
   *  Private, not for public use
   *  To update a closed knot vector from an old knot vector when polynomial degree is changed
   *
   *  \param[in]  d  The new polynomial degree
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::updateKnotOpen(int d) {

      int           n = _c.getDim();
      DVector<T>    t = _t;

      _t.setDim(n+d+1);
      int i=0, j=_d;
      for( ; i <= d; i++)        _t[i]   = t.front();
      for( ; j < n+_d-d-1; j++)  _t[i++] = t.getSum(j,j+d)/d;
      for( j=0; j <= d; j++)     _t[i++] = t.back();
      _d = d;
      _k = d+1;
  }





  /*! void PBSplineCurve<T>::comp(DVector<Vector<T,3>>& p, const DMatrix<T>& m, const Vector<T,3>& c, int k) const
   *  Private, not for public use
   *  Partial vector-matrix computation, ie. actually a vector-vector innerproduct.
   *  where vi compute a vector vith a column vector (index k in the matrix m))
   *  p = v * B_column[k]
   *
   *  \param[out]  p  Updating the position and d derivatives
   *  \param[in]   B  The B-spline Hermite matrix to compute the vector c with.
   *  \param[in]   c  The distance vector one control point has been moved
   *  \param[in]   k  the index of the column to compute with, and also the index of the control point that has been moved
   */
  template <typename T>
  inline
  void  PBSplineCurve<T>::comp(DVector<Vector<T,3>>& p, const DMatrix<T>& B, const Vector<T,3>& c, int k) const {

      for(int i=0; i<p.getDim(); i++)
          p[i] += B(i)(k)*c;
  }




  /*! void PBSplineCurve<T>::multEval(DVector<Vector<T,3>>& p, const DMatrix<T>& b, const std::vector<int>& ii, int d) const
   *  Private, not for public use
   *  Partial matrix vector multiplication,
   *  only the d+1 first rows of the matrix,
   *  and the part of the vector _c (the control points) with the indices in int-vector ii.
   *  p = m * _c
   *
   *  \param[out]  p   Return value - The position and d derivatives
   *  \param[in]   p   The B-spline Hermite matrix to compute the control points _c with
   *  \param[in]   ii  The index vector (size _k - order) to be used in _c
   *  \param[in]   d   The number of derivatives to compute
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::multEval(DVector<Vector<T,3>>& p, const DMatrix<T>& B, const std::vector<int>& ii, int d) const {

      p.setDim(d+1);
      // p = B * c;
      // We do this manually because we only want to compute the d first rows of b.
      for(int i=0; i<=d; i++) {
          p[i] = B(i)(0)*_c[ii[0]];
          for(int k=1; k<_k; k++)
             p[i] += B(i)(k)*_c[ii[k]];
      }
  }






  /*! void  PBSplineCurve<T>::preSample( int d )
   *  Private, not for public use
   *  Compute all sample points for all partitions
   *
   *  \param[in]  d   The number of derivatives to compute
   */
  template <typename T>
  void  PBSplineCurve<T>::preSample( int d ) {

      _cp_index.resize(_c.getDim());
      for(unsigned int i=0; i < _cp_index.size(); i++) {
          _cp_index[i].setDim(_pre_basis.size());
          for(int j=0; j < _cp_index[i].getDim(); j++)
              _cp_index[i][j] = {std::numeric_limits<int>::max(),-1};
      }

      for(unsigned int i=0; i<_pre_basis.size(); i++) {
          this->_visu[i].sur_sphere.reset();
          this->_visu[i].sample_val.resize(_pre_basis[i].size());
          for(unsigned int j=0; j<_pre_basis[i].size(); j++) {
              multEval( this->_visu[i].sample_val[j], _pre_basis[i][j], _pre_basis[i][j].ind, d);
              this->_visu[i].sur_sphere += this->_visu[i].sample_val[j][0];
              for(unsigned int k=0; k<_pre_basis[i][j].ind.size(); k++ ) {
                  int i_p = _pre_basis[i][j].ind[k];
                  if(int(j) < _cp_index[i_p][i][0]) _cp_index[i_p][i][0] = j;
                  if(int(j) > _cp_index[i_p][i][1]) _cp_index[i_p][i][1] = j;
              }
          }
      }
  }





  /*! void  PBSplineCurve<T>::updatSamples() const
   *  Private, not for public use
   *  Update affected sample points for all partitions when some control points have been moved
   */
  template <typename T>
  void  PBSplineCurve<T>::updatSamples() const {

      while(_pos_change.size()>0) {
          EditSet es = _pos_change.back();
          for(int i=_pos_change.size()-2; i>=0; i--)
              if (_pos_change[i].ind == es.ind) {
                  es.dp += _pos_change[i].dp;
                  _pos_change.erase(_pos_change.begin()+i);
              }
          for(unsigned int i=0; i<this->_visu.size(); i++)
              for(int j =_cp_index[es.ind][i][0]; j <= _cp_index[es.ind][i][1]; j++)
                  for(unsigned int k=0; k < _pre_basis[i][j].ind.size(); k++)
                      if(_pre_basis[i][j].ind[k] == es.ind)
                          comp(this->_visu[i].sample_val[j], _pre_basis[i][j], es.dp, k);
          _pos_change.pop_back();
      }
  }





  /*!  void  makePartition(  int m ) const
   *  Private, not for public use
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
    void PBSplineCurve<T>::makePartition( int m ) const {

        VisPart<T> pu( _t, _k, _pct );
        SampNr<T>  su( _t, pu, m );
        this->_visu.resize(su.size());
        _pre_basis.resize(su.size());

        for(unsigned int i=0; i<this->_visu.size(); i++) {
            computeUniformParamVal(this->_visu[i], su[i], _t[pu[2*i]], _t[pu[2*i+1]]);
            _pre_basis[i].resize(su[i]);

            for(int j=0; j<su[i]-1; j++)
                _pre_basis[i][j].ind.init( EvaluatorStatic<T>::evaluateBSp( _pre_basis[i][j], this->_visu[i][j], _t, _d, false), _k, _c.getDim() );
            _pre_basis[i][su[i]-1].ind.init( EvaluatorStatic<T>::evaluateBSp( _pre_basis[i][su[i]-1], this->_visu[i][su[i]-1], _t, _d, true), _k, _c.getDim() );
        }
    }





  /*!  void PBSplineCurve<T>::makeLeastSquareMatrix( DMatrix<T>& A, const std::vector<T>& sample, int d, int n) const
   *  Private, not for public use
   *  To make a least square matrix to compute the coeffisients.
   *
   *  \param[out] A          the least square matrix
   *  \param[in]  sample     the sample points (parameter values)
   *  \param[in]  d          the polynomial degree of the splines
   *  \param[in]  n          the number of control points to make
   */
   template <typename T>
   inline
   void PBSplineCurve<T>::makeLeastSquareMatrix( DMatrix<T>& A, const std::vector<T>& sample, int d, int n) const {

       for(int i=0; i < A.getDim1(); i++) {
           DMatrix<T> bsp(d+1,d+1,T(0));
           int idx = EvaluatorStatic<T>::evaluateBSp( bsp, sample[i], _t, d);
           IndexBsp ind(idx, d+1, n);
           for(int j=0; j < A.getDim2(); j++)
               A[i][j] = T(0);
           for(uint j=0; j < ind.size(); j++)
               A[i][ind[j]] = bsp[0][j];
       }
   }






  /*! void PBSplineCurve<T>::generateKnotVector( T s, T e, bool closed )
   *  Private, not for public use
   *  To generate a new knot vector.
   *  The function generate a knot vector where the domain start at "s" and end at "e".
   *  The number of control poits and the polynomial degree is taken from the class
   *
   *  \param[in]  s       start parameter value t[d]
   *  \param[in]  e       end parameter value t[n]
   *  \param[in]  closed  closed or open curve
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::generateKnotVector( T s, T e,  bool closed ) {

      if(closed) {
          _t.setDim( _c.getDim() + _k + _d);
          T dt = (e-s)/_c.getDim();

          for(int i = 0; i < _t.getDim(); i++ )
              _t[i] = s+(i-_d)*dt;
      }
      else {
          _t.setDim( _c.getDim() + _k );
          T dt = (e-s)/(_c.getDim()-_d);

          int step = _t.getDim() - (2 * _k);
          int i = 0;
          for( ; i < _k; i++ )              // Set the start knots
              _t[i] = s;
          for( int j = 1; j <= step; j++ )  // Set the "step"-knots
              _t[i++] = s+j*dt;
          for( ; i < _t.getDim(); i++ )     // Set the end knots
              _t[i] = e;
      }
      _cl = closed;
  }





  /*! void PBSplineCurve<T>::generateKnotVector( bool closed )
   *  Private, not for public use
   *  To generate a new knot vector.
   *  If you skip the parameter "closed", then the value will be false (open curve).
   *  The function generate a regular knot vector where the numbers are integers.
   *  The number of control poits and the polynomial degree is taken from the class
   *
   *  Note that you have to call:
   *            sample( int m, int d ) after calling this function.
   *
   *  \param[in]  closed  (default false) closed or open curve
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::generateKnotVector( bool closed ) {

      if(closed) {
          _t.setDim( _c.getDim() + _k + _d);

          for(int i = 0; i < _t.getDim(); i++ )
              _t[i] = T(i-_d);
      }
      else {
          _t.setDim( _c.getDim() + _k );

          int step = _t.getDim() - (2 * _k);
          int i = 0;
          for( ; i < _k; i++ )              // Set the start knots
              _t[i] = T(0);
          for( int j = 1; j <= step; j++ )  // Set the "step"-knots
              _t[i++] = T(j);
          for( ; i < _t.getDim(); i++ )     // Set the end knots
              _t[i] = T(step+1);
      }
      _cl = closed;
  }




 //***************************************
  //**   Local (private) help functons   **
  //***************************************

  /*! void  PBSplineCurve<T>::init(int d)
   *  Private, not for public use
   *  Default initiation for the constructors
   *
   *  \param[in]  d   The polynomial degree of the curve
   */
  template <typename T>
  inline
  void PBSplineCurve<T>::init(int d) {

      this->_type_id = GM_SO_TYPE_CURVE_BSPLINE;
      _d            = d;
      _k            = d+1;
      _selectors    = false;
      _sgv          = 0x0;
      _pct          = 1;
      _cl           = false;

      _selector_radius = T(1);
      _grid            = true;
      _selector_color  = GMcolor::darkBlue();
      _grid_color      = GMcolor::lightGreen();
  }

} // END namespace GMlib


