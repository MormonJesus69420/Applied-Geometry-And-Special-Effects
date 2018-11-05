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
//#include <scene/visualizers/gmselectorgridvisualizer.h>
#include <scene/selector/gmselector.h>

namespace GMlib {



//*****************************************
// Constructors and destructor           **
//*****************************************

/*! PBezierCurve<T>::PBezierCurve(const DVector< Vector<T, 3> >& c )
 *  Constructor taking controll points
 *  Making an ordinary Bezier curve from a set of control points
 *
 *  \param c The controll points
 */
  template <typename T>
  inline
  PBezierCurve<T>::PBezierCurve( const DVector<Vector<T,3>>& c ):PCurve<T,3>(0,0,10){

    init();
    setControlPoints(c);
  }




  /*! PBezierCurve<T>::PBezierCurve(const DVector< Vector<T, 3> >& g,  T s, T t, T e)
   *  Constructor taking controll points, parameter start value, end valu and center valuee
   *  Making a Bezier curve from taylor expansion of g - a vector of position and d derivatives
   *  and it follows that d will be the polynomial degree of the Bezier curve.
   *  The domain of the curve is set to [s, e]
   *  The local coordinate system is organized such that c(t) = g(0) is the origin.
   *
   *  \param g A position and d-derivatives at the center point c(t),
   *  \param s The domain start (start parameter value), c(s) - the start of the curve
   *  \param t The center parameter value: c(t) = g[0], also origin in local coordinate system
   *  \param e The domain end (end parameter value), c(e) - the end of the curve
   */
  template <typename T>
  inline
  PBezierCurve<T>::PBezierCurve( const DVector< Vector<T, 3> >& g, T s, T t, T e ):PCurve<T,3>(0,0,10) {

    init();
    this->setDomain(s, e);

    // Generate the control points
    DMatrix<T> bhp;
    EvaluatorStatic<T>::evaluateBhp( bhp, g.getDim()-1, this->_map(t), T(1)/(e-s) );
    bhp.invert();
    _c = bhp * g;

    // Set origin in local coordinate system to c(t) = g(0)
    for( int i = 0; i < g.getDim(); i++ )
      _c[i] -= g[0];
    this->translateParent( g[0] );

//    std::cout << "CP:" << _c << std::endl;
//    std::cout << "g:" << g << std::endl;
  }



  /*! PBezierCurve<T>::PBezierCurve(const PBezierCurve<T>& copy )
   *  Copy constructor, to making a copy of the Bezier curve
   *
   *  \param copy The curve to copy
   */
  template <typename T>
  inline
  PBezierCurve<T>::PBezierCurve( const PBezierCurve<T>& copy ) : PCurve<T,3>( copy ) {

    init();
    _c = copy._c;
  }




  /*! PBezierCurve<T>::~PBezierCurve()
   *  Virtual destructor
   *  Cleaning memory
   */
  template <typename T>
  PBezierCurve<T>::~PBezierCurve() {

    hideSelectors();
    if(_sgv) delete _sgv;
  }




  //*********************************
  //**   Public local functons     **
  //*********************************


  /*! int PBezierCurve<T>::getDegree() const
   *  Return the degree of the curve
   *
   *  \return The polynomial degree of the curve (int)
   */
  template <typename T>
  inline
  int PBezierCurve<T>::getDegree() const {
    return _c.getDim() - 1;
  }




  /*! bool PBezierCurve<T>::isSelectorsVisible() const
   *  Telling you if the selectors are visible
   *
   *  \return Is the selectors visible? (bool)
   */
  template <typename T>
  bool PBezierCurve<T>::isSelectorsVisible() const {
    return _selectors;
  }




  /*! void PBezierCurve<T>::updateCoeffs( const Vector<T,3>& d )
   *  !!!!!!! Must be investigated
   *
   *  \param  d Vector of movement
   */
  template <typename T>
  inline
  void PBezierCurve<T>::updateCoeffs( const Vector<T,3>& d ) {

    if( _c_moved ) {

      HqMatrix<T,3> invmat = this->_matrix;
      invmat.invertOrthoNormal();

      Vector<T,3> diff = invmat*d;
      for( int i = 0; i < _c.getDim(); i++ ) {
        _c[i] += diff;
        _s[i]->translateParent( diff );
      }

      this->translateParent( -d, false );
      this->setEditDone(true);
    }
  }





  /*! void PBezierCurve<T>::setControlPoints( const DVector< Vector<T,3> >& c )
   *  To replace the controll points of the curve with a new set
   *
   *  Note that you have to call:
   *            sample( int m, int d ) after calling this function.
   *
   *  \param  c      The new controll points
   */
  template <typename T>
  inline
  void PBezierCurve<T>::setControlPoints( const DVector< Vector<T,3> >& c ) {

    _c = c;

    if(_selectors) {
        hideSelectors();
        showSelectors( _selector_radius, _grid, _selector_color, _grid_color );
    }
  }




  /*!  const DVector< Vector<T,3> >& PBezierCurve<T>::getControlPoints() const
   *  To get the controll points, only for read (not wright)
   *
   *  \return    The controll points
   */
  template <typename T>
  inline
  const DVector< Vector<T,3> >& PBezierCurve<T>::getControlPoints() const {
    return _c;
  }





  //********************************************************
  // Overrided (public) virtual functons from SceneObject **
  //********************************************************


  // This function is not meant for public use

  /*! void PBezierCurve<T>::edit( int selector_id, Vector<T,3> dp )
   *  Not for public use,
   *  For the system to take action when a selector has been moved
   *
   *  This function is not meant for public use
   *
   *  \param _id     The identity of the selected object
   *  \param dp      The translation vector that the object has been moved
   */
  template <typename T>
  void PBezierCurve<T>::edit( int selector_id, const Vector<T,3>& dp ) {

    _c_moved = true;
      if( this->_parent ) this->_parent->edit( this );
      _pos_change.push_back(EditSet(selector_id, dp));
      this->setEditDone(true);
    _c_moved = false;
  }



  // This function is not meant for public use

  /*! void PBezierCurve<T>::replot() const
   *  Not for public use,
   *  For the system to replot after object editing.
   *  We update sampling when control points has been moved.
   *
   */
  template <typename T>
  void PBezierCurve<T>::replot() const {

      updateSamples();
      PCurve<T,3>::replot();
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  /*! void PBezierCurve<T>::sample( int m, int d )
   *  To sample and plot the curve.
   *
   *  \param  m  The number of sample to make
   *  \param  d  The number of derivatives to compute
   */
  template <typename T>
  void PBezierCurve<T>::sample( int m, int d ) {

     // Make new pre-evaluation if necessary
     if( m != this->_visu.no_sample && m > 1) {
         this->makeUniformSampleValues(this->_visu[0], m);
         makeBernsteinMat(m, _c.getDim()-1, this->_sc);
     }

     // Corrigate number of sampler and derivative if nessesary
     this->_checkSampleVal( m, d );

     // Re-sample data (points, related derivatives and a surrounding sphere)
     reSample(d, this->_visu[0].sur_sphere);
     this->setEditDone();
  }




  /*! void PBezierCurve<T>::showSelectors( T rad, bool grid, const Color& selector_color, const Color& grid_color )
   *  To generate the selecors and optionally the grid.
   *
   *  The frace (optional, d) below means that if you skip the parameter,
   *  the default value is d.
   *
   *  \param  rad - (optional, 1) Radius of the selectors
   *  \param  grid - (optional, true)  Do we plot the grid also?
   *  \param  selector_color (optional, darkBlue) Color of the selectors
   *  \param  grid_color (optional, lightGreen) Color of the grid
   */
  template <typename T>
  void PBezierCurve<T>::showSelectors( T rad, bool grid, const Color& selector_color, const Color& grid_color ) {

      if(!_selectors) {
          _s.resize(_c.getDim());

          for( int i = 0; i < _c.getDim(); i++ )
              if(this->isScaled())
                  this->insert( _s[i] = new Selector<T,3>(_c[i], i, this, this->_scale.getScale(), rad, selector_color));
              else
                  this->insert( _s[i] = new Selector<T,3>(_c[i], i, this, rad, selector_color));
          _selectors       = true;
      }
      _selector_radius = rad;
      _selector_color  = selector_color;

      if(grid) {
          if(!_sgv) _sgv = new SelectorGridVisualizer<T>;
          _sgv->setSelectors( _c, 0);
          _sgv->setColor( grid_color );
          SceneObject::insertVisualizer( _sgv );
          this->setEditDone();
      }
      _grid_color      = grid_color;
      _grid            = grid;
  }




  /*! void PBezierCurve<T>::hideSelectors()
   *  To remove the selectors and selector grid.
   */
  template <typename T>
  void PBezierCurve<T>::hideSelectors() {

    // Remove Selectors
    if( _selectors ) {
        for( uint i = 0; i < _s.size(); i++ ) {
            this->remove( _s[i] );
            delete _s[i];
        }
        _s.clear();
        _selectors = false;
    }

    // Remove Selector Grid
    if(_sgv) {
      SceneObject::removeVisualizer( _sgv );
      _sgv->reset();
      _grid = false;
    }
  }





/*! void PBezierCurve<T>::toggleSelectors()
 *  To toggle the selectors and selector grid.
 */
template <typename T>
void PBezierCurve<T>::toggleSelectors() {

  if(_selectors)  hideSelectors();
  else            showSelectors();
}




//*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************


  /*! void PBezierCurve<T>::eval( T t, int d ) const
   *  Protected,
   *  Evaluation of the curve at a given parameter value
   *  in intrinsiq coordinates.
   *
   *  \param  t  The parameter value to evaluate at
   *  \param  d  The number of derivatives to compute
   */
  template <typename T>
  void PBezierCurve<T>::eval( T t, int d, bool /*l*/ ) const {

    // Compute the Bernstein-Hermite Polynomials
    DMatrix< T > bhp;
    EvaluatorStatic<T>::evaluateBhp( bhp, getDegree(), this->_map(t), 1/this->_sc );

    multEval(this->_p, bhp, d);
  }




  /*! T PBezierCurve<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *  (the start parameter value = 0).
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PBezierCurve<T>::getStartP() const {
    return T(0);
  }




  /*! T PBezierCurve<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *  (the end parameter value = 1).
   *
   *  \return The parametervalue at end of the intrinsic domain
   */
  template <typename T>
  T PBezierCurve<T>::getEndP() const {
    return T(1);
  }




  //*****************************************
  //     Local (protected) help functons   **
  //*****************************************


  /*! void PBezierCurve<T>::reSample( int d, Sphere<T,3>& s ) const
   *  Protected,
   *  Computing all sample points including d derivatives
   *  for each point. Also computing the surrounding sphere
   *
   *  \param   d    The polynomial degree
   *  \return  s    The surrounding sphere to be updated
   */
  template <typename T>
  void  PBezierCurve<T>::reSample( int d, Sphere<T,3>& s ) const {

      std::vector<DVector<Vector<T,3>>>& pre_val = this->_visu[0].sample_val;

      pre_val.resize(this->_visu[0].size());
      s.reset();
      for(uint i=0; i<this->_visu[0].size(); i++) {
         multEval(pre_val[i], _pre[i], d);
         s += pre_val[i][0];
      }
  }




  /*! void  PBezierCurve<T>::updateSamples() const
   *  Protected,
   *  Updating sample points and derivatives when control points has been moved,
   */
  template <typename T>
  void  PBezierCurve<T>::updateSamples() const {

      while(_pos_change.size()>0) {
          EditSet es = _pos_change.back();
          for(int i=_pos_change.size()-2; i>=0; i--)
              if (_pos_change[i].ind == es.ind) {
                  es.dp += _pos_change[i].dp;
                  _pos_change.erase(_pos_change.begin()+i);
              }
          for(uint i=0; i<_pre.size(); i++)
              comp(this->_visu[0].sample_val[i], _pre[i], es.dp, es.ind);
          _pos_change.pop_back();
      }
  }




  /*! void PBezierCurve<T>::makeBernsteinMat( int m, int d, T scale ) const
   *  Protected,
   *  Compute the Bernstein-Hermite matrix for all sample values
   *
   *  \param  m        The number of samples
   *  \param  d        The polynomial degree
   *  \param  scale    (default 1) The inverse scale of the domain
   */
  template <typename T>
  inline
  void PBezierCurve<T>::makeBernsteinMat( int m, int d, T scale ) const {

      _pre.resize(m);
      for(int i=0; i<m; i++ )
          EvaluatorStatic<T>::evaluateBhp(_pre[i], d, this->_map(this->_visu[0][i]), 1/scale);
  }




  /*! void PBezierCurve<T>::multEval(DVector<Vector<T,3>>& p, const DMatrix<T>& B, int d) const
   *  Protected,
   *  Partial matrix vector multiplication, that is only the d+1 first rows of the matrix B,
   *  p = B * _c
   *
   *  \param[out]  p   Return value - The position and d derivatives
   *  \param[in]   B   The Bernstein-Hermite matrix to compute the control points _c with
   *  \param[in]   d   The number of derivatives to compute
   */
  template <typename T>
  inline
  void PBezierCurve<T>::multEval(DVector<Vector<T,3>>& p, const DMatrix<T>& B, int d) const {

      p.setDim(d+1);
      // p = B * c;
      // We do this manually because we only want to compute the d first rows of B.
      for(int i=0; i<=d; i++) {
          p[i] = B(i)(0)*_c[0];
          for(int k=1; k<_c.getDim(); k++)
             p[i] += B(i)(k)*_c[k];
      }
  }




  /*! void PBezierCurve<T>::comp(DVector<Vector<T,3>>& p, const DMatrix<T>& B, const Vector<T,3>& c, int k) const
   *  Protected,
   *  Partial vector-matrix computation, ie. actually a vector-vector innerproduct.
   *  where vi compute a vector vith a column vector with index k in the matrix B,
   *  i.e. p = v * B_column[k]
   *
   *  \param[out]  p  Updating the position and d derivatives
   *  \param[in]   B  The Bernstein-Hermite matrix to compute the vector c with.
   *  \param[in]   c  The distance vector one control point has been moved
   *  \param[in]   k  the index of the column to compute with, and also the index of the control point that has been moved
   */
  template <typename T>
  inline
  void  PBezierCurve<T>::comp(DVector<Vector<T,3>>& p, const DMatrix<T>& B, const Vector<T,3>& c, int k) const {

      for(int i=0; i<p.getDim(); i++)
          p[i] += B(i)(k)*c;
  }





  /*! void PBezierCurve<T>::init()
   *  Protected,
   *  Private function for constructors to secure vital initialization
   */
  template <typename T>
  void PBezierCurve<T>::init() {

    this->_type_id = GM_SO_TYPE_CURVE_BEZIER;
    _selectors = false;
    _c_moved   = false;
    _sgv       = nullptr;
  }



} // END namespace GMlib

