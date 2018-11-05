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




#ifndef GM_PARAMETRICS_SURFACE_PSPHERE_H
#define GM_PARAMETRICS_SURFACE_PSPHERE_H

#include "../gmpsurf.h"


namespace GMlib {

  template <typename T>
  class PSphere : public PSurf<T,3> {
    GM_SCENEOBJECT(PSphere)
  public:
    PSphere( T radius = T(1) );
    PSphere( T radius, int s1, int s2 );
    PSphere( const PSphere<T>& copy );
    virtual ~PSphere();

    // Public local functions
    T             getRadius() const;
    void          setRadius( T radius );

    //***************************************
    //****** Virtual public functions  ******
    //***************************************

    // from PSurf
    void          replot( int m1 = 0, int m2 = 0, int d1 = 0, int d2 = 0 ) override;
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
    T             _radius;

  private:
    // Virtual function from PSurf
    void   resampleNormals( const DMatrix<DMatrix<Vector<T,3> > >& sample, DMatrix<Vector<T,3> >& normals ) const override;

    // Help function to initiate
    void   makeNmap( int s = 64, int t = 64) const;
    void   makeNmap( DMatrix<Vector<T,3>>& nm, int m, int s = 64, int t = 64) const;
    void   resample( DVector< DVector< Vector<T,3> > >& p, int m ) const;
    void   init_mat1();

    // static normal map for display
    static DMatrix< Vector<T,3> >  _nmap;
    static DVector<DVector<int> >  _mat1;
    static DVector<Vector<int,2> > _mat1_size;


  }; // END class PSphere

} // END namespace GMlib

// Include PSphere class function implementations
#include "gmpsphere.c"


#endif // GM_PARAMETRICS_SURFACE_PSPHERE_H

