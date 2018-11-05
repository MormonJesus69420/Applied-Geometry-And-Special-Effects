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





#ifndef GM_SCENE_VISUALIZERS_VISUALIZERSTDREP_H
#define GM_SCENE_VISUALIZERS_VISUALIZERSTDREP_H

#include "../gmvisualizer.h"

// gmlib
#include <core/utils/gmcolor.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/gmprogram.h>


namespace GMlib {

  class Camera;

  class VisualizerStdRep : public Visualizer {
    GM_VISUALIZER(VisualizerStdRep)
  public:
    VisualizerStdRep();

    void              render( const SceneObject* obj, const DefaultRenderer* renderer ) const override;
    void              renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const override;


    void              render( const HqMatrix<float,3>& mvmat, const HqMatrix<float,3>& pmat ) const;

    static VisualizerStdRep*    getInstance();


  private:
    GL::Program               _prog;

    // Cube buffer objects
    GL::VertexBufferObject    _bo_cube;
    GL::IndexBufferObject     _bo_cube_indices;
    GL::IndexBufferObject     _bo_cube_frame_indices;

    static VisualizerStdRep*  _s_instance;

  }; // END class VisualizerStdRep

} // END namespace GMlib


#endif // GM_SCENE_VISUALIZERS_VISUALIZERSTDREP_H
