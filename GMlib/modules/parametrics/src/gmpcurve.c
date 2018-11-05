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





// gmlib
#include "visualizers/gmpcurvedefaultvisualizer.h"
#include <core/utils/gmdivideddifferences.h>

// stl
#include <cmath>

namespace GMlib {


/*! PCurve<T,n>::PCurve( int s, int d )
 *  Default constructor
 *  Making an ordinary curve
   *
   *  \param[in]  s   (default = 20) The number of samples to use in the samling of the curve
   *  \param[in]  d   (default = 0) The number of derivatives to compute at each sample value
 */
  template <typename T, int n>
  inline
  PCurve<T,n>::PCurve( int s, int d, int der_implemented ): _visu(1), _der_implemented(der_implemented)  {

    _visu.no_sample      = s;
    _visu.no_derivatives = d;
    _sampler             = &_visu;
    _d                   = -1;
    _tr                  = T(0);
    _sc                  = T(1);
    _is_scaled           = false;
    setNoDer(2);

    this->_lighted       = false;
  }





  /*! PCurve<T,n>::PCurve( const PCurve<T,n>& copy )
   *  Copy constructor
   *  Making a copy of a curve
   *
   *  \param[in]  copy   The curve to copy
   */
  template <typename T, int n>
  inline
  PCurve<T,n>::PCurve( const PCurve<T,n>& copy ) : Parametrics<T,1,n>( copy ), _visu(1), _der_implemented(copy._der_implemented) {

    _visu.no_sample      = copy._visu.no_sample;
    _visu.no_derivatives = copy._visu.no_derivatives;
    _p                   = copy._p;
    _t                   = copy._t;
    _d                   = copy._d;
    _tr                  = copy._tr;
    _sc                  = copy._sc;
    _is_scaled           = copy._is_scaled;
    _sampler             = &_visu;
    setNoDer(2);
  }





  /*! PCurve<T,n>::~PCurve()
   *  Destructor of the curve
   *  Cleaning memory
   */
  template <typename T, int n>
  PCurve<T,n>::~PCurve() {

//    enableDefaultVisualizer( false );
//    if( _default_visualizer )
//      delete _default_visualizer;
  }




  //******************************************************
  //      public evaluate functons for PCurve           **
  //******************************************************


  /*! DVector<Vector<T,n> >& PCurve<T,n>::evaluate( T t, int d ) const
   *  Evaluator for the curve
   *  Computing values in local coordinate system
   *
   *  \param[in] t   The parameter value to compute at
   *  \param[in] d   The number of derivatives to compute
   *  \return        Position and d-derivatives in local coordinates
   */
  template <typename T, int n>
  DVector<Vector<T,n>>& PCurve<T,n>::evaluate( T t, int d ) const {

    _eval(t,d);
    return _p;
  }





  /*! DVector<Vector<T,n>>& PCurve<T,n>::evaluateParent( T t, int d ) const
   *  Evaluator for the curve
   *  Computing values in parent coordinate system
   *
   *  \param[in] t   The parameter value to compute at
   *  \param[in] d   The number of derivatives to compute
   *  \return        Position and d-derivatives in the coordinate system of the parent
   */
  template <typename T, int n>
  DVector<Vector<T,n>>& PCurve<T,n>::evaluateParent( T t, int d ) const {

    static DVector< Vector<T,n> > p;

    _eval(t,d);
    p.setDim(_p.getDim());
    _mat = this->_matrix;

    if(this->_scale.isActive())
        for(int i=0; i<=d; i++)
            this->_p[i] %= this->_scale.getScale();

    p[0] = _mat * _p[0].toPoint();
    for( int i = 1; i <= d; i++ )
      p[i] = _mat * _p[i];

    return p;
  }





  /*! DVector<Vector<T,n>>& PCurve<T,n>::evaluateGlobal( T t, int d ) const
   *  Evaluator for the curve
   *  Computing values in global (scene) coordinate system
   *
   *  \param[in] t   The parameter value to compute at
   *  \param[in] d   The number of derivatives to compute
   *  \return        Position and d-derivatives in global coordinates
   */
  template <typename T, int n>
  DVector<Vector<T,n>>& PCurve<T,n>::evaluateGlobal( T t, int d ) const {

    static DVector<Vector<T,n> > p;

    _eval(t,d);
    p.setDim(_p.getDim());
    _mat = this->_present.template toType<T>();

    if(this->_scale.isActive())
        for(int i=0; i<=d; i++)
            this->_p[i] %= this->_scale.getScale();

    p[0] = _mat * _p[0].toPoint();
    for( int i = 1; i <= d; i++ )
      p[i] = _mat * _p[i];

    return p;
  }





  /*! DVector<Vector<T,n>>& PCurve<T,n>::evaluate( int i ) const
   *  Evaluator for the curve
   *  Used when pre-evaluation is done.
   *  Computing values in local coordinate system
   *
   *  \param[in] i   The sample index of the pre-evaluation
   *  \param[in] j   The partition index of the pre-evaluation
   *  \return        Position and d-derivatives in local coordinates
   */
  template <typename T, int n>
  DVector<Vector<T,n>>& PCurve<T,n>::evaluate( int i, int j ) const {

      std::vector<DVector<Vector<T,n>>>& pre_val = (*_sampler)[j].sample_val;

      return pre_val[i];
  }





  /*! DVector<Vector<T,n>>& PCurve<T,n>::evaluateParent( int i ) const
   *  Evaluator for the curve
   *  Used when pre-evaluation is done.
   *  Computing values in parent coordinate system
   *  NB!! REMEMBER TO SET _mat BEFORE USING THIS !!!
   *
   *  \param[in] i  The sample index of the pre-evaluation
   *  \param[in] j  The partition index of the pre-evaluation
   *  \return       Position and d-derivatives in the coordinate system of the parent
   */
  template <typename T, int n>
  DVector<Vector<T,n> >& PCurve<T,n>::evaluateParent( int i, int j ) const {

      std::vector<DVector<Vector<T,n>>>& pre_val = (*_sampler)[j].sample_val;

      static DVector< Vector<T,n> > p;
      uint k = pre_val[i].getDim();
      p.setDim(k);

      if(this->_scale.isActive()) {
          p[0] = _mat * (getSamplePoint(i,j) % this->_scale.getScale());
          for( uint j = 1; j < k; j++ )
              p[i] = _mat * static_cast<Vector<T,n>>(pre_val[i][j] % this->_scale.getScale());
      }
      else {
          p[0] = _mat * getSamplePoint(i,j).toPoint();
          for( uint j = 1; j < k; j++ )
              p[j] = _mat * pre_val[i][j];
      }

      return p;
  }





  /*! void PCurve<T,n>::estimateClpPar( const Point<T,n>& p, T& t, int m) const
   *  To estimate parameter value for closest point
   *  To be used before getClosestPoint if we do not have a good guess
   *  The function just make sample points and compare.
   *
   *  \param[in]  p The point to be close to
   *  \param[out] t The number of derivatives to compute
   *  \param[in]  m (default 30) The number of samples in the algorithms
   */
  template <typename T, int n>
  void PCurve<T,n>::estimateClpPar( const Point<T,n>& p, T& t, int m) const {

      T st = getParStart();
      T dt = getParDelta()/(m-1);

      Vector<T,n> q = getPosition(t = st);
      T min = (p-q).getLength();

      for(int i=1; i<m; i++) {
          q = getPosition(st+i*dt);
          T mn = (p-q).getLength();
          if (mn < min) {
              min = mn;
              t = st + i*dt;
          }
      }
  }





  /*! bool PCurve<T,n>::getClosestPoint(const Point<T,n>& q, T& t, Point<T,n>& p, double eps, int max_iterations) const
   *  To find closest point on a curve from a point
   *  Using a 2nd degree taylor expansion
   *  <c-p + dt*c' + 0.5dt^2*c'' , c' + dt*c''> = 0
   *
   *  \param[in]  q              The point to be close to
   *  \param[in]  t              The parameter value at closest point, bout guess and result (also return value)
   *  \param[out] p              The closest point on the curve
   *  \param[in]  eps            (default 10e-6) The tolerance (in parametric space)
   *  \param[in]  max_iterations (default 20) The maximum number of iterations in the algorithm
   *  \return                    "true" if a closest point is found "false" if not.
   */
  template<typename T, int n>
  inline
  bool PCurve<T,n>::getClosestPoint(const Point<T,n>& q, T& t, Point<T,n>& p, double eps, int max_iterations) const {

    DVector<Vector<T,n> > c;
    for (int i=0; i < max_iterations; i++) {
      c = evaluate(t, 2);
      T dt = -( (c[0] - q) * c[1]) / ( (c[0] - q) * c[2] + c[1] * c[1] );

      p = c[0];
      t += dt;
      if (std::abs(dt) <= eps) return true;
    }
    return false;
  }





  /*! T PCurve<T,n>::getCurvature( T t ) const
   *  To compute the curvature at a given parameter value
   *
   *  \param[in]  t   The parameter value for where to compute the curvature
   *  \return         The curvature at parameter value "t"
   */
  template <typename T, int n>
  inline
  T PCurve<T,n>::getCurvature( T t ) const {

    _eval( t, 2 );
    Vector<T,n> d1 = _p[1];
    T a1= d1.getLength();

    if( a1 < T(1.0e-5) ) return T(0);

    return (d1^_p[2]).getLength() / pow(a1,3);
  }





  /*! T PCurve<T,n>::getRadius( T t ) const
   *  To compute the radius at a given parameter value
   *  The radius is the inverse of the curvature
   *
   *  \param[in]  t   The parameter value for where to compute the radius
   *  \return         The radius (inverse of curvature) at parameter value "t"
   */
  template <typename T, int n>
  inline
  T PCurve<T,n>::getRadius( T t ) const {

    return T(1) / getCurvature(t);
  }





  /*! T PCurve<T,n>::getSpeed( T t ) const
   *  To compute the speed at a given parameter value
   *  The speed is the length of the velosity vector (the first derivative)
   *
   *  \param[in]  t   The parameter value for where to compute the speed
   *  \return         The speed (length of first derivatives) at parameter value "t"
   */
  template <typename T, int n>
  inline
  T PCurve<T,n>::getSpeed( T t ) const {

    return getDer1(t).getLength();
  }





  /*! T PCurve<T,n>::getCurveLength( T a , T b ) const
   *  To compute the curve length on the curve c, from c(a) to c(b)
   *  Numerical integration is used (Romberg integration)
   *
   *  \param[in]  a   The parameter value at start
   *  \param[in]  b   The parameter value at end
   *  \return         The curve length between the parameter values "a" and "b"
   */
  template <typename T, int n>
  inline
  T PCurve<T,n>::getCurveLength( T a , T b ) const {

    if(b<a)	{
      a = getParStart();
      b = getParEnd();
    }

    return _integral( a, b, 1e-10 );
  }





  /*! int PCurve<T,n>::getNumDerivatives() const
   *  Returns the number of subsequent derivatives computed in the sampling
   *
   *  \return     The number of subsequent derivatives computed in the sampling
   */
  template <typename T, int n>
  inline
  int PCurve<T,n>::getNumDerivatives() const {

    return _visu.no_derivatives;
  }





  //****************************************************************
  //   To get the position or given derivatives at a given (u,v)
  //****************************************************************

  /*! const Point<T,n>& PCurve<T,n>::operator()( T t ) const
   *  Evaluator for the curve, returns only the position.
   *  Computing values in local coordinate system
   *  Note that Derivatives up to "defalt_d" is computed and will be returned
   *  if you call getDer1(t), ...
   *
   *  \param[in]  t   The parameter value to compute at
   *  \return         The position at the parameter value t
   */
  template <typename T, int n>
  inline
  const Point<T,n>& PCurve<T,n>::operator()( T t ) const {

    _eval( t, _defalt_d );
    return _p[0];
  }





  /*! const Point<T,n>& PCurve<T,n>::getPosition( T t ) const
   *  Evaluator for the curve, returns only the position.
   *  Computing values in local coordinate system
   *  Note that no derivatives are computed here
   *
   *  \param[in]  t   The parameter value to compute at
   *  \return         The position at the parameter value t
   */
  template <typename T, int n>
  inline
  const Point<T,n>& PCurve<T,n>::getPosition( T t ) const {

    _eval( t, 0 );
    return _p[0];
  }





  /*! const Vector<T,n>& PCurve<T,n>::getDer1( T t ) const
   *  Evaluator for the curve, returns only the first derivative.
   *  Computing values in local coordinate system
   *  Note that the position is also computed and can be used by getPosition(t)
   *
   *  \param[in]  t   The parameter value to compute at
   *  \return         The first derivative at the parameter value t
   */
  template <typename T, int n>
  inline
  const Vector<T,n>& PCurve<T,n>::getDer1( T t ) const {

    _eval( t, 1 );
    return _p[1];
  }





  /*! const Vector<T,n>& PCurve<T,n>::getDer2( T t ) const
   *  Evaluator for the curve, returns only the second derivative.
   *  Computing values in local coordinate system
   *  Note that the position and first derivative are also computed
   *  and can be used by getPosition(t) and getDer1(t)
   *
   *  \param[in]  t   The parameter value to compute at
   *  \return         The second derivative at the parameter value t
   */
  template <typename T, int n>
  inline
  const Vector<T,n>& PCurve<T,n>::getDer2( T t ) const {

    _eval(t,2);
    return _p[2];
  }





  /*! const Vector<T,n>& PCurve<T,n>::getDer3( T t ) const
   *  Evaluator for the curve, returns only the third derivative.
   *  Computing values in local coordinate system
   *  Note that the position, first derivative  and second derivative are
   *  also computed and can be used by getPosition(t), getDer1(t) and getDer2(t)
   *
   *  \param[in]  t   The parameter value to compute at
   *  \return         The third derivative at the parameter value t
   */
  template <typename T, int n>
  inline
  const Vector<T,n>& PCurve<T,n>::getDer3( T t ) const {

    _eval(t,3);
    return _p[3];
  }





  /*! const Vector<T,n>& PCurve<T,n>::getDer3( T t ) const
   *  Evaluator for the curve, returns only the third derivative.
   *  Computing values in local coordinate system
   *  Note that the position, first derivative  and second derivative are
   *  also computed and can be used by getPosition(t), getDer1(t) and getDer2(t)
   *
   *  \param[in]  t   The parameter value to compute at
   *  \return         The third derivative at the parameter value t
   */
  template <typename T, int n>
  inline
  int PCurve<T,n>:: getDerivativesImplemnted() const {
    return _der_implemented;
  }

  //***********************************************************
  //   To get the desired domain of the surface
  //***********************************************************


  /*! T PCurve<T,n>::getParStart() const
   *  Return the start parameter value of the curve
   *
   *  \return       The start parameter value of the curve
   */
  template <typename T, int n>
  inline
  T PCurve<T,n>::getParStart() const {

    return getStartP() + _tr;
  }





  /*! T PCurve<T,n>::getParDelta() const
   *  Return the length of the domain of the curve
   *
   *  \return       The length of the domain of the curve
   */
  template <typename T, int n>
  inline
  T PCurve<T,n>::getParDelta() const {

    return _sc*( getEndP() - getStartP() );
  }





  /*! T PCurve<T,n>::getParEnd() const
   *  Return the end parameter value of the curve
   *
   *  \return       The end parameter value of the curve
   */
  template <typename T, int n>
  inline
  T PCurve<T,n>::getParEnd() const {

    return getParStart() + getParDelta();
  }





  /*! int PCurve<T,n>::getNumSamples() const
   *  Return the number used when samling the curve
   *
   *  \return       The number used when samling the curve
   */
  template <typename T, int n>
  inline
  int PCurve<T,n>::getNumSamples() const {

    return _visu.no_sample;
  }





  /*! const std::vector<T>&  PCurve<T,n>::getSampleValues(int i) const
   *  Return the sample values for a given partition "i"
   *
   *  \param[in] i  The index of the partition to investigate
   *  \return       The sample values for a given partition "i"
   */
  template <typename T, int n>
  const std::vector<T>&  PCurve<T,n>::getSampleValues(int i) const {

    return _visu[i];
  }





  /*! Vector<T,n>  PCurve<T,n>::getSamplePoint(int i, int j) const
   *  Return the sample values for a given partition "i"
   *
   *  \param[in] i  The index of the point
   *  \param[in] j  The index of the partition
   *  \return       The sample point for a given partition "j" and point "i"
   */
  template <typename T, int n>
  Vector<T,n>  PCurve<T,n>::getSamplePoint(int i, int j) const {

    return _visu[i].sample_val[j][0];
  }


  //*******************************************************
  //***  Virtual functons for pre-samling  and plotting  **
  //*******************************************************



  /*! bool PCurve<T,n>::isClosed() const
   *  Is this curve closed (true) or open (false)?
   *
   *  \return      true if closed, false if open.
   */
  template <typename T, int n>
  bool PCurve<T,n>::isClosed() const {

    return false;
  }





  /*! void  PCurve<T,n>::sample( int m, int d )
   *  This function is the sample function for static curves
   *
   *  \param[in] m The number of samples
   *  \param[in] d The number of derivatives at each sample
   */
  template <typename T, int n>
  void PCurve<T,n>::sample( int m, int d ) {

    _checkSampleVal( m, d );

    if(_visu.size()>1) std::cerr << "Error, more than 1 partition in simple curve!!";

    preSample( _visu[0], _visu.no_sample, _visu.no_derivatives, getParStart(), getParEnd() );
  }





  /*! void PCurve<T,n>::preSample( Partition& v, int m, int d, T s, T e ) const
   *  Pre sampling function.
   *  First  "m" uniform  sample parameter values is computed,
   *  then  the resample is done, including creating the surounding sphere
   *
   *  \param[out] v   The partition to uppdate with samples
   *  \param[in]  m   The number of samples to compute
   *  \param[in]  d   The number of derivatives at each sample
   *  \param[in]  s   The start parameter value (to compute from)
   *  \param[in]  e   The end parameter value (to compute to)
   */
  template <typename T, int n>
  inline
  void PCurve<T,n>::preSample( Partition& v, int m, int d, T s, T e ) const {

      computeUniformParamVal(v, m, s, e);
      resample( v, d);
      this->setEditDone();
  }





  /*! void PCurve<T,n>::preSample( const std::vector<T>& sample, int d, int i ) const
   *  Pre sampling function.
   *  Sample parameter values is her given,
   *  The function do the resampling, including creating the surounding sphere
   *
   *  \param[in]  sample   The parameter to sample at
   *  \param[in]  d        The number of derivatives at each sample
   *  \param[in]  i        The index of the partition to uppdate
   */
  template <typename T, int n>
  inline
  void PCurve<T,n>::preSample( const std::vector<T>& sample, int d, unsigned int i ) const {

      if(i< _visu.size()) {
        _visu[i] = sample;
        resample( _visu[i], d);
        this->setEditDone();
      } else
          std::cerr << "Wrong partition number !!";
  }





  /*! void PCurve<T,n>::preSample( const Sampler& sample ) const
   *  Pre sampling function.
   *  Sample parameter values is her given,
   *  The function do the resampling, including creating the surounding sphere
   *
   *  \param[in]  sample   The presample data to copy
   */
  template <typename T, int n>
  inline
  void PCurve<T,n>::preSample( const Sampler& sample ) const {

      _visu.stored         = true;
      _visu.no_derivatives = sample.no_derivatives;
      _visu.no_sample      = sample.no_sample;
      _visu.resize(sample.size());
    for(unsigned int i=0; i<_visu.size(); i++)
      preSample( sample[i], _visu.no_derivatives, i );
}





  /*! void  PCurve<T,n>::replot() const
   *  Private, not for public use
   *  Update references to data and set the surrounding sphere
   *  this function shall only be used for dynamic curves
   */
  template <typename T, int n>
  void  PCurve<T,n>::replot() const {

      // Give reference to updated data to all visualizers
      for(unsigned int i=0; i<this->_visu.size(); i++)
          for(unsigned int j=0; j<this->_visu[i].vis.size(); j++)
              this->_visu[i].vis[j]->set(this->_visu[i].sample_val);

      // Updating surrounding sphere
      Sphere<T,3> sph = this->_visu[0].sur_sphere;
      for(unsigned int i=1; i<this->_visu.size(); i++)
          sph += this->_visu[i].sur_sphere;
      SceneObject::setSurroundingSphere(sph);
// std::cout << "Objekt type - " << this->getIdentity() << " Replot for objekt:" << this->getName() << std::endl;
      SceneObject::replot();
  }





  /*! void  PCurve<T,n>::setDomain(T start, T end)
   *  Set the domain of the curve
   *
   *  \param[in]  start  The start parameter value of the domain
   *  \param[in]  end    The end parameter value of the domain
   */
  template <typename T, int n>
  inline
  void PCurve<T,n>::setDomain(T start, T end) {
     setDomainScale((end - start)/(getEndP()-getStartP()));
     setDomainTrans(start - getStartP());
  }





  /*! void  PCurve<T,n>::setDomainScale( T sc )
   *  Change the domain of the curve by scaling the "formula" domain,
   *  The start parameter value is retained,
   *
   *  \param[in]  sc  The scaling value
   */
  template <typename T, int n>
  inline
  void PCurve<T,n>::setDomainScale( T sc ) {
    _sc = sc;
    if(GMutils::compValueF(sc, T(1)))
      _is_scaled = false;
    else
      _is_scaled = true;
  }





    /*! void  PCurve<T,n>::setDomainTrans( T tr )
     *  Change the domain of the curve by translating the "formula" domain,
     *  The size of the domain is retained,
     *
     *  \param[in]  tr  The translation value
     */
    template <typename T, int n>
    inline
    void PCurve<T,n>::setDomainTrans( T tr ) {
      _tr = tr;
    }





    /*! void  PCurve<T,n>::setNoDer( int d )
     *  Set number of derivatives to compute when using operator() in evaluation
     *
     *  \param[in]  d  The number of derivatives to compute
     */
    template <typename T, int n>
    inline
    void PCurve<T,n>::setNoDer( int d ) {
      _defalt_d = d;
    }





    /*! void PCurve<T,n>::setSurroundingSphere( const std::vector< DVector< Vector<T,n> > >& p ) const
     *  To compute and set the surrounding sphere based on a set of sample points.
     *  The function is defined as virtual, so spesific curves can have their own implementation
     *
     *  \param[in]  p  The set of sample points
     */
    template <typename T, int n>
    void PCurve<T,n>::setSurroundingSphere( const std::vector< DVector< Vector<T,n> > >& p ) const {

      Sphere<T,n>  s(p[0](0), p[p.size() - 1](0));
      for( int i = p.size() - 2; i > 0; i-- )
        s += p[i](0);
      Parametrics<T,1,n>::setSurroundingSphere( s.template toType<float>() );
    }





    /*! void PCurve<T,n>::updateMat() const
     *  To update _mat to be equal _matrix
     *  This is due to converting between float and double
     *  The function is defined as virtual, so specific curves can have their own implementation
     */
    template <typename T, int n>
    void PCurve<T,n>::updateMat() const {
//       _mat = this->_matrix.template toType<T>();
          _mat = this->_matrix;
    }





    /*! void PCurve<T,n>::enableDefaultVisualizer( bool enable )
     *  To set the default visualizer active or not active
     *
     *  \param[in]  enable   make the default visualizer activ (true) or not (false)
     */
    template <typename T, int n>
    void PCurve<T,n>::enableDefaultVisualizer( bool enable ) {

      if( !enable  )
        removeVisualizer( _visu.default_visualizer );
      else {
        if( !_visu.default_visualizer )
          _visu.default_visualizer = new PCurveDefaultVisualizer<T,n>(_visu[0].sample_val);

        insertVisualizer( _visu.default_visualizer );
      }
    }





    /*! void PCurve<T,n>::toggleDefaultVisualizer()
     *  To swap between that the default visualizer is active or not
     */
    template <typename T, int n>
    void PCurve<T,n>::toggleDefaultVisualizer() {

      if( !this->_visualizers.exist( _visu.default_visualizer ) )
        enableDefaultVisualizer( true );
      else
        enableDefaultVisualizer( false );
    }





    /*! const PCurveVisualizer<T, n>* PCurve<T,n>::getDefaultVisualizer() const
     *  To return the default visualizer of this curve if present, or null pointer if not
     *
     *  \return   Gives the default visualizer
     */
    template <typename T, int n>
    inline
    const PCurveVisualizer<T, n>* PCurve<T,n>::getDefaultVisualizer() const {

      return _visu.default_visualizer;
    }





    /*! void PCurve<T,n>::insertVisualizer( Visualizer* visualizer )
     *  To insert a visualizer into the active-list
     *  If there are more than one partition and the visualiser is a curve visualizer,
     *  then a copy of the visualiser is inserted in each partion and also made active
     *
     *  \param[in]   visualizer   The visualizer that are going to be inserted
     */
    template <typename T, int n>
    void PCurve<T,n>::insertVisualizer( Visualizer* visualizer ) {

      // Is it already active
      if(this->_visualizers.exist( visualizer)) return;

      // If it is not a curve visualiser, we just insert it as active
      PCurveVisualizer<T,n>* visu = dynamic_cast<PCurveVisualizer<T,n>*>( visualizer );
      if( !visu ) {
        SceneObject::insertVisualizer( visualizer );
        return;
      }

      // Is this a default visualiser that is not enabled
      if(_visu.default_visualizer == nullptr){
        PCurveDefaultVisualizer<T,n>* visu = dynamic_cast<PCurveDefaultVisualizer<T,n>*>(visualizer);
        if(visu)
          _visu.default_visualizer = visu;
      }

      // If it is a curve visualiser we need to know if it only must be made active or made in total
      int j = -1;
      for(unsigned int i=0; i<_visu[0].vis.size(); i++)
        if(visu == _visu[0].vis[i])   j=i;

      if(j>=0){
          for(unsigned int i=0; i< _visu.size(); i++)
            SceneObject::insertVisualizer(_visu[i].vis[j]);
      } else{
        _visu[0].vis.push_back(visu);
        SceneObject::insertVisualizer(visu);
        for(unsigned int i=1; i< _visu.size(); i++) {
          PCurveVisualizer<T,n>* v = dynamic_cast<PCurveVisualizer<T,n>*>(visu->makeCopy());
            _visu[i].vis.push_back(v);
            SceneObject::insertVisualizer(v);
        }
      }
    }



    /*! void PCurve<T,n>::removeVisualizer( Visualizer* visualizer )
     *  To remove a visualizer from the active-list
     *
     *  \param[in]   visualizer   The visualizer that are going to be removed from the active-list
     */
    template <typename T, int n>
    void PCurve<T,n>::removeVisualizer( Visualizer* visualizer ) {

      SceneObject::removeVisualizer( visualizer );
    }







   /*! void PCurve<T,n>::resample( std::vector<DVector<Vector<T,n>>>& p, Sphere<T,3>& s, const std::vector<T>& t, int d) const
    *  Resampling when the curve has changed, can be used flexible, for each partition separately
    *
    *  \param[out] p  Sample points and belonging derivatives to be computed
    *  \param[out] s  Surrounding sphere to uppdate
    *  \param[in]  t  Vector of parameter values at the sample points
    *  \param[in]  d  Number of derivatives to compute
    */
    template <typename T, int n>
    inline
    void PCurve<T,n>::resample( std::vector<DVector<Vector<T,n>>>& p, Sphere<T,3>& s, const std::vector<T>& t, int d) const {

      p.resize(t.size());
      s.reset();
      for( unsigned int i = 0; i < t.size(); i++ ) {
        _eval( t[i], d, true);
        p[i] = _p;
        s += _p[0];
      }
      if(d>_der_implemented || (d>0 && this->_dm == GM_DERIVATION_DD));
          DD::compute1D(p, t, isClosed(), d, _der_implemented);
    }





    /*! void PCurve<T,n>::resample( std::vector<DVector<Vector<T,n>>>& p, Sphere<T,3>& s, const std::vector<T>& t, int d) const
     *  Resampling when the curve has changed, to use for one partition
     *
     *  \param[in-out] v  Partition to be updated (samples and surrounding sphere)
     *  \param[in]     d  Number of derivatives to compute
     */
     template <typename T, int n>
     inline
     void PCurve<T,n>::resample( Partition& v, int d ) const {

       resample( v.sample_val, v.sur_sphere , v, d);
     }




    /*! void PCurve<T,n>::resample( std::vector<DVector<Vector<T,n>>>& p, Sphere<T,3>& s, const std::vector<T>& t, int d) const
     *  Resampling when the curve has changed, to use when the curve has only one partition
     */
     template <typename T, int n>
     inline
     void PCurve<T,n>::resample() const {

       resample( _visu[0].sample_val, _visu[0].sur_sphere , _visu[0], _visu.no_derivatives);
     }




   /*! void PCurve<T,n>::makeUniformSampleValues( Partition& sample, int m ) const
    *  Curve integration, using Romberg integration method.
    *
    *  \param[out]  sample    Partition where parameter values for the sample points are to be updated
    *  \param[in]   m         Number of sample points in the partition
    */
    template <typename T, int n>
    inline
    void PCurve<T,n>::makeUniformSampleValues( Partition& sample, int m ) const {

       computeUniformParamVal( sample, m, getParStart(), getParEnd() );
    }




    /*! void  PCurve<T,n>::prepareVisualizers()
     *  Private, not for public use
     *  Remove all old visualizers
     *  Creates and inserts new default visualizers for all partitions
     */
    template <typename T, int n>
    void  PCurve<T,n>::prepareVisualizers() {

        // Updating visualizers in new partitions
        for(unsigned int i=1; i < _visu.size(); i++)
            if(_visu[i].vis.size() == 0) {
                for(unsigned int j=0; j < _visu[0].vis.size(); j++) {
                    PCurveVisualizer<T,3>* vis = dynamic_cast<PCurveVisualizer<T,3>*>(_visu[0].vis[j]->makeCopy());
                    _visu[i].vis.push_back(vis);
                    SceneObject::insertVisualizer(vis);
                }
            }
    }





    /*! void  PCurve<T,n>::cleanVisualizers()
     *  Private, not for public use
     *  Remove all old visualizers exept the one in _visu[0]
     *
     *  \param[in] i    the visualizer to remove and delete.
     *                  If i=0, then all curve-visualisers are deleted
     *                  if i=1, then all exept _visu[0] (the original inserted ones) are deleted
     */
    template <typename T, int n>
    void  PCurve<T,n>::cleanVisualizers(unsigned int i) {

        // Updating visualizers in new partitions
        for( ; i < _visu.size(); i++) {
            for(unsigned int j=0; j < _visu[i].vis.size(); j++) {
                SceneObject::removeVisualizer(_visu[i].vis[j]);
                delete _visu[i].vis[j];
                _visu[i].vis[j] = nullptr;
            }
            _visu[i].vis.clear();
        }
    }





    //******************************************************
    //  private system and help functons from PCurve      **
    //******************************************************

    /*! void	PCurve<T,n>::_eval( T t, int d, bool left ) const
     *  The reparametrization before and after the eval() is called.
     *  \param[in]  t     The parameter value for the evaluation
     *  \param[in]  d     The number of derivatives to compute
     *  \param[in]  left  Compute from left or right side of t.
     */
    template <typename T, int n>
    inline
    void	PCurve<T,n>::_eval( T t, int d, bool left ) const {

      if( d <= _d && t == _t ) return;
      _t = t; _d = d;
      eval( _map(t), d, left );
      if(_is_scaled) _corrEval(_sc, d);
    }



    /*! T PCurve<T,n>::_integral(T a, T b, double eps) const
     *  Curve integration, using Romberg integration method.
     *  \param[in]  a    start parameter value
     *  \param[in]  b    end parameter value
     *  \param[in]  eps  Tolerance, telling when to stop refinement
     */
    template <typename T, int n>
    inline
    T PCurve<T,n>::_integral(T a, T b, double eps) const {

      T t = b - a;
      T sum = (getSpeed(a)+getSpeed(b))/2;

      T mat[16][16];
      mat[0][0] = sum * t;

      T s;
      int i,j,k;
      for( i = 1; i < 16; i++ ) {
        s = T(0);
        k = 1 << i;
        t /= 2.0;
        for( j = 1; j < k; j += 2 )
          s += getSpeed(a + t*j);
        mat[0][i] = t * (sum += s);
        for (j=1; j<=i; j++) {
          b = 1 << (j << 1);
          mat[j][i-j] = (b*mat[j-1][i-j+1] - mat[j-1][i-j])/(b-1.0);
        }
        if(std::abs(mat[i][0] - mat[i-1][0]) < eps) return mat[i][0];
      }
      return mat[15][0];
    }



    /*! T PCurve<T,n>::_map( T t ) const
     *  Mapping paramerer values from defined value to function value
     *  \param[in]    t   parameter value in defined coordinates
     *  \return           parameter value in function domain
     */
    template <typename T, int n>
    inline
    T PCurve<T,n>::_map( T t ) const {

       return getStartP() + ( t - _tr - getStartP())/_sc;
    }



    /*! void PCurve<T,n>::_corrEval( T s, int d ) const
     *  Mapping paramerer values from defined value to function value
     *  \param[in]    s   scaling value to scale derivatives
     *  \param[in]    d   number of derivatives to corrigate
     */
    template <typename T, int n>
    inline
    void PCurve<T,n>::_corrEval( T s, int d ) const {
        for(int j =1; j<=d; j++)
            for(int i=j; i<=d;i++)
                _p[i] /=s;
    }



    /*! T PCurve<T,n>::_map( T t ) const
     *  Mapping paramerer values from defined value to function value
     *  \param[in-out]    m   number of sample points
     *  \param[in-out]    d   number of derivatives to cmpute
     */
    template <typename T, int n>
    inline
    void PCurve<T,n>::_checkSampleVal( int& m, int& d ) const {

        // Correct sample domain
        if( m < 2 )             m = _visu.no_sample;
        else      _visu.no_sample = m;
        // Correct derivatives
        if( d < 0 )             d = _visu.no_derivatives;
        else _visu.no_derivatives = d;
    }


  } // END namespace GMlib
