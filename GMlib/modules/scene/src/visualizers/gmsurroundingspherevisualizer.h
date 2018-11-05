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





#ifndef GM_SCENE_VISUALIZERS_SURROUNDINGSPHEREVISUALIZER_H
#define GM_SCENE_VISUALIZERS_SURROUNDINGSPHEREVISUALIZER_H

#include "../gmvisualizer.h"

// gmlib
#include <core/utils/gmcolor.h>
#include <opengl/gmopengl.h>


namespace GMlib {

  class DefaultRenderer;
  class Sphere3D;

  class SurroundingSphereVisualizer : public GMlib::Visualizer {
    public:
      SurroundingSphereVisualizer();
      ~SurroundingSphereVisualizer();

      void              render(const SceneObject *obj, const DefaultRenderer *renderer) const override;

      void              enableCleanSphere( bool enable );
      void              enableTotalSphere( bool enable );
      const Color&      getCleanSphereColor() const;
      const Color&      getTotalSphereColor() const;
      bool              isCleanSphereEnabled() const;
      bool              isTotalSphereEnabled() const;
      void              setCleanSphereColor( const Color& color );
      void              setTotalSphereColor( const Color& color );
      void              setWireframe( bool wireframe );

    protected:
      enum {
        TOTAL_SPHERE =  0x01,
        CLEAN_SPHERE =  0x02
      };

      unsigned int      _spheres;
      bool              _wireframe;
      Color             _cs_color;
      Color             _ts_color;

      Sphere3D          *_sphere;

  private:
      void              displaySphere(const Sphere<float,3>& ss) const;

    }; // END class SurroundingSphereVisualizer

} // END namespace GMlib

#endif // GM_SCENE_VISUALIZERS_SURROUNDINGSPHEREVISUALIZER_H
