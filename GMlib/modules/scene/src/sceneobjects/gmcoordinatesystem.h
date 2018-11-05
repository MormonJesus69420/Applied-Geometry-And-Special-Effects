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



#ifndef GM_SCENE_SCENEOBJECTS_COORDINATESYSTEM_H
#define GM_SCENE_SCENEOBJECTS_COORDINATESYSTEM_H


#include "../gmsceneobject.h"

// stl
#include <string>



namespace GMlib {


  enum GM_COORDSYS {
    GM_COORDSYS_LOCAL,
    GM_COORDSYS_GLOBAL,
    GM_COORDSYS_PARENT
  };

  class CoordinateSystem : public SceneObject {
    GM_SCENEOBJECT(CoordinateSystem)
  public:
    CoordinateSystem();
    CoordinateSystem( const CoordinateSystem& copy );
    ~CoordinateSystem();

    GM_COORDSYS        getCoordinateSystem() const;
    void               setCoordinateSystem( GM_COORDSYS coordsys );

  protected:
    void               localDisplay( const DefaultRenderer* ) const override;

    GM_COORDSYS        _cs;


  }; // END class CoordinateSystem

} // END namespace GMlib


#endif // __gmCOORDINATESYSTEM_H__





