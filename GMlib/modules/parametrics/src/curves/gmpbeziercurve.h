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



#ifndef GM_PARAMETRICS_CURVES_PBEZIERCURVE_H
#define GM_PARAMETRICS_CURVES_PBEZIERCURVE_H


#include "../gmpcurve.h"



namespace GMlib {

  template <typename T, int n>
  class Selector;

  template <typename T>
  class SelectorGridVisualizer;





  template <typename T>
  class PBezierCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(PBezierCurve)

  private:
    struct EditSet {
       int         ind;            //!< Index of a selector and thus a control point
       Vector<T,3> dp;             //!< The distance the control point has been moved
       EditSet(int i, const Vector<T,3>& d): ind(i),dp(d){}
    };



  public:
    PBezierCurve( const DVector< Vector<T,3> >& c );
    PBezierCurve( const DVector< Vector<T,3> >& c, T s, T t, T e );
    PBezierCurve( const PBezierCurve<T>& copy );
    virtual ~PBezierCurve();

    int             getDegree() const;
    bool            isSelectorsVisible() const;
    void            updateCoeffs( const Vector<T,3>& d );

    const DVector<Vector<T,3>>&
                    getControlPoints() const;
    void            setControlPoints( const DVector<Vector<T,3>>& cv );

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from SceneObject
    // These functions are not meant for public use, it is for editing on the curve
    void            edit( int selector, const Vector<T,3>& dp) override;
    void            replot() const override;
    void            toggleSelectors() override;

    // virtual from PCurve
    void            sample( int m, int d ) override;
    void            showSelectors( T rad = T(1), bool grid = true,
                                   const Color& selector_color = GMcolor::darkBlue(),
                                   const Color& grid_color = GMcolor::lightGreen() ) override;
    void            hideSelectors() override;

  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval( T t, int d = 0, bool l = false ) const override;
    T               getStartP() const override;
    T               getEndP()   const override;

    // Protected intrinsic data for the curve
    DVector< Vector<T, 3> >     _c;

    // Selectors and selector grid
    bool                        _selectors;  //!< Mark if we have selectors or not
    bool                        _grid;       //!< Mark if we have a selector grid or not
    SelectorGridVisualizer<T>*  _sgv;        //!< Selectorgrid
    std::vector<Selector<T,3>*> _s;          //!< A set of selectores (spheres)
    T                           _selector_radius;
    Color                       _selector_color;
    Color                       _grid_color;

    // Pre-evaluation of bernstein polynomials at the sample values (basis functions)
    mutable std::vector<DMatrix<T>> _pre;        //!< Pre-evaluated basis functions

    mutable bool                    _c_moved;    //!< Mark that we are editing, moving controll points
    mutable std::vector<EditSet>    _pos_change; //!< The step vector of control points that has been moved


    // Local help functions
    void       reSample( int d, Sphere<T,3>& s ) const;

    void       updateSamples() const;
    void       makeBernsteinMat( int m, int d = 0, T scale = T(1) ) const;
    void       multEval(DVector<Vector<T,3>>& p, const DMatrix<T>& bsh, int d) const;
    void       comp(DVector<Vector<T,3>>& p, const DMatrix<T>& m, const Vector<T,3>& c, int k) const;
    void       init(); // Help function to ensure consistent initialization


#ifdef GM_STREAM
      //**********************************************
      //****** IOSTREAM overloaded operators *********
      //**********************************************

  public:

      // Constructor
      template <typename T_Stream>
      PBezierCurve( T_Stream& in, int /*st*/ ):PCurve<T,3>(0,0) { init(); prIn(in); }

      // ordinary stream operators
      template <typename T_Stream>
      friend T_Stream& operator<<( T_Stream& o, const PBezierCurve<T>& s ) { s.prOut(o); return o; }

      template <typename T_Stream>
      friend T_Stream& operator>>( T_Stream& i, PBezierCurve<T>& s ) { s.prIn(i); return i; }

  private:

      // privat help function for streaming
      template <typename T_Stream>
      void prOut(T_Stream& out) const { out << _c << std::endl; }

      template <typename T_Stream>
      void prIn(T_Stream& in) { in >> _c; }

#endif

  }; // END class PBezierCurve


} // END namepace GMlib

// Include PBezierCurve class function implementations
#include "gmpbeziercurve.c"


#endif // GM_PARAMETRICS_CURVES_PBEZIERCURVE_H

