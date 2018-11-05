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




#ifndef GM_PARAMETRICS_SURFACE_PCYLINDER_H
#define GM_PARAMETRICS_SURFACE_PCYLINDER_H


#include "../gmpsurf.h"



namespace GMlib {

  template <typename T>
  class PCylinder : public PSurf<T,3> {
    GM_SCENEOBJECT(PCylinder)
  public:
    PCylinder( T rx = 1, T ry = 2, T h = 2 );
    PCylinder( const PCylinder<T>& copy );
    virtual ~PCylinder();

    // Public local functions
    T             getHeight() const;
    T             getRadiusX() const;
    T             getRadiusY() const;
    void          setConstants( T rx, T ry, T h );


    //***************************************
    //****** Virtual public functions  ******
    //***************************************

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
    virtual void      init();

    // Protected data for the surface
    T             _rx;
    T             _ry;
    T             _h;

  }; // END class PCylinder


} // END namespace GMlib

// Include PCylinder class function implementations
#include "gmpcylinder.c"



#endif // GM_PARAMETRICS_SURFACE_PCYLINDER_H


