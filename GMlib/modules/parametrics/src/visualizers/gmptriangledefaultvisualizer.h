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





#ifndef GM_PARAMETRICS_VISUALIZERS_PTRIANGLEDEFAULTVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PTRIANGLEDEFAULTVISUALIZER_H


#include "gmptrianglevisualizer.h"

// gmlib
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/gmprogram.h>


namespace GMlib {

  template <typename T, int n>
  class PTriangleDefaultVisualizer : public PTriangleVisualizer<T,n> {
    GM_VISUALIZER(PTriangleDefaultVisualizer)
  public:
    PTriangleDefaultVisualizer();

    void            render(const SceneObject *obj, const DefaultRenderer *renderer) const override;
    void            renderGeometry(const SceneObject* obj, const Renderer* renderer, const Color& color) const override;

    virtual void    replot(const DVector< DVector< Vector<T,3> > >& p,int m) override;

  private:
    GL::Program               _prog;
    GL::Program               _geometry_prog;

    GL::VertexBufferObject    _vbo;
    GL::IndexBufferObject     _ibo;

    int                       _no_elements;

    void                      draw() const;

  }; // END class PTriangleDefaultVisualizer

} // END namespace GMlib

// Include PTriangleDefaultVisualizer class function implementations
#include "gmptriangledefaultvisualizer.c"



#endif // GM_PARAMETRICS_VISUALIZERS_PTRIANGLEDEFAULTVISUALIZER_H
