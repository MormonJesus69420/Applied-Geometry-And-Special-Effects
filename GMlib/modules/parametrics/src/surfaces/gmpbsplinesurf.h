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




#ifndef GM_PARAMETRICS_SURFACE_PBSPLINESURF_H
#define GM_PARAMETRICS_SURFACE_PBSPLINESURF_H


#include "../gmpsurf.h"


namespace GMlib {

  template <typename T, int n>
  class Selector;

  template <typename T>
  class SelectorGridVisualizer;





  template <typename T>
  class PBSplineSurf : public PSurf<T,3> {
      GM_SCENEOBJECT(PBSplineSurf)

  private:
    struct EditSet {
        Vector<int,2> ind;         //!< Index of a selector and thus a control point
        Vector<T,3>   dp;          //!< The distance the control point has been moved
        EditSet(const Vector<int,2>& i, const Vector<T,3>& d): ind(i),dp(d){}
    };

  struct CpIndex : public DVector<Matrix<int,2,2>> {
  };                      //!<Indices of sample points affected by a given control point


      // For storing of pre evaluated basis functions
//      struct PreMat  { DMatrix< T > m; DVector<int> ind; };
//      struct VisuPar { int m, is, ie; };
      struct VisuSet { Array< PSurfVisualizer<T,3>* > vis; Vector<T,2> s_u, s_v;};

  public:
      // Two of three constructors, the last is using stream and is defined further down in this file
      PBSplineSurf( const DMatrix< Vector<T,3> >& c, const DVector<T>& u,  const DVector<T>& v, int du = 0, int dv = 0);
      PBSplineSurf( const PBSplineSurf<T>& dpbs );

      virtual ~PBSplineSurf();

      DMatrix< Vector<T,3> >&    getControlPoints() const;
      int                        getDegreeU() const;
      int                        getDegreeV() const;
      bool                       isSelectorsVisible() const;
      void                       setClosed( bool closed_u, bool closed_v, T du=T(0), T dv=T(0) );
      void                       setPartitionCritere(int u, int v);

      // Can only be used if the dimensions are not changed
      void                       setControlPoints( const DMatrix< Vector<T,3> >& cp );

      // This function is not meant for public use, it is for editing on the surface
      void                       updateCoeffs( const Vector<T,3>& d );

      // This function is enabeling visulization - an alternative to toggleDefaultVisualizer()
      void                       enablePartitionVisualizer(int u, int v);

      //***************************************
      //****** Virtual public functions  ******
      //***************************************

      // from SceneObject
      // This function is not meant for public use, it is for editing on the surface
      void                       edit( int selector, const Vector<T,3>& dp ) override;
      void                       replot() const override;

//      void                       insertVisualizer( Visualizer *visualizer ) override; // Is also in PSurf
//      void                       removeVisualizer( Visualizer *visualizer ) override; // Is also in PSurf


      // from PSurf
      void                       replot( int m1, int m2, int d1 = 0, int d2 = 0 ) override;
      bool                       isClosedU() const override;
      bool                       isClosedV() const override;
      void                       showSelectors( T rad = T(1), bool grid = false,
                                                const Color& selector_color = GMcolor::darkBlue(),
                                                const Color& grid_color = GMcolor::lightGreen() ) override;
      void                       hideSelectors() override;
      void                       toggleSelectors() override;


  protected:
      // Protected data for the surface

      DMatrix< Vector<T,3> >     _c;       // control polygon

      DVector<T>                 _u;       // knot vector in u-direction
      int                        _du;      // degree in u-direction
      int                        _ku;      // order in u-direction, _ku = _du+1
      bool                       _cu;      // closed in u-direction

      DVector<T>                 _v;       // knot vector in v-direction
      int                        _dv;      // degree in v-direction
      int                        _kv;      // order in v-direction, _kv = _dv+1
      bool                       _cv;      // closed in v-direction

      // Visualization with partitioning
      bool                       _part_viz; // Whether we use default or partial visualization
      bool                       _partitioned[2]; // The partition has / has'nt been computet
      int                        _pcu;     // Partition criteria u-dir (continuity C^_pcu)
      int                        _pcv;     // Partition criteria v-dir (continuity C^_pcv)

      // Pre-evaluation in visualization
      mutable DVector<DVector<PreMat<T>>>   _ru;      // Pre-evaluation of basis in u-direction
      mutable DVector<DVector<PreMat<T>>>   _rv;      // Pre-evaluation of basis in v-direction
      mutable DVector<VisuPar>           _vpu;
      mutable DVector<VisuPar>           _vpv;
      mutable DMatrix<VisuSet>           _visu;

      // Selectors and selector grid
      bool                       _selectors;        // Mark if we have selectors or not
      SelectorGridVisualizer<T>* _sgv;              // Selectorgrid
      DMatrix< Selector<T,3>* >  _s;                // A net of selectores (spheres)
      T                          _selector_radius;
      Color                      _selector_color;
      Color                      _grid_color;
      bool                       _grid;

      bool                         _c_moved; // Mark that we are editing, moving controll points
      mutable std::vector<EditSet> _pos_change; //!< The step vector of control points that is moved

      // Virtual function from PSurf that has to be implemented locally
      void                       eval( T u, T v, int d1 = 0, int d2 = 0, bool lu = false, bool lv = false ) const override; // Abstract in PSurf
      T                          getStartPU() const override;
      T                          getEndPU()   const override;
      T                          getStartPV() const override;
      T                          getEndPV()   const override;

      // Help function to ensure consistent initialization
      virtual void               init();

  private:

      // Virtual function from PSurf
      void                       preSample( int dir, int m ) override;

      void                       resample( DMatrix<DMatrix <Vector<T,3> > >& a, int m1, int m2, int d1, int d2, T s_u = T(0), T s_v = T(0), T e_u = T(0), T e_v = T(0)) const override;
      void                       resample( DMatrix<DMatrix <Vector<T,3> > >& p, const DVector<PreMat<T>>& bu, const DVector<PreMat<T>>& bv, int m1, int m2, int d1, int d2 ) const;

      // Help functions
      void                       makeIndex( std::vector<int>& ind, int i, int k, int n) const;
      void                       multEval( DMatrix<Vector<T,3>>& p, const DMatrix<T>& bu, const DMatrix<T>& bv, const std::vector<int>& i, const std::vector<int>&  j, int du, int dv) const;
      void                       initKnot( DVector<T>& t, bool& c, int& k, const DVector<T>& g, int n, int d, T dt = T(0) );
      void                       initKnot2( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d );

      void                       preSample( DVector< PreMat<T> >& p, const DVector<T>& t, int m, int d, int n, T start, T end );
      void                       makePartition( DVector<VisuPar>& vp, const DVector<T>& t, int k, int dis, int m );
      Vector<int,2>              _map1(int i) const;
      int                        _map2(int i, int j) const;



#ifdef GM_STREAM
      //**********************************************
      //****** IOSTREAM overloaded operators *********
      //**********************************************

  public:

      // Constructor
      template <typename T_Stream>
      PBSplineSurf( T_Stream& in, int /*st*/ )     { init(); prIn(in); }

      // ordinary stream operators
      template <typename T_Stream>
      friend T_Stream& operator << ( T_Stream& o, const PBSplineSurf<T>& s ) { s.prOut(o); return o; }

      template <typename T_Stream>
      friend T_Stream& operator >> ( T_Stream& i, PBSplineSurf<T>& s )       { s.prIn(i); return i; }

  private:

      // privat help function for streaming
      template <typename T_Stream>
      void prOut(T_Stream& out) const {

          if(_cu) {  // closed in u-direction
              DVector<T> u(_u.getDim()-2*_du);
              for(int i = 0; i < u.getDim(); i++) u[i] = _u(i+_du);
              out << u << std::endl << std::endl;
          }
          else
              out << _u << std::endl << std::endl;

          if(_cv) {  // closed in v-direction
              DVector<T> v(_v.getDim()-2*_dv);
              for(int i = 0; i < v.getDim(); i++) v[i] = _v(i+_dv);
              out << v << std::endl << std::endl;
          }
          else
              out << _v << std::endl << std::endl;

          out << _c << std::endl << std::endl;
      }

      template <typename T_Stream>
      void prIn(T_Stream& in) {

          in >> _u >> _v;
          in >> _c;

          // Set order
          _ku = _u.getDim()-_c.getDim1();
          _kv = _v.getDim()-_c.getDim2();

          if(_ku == 1) {  // closed in u-direction
              DVector<T> u = _u;
              in >> _du;
              initKnot( _u, _cu, _ku, u, _c.getDim1(), _du);
          }
          else {
              _cu = false;
              _du = _ku-1;
          }

          if(_kv == 1) {  // closed in v-direction
              DVector<T> v = _v;
              in >> _dv;
              initKnot( _v, _cv, _kv, v, _c.getDim2(), _dv);
          }
          else {
              _cv = false;
              _dv = _kv-1;
          }
      }
#endif


  }; // END class PBSplineSurf


} // END namepace GMlib


// Include PBSplineSurf class function implementations
#include "gmpbsplinesurf.c"


#endif // GM_PARAMETRICS_SURFACE_PBSPLINESURF_H


