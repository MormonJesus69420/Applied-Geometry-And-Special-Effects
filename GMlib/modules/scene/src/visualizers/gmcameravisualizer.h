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



#ifndef GM_SCENE_VISUALIZERS_CAMERAVISUALIZER_H
#define GM_SCENE_VISUALIZERS_CAMERAVISUALIZER_H

#include "../gmvisualizer.h"

// gmlib
#include <core/utils/gmcolor.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/gmprogram.h>


namespace GMlib {

  class Camera;

  class CameraVisualizer : public Visualizer {
    GM_VISUALIZER(CameraVisualizer)
  public:
    CameraVisualizer();

    void              render( const SceneObject* obj, const DefaultRenderer* renderer ) const override;
    void              renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const override;


  private:
    GL::Program               _prog;
    GL::Program               _color_prog;

    GL::VertexBufferObject    _vbo_display;
    GL::VertexBufferObject    _vbo_grip;
    GL::VertexBufferObject    _vbo_viewfinder;
    GL::VertexBufferObject    _vbo_viewfinder_2;
    GL::VertexBufferObject    _vbo_lens;

    GL::VertexBufferObject    _vbo_frame;
    GL::IndexBufferObject     _ibo_frame_np;
    GL::IndexBufferObject     _ibo_frame_fp;
    GL::IndexBufferObject     _ibo_frame_lines;
    GL::VertexBufferObject    _vbo_np, _vbo_fp, _vbo_flines;

    void              initGeometry();

  }; // END class CameraVisualizer

} // END namespace GMlib


#endif // GM_SCENE_VISUALIZERS_CAMERAVISUALIZER_H
