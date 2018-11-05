/**********************************************************************************
**
** Copyright (C) 1994/2016 University of Tromsø - The Arctic University of Norway
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




#ifndef GM_PARAMETRICS_PCURVE_H
#define GM_PARAMETRICS_PCURVE_H



#include "gmparametrics.h"

// gmlib
#include <core/containers/gmarray.h>
#include <core/containers/gmdvector.h>



namespace GMlib {

  template <typename T, int n>
  class PCurveVisualizer;

  template <typename T, int n>
  class PCurveDefaultVisualizer;





  /*! \class PCurve gmpcurv.h <gmpcurv>
   *  \brief The abstract PCurve Class is to be used as base class for parametric curves
   *
   *  The abstract PCurve Class is to be used as base class for parametric curves
   *
   */
  template <typename T,int n>
  class PCurve : public Parametrics<T,1,n> {

  protected:
    struct Partition: public std::vector<T> {       //!< Parameter values for each sample points
      std::vector<DVector<Vector<T,3>>> sample_val; //!< Vertices and derivatives for plotting
      Sphere<T,3>                       sur_sphere; //!< Surrounding sphere of this partition
      Vector<T,2>                       s_e;        //!< Start and end parameter value for this partition
      std::vector<PCurveVisualizer<T,n>*> vis;      //!< Visualizers for plotting (default always first)
      const std::vector<T>& operator=(const std::vector<T>& p) { return std::vector<T>::operator=(p); }
    };

    struct Sampler: public std::vector<Partition> {
      bool                   stored;                //!< Whether to keep or delete sampling data
      int                    no_derivatives;        //!< Number of derivatives computed in the samples
      int                    no_sample;             //!< Number of samples for direct single sampling
      PCurveVisualizer<T,n>* default_visualizer;
      Sampler(int i=1): std::vector<Partition>(i)
        { stored=false; no_derivatives=no_sample=0; default_visualizer=nullptr; }
    };


  public:
      //****  Constructors and destructors  ****
    PCurve( int s = 20, int d = 0, int der_implemented=0 );
    PCurve( const PCurve<T,n>& copy );
    ~PCurve();

    //****  Evaluation functons  ****
    DVector<Vector<T,n> >&       evaluate( T t, int d ) const;
    DVector<Vector<T,n> >&       evaluateGlobal( T t, int d ) const;
    DVector<Vector<T,n> >&       evaluateParent( T t, int d ) const;

    DVector<Vector<T,n> >&       evaluate( int i, int j=0 ) const;
    DVector<Vector<T,n> >&       evaluateParent( int i, int j=0 ) const;

    //****  Closest point functons  ****
    virtual void                 estimateClpPar( const Point<T,n>& q, T& t, int m=30) const;
    bool                         getClosestPoint(const Point<T,n>& q, T& t, Point<T,n>& p,
                                                  double eps = 10e-6, int max_iterations = 20) const;

    //****  Curvature, curve length and speed functons  ****
    T                            getCurvature( T t ) const;
    T                            getRadius( T t ) const;
    T                            getSpeed( T t ) const;
    T                            getCurveLength( T a = 0, T b = -1 ) const;

    int                          getNumDerivatives() const;

    //****  Single evaluation, position or derivatives  ****
    const Point<T,n>&            operator()( T t ) const;
    const Point<T,n>&            getPosition( T t ) const;
    const Vector<T,n>&           getDer1( T t ) const;
    const Vector<T,n>&           getDer2( T t ) const;
    const Vector<T,n>&           getDer3( T t ) const;

    int                          getDerivativesImplemnted() const;

    //****  To see the domain of the curve  ****
    T                            getParStart() const;
    T                            getParDelta() const;
    T                            getParEnd()   const;

    //****  To see numbers for sampling and the sample points of the curve  ****
    int                          getNumSamples() const;

    const std::vector<T>&        getSampleValues(int i=0) const;
    virtual Vector<T,n>          getSamplePoint(int i, int j) const;

    // Virtual functions defined in PCurve and with default implementation here
    virtual bool                 isClosed() const;               // Default is true - "closed"
    virtual void                 sample(int m, int d = 0);       // Default is sampling inline.

    void                         preSample( const std::vector<T>& sample, int d, unsigned int i=0 ) const;
    void                         preSample( Partition& v, int m, int d, T s=T(0), T e=T(0) ) const;
    void                         preSample( const Sampler& sample) const;
    Sampler*                     getSampler() const { return _sampler; }

    // virtual from SceneObject, must be implemented in the specific curve if the curve is editable.
    void                         replot() const override;
    int                          getNumber() const override {return _number;}

    // To set the actual domain. All mappings (both parametric and scaling of derivatives) will then automatical be done.
    void                         setDomain( T start, T end );
    void                         setDomainScale( T sc );
    void                         setDomainTrans( T tr );

    void                         setNumber(int m) {_number = m;}
    void                         setNoDer( int d );
    virtual void                 setSurroundingSphere( const std::vector< DVector< Vector<T,n> > >& p ) const;
    virtual void                 updateMat() const;

    //****  To handle visualizers to the surface  ****
    void                         enableDefaultVisualizer( bool enable = true );
    void                         toggleDefaultVisualizer();
    const PCurveVisualizer<T,n>* getDefaultVisualizer() const;
    // Virtual  from  SceneObject
    void                         insertVisualizer( Visualizer* visualizer ) override;
    void                         removeVisualizer( Visualizer* visualizer ) override;

    //****  Virtual to insert and remove selectors, must be implemented locally  ****
    virtual void                 showSelectors(T radius = T(1), bool grid = true,
                                                const Color& selector_color = GMcolor::darkBlue(),
                                                const Color& grid_color = GMcolor::lightGreen() ) {}
    virtual void                 hideSelectors() {}

    virtual void                 openClosedChanged(T s, T t, T e) {}


  protected:

    // Preevaluation/sampling
    mutable Sampler              _visu;        //!< visualizers and vertices ... for plotting
    mutable Sampler*             _sampler;
//    mutable std::vector<int>     _index_map;   //!< Map of indices
    mutable HqMatrix<T,3>        _mat;         //!< This is to convert float to T in _present or _matrix

    // The result of the previous evaluation
    mutable DVector<Vector<T,n>> _p;           // Position and belonging derivatives
    mutable T                    _t;           // The parameter value used for last evaluation
    mutable int                  _d;           // Number of derivatives computed last time

    mutable int                  _defalt_d;    // used by operator() for number of derivative to evaluate.
    unsigned int                 _number;

    // Shift of domain (parameter interval) from [getStartP(),getEndP()] to [getParStart(),getParEnd()].
    T                            _tr;          // translate
    T                            _sc;          // scales
    bool                         _is_scaled;   // Is scaled ?

    const int                    _der_implemented;


    // The three following functions defines the curve.
    // The first one is the formula, the two other set the domain conected to the formula

    /*! virtual void PCurve<T,3>::eval( T t, int d, bool left = true  ) const = 0
     *  Surface evaluator, abstract. Requires implementation of a sub-class.
     *  \param[in]  t     Parameter value to evaluate
     *  \param[in]  d     Number of derivatives to be computed
     *  \param[in]  left  (default - true) , whether to evaluate from left or right
     */
    virtual void                 eval(T t, int d, bool left = true ) const = 0;


    /*! virtual T PCurve<T,3>::getStartP() const = 0
     *  Returns the parametric start value. (Requires implementation in PCurve sub-classes.)
     *  \return Parametric start value.
     */
    virtual T                    getStartP() const = 0;

    /*! virtual T PCurve<T,3>::getEndPV() const = 0
     *  Returns the parametric end value. (Requires implementation in PCurve sub-classes.)
     *  \return Parametric end value.
     */
    virtual T                    getEndP() const = 0;


    void                         resample( std::vector<DVector<Vector<T,n>>>& p, Sphere<T,3>& s, const std::vector<T>& t, int d) const;
    void                         resample( Partition& v, int d ) const;
    void                         resample() const;

    // For preevaluation.
    void                         makeUniformSampleValues( Partition& v, int m ) const;
    void                         prepareVisualizers();
    void                         cleanVisualizers(unsigned int i=1);


    T                            _map(T t) const;
    void                         _checkSampleVal( int& m, int& d ) const;


  private:
    void                         _eval( T t, int d, bool left = true  ) const;
    T                            _integral(T a, T b, double eps) const;
    void                         _corrEval(T sc, int d) const;

  }; // END class PCurve

} // END namespace GMlib

// Include PCurve class function implementations
#include "gmpcurve.c"


#endif  // GM_PARAMETRICS_PCURVE_H

