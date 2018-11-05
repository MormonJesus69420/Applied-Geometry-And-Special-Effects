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




#ifndef GM_PARAMETRICS_SURFACE_PDINISURFACE_H
#define GM_PARAMETRICS_SURFACE_PDINISURFACE_H

#include "../gmpsurf.h"


namespace GMlib {

  template <typename T>
  class PDiniSurface : public PSurf<T,3> {
    GM_SCENEOBJECT(PDiniSurface)
  public:
    PDiniSurface( T radius = T(1), T dist_turns = T(0.1) );
    PDiniSurface( const PDiniSurface<T>& copy );
    virtual ~PDiniSurface();

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PSurf
    bool          isClosedU() const override;
    bool          isClosedV() const override;

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
    T             _r1; // Radius
    T             _r2; // Distance between turns

  }; // END class PDiniSurface


} // END namespace GMlib

// Include PDiniSurface class function implementations
#include "gmpdinisurface.c"



#endif // GM_PARAMETRICS_SURFACE_PDINISURFACE_H
