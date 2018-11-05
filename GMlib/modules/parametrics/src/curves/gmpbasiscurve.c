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


#include "../evaluators/gmerbsevaluator.h"

namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************


/*! PBasisCurve<T,G>::PBasisCurve( T scale )
 *  Default constructor, to make a GERBS B-function
 *
 *  \param[in] scale  Scale of the x-axis
 */
  template <typename T, typename G>
  PBasisCurve<T,G>::PBasisCurve( T scale) : PCurve<T,3>(0, 0, 0){
    _B = new ERBSEvaluator<G>();
    _B->set( 0.0f, 1.0f );
    _d_no  = 0;
    _scale = scale;
  }



  /*! PBasisCurve<T,G>::PBasisCurve(const PBasisCurve<T,G>& copy )
   *  A copy constructor
   *  Making a copy of the "B-function" curve
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T, typename G>
  inline
  PBasisCurve<T,G>::PBasisCurve( const PBasisCurve<T,G>& copy ) : PCurve<T,3>(copy) {}



  /*! PBasisCurve<T,G>::~PBasisCurve()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T, typename G>
  PBasisCurve<T,G>::~PBasisCurve() {
    delete _B;
  }



  //**************************************
  //        Public local functons       **
  //**************************************

  /*! void PBasisCurve<T,G>::setDisplay( int display )
   *  Will change what we visualize, either
   *  the function itself or the first or second derivative
   *
   *  \param[in] display  0 - is the function
   *                      1 - is the first derivative
   *                      0 - is the second derivative
   */
  template <typename T, typename G>
  void PBasisCurve<T,G>::setDisplay( int display ) {
    _d_no = display;
  }

  /*! void PBasisCurve<T,G>::setEvaluator( BasisEvaluator<G> *e )
   *  Will change the evaluator we use
   *
   *  \param[in] e  Pointer to the evaluator we want to use
   */
  template <typename T, typename G>
  void PBasisCurve<T,G>::setEvaluator( BasisEvaluator<G>* e ) {
    if( !e ) return;
    if( _B ) delete _B;
    _B = e;
  }


  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************

  /*! bool PBasisCurve<T,G>::isClosed() const
   *  To tell that this basis curve is open.
   */
  template <typename T, typename G>
  bool PBasisCurve<T,G>::isClosed() const {
    return false;
  }


  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  /*! void PBasisCurve<T,G>::eval( T t, int d, bool l ) const
   *  Evaluation of the basis curve at a given parameter value
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  (dummy) because this is desided by _d_no.
   *  \param  l[in]  (dummy) because left and right are always equal
   */
  template <typename T, typename G>
  void PBasisCurve<T,G>::eval( T t, int /*d*/, bool /*l*/ ) const {

    this->_p.setDim(1);
    float value = (float)_B->operator()(t);

    switch( _d_no ) {
    case 1: value = _B->getDer1();  break;
    case 2: value = _B->getDer2();  break;
    }


    this->_p[0][0] = t*_scale;
    this->_p[0][1] = value;
    this->_p[0][2] = 0.0f;
  }



  /*! T PBasisCurve<T,G>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *  (the start parameter value = 0).
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T, typename G>
  T PBasisCurve<T,G>::getStartP() const {
    return T(0);
  }


  /*! T PBasisCurve<T,G>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *  (the end parameter value = 1).
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T, typename G>
  T PBasisCurve<T,G>::getEndP() const {
    return T(1);
  }

} // END namespace GMlib
