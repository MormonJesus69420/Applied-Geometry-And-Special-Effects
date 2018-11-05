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



#ifndef GM_SCENE_LIGHT_POINTLIGHT_H
#define GM_SCENE_LIGHT_POINTLIGHT_H


#include "gmlight.h"
#include "../gmsceneobject.h"

// stl
#include <string>



namespace GMlib{


  class Camera;


  /*! \class PointLight gmpointlight.h <gmPointLight>
   * \brief Pending Documentatioo
   *
   *  Pending Documentatioo
   */
  class PointLight : public Light, public SceneObject {
    GM_SCENEOBJECT(PointLight)
  public:
    PointLight();
    PointLight(  const Point<float,3>& pos);
    PointLight(
      const Color& amb,
      const Color& dif,
      const Color& spe,
      const Point<float,3>& pos
    );
    PointLight(  const PointLight& pl);
    virtual ~PointLight();

    void              culling( const Camera& cam ) override;
    const Point<float,3>&     getAttenuation() const;
    void                      setAttenuation(float constant, float linear, float quadratic);
    void                      setAttenuation(const float att[]);


  protected:
    void                      calculateRadius(float constant, float linear, float quadratic);

    Point<float,3>            _pos;
    Point<float,3>            _attenuation;

  private:
    Sphere<float,3>           _light_sphere;

  };  // END class PointLight












  inline
  const Point<float,3> &PointLight::getAttenuation() const {

    return _attenuation;
  }


}  // END namespace GMlib

#endif // GM_SCENE_LIGHT_POINTLIGHT_H
