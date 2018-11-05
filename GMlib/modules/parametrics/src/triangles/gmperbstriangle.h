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




#ifndef GM_PARAMETRICS_TRIANGLES_PERBSTRIANGLE_H
#define GM_PARAMETRICS_TRIANGLES_PERBSTRIANGLE_H


#include "../gmptriangle.h"
#include "../evaluators/gmbasistriangleerbs.h"


namespace GMlib {

  template <typename T>
  class PERBSTriangle : public PTriangle<T,3> {
    GM_SCENEOBJECT(PERBSTriangle)
  public:
    PERBSTriangle( PTriangle<T,3>* c0, PTriangle<T,3>* c1, PTriangle<T,3>* c2,
                   BasisTriangleType t = BFBS_2 );
    virtual ~PERBSTriangle();

    void                            edit( SceneObject *obj ) override;
    DVector< PTriangle<T,3>* >&     getLocalPatches();
    virtual void                    hideLocalPatches();
    bool                            isLocalPatchesVisible() const;
    virtual void                    showLocalPatches();

    void                            resetBasis( BasisTriangleType type );

    /* virtual from PTriangle */
    Vector<Point<T,3>,3>            getPoints() const override;


  protected:
    BasisTriangleERBS<T>            *_B;
    DVector< PTriangle<T,3>* >        _c;

    void                            eval( T u, T v, T w, int d ) const override;

    void                            insertPatch( PTriangle<T,3> *patch );

  }; // END class PERBSTriangle


} // END namespace GMlib

// Include PERBSTriangle class function implementations
#include "gmperbstriangle.c"

#endif // GM_PARAMETRICS_TRIANGLES_PERBSTRIANGLE_H
