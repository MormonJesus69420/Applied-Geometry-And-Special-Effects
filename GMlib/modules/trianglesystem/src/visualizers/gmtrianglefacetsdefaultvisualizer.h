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




#ifndef GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSDEFAULTVISUALIZER_H
#define GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSDEFAULTVISUALIZER_H


#include "gmtrianglefacetsvisualizer.h"

// gmlib
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/gmprogram.h>


namespace GMlib {

  template <typename T>
  class TriangleFacets;

  template <typename T>
  class TriangleFacetsDefaultVisualizer : public TriangleFacetsVisualizer<T> {
    GM_VISUALIZER(TriangleFacetsDefaultVisualizer)
  public:
    TriangleFacetsDefaultVisualizer();
    ~TriangleFacetsDefaultVisualizer();

    /* virtual from TriangleFacetsVisualizer */
    void          render(const SceneObject *obj, const DefaultRenderer *renderer) const;
    void          renderGeometry( const SceneObject *obj, const Renderer *renderer, const Color &color ) const;

    void          replot(const TriangleFacets<T> *tf);



  protected:
    GL::VertexBufferObject        _vbo;
    GL::IndexBufferObject         _ibo;
    void                          draw() const;


  private:
    GL::Program                   _prog;
    GL::Program                   _color_prog;

    std::vector<Color>            _colors;

    int                           _no_elements;

    void                          initShader();

  }; // END class TriangleFacetsDefaultVisualizer

} // END namespace GMlib

// Include TriangleFacetsDefaultVisualizer class function implementations
#include "gmtrianglefacetsdefaultvisualizer.c"

#endif // GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSDEFAULTVISUALIZER_H
