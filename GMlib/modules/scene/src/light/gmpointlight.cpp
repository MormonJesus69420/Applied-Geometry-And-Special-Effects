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



// System includes
#include <float.h>

// GMlib includes
#include "gmpointlight.h"
#include "../gmscene.h"
#include "../camera/gmcamera.h"


namespace GMlib {



  /*! PointLight::PointLight()
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  PointLight::PointLight() : Light(), SceneObject(), _pos(0.0f), _light_sphere(Point<float,3>(0,0,0), FLT_MAX ) {

    _type_id  = GM_SO_TYPE_LIGHT;
    setAttenuation( 0.8, 0.002, 0.0008 );
  }


  /*! PointLight::PointLight(	const Point<float,3>& pos)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  PointLight::PointLight(	const Point<float,3>& pos) : Light(), _pos(0.0f) {

    translateParent(pos);
    _type_id  = GM_SO_TYPE_LIGHT;
    setAttenuation( 0.8, 0.002, 0.0008 );
  }


  /*! PointLight::PointLight(	const Color& amb, const Color& dif, const Color& spe, const Point<float,3>& pos )
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  PointLight::PointLight(
        const Color& amb,
        const Color& dif,
        const Color& spe,
        const Point<float,3>& pos
  ) : Light(amb,dif,spe),SceneObject(),_pos(0.0f) {

    translateParent(pos);
    _type_id  = GM_SO_TYPE_LIGHT;
    setAttenuation( 0.9, 0.0002, 0.00008 );
  }


  /*! PointLight::PointLight(	const PointLight& pl)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  PointLight::PointLight(	const PointLight& pl) : Light(pl), SceneObject(pl), _pos(pl._pos) {

    _type_id  = GM_SO_TYPE_LIGHT;
  }


  /*! PointLight::~PointLight()
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  PointLight::~PointLight() {}


  /*! void PointLight::culling( const Frustum& frustum )
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  void PointLight::culling( const Camera& cam ) {

    if (!isCullable()) { Light::_culled = false; return;}
    int k = cam.isInsideFrustum(_light_sphere);
    if (k < 0) Light::_culled = true;
    else Light::_culled = false;
  }


  /*! void PointLight::setAttenuation(float constant, float linear, float quadratic)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  void PointLight::setAttenuation(float constant, float linear, float quadratic) {

    _attenuation[0] = constant;
    _attenuation[1] = linear;
    _attenuation[2] = quadratic;

    calculateRadius( constant, linear, quadratic );
  }


  /*! void PointLight::setAttenuation(const float att[])
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  void PointLight::setAttenuation(const float att[]) {

    setAttenuation(att[0],att[1],att[2]);
  }


  /*! void PointLight::calculateRadius(float constant, float linear, float quadratic)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  void PointLight::calculateRadius(float constant, float linear, float quadratic) {

    double aa, solution;
    if (GMutils::compValueF(linear,0.0f) && GMutils::compValueF(quadratic,0.0f))
    {
      setCullable(false); // Disable culling
      solution = FLT_MAX;
    }
    else if (GMutils::compValueF(quadratic,0.0f))
    {
      if(constant <= _min_light_contribution)
        solution = (_min_light_contribution-constant) / linear;
      else
        solution = 0;
    }
    else
    {
      if(constant <= _min_light_contribution)
        aa= sqrt(linear*linear + 4 * (_min_light_contribution-constant) * quadratic);
      else
        aa = linear;
      solution = (aa -linear)/(2 * quadratic);
    }
    _light_sphere.resetValue(solution);
  }
}
