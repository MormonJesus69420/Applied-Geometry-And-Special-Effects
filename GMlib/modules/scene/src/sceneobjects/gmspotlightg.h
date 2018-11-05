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





#ifndef GM_SCENE_SCENEOBJECTS_SPOTLIGHTG_H
#define GM_SCENE_SCENEOBJECTS_SPOTLIGHTG_H


#include "../light/gmspotlight.h"
#include "gmsphere3d.h"

// stl
#include <string>


namespace GMlib {

  class SpotLightG : public SpotLight {
    GM_SCENEOBJECT(SpotLightG)
  public:
    SpotLightG();
    SpotLightG(
      const Point<float,3>& pos,
      const Vector<float,3>& dir,
      Angle cut_off
    );
    SpotLightG(
      const Color& amb,
      const Color& dif,
      const Color& spe,
      const Point<float,3>& pos,
      const Vector<float,3>& dir,
      Angle cut_off = 90
    );

    SpotLightG( const SpotLight& copy );
    SpotLightG( const SpotLightG& copy );

    void            setCutOff(const Angle& cut_off) override;

  protected:
    // inherited from SceneObject
    void            localDisplay(const DefaultRenderer *) const override;
    void            localSelect(const Renderer *, const Color &) const override;

  private:
    GL::VertexBufferObject    _light_box_geom_vbo;
    int                       _light_box_geom_elements;

    GL::VertexBufferObject    _light_geom_vbo;
    int                       _light_geom_elements;

    GL::Program               _shading_prog;
    GL::Program               _color_prog;


    void            init();


  }; // END class SpotLightG

} // END namespace GMlib


#endif // GM_SCENE_SCENEOBJECTS_SPOTLIGHTG_H
