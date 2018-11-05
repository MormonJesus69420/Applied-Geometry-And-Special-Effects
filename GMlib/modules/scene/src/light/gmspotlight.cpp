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




#include "gmspotlight.h"
#include "../gmscene.h"


namespace GMlib {





  /*! SpotLight::SpotLight(): PointLight()
   * \brief Pending Documentation
   *
   * Pending Documentation
   */
  SpotLight::SpotLight() {

    _dir = Vector<float,3>(0,0,1);
  }


  /*! SpotLight::SpotLight( const Point<float,3>& pos, const Vector<float,3>& dir, Angle cut_off = 90 )
   * \brief Pending Documentation
   *
   * Pending Documentation
   */
  SpotLight::SpotLight(
    const Point<float,3>& pos,
    const Vector<float,3>& dir,
    Angle cut_off
  ) : PointLight(pos) {

    setCutOff(cut_off);
    rotateParent( getDir().getAngle(dir), getDir()^dir );
  }


  /*! SpotLight::SpotLight( const Color& amb, const Color& dif, const Color& spe, const Point<float,3>& pos, const Vector<float,3>& dir, Angle cut_off = 90 )
   * \brief Pending Documentation
   *
   * Pending Documentation
   */
  SpotLight::SpotLight(
    const Color& amb,
    const Color& dif,
    const Color& spe,
    const Point<float,3>& pos,
    const Vector<float,3>& dir,
    Angle cut_off
  ) : PointLight(amb,dif,spe,pos) {

    setCutOff(cut_off);
    rotateParent( getDir().getAngle(dir), getDir()^dir );
  }


  /*! SpotLight::SpotLight( const SpotLight& pl)
   * \brief Pending Documentation
   *
   * Pending Documentation
   */
  SpotLight::SpotLight( const SpotLight& copy ) : PointLight( copy ) {

    setCutOff(copy._cutoff);
  }


  /*! SpotLight::~SpotLight()
   * \brief Pending Documentation
   *
   * Pending Documentation
   */
  SpotLight::~SpotLight() {}


  const Angle& SpotLight::getCutOff() const {

    return _cutoff;
  }

  /*! void SpotLight::setCuttoff(const Angle cut_off)
   * \brief Pending Documentation
   *
   * Pending Documentation
   */
  void SpotLight::setCutOff( const Angle& cut_off ) {

    _cutoff = cut_off;
  }
}
