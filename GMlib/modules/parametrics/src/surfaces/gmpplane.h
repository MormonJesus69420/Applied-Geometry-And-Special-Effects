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




#ifndef GM_PARAMETRICS_SURFACE_PPLANE_H
#define GM_PARAMETRICS_SURFACE_PPLANE_H

#include "../gmpsurf.h"


namespace GMlib {

  template <typename T>
  class PPlane : public PSurf<T,3> {
    GM_SCENEOBJECT(PPlane)
  public:
    PPlane( const Point<T,3>& p, const Vector<T,3>& u, const Vector<T,3>& v );
    PPlane( const PPlane<T>& copy );
    virtual ~PPlane();

    const UnitVector<T,3>&    getNormal() const;
    const Vector<T,3>&        getU() const;
    const Vector<T,3>&        getV() const;

    void                      setP( const Point<T,3>& p );
    void                      setU( const Vector<T,3>& u );
    void                      setV( const Vector<T,3>& v );

    //***************************************
    //****** Virtual public functions  ******
    //***************************************

    // from PSurf
    bool          isClosedU()  const override;
    bool          isClosedV()  const override;

  protected:
    // Virtual function from PSurf that has to be implemented locally
    void          eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true ) const override;
    T             getStartPU() const override;
    T             getEndPU()   const override;
    T             getStartPV() const override;
    T             getEndPV()   const override;

    // Help function to ensure consistent initialization
    virtual void  init();

    // Protected data for the surface
    Point<T,3>		          _pt;
    Vector<T,3>		          _u;
    Vector<T,3>               _v;
    UnitVector<T,3>           _n;

  }; // END class PPlane

} // END namespace GMlib

// Include PPlane class function implementations
#include "gmpplane.c"



#endif // GM_PARAMETRICS_SURFACE_PPLANE_H
