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




#ifndef GM_PARAMETRICS_SURFACE_PERBSSURF_H
#define GM_PARAMETRICS_SURFACE_PERBSSURF_H

#include "../gmpsurf.h"

#include "gmpbeziersurf.h"
#include "../evaluators/gmerbsevaluator.h"


namespace GMlib {



  template <typename T>
  class PSurfVisualizerSet {
  public:
    PSurfVisualizerSet() {}
    PSurfVisualizerSet( const Array<PSurfVisualizer<T,3>*>& skel ) { updateVisualizerSet(skel); }

    void    updateVisualizerSet( const Array<PSurfVisualizer<T,3>*>& skel ) {

      // Delete visualizers
      for( int i = 0; i < visus.getSize(); ++i )
        delete visus[i];

      visus.clear();

      // Create new ones
      for( int i = 0; i < skel.getSize(); ++i )
        visus += static_cast<PSurfVisualizer<T,3>*>(skel(i)->makeCopy());
    }

    Array< PSurfVisualizer<T,3>* >    visus;
    Vector<T,2>                       seg_u;
    Vector<T,2>                       seg_v;
  };



  template <typename T>
  class PERBSSurf : public PSurf<T,3> {
    GM_SCENEOBJECT(PERBSSurf)

    struct PreVec { // For storing of pre evaluated basis functions
        DVector< T > m;
        int ind; };

  public:
    PERBSSurf(); // Dummy
    PERBSSurf( const DMatrix< DMatrix< Vector<T,3> > >& c, const DVector<T>& u, const DVector<T>& v, bool closed_u = false, bool closed_v = false );
    PERBSSurf( const DMatrix< DMatrix< Vector<T,3> > >& c, T s_u = T(0), T e_u = T(1), T s_v = T(0), T e_v = T(1), bool closed_u = false, bool closed_v = false );
    PERBSSurf( const DMatrix< PBezierSurf<T>* >& c, DVector<T> u, DVector<T> v, bool closed_u, bool closed_v );
    PERBSSurf( PSurf<T,3>* g, int no_locals_u, int no_locals_v, int d1, int d2 );
    PERBSSurf( PSurf<T,3>* g, int no_locals_u, int no_locals_v, int d1, int d2, T u_s, T u_e, T v_s, T v_e );
    PERBSSurf( const PERBSSurf<T>& copy );
    virtual ~PERBSSurf();


    void                                generateKnotVector( PSurf<T,3>* g );
    void                                generateKnotVector( PSurf<T,3>* g, T u_s, T u_e, T v_s, T v_e );
    DVector<T>&                         getKnotsU();
    DVector<T>&                         getKnotsV();
    void                                setResampleMode( GM_RESAMPLE_MODE mode );

    // Local patches
    DMatrix<PSurf<T,3>* >&              getLocalPatches();
    int                                 getNoLocalPatchesU() const;
    int                                 getNoLocalPatchesV() const;
    bool                                isLocalPatchesVisible() const;
    virtual void                        hideLocalPatches();
    virtual void                        showLocalPatches();
    virtual void                        toggleLocalPatches();

    // Knot insertion
    void                                splitKnot( int uk, int vk );
    void                                splitKnotInU( int uk ) { assert(false); }
    void                                splitKnotInV( int vk ) { assert(false); }
    void                                splitKnotAlong( int uk, int vk );
    void                                splitKnotAlongU( int uk ) { assert(false); }
    void                                splitKnotAlongV( int vk ) { assert(false); }

    // virtual functions from DO/PSurf
    void                                insertVisualizer( Visualizer* visualizer ) override;
    void                                removeVisualizer( Visualizer* visualizer ) override;

    // virtual functions from PSurf
    void                                edit( SceneObject *obj ) override;
    bool                                isClosedU() const override;
    bool                                isClosedV() const override;
//    void                                preSample( int m1, int m2, int d1, int d2, T s_u, T s_v, T e_u, T e_v ) override;
    void                                replot(int m1 = 0, int m2 = 0, int d1 = 0, int d2 = 0) override;

  protected:
    bool                                _closed_u;
    bool                                _closed_v;

    ERBSEvaluator<long double>          *_evaluator;

    DVector< PreVec >                   _ru;
    DVector< PreVec >                   _rv;
    DVector< DVector<T> >               _Bc;
    DMatrix< DMatrix< Vector<T,3> > >   _c0;
    DMatrix< DMatrix< Vector<T,3> > >   _c1;

    DMatrix< int >                      _uk;
    DMatrix< int >                      _vk;
    DVector<T>                          _u;
    DVector<T>                          _v;

    GM_RESAMPLE_MODE                    _resamp_mode;
    bool                                _pre_eval;

    DMatrix< PSurf<T,3>* >              _c;

    void	                              eval( T u, T v, int d1 = 0, int d2 = 0, bool lu = false, bool lv = false ) const override;
    void                                evalPre( T u, T v, int d1 = 0, int d2 = 0, bool lu = false, bool lv = false );
    void                                findIndex( T u, T v, int& iu, int& iv );
    void                                generateKnotVector( DVector<T>& kv, const T s, const T d, int kvd, bool closed );
    void                                getB( DVector<T>& B, const DVector<T>& kv, int tk, T t, int d );
    DMatrix< Vector<T,3> >              getC( T u, T v, int uk, int vk, T du, T dv ) const;
    DMatrix< Vector<T,3> >              getCPre( T u, T v, int uk, int vk, T du, T dv, int iu, int iv );
    T                                   getStartPU() const override;
    T                                   getEndPU()   const override;
    T                                   getStartPV() const override;
    T                                   getEndPV()   const override;
    virtual void                        init();
    void                                insertPatch( PSurf<T,3> *patch );
    void                                padKnotVector( DVector<T>& kv, bool closed );

  private:
    int                                 _no_sam_u;    // Number of samples u for single sampling
    int                                 _no_sam_v;    // Number of samples v for single sampling

    int                                 _no_der_u;    // Number of derivatives u
    int                                 _no_der_v;    // Number of derivatives u

    Array<PSurfVisualizer<T,3>*>        _pv;
    DMatrix<PSurfVisualizerSet<T> >     _pvi;


    // Virtual function from PSurf
    void                                preSample( int dir, int m ) override;


    // Help functions
    Point<T,2>                          mapToLocal( T u, T v, int uk, int vk ) const;

    void                                internalPreSample( DVector< PreVec >& p, const DVector<T>& t, int m, T start, T end );


  }; // END class PERBSSurf

} // END namepace GMlib

// Include PERBSSurf class function implementations
#include "gmperbssurf.c"


#endif // GM_PARAMETRICS_SURFACE_PERBSSURF_H



