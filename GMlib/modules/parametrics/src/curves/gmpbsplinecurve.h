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



#ifndef GM_PARAMETRICS_CURVES_PBSPLINECURVE_H
#define GM_PARAMETRICS_CURVES_PBSPLINECURVE_H


#include "../gmpcurve.h"



namespace GMlib {

template <typename T, int n>
class Selector;

template <typename T>
class SelectorGridVisualizer;





  template <typename T>
  class PBSplineCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(PBSplineCurve)

  private:
    struct EditSet {
        int         ind;            //!< Index of a selector and thus a control point
        Vector<T,3> dp;             //!< The distance the control point has been moved
        EditSet(int i, const Vector<T,3>& d): ind(i),dp(d){}
    };

    struct CpIndex : public DVector<Vector<int,2>> {
    };                   //!<Indices of sample points affected by a given control point




  public:
    PBSplineCurve( const DVector< Vector<T, 3> >& c, const DVector<T>& t, int d=0 );
    PBSplineCurve( const DVector< Vector<T, 3> >& c, int d, bool closed );
    PBSplineCurve( const PCurve<T,3>& curve, int d, int n, int m );
    PBSplineCurve( const PBSplineCurve<T>& curve );
    virtual ~PBSplineCurve();

    // Public local functions
    void            setDegree( int d );
    int             getDegree() const;
    void            setClosed( bool closed, T dt=T(0));
    void            setControlPoints( const DVector< Vector<T,3> >& cv );
    void            setKnotVector( const DVector<T>& t, bool closed = false );
    void            setPartitionCriterion(int d);

    const DVector<Vector<T,3>>& getControlPoints() const;
    const DVector<T>&           getKnotVector() const;

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from SceneObject
    // The two first functions below are not meant for public use, they are for editing on the curve
    void            edit(int selector, const Vector<T,3>& dp) override;
    void            replot() const override;
    void            toggleSelectors() override;
    void            toggleClose() override;

    // from PCurve
    bool            isClosed() const override;
    void            sample( int m, int d ) override;
    void            showSelectors( T radius = T(1), bool grid = true,
                                   const Color& selector_color = GMcolor::darkBlue(),
                                   const Color& grid_color = GMcolor::lightGreen() ) override;
    void            hideSelectors() override;


  protected:
    // Virtual protected functions from PCurve, which have to be implemented locally
    void            eval(T t, int d = 0, bool l = false) const override;
    T               getStartP() const override;
    T               getEndP()   const override;


    // Protected intrinsic data for the curve
    DVector<Vector<T,3>>         _c;   //!< control points (control polygon)
    DVector<T>                   _t;   //!< knot vector
    int                          _d;   //!< polynomial degree
    int                          _k;   //!< order of B-spline (_k = _d + 1)
    bool                         _cl;  //!< closed (or open) curve?

    // Selectors and selector grid
    bool                         _selectors;        //!< Mark if we have selectors or not
    bool                         _grid;             //!< Mark if we have a selector grid or not
    SelectorGridVisualizer<T>*   _sgv;              //!< Selectorgrid
    std::vector<Selector<T,3>*>  _s;                //!< A set of selectors (spheres)
    T                            _selector_radius;
    Color                        _selector_color;
    Color                        _grid_color;

    // Pre-evaluation and visualization
    mutable std::vector<CpIndex> _cp_index;   //!< Indices of sample points affected by the control points

    // Partitioning of the curve based on continuity criteria
    mutable int                      _pct;        //!< Partition criteria (continuity C^_pct)
    mutable std::vector<PreBasis<T>> _pre_basis;  //!< Pre-evaluated basis functions for each partition

    mutable bool                 _c_moved;    //!< Mark that we are editing, moving controll points
    mutable std::vector<EditSet> _pos_change; //!< The step vector of control points that is moved


    // Local help functions
    void      comp(DVector<Vector<T,3>>& p, const DMatrix<T>& m, const Vector<T,3>& c, int k) const;
    void      transKnot( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d );
    void      transKnotClose( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d, T dt = T(0) );
    void      spesKnotClose( DVector<T>& t, bool& c, const DVector<T>& g, int n, int d );
    void      updateKnotOpen(int d);
    void      updateKnotClosed(int d);
    void      multEval(DVector<Vector<T,3>>& p, const DMatrix<T>& bsh, const std::vector<int>& ii, int d) const;
    void      preSample( int d );
    void      updatSamples() const;
    void      makePartition( int m ) const;
    void      makeLeastSquareMatrix( DMatrix<T>& A, const std::vector<T>& sample, int d, int n) const;
    void      generateKnotVector( T s, T e, bool closed );
    void      generateKnotVector( bool closed = false );


    void      init(int d);  // Help function to ensure consistent initialization


#ifdef GM_STREAM
    //**********************************************
    //****** IOSTREAM overloaded operators *********
    //**********************************************

  public:

    // Constructor
    template <typename T_Stream>
    PBSplineCurve( T_Stream& in, bool /*st*/ ) { init(1); prIn(in);}

    // ordinary stream operators
    template <typename T_Stream>
    friend T_Stream& operator<<( T_Stream& o, const PBSplineCurve<T>& s ) { s.prOut(o); return o; }

    template <typename T_Stream>
    friend T_Stream& operator>>( T_Stream& i, PBSplineCurve<T>& s ) { s.prIn(i); return i; }

  private:
    // privat help function for streaming

    template <typename T_Stream>
    void prOut(T_Stream& out) const {
        if(isClosed()){
            DVector<T> nt(_t.getDim()-_d-_d);
            for(int i=0; i<nt.getDim(); i++)
                nt[i]=_t[i+_d];
            out << nt << "\n\n" << _c << "\n\n" << _d << std::endl;
        }
        else {
            out << _t << "\n\n" << _c << std::endl;
        }
    }

      template <typename T_Stream>
    void prIn(T_Stream& in) {
        in >> _t;
        in >> _c;

        _k = _t.getDim() - _c.getDim();
        if(_k == 1) {     // closed
            in >> _d;
            _k = _d+1;
            DVector<T> u = _t;
            spesKnotClose( _t, _cl, u, _c.getDim(), _d);
        }
        else              // open
            _d = _k-1;
    }

#endif
  }; // END class PBSplineCurve


} // END namepace GMlib

// Include PBSplineCurve class function implementations
#include "gmpbsplinecurve.c"


#endif // GM_PARAMETRICS_CURVES_PBSPLINECURVE_H


