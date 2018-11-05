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



#ifndef GM_SCENE_LIGHT_SPOTLIGHT_H
#define GM_SCENE_LIGHT_SPOTLIGHT_H


#include "gmpointlight.h"

// stl
#include <string>


namespace GMlib{

  /*! \class SpotLight gmspotlight.h <gmSpotLight>
   * \brief Pending Documentatioo
   *
   *  Pending Documentatioo
   */

  class SpotLight : public PointLight {
    GM_SCENEOBJECT(SpotLight)
  public:
    SpotLight();
    SpotLight( const Point<float,3>& pos,
               const Vector<float,3>& dir,
               Angle cut_off );
    SpotLight( const Color& amb,
               const Color& dif,
               const Color& spe,
               const Point<float,3>& pos,
               const Vector<float,3>& dir,
               Angle cut_off = 90);
    SpotLight( const SpotLight& pl);

    virtual ~SpotLight();

    const Angle&          getCutOff() const;
    virtual void          setCutOff( const Angle& cut_off);

  protected:
    Angle                 _cutoff;


  };  // END class SpotLight




}  // END namespace GMlib


#endif // GM_SCENE_LIGHT_SPOTLIGHT_H
