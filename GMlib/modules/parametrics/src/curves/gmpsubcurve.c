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


/*! PSubCurve<T>::PSubCurve( PCurve<T,3>* c, T s, T e )
 *  Default constructor,
 *  to make a sub curve from a curve c determined by start and end
 *  The local origin is located at c((e+s)/2)
 *
 *  \param[in] c      The original master curve
 *  \param[in] s      The start parameter value of the sub curve
 *  \param[in] e      The end parameter value of the sub curve
 */
  template <typename T>
  PSubCurve<T>::PSubCurve( PCurve<T,3>* c, T s, T e ) : PCurve<T,3>(20, 0, c->getDerivativesImplemnted()) {

    set(c, s, e, (e+s)/2);
    DVector<Vector<T,3> > tr = _c->evaluateParent(_t, 0);
    _trans = tr[0];
    this->translateParent( _trans );
  }



  /*! PSubCurve<T>::PSubCurve( PCurve<T,3>* c, T s, T e, T t  )
   *  Default constructor,
   *  to make a sub curve from a curve c determined by start and end
   *  The local origin is located at c(t)
   *
   *  \param[in] c      The original master curve
   *  \param[in] s      The start parameter value of the sub curve
   *  \param[in] e      The end parameter value of the sub curve
   *  \param[in] t      The parameter value of the local origin
   */
  template <typename T>
  inline
  PSubCurve<T>::PSubCurve( PCurve<T,3>* c, T s, T e, T t ) : PCurve<T,3>(20, 0, c->getDerivativesImplemnted()) {

    set(c, s, e, t);
    DVector<Vector<T,3> > tr = _c->evaluateParent(_t, 0);
    _trans = tr[0];
    this->translateParent( _trans );
  }



  /*! PSubCurve<T>::PSubCurve(const PSubCurve<T>& copy )
   *  A copy constructor
   *  Making a copy of the sub curve
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  inline
  PSubCurve<T>::PSubCurve( const PSubCurve<T>& copy ) : PCurve<T,3>( copy ) {

    set(copy._c, copy._s, copy._e, copy._t);
    _trans = copy._trans;
  }



  /*! PSubCurve<T>::~PSubCurve()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T>
  PSubCurve<T>::~PSubCurve() {}




  template <typename T>
  void PSubCurve<T>::edit( SceneObject *obj )
  {
    unsigned int i = obj->getName();
    if(i == _c->getName()) {
        DVector<Vector<T,3> > tr = _c->evaluateParent(_t, 0);
        this->translateParent( tr[0] - _trans );
        _trans = tr[0];
        this->resample();
        this->setEditDone();
    }
  }



  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************



  /*! bool PSubCurve<T>::isClosed() const
   *  To tell that this sub curve is always open.
   */
  template <typename T>
  bool PSubCurve<T>::isClosed() const {
    return false;
  }



  /*! void PSubCurve<T>::updateMat() const
   *  To update _mat to be equal _matrix
   *  This is due to converting between float and double
   *  The specific implementation for SubCurves is the revers translation, see the constructors.
   */
  template <typename T>
  void PSubCurve<T>::updateMat() const {
     this->_mat = this->_matrix.template toType<float>();
     this->_mat.translate(-_trans);
  }




  template <typename T>
  void PSubCurve<T>::openClosedChanged(T s, T t, T e)
  {
     set(_c, s, e, t);
     eval(_t, 0, true);
     this->translateParent(this->_p[0] - _trans);
     _trans = this->_p[0];
  }




  /*! Vector<T,3>  PSubCurve<T>::getSamplePoint(int i, int j) const
   *  Return the sample values for a given partition "j" and point nr. "i"
   *
   *  \param[in] i  The index of the point
   *  \param[in] j  The index of the partition
   *  \return       The sample point for a given partition "j" and point "i"
   */
  template <typename T>
  Vector<T,3>  PSubCurve<T>::getSamplePoint(int i, int j) const {

     return (*(this->_sampler))[j].sample_val[i][0] + _c->getPos();
  }


  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************


  /*! void PArc<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  The number of derivatives that are implemented is equal The mother curve
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  To evauate from left or from right
   */
  template <typename T>
  void PSubCurve<T>::eval( T t, int d, bool /*l*/ ) const {

    if(_parent_closed && t < _t) t += _c->getParDelta();
    this->_p     = _c->evaluateParent(t , d);
    this->_p[0] -= _trans;
  }



  /*! T PSubCurve<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PSubCurve<T>::getStartP() const {
    return _s;
  }



  /*! T PSubCurve<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PSubCurve<T>::getEndP() const {
    return _e;
  }


  //***************************
  // Private help functions  **
  //***************************

  /*! void  PSubCurve<T>::set(PCurve<T,3>* c, T s, T e, T t)
   *  Private, not for public use
   *  Default initiation for the constructors
   *
   *  \param[in] c      The original master curve
   *  \param[in] s      The start parameter value of the sub curve
   *  \param[in] e      The end parameter value of the sub curve
   *  \param[in] t      The parameter value of the local origin
   */
  template <typename T>
  inline
  void PSubCurve<T>::set(PCurve<T,3>* c, T s, T e, T t) {

    if(c->isClosed() && s < c->getParStart())
        _parent_closed = true;
    else
        _parent_closed = false;
    _c = c;
    _s = s;
    _t = t;
    _e = e;

    this->_sampler = _c->getSampler();
  }


} // END namespace GMlib

