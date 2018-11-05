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




#ifndef GM_PARAMETRICS_SURFACE_PROTATIONALSURF_H
#define GM_PARAMETRICS_SURFACE_PROTATIONALSURF_H

// STL
#include <string>

// GMlib
#include "../gmpsurf.h"
#include "../gmpcurve.h"


namespace GMlib {

  template <typename T>
  class PRotationalSurf : public PSurf<T,3> {
    GM_SCENEOBJECT(PRotationalSurf)
  public:
    PRotationalSurf( PCurve<T,3>* cu, T start_par = T(0), T end_par = T(M_2PI) );
    PRotationalSurf( const PRotationalSurf<T>& su );
    virtual ~PRotationalSurf();

    // Public local functions
    T             getRadius() const;
    void          setRadius( T radius );

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
    PCurve<T,3>*  _cu;
    T             _start_par;
    T             _end_par;

  }; // END class PRotationalSurf

} // END namespace GMlib


// Include PRotationalSurf class function implementations
#include "gmprotationalsurf.c"


#endif // GM_PARAMETRICS_SURFACE_PROTATIONALSURF_H
