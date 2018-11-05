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


#ifndef GM_SCENE_VISUALIZERS_SELECTORVISUALIZER_H
#define GM_SCENE_VISUALIZERS_SELECTORVISUALIZER_H

#include "../gmvisualizer.h"

// local
#include "../utils/gmmaterial.h"

// gmlib
#include <core/utils/gmcolor.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/gmprogram.h>


namespace GMlib {

  class Camera;

  class SelectorVisualizer: public Visualizer {
    GM_VISUALIZER(SelectorVisualizer)
  public:
    SelectorVisualizer( float radius = 0.27, Material mat = GMmaterial::jade() );
    SelectorVisualizer( int m1, int m2, float radius = 0.27, Material mat = GMmaterial::jade() );

    void                          render( const SceneObject* obj, const DefaultRenderer* renderer) const override;
    void                          renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const override;

    static SelectorVisualizer*    getInstance();

  private:
    void                          makeGeometry( float radius, int m1, int m2 );

    GL::Program                   _prog;
    GL::Program                   _color_prog;

    // Cube buffer objects
    GL::VertexBufferObject        _vbo;
    GL::IndexBufferObject         _ibo;

    int                           _top_bot_verts;
    int                           _mid_strips;
    int                           _mid_strips_verts;

    Material                      _mat;



    static SelectorVisualizer*    _s_instance;

  }; // END class SelectorVisualizer

} // END namespace GMlib


#endif // GM_SCENE_VISUALIZERS_SELECTORVISUALIZER_H
