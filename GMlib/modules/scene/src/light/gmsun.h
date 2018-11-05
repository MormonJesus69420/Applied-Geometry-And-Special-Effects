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




#ifndef GM_SCENE_LIGHT_SUN_H
#define GM_SCENE_LIGHT_SUN_H


#include "../gmsceneobject.h"
#include "gmlight.h"

// stl
#include <string>


namespace GMlib{



  /*!	\class Sun gmsun.h <gmSun>
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  class Sun : public Light, public SceneObject {
    GM_SCENEOBJECT(Sun)

  public:
    Sun(const Vector<float,3>& dir = Vector<float,3>(-1,-1,-1));

    Color                   getGlobalAmbient() const;
    const Vector<float,3>&  getDir() const;
    void                    scaleDayLight( double d );
    void                    setDayLight( const Color& amb = Color( 1.0f, 1.0f, 1.0f ) );

  private:
    Vector<float,3>         _dir;

  }; // END class Sun






  /*! void Sun::scaleDayLight(double d)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  inline
  void Sun::scaleDayLight(double d) {

    setIntensity( d, 1 );
  }


}	// END namespace GMlib


#endif // GM_SCENE_LIGHT_SUN_H
