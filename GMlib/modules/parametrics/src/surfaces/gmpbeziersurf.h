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




#ifndef GM_PARAMETRICS_SURFACE_PBEZIERSURF_H
#define GM_PARAMETRICS_SURFACE_PBEZIERSURF_H


#include "../gmpsurf.h"


namespace GMlib {

  template <typename T, int n>
  class Selector;

  template <typename T>
  class SelectorGridVisualizer;




  template <typename T>
  class PBezierSurf : public PSurf<T,3> {
      GM_SCENEOBJECT(PBezierSurf)

  public:
      // Three of four constructors, the last one is defined further down in this file
      PBezierSurf( const DMatrix< Vector<T,3> >& cp);
      PBezierSurf( const DMatrix< Vector<T,3> >& cp, T u_s, T u, T u_e, T v_s, T v, T v_e);
      PBezierSurf( const PBezierSurf<T>& dpbs );

      virtual ~PBezierSurf();

      // Public local functions
      DMatrix< Vector<T,3> >&    getControlPoints() const;
      int                        getDegreeU() const;
      int                        getDegreeV() const;
      bool                       isSelectorsVisible() const;
      void                       setClosed( bool closed_u, bool closed_v );
      void                       setControlPoints( const DMatrix< Vector<T,3> >& cp );
      void                       setScale( T du, T dv );

      // This function is not meant for public use, it is for editing on hierarchically defined surfaces
      void                       updateCoeffs( const Vector<T,3>& d );

      //***************************************
      //****** Virtual public functions  ******
      //***************************************

      // from SceneObject
//      void                       edit(int selector) override;
      void                       edit( int selector, const Vector<T,3>& dp ) override;

      // from PSurf
      bool                       isClosedU() const override;
      bool                       isClosedV() const override;
      void                       showSelectors(T rad = T(1), bool grid = false, const Color& _selector_color = GMcolor::darkBlue(), const Color& grid_color = GMcolor::lightGreen() ) override;
      void                       hideSelectors() override;
      void                       toggleSelectors() override;

  protected:
      // Protected data for the surface
      mutable
      DMatrix< Vector<T,3> >     _c;       // control polygon
      bool                       _cu;      // closed in u-direction
      bool                       _cv;      // closed in v-direction

      T                          _su;      // scale of domain in u-direction (parameter)
      T                          _sv;      // scale of domain in u-direction (parameter)

      DVector< DMatrix< T > >    _ru;      // Pre-evaluation of basis in u-direction
      DVector< DMatrix< T > >    _rv;      // Pre-evaluation of basis in v-direction

      DMatrix< DMatrix< Vector<T,3>>> _pr; // preeval as local surface

      bool                       _selectors;   // Mark if we have selectors or not
      SelectorGridVisualizer<T>* _sgv;         // Selectorgrid
      DMatrix< Selector<T,3>* >  _s;           // A net of selectores (spheres)

      bool                       _c_moved; // Mark that we are editing, moving controll points

      // Virtual function from PSurf that has to be implemented locally
      void                       eval( T u, T v, int d1 = 0, int d2 = 0, bool lu = false, bool lv = false ) const override;
      T                          getStartPU() const override;
      T                          getEndPU()   const override;
      T                          getStartPV() const override;
      T                          getEndPV()   const override;

      // Help function to ensure consistent initialization
      virtual void               init();

  private:

      // Virtual function from PSurf
      void                       resample(DMatrix<DMatrix <Vector<T,3> > >& a, int m1, int m2, int d1, int d2, T s_u = T(0), T s_v = T(0), T e_u = T(0), T e_v = T(0)) const override;
      void                       preSample( int dir, int m ) override;

      // Help functions
      void                       internalPreSample( DVector< DMatrix< T > >& p, int m, int d, T scale, T start, T end );
      void                       multEval(const DMatrix<T>& bu, const DMatrix<T>& bv, int du, int dv) const;


#ifdef GM_STREAM
      //**********************************************
      //****** IOSTREAM overloaded operators *********
      //**********************************************

  public:

      // Constructor
      template <typename T_Stream>
      PBezierSurf( T_Stream& in, int /*st*/ ) { init(); prIn(in); }

      // ordinary stream operators
      template <typename T_Stream>
      friend T_Stream& operator<<( T_Stream& o, const PBezierSurf<T>& s ) { s.prOut(o); return o; }

      template <typename T_Stream>
      friend T_Stream& operator>>( T_Stream& i, PBezierSurf<T>& s ) { s.prIn(i); return i; }

  private:

      // privat help function for streaming
      template <typename T_Stream>
      void prOut(T_Stream& out) const {std::cout << "her er vi" << std::endl; out << _c << std::endl; }

      template <typename T_Stream>
      void prIn(T_Stream& in) { in >> _c; }

#endif
  }; // END class PBezierSurf


} // END namepace GMlib

// Include PBezierSurf class function implementations
#include "gmpbeziersurf.c"


#endif // GM_PARAMETRICS_SURFACE_PBEZIERSURF_H


