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





#ifndef GM_PARAMETRICS_VISUALIZERS_PCURVEDEFAULTVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PCURVEDEFAULTVISUALIZER_H

#include "gmpcurvevisualizer.h"

// gmlib
#include <opengl/gmprogram.h>


namespace GMlib {

  template <typename T, int n>
  class PCurveDefaultVisualizer : public PCurveVisualizer<T,n> {
    GM_VISUALIZER(PCurveDefaultVisualizer)
  public:
    PCurveDefaultVisualizer();
    PCurveDefaultVisualizer(std::vector<DVector<Vector<T,3>>>& p);
    PCurveDefaultVisualizer( const PCurveDefaultVisualizer<T,n>& copy );

    GLfloat       getLineWidth() const;
    void          setLineWidth( GLfloat width );

    void          render(const SceneObject* obj, const DefaultRenderer* render) const override;
    void          renderGeometry( const SceneObject* obj, const Renderer* render, const Color& color ) const override;

    void          replot( const std::vector< DVector< Vector<T, n> > >& p, int m, int d, bool closed = false ) override;
    void          update() override;

  protected:
    GL::Program               _prog;
    GL::VertexBufferObject    _vbo;
    int                       _no_vertices;

    GLfloat                   _line_width;

  }; // END class PCurveDefaultVisualizer

} // END namespace GMlib

// Include PCurveDefaultVisualizer class function implementations
#include "gmpcurvedefaultvisualizer.c"

#endif // GM_PARAMETRICS_VISUALIZERS_PCURVEDEFAULTVISUALIZER_H
