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




#ifndef GM_PARAMETRICS_CURVES_PERBSCURVE_H
#define GM_PARAMETRICS_CURVES_PERBSCURVE_H


#include "../gmpcurve.h"

#include "../evaluators/gmerbsevaluator.h"


namespace GMlib {


  template <typename T>
  class PERBSCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(PERBSCurve)

    struct PreBas  {
        Vector<T,3>   B;     //!< The B-function for a given
        Vector<int,2> ind;   //!< The index of the two local curves to compute
    };

    struct PreEvalB : public std::vector<PreBas> {
    };

    struct CpIndex : public DVector<Vector<int,2>> {
    };                      //!<Indices of sample points affected by a given local curve


  public:

    enum CURVE_TYPE {
      SUB_CURVE   = 0,
      ARC_CURVE   = 1,
      BEZIER_CURVE = 2 };

    PERBSCurve(); // Dummy
    PERBSCurve( PCurve<T,3>* g, int n, bool cu_len=false );        // SUB_CURVE type constructor
    PERBSCurve( PCurve<T,3>* g, int n, int d, bool cu_len=false ); // BEZIER_CURVE type constructor
    PERBSCurve( int n, PCurve<T,3>* g, bool cu_len=false );        // ARC_CURVE type constructor
    PERBSCurve( const PERBSCurve<T>& copy );
    virtual ~PERBSCurve();

    // Local curves
    DVector< PCurve<T,3>* >&        getLocalCurves();
    const DVector< PCurve<T,3>* >&  getLocalCurves() const;
    int                             getNoLocalCurves() const;
    virtual void                    hideLocalCurves();
    virtual void                    showLocalCurves();
    virtual void                    toggleLocalCurves();
    bool                            isLocalCurvesVisible() const;

    void                            setPartitionCriterion(int d);

    // Knot insertion
    void                            splitKnot( int tk );


    // virtual functions from DO/PCurve
//    void                            insertVisualizer(Visualizer* visualizer ) override;
//    void                            removeVisualizer(Visualizer *visualizer) override;

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from SceneObject
    // The two first functions below are not meant for public use, they are for editing on the curve
    void                   edit( SceneObject *obj ) override;
    void                   replot() const override;

    // from PCurve
    bool                   isClosed() const override;
    void                   sample(int m, int d) override;

  protected:

    // Protected intrinsic data for the curve
    CURVE_TYPE                   _type_local;
    PCurve<T,3>*                 _origin;     //!< Optional - Original curve (to copy)
    DVector<PCurve<T,3>*>        _c;          //!< Local curves (control curves)
    DVector<T>                   _t;          //!< knot vector
    bool                         _cl;         //!< closed (or open) curve?

    BasisEvaluator<long double>* _evaluator;  //!< Evaluator for Expo-rational B-functions

    // Pre-evaluation and visualization
    mutable std::vector<CpIndex> _cp_index;   //!< Indices of sample points affected by the local curves.
                                              //!< I.e. _cp_index[local curve nr.][partition nr.][start, end - index in _visu]
    mutable std::vector<int>     _local_change; //!< The local curves that has changed



    // Partitioning of the curve based on continuity criteria
    mutable int                   _pct;        //!< Partition criteria (continuity C^_pct)
    mutable std::vector<PreEvalB> _pre_basis;  //!< Pre evaluated b-functions for each partitions

    // Virtual functions from PCurve, which have to be implemented locally
    void                   eval( T t, int d = 0, bool l = false ) const override;
    T                      getEndP()   const override;
    T                      getStartP() const override;

    // Local help functions
    Vector<T,3>&           getB(T t, int k, int d) const;


  private:
    // Local help functions
    void                   compBlend(int d, const Vector<T,3>& B, DVector<Vector<T,3>>& c0, DVector<Vector<T,3>>& c1) const;
    void                   generateKnotVector(PCurve<T,3>* g, int n, bool closed);
    void                   generateCuLenKnotVector(PCurve<T,3>* g, int n, bool closed);
    void                   insertLocal(PCurve<T,3> *local_curve);
    void                   preSample(int d);
    void                   updatSamples() const;
    void                   makePartition(int m) const;
    void                   prepareSampling(int d) const;
    void                   multEval(DVector<Vector<T,3>>& p, const Vector<T,3>& B, const Vector<int,2>& ii, int j, int i) const;
    int                    isIntersecting(const Vector<int,2>& a, const Vector<int,2>& b) const;

    void                   init(bool closed);

  }; // END class PERBSCurve

} // END namepace GMlib

// Include PERBSCurve class function implementations
#include "gmperbscurve.c"


#endif // GM_PARAMETRICS_CURVES_PERBSCURVE_H


