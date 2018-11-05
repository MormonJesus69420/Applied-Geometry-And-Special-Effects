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




#ifndef GM_PARAMETRICS_PTRIANGLE_H
#define GM_PARAMETRICS_PTRIANGLE_H


#include "gmparametrics.h"

// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmdvector.h>


namespace GMlib {

  template <typename T, int n>
  class PTriangleVisualizer;

  template <typename T, int n>
  class PTriangle : public Parametrics<T,2,n> {

  public:
    PTriangle( int samples = 20 );
    PTriangle( const PTriangle<T,n>& copy );

    ~PTriangle();


    const DVector<Vector<T,n> >&      evaluate(const Point<T,3> & p, int d) const;
    const DVector<Vector<T,n> >&      evaluate( T u, T v, int d) const;
    const DVector<Vector<T,n> >&      evaluateGlobal(const Point<T,3> & p, int d) const;
    const DVector<Vector<T,n> >&      evaluateGlobal( T u, T v, int d) const;
    const DVector<Vector<T,n> >&      evaluateParent(const Point<T,3> & p, int d) const;
    const DVector<Vector<T,n> >&      evaluateParent( T u, T v, int d) const;

    const Point<T,n>&                 operator()( T u, T v ); // w = 1-u-v
    const Vector<T,n>&                getDerU( T u, T v ) const;
    const Vector<T,n>&                getDerV( T u, T v ) const;
    const Vector<T,n>&                getDerW( T u, T v ) const;
    const Vector<T,n>&                getDerUU( T u, T v ) const;
    const Vector<T,n>&                getDerUV( T u, T v ) const;
    const Vector<T,n>&                getDerUW( T u, T v ) const;
    const Vector<T,n>&                getDerVV( T u, T v ) const;
    const Vector<T,n>&                getDerVW( T u, T v ) const;
    const Vector<T,n>&                getDerWW( T u, T v ) const;
    const Vector<T,n>&                getNormal( T u, T v ) const;
    UnitVector<T,n>                   getUnitNormal( T u, T v ) const;

    virtual T                         getCurvatureGauss( T u, T v ) const;
    virtual T                         getCurvatureMean( T u, T v ) const;



    const Point<T,n>&                 operator()(const Point<T,3> & p);
    const Vector<T,n>&                getDerU(const Point<T,3> & p) const;
    const Vector<T,n>&                getDerV(const Point<T,3> & p) const;
    const Vector<T,n>&                getDerW(const Point<T,3> & p) const;
    const Vector<T,n>&                getDerUU(const Point<T,3> & p) const;
    const Vector<T,n>&                getDerUV(const Point<T,3> & p) const;
    const Vector<T,n>&                getDerUW(const Point<T,3> & p) const;
    const Vector<T,n>&                getDerVV(const Point<T,3> & p) const;
    const Vector<T,n>&                getDerVW(const Point<T,3> & p) const;
    const Vector<T,n>&                getDerWW(const Point<T,3> & p) const;
    const Vector<T,n>&                getNormal(const Point<T,3> & p) const;
    UnitVector<T,n>                   getUnitNormal(const Point<T,3> & p) const;

    T                                 getCurvatureGauss(const Point<T,3> & p) const;
    T                                 getCurvatureMean(const Point<T,3> & p) const;

    const Vector<T,n>&                getDer_d(const Point<T,3> & p, const Vector<T,3> & d) const;


    void                              enableDefaultVisualizer( bool enable = true );
    const PTriangleVisualizer<T,n>*   getDefaultVisualizer() const;
    void                              toggleDefaultVisualizer();
    void                              insertVisualizer( Visualizer *visualizer );
    void                              removeVisualizer( Visualizer *visualizer );

    virtual void                      replot( int m = 2, int d = 1 );
    virtual void                      resample( DVector<DVector<Vector<T,n> > > &p, int m, int d );


//    std::string                       getIdentity() const;

    void                              setEval(int d);
    Parametrics<T,2,n>*               split( T t, int uv );

    virtual void                      updateCoeffs( const Vector<T,n>& d );
    virtual bool                      isClosestPoint( const Point<T,n>& p, T& u, T& v ) const;
//    virtual void                      estimateClpPar(const Point<T,n>& p, T& u, T& v) {}
//    virtual void                      setBp( Array<Point<T,n> >bp ) {}

    void                              setTriangNr(bool all = true, int nr = 0);

    virtual Vector<Point<T,n>,3>      getPoints() const;

  protected:
    Array< PTriangleVisualizer<T,n>* >  _ptriangle_visualizers;
    PTriangleVisualizer<T,n>            *_default_visualizer;

    mutable int                       _no_sam;      //  int		__sam;
    mutable DVector< Vector<T,n> >    _p;           //  DMatrix<Vector<T,n> >	__p;
    mutable Vector<T,n>               _n;           //  Vector<T,n>		__n; // For display in 3D
    mutable T                         _u;           //  T	__u;
    mutable T                         _v;           //  T	__v;
    mutable int                       _d;           //  int	__d;
    mutable bool                      _diagonal;    //  bool	__diagonal; // True if only upper left half is evaluated.
    mutable int                       _default_d;   //  int	__defalt_d; // used by operator() for number of derivative to evaluate.

    bool                              _all;
    int                               _t_nr;
    Point<T,n>                        _pt[4];

    virtual void                      eval( T u, T v, T w, int d ) const = 0;
    void                              eval( const Point<T,3>& p, int d ) const;

    virtual void                      setSurroundingSphere( const DVector<DVector<Vector<T,n> > > &p );

  private:
    void                              _eval( T u, T v, int d ) const;
    int                               _sum(int i) const;
    void                              _init();

    void                              _fuForm( T u, T v, T& E, T& F, T& G, T& e, T& f, T& g) const;

    void                              resample1( DVector<DVector<Vector<T,n> > > &p, int m, int d );
    void                              resample2( DVector<DVector<Vector<T,n> > > &p, int m, int a, int b );
  }; // END class PTriangle





} // END namespace GMlib



// Include PTriangle class function implementations
#include "gmptriangle.c"

#endif // GM_PARAMETRICS_PTRIANGLE_H
