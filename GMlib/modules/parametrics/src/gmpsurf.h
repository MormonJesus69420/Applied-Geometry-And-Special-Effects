/**********************************************************************************
**
** Copyright (C) 1994 - 2017 University of Tromsø - The Arctic University of Norway
** Contact: GMlib Online Portal at https://source.uit.no/gmlib/gmlib/wikis/home
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




#ifndef GM_PARAMETRICS_PSURF_H
#define GM_PARAMETRICS_PSURF_H


#include "gmparametrics.h"

// gmlib
#include <core/containers/gmarray.h>
#include <core/containers/gmdvector.h>
#include <core/containers/gmdmatrix.h>

// stl
#include <fstream>


namespace GMlib {

  template <typename T, int n>
  class PSurfVisualizer;

  template <typename T, int n>
  class PSurfDefaultVisualizer;





  /*! \class PSurf gmpsurf.h <gmpsurf>
   *  \brief The abstract PSurf Class is to be used as base class for parametric surfaces
   *
   *  The abstract PSurf Class is to be used as base class for rectangular parametric surfaces
   *
   */
  template <typename T,int n>
  class PSurf : public Parametrics<T,2,n> {

  public:
    //****  Constructors and destructors  ****
    PSurf( int s1 = 20, int s2 = 20 );
    PSurf( const PSurf<T,n>& copy );
    virtual ~PSurf();

    //****  Evaluation functons  ****
    DMatrix<Vector<T,n> >&        evaluate( T u, T v, int d1, int d2 ) const;
    DMatrix<Vector<T,n> >&        evaluateParent( T u, T v, int d1, int d2 ) const;
    DMatrix<Vector<T,n> >&        evaluateGlobal( T u, T v, int d1, int d2 ) const;
    DVector<Vector<T,n> >&        evaluateD( T u, T v, int d ) const;

    DMatrix<Vector<T,n> >&        evaluate( const Point<T,2>& p, const Point<int,2>& d ) const;
    DMatrix<Vector<T,n> >&        evaluateParent( const Point<T,2>& p, const Point<int,2>& d ) const;
    DMatrix<Vector<T,n> >&        evaluateGlobal( const Point<T,2>& p, const Point<int,2>& d ) const;
    DVector<Vector<T,n> >&        evaluateD( const Point<T,2>& p, int d ) const;

    DMatrix<Vector<T,n> >&        evaluate( int i, int j ) const;
    DMatrix<Vector<T,n> >&        evaluateParent( int i, int j  ) const;

    //****  Closest point functons  ****
    virtual void                  estimateClpPar( const Point<T,n>& p, T& u, T& v, int m=20 ) const;
    virtual bool                  getClosestPoint( const Point<T,n>& q, T& u, T& v,
                                                   double eps = 10e-6, int max_iterations = 20 ) const;
    bool                          getClosestPoint( const Point<T,n>& q, Point<T,2>& uv,
                                                   double eps = 10e-6, int max_iterations = 20 ) const;

    //****  Curvature functons  ****
    virtual T                     getCurvatureGauss( T u, T v ) const;
    virtual T                     getCurvatureMean( T u, T v ) const;
    virtual T                     getCurvaturePrincipalMax( T u, T v ) const;
    virtual T                     getCurvaturePrincipalMin( T u, T v ) const;

    int                           getDerivativesU() const;
    int                           getDerivativesV() const;

    //****  Single evaluation, position or derivatives  ****
    const Point<T,n>&             operator () ( T u, T v ) const;
    const Point<T,n>&             getPosition( T u, T v ) const;
    const Vector<T,n>&            getDerU( T u, T v ) const;
    const Vector<T,n>&            getDerV( T u, T v ) const;
    const Vector<T,n>&            getDerUU( T u, T v ) const;
    const Vector<T,n>&            getDerVV( T u, T v ) const;
    const Vector<T,n>&            getDerUV( T u, T v ) const;
    const Vector<T,n>&            getNormal() const;

    //****  To see the domain of the surface  ****
    T                             getParStartU() const;
    T                             getParEndU()   const;
    T                             getParDeltaU() const;
    T                             getParStartV() const;
    T                             getParEndV()   const;
    T                             getParDeltaV() const;

    //****  To see numbers for sampling of the surface  ****
    int                           getNumSamIntPU() const;
    int                           getNumSamIntPV() const;
    int                           getSamPU( int i = 0 ) const;
    int                           getSamPV( int i = 0 ) const;
    int                           getSamplesU() const;
    int                           getSamplesV() const;

    //****  Virtual to see if the surface is open or closed, must be implemented local if closed ****
    virtual bool                  isClosedU() const;
    virtual bool                  isClosedV() const;

    //**** Virtual functions defined in PSurf, with default implementation here ****
    virtual void                  replot( int m1, int m2, int d1 = 0, int d2 = 0 );

    // virtual from SceneObject, must be implemented in the specific surface if it is editable/ changing shape
    void                          replot() const override;

    // To set the actual domain. All mappings (both parametric and scaling of derivatives) will then automatical be done.
    void                          setDomainU( T start, T end );
    void                          setDomainUScale( T sc );
    void                          setDomainUTrans( T tr );
    void                          setDomainV( T start, T end );
    void                          setDomainVScale( T sc );
    void                          setDomainVTrans( T tr );

    void                          setNoDer( int d );
    virtual void                  setSurroundingSphere( const DMatrix< DMatrix< Vector<T,n> > >& p ) const;
    virtual Parametrics<T,2,n>*   split( T t, int uv );

    //****  To handle visualizers to the surface  ****
    void                          enableDefaultVisualizer( bool enable = true );
    PSurfVisualizer<T,n>*         getDefaultVisualizer() const;
    void                          toggleDefaultVisualizer();
    // Virtual  from  SceneObject
    void                          insertVisualizer( Visualizer *visualizer ) override;
    void                          removeVisualizer( Visualizer *visualizer ) override;

    //****  Virtual to insert and remove selectors to the surface, must be implemented locally  ****
    virtual void                  showSelectors(T rad = T(1), bool grid = false,
                                                const Color& selector_color = GMcolor::darkBlue(),
                                                const Color& grid_color = GMcolor::lightGreen() ) {}
    virtual void                  hideSelectors() {}


  protected:

    mutable int                   _no_sam_u;    // Number of samples u for single sampling
    mutable int                   _no_sam_v;    // Number of samples v for single sampling

    mutable int                   _no_der_u;    // Number of u-derivatives in pre-evaluation
    mutable int                   _no_der_v;    // Number of v-derivatives in pre-evaluation

    mutable DVector< Vector<T,2>> _sam_p_u;     // Sample partition u (start/stop)
    mutable DVector< Vector<T,2>> _sam_p_v;     // Sample partition v (start/stop)

    mutable DVector< int >        _no_sam_p_u;  // Number of samples for each sampling partition in u
    mutable DVector< int >        _no_sam_p_v;  // Number of samples for each sampling partition in v

    mutable int                   _default_d;   // Used by operator() for number of derivative to evaluate.

    // Can be used by resample -- index in pre-eval
    mutable int                   _ind[2];
    mutable bool                  _resample;
    mutable int                   _pre_eval_kode;

    // The result of the previous evaluation
    mutable DMatrix< Vector<T,n>> _p;           // Position and partial derivatives in local coordinates
    mutable Vector<T,n>           _n;           // Surface normal
    mutable T                     _u;           // The parameter value in u-direction used for last evaluation
    mutable T                     _v;           // The parameter value in v-direction used for last evaluation
    mutable int                   _d1;          // Number of derivatives in u-direction computed last time
    mutable int                   _d2;          // Number of derivatives in v-direction computed last time
    mutable bool                  _diagonal;    // True if only upper left half of matrix is evaluated.

    // Shift of domain (parameter interval) from [getStartP*(),getEndP*()] to [getParStart*(),getParEnd*()].
    mutable T                     _tr_u;        // Translate u-parametre
    mutable T                     _sc_u;        // Scale u-parametre

    mutable T                     _tr_v;        // Translate v-parametre
    mutable T                     _sc_v;        // Scale v-parametre

    // Preevaluation/sampling
    mutable DMatrix<DMatrix<Vector<T,n>>> _pre_val; // Position and derivatives at all sample values
    mutable HqMatrix<T,3>                 _mat; // This is to convert float to T in _present
    mutable DMatrix< Vector<T,n>>          p;   // Position and derivatives in parent or global coordinates

    // Visualizers
    Array< PSurfVisualizer<T,n>*> _psurf_visualizers;
    PSurfVisualizer<T,n>*         _default_visualizer;




    /*! virtual void PSurf<T,3>::eval( T u, T v, int d1, int d2, bool lu , bool lv ) const = 0
     *  Surface evaluator, the formula of the surface definition
     *  Requires implementation in PSurf sub-classes.
     *  \param[in]  u   Evaluation parameter in u-direction.
     *  \param[in]  v   Evaluation parameter in v-direction.
     *  \param[in]  d1  Number of derivatives to be computed for u.
     *  \param[in]  d2  Number of derivatives to be computed for v.
     *  \param[in]  lu  (default true) Whether to evaluate from left (or right) at u.
     *  \param[in]  lv  (default true) Whether to evaluate from left (or right) at v.
     */
    virtual void        eval( T u, T v, int d1, int d2, bool lu = true, bool lv = true ) const = 0;


    /*! virtual T PSurf<T,3>::getStartPU() const = 0
     *  Returns the start parameter value in u-direction of the formula used in eval().
     *  Requires implementation in PSurf sub-classes.
     *  \return Start parametric value in u-direction of the formula used in eval().
     */
    virtual T           getStartPU() const = 0;


    /*! virtual T PSurf<T,3>::getEndPU() const = 0
     *  Returns the end parameter value in u-direction of the formula used in eval().
     *  Requires implementation in PSurf sub-classes.
     *  \return Parametric end value in u-direction of the formula used in eval().
     */
    virtual T           getEndPU() const = 0;


    /*! virtual T PSurf<T,3>::getStartPV() const = 0
     *  Returns the start parameter value in v-direction of the formula used in eval().
     *  Requires implementation in PSurf sub-classes.
     *  \return Parametric start value in v-direction of the formula used in eval().
     */
    virtual T           getStartPV() const = 0;


    /*! virtual T PSurf<T,3>::getEndPV() const = 0
     *  Returns the end parameter value in v-direction of the formula used in eval().
     *  Requires implementation in PSurf sub-classes.
     *  \return Parametric end value in v-direction of the formula used in eval().
     */
    virtual T           getEndPV() const = 0;




    virtual void      preSample( int dir, int m );
    virtual void      preSample( int m1, int m2, int d1, int d2, T s_u = T(0), T s_v = T(0), T e_u = T(0), T e_v = T(0) );

    void              resample(DMatrix<DMatrix <DMatrix <Vector<T,n> > > >	& a, int m1, int m2, int d1, int d2 );
    virtual void      resample(DMatrix<DMatrix <Vector<T,n> > >& a, int m1, int m2, int d1, int d2,
                                                                T s_u = T(0), T s_v = T(0), T e_u = T(0), T e_v = T(0)) const;

    virtual void      resampleNormals( const DMatrix<DMatrix<Vector<T,n>>> &sample, DMatrix<Vector<float,3>> &normals ) const;

    void              uppdateSurroundingSphere( Sphere<T,n>& s, const DMatrix<DMatrix<Vector<T,n>>>& p ) const;

    T                 shiftU(T u) const;
    T                 shiftV(T v) const;

  private:

    void              _eval( T u, T v, int d1, int d2 ) const;
    void              _computeEFGefg( T u, T v, T& E, T& F, T& G, T& e, T& f, T& g ) const;

  }; // END class PSurf


} // END namespace GMlib

// Include PSurf class function implementations
#include "gmpsurf.c"


#endif  //GM_PARAMETRICS_PSURF_H
