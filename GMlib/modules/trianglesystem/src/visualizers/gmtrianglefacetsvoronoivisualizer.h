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




#ifndef GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSVORONOIVISUALIZER_H
#define GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSVORONOIVISUALIZER_H


#include "gmtrianglefacetsvisualizer.h"


namespace GMlib {

  template <typename T>
  class TriangleFacetsVoronoiVisualizer : public TriangleFacetsVisualizer<T> {
  public:
    TriangleFacetsVoronoiVisualizer();
    ~TriangleFacetsVoronoiVisualizer();
    void          display();
    virtual void  replot() const;

  protected:
    GLuint        _vbo;
    GLuint        _ibo;

  }; // END class TriangleFacetsVoronoiVisualizer

} // END namespace GMlib

// Include TriangleFacetsVoronoiVisualizer class function implementations
#include "gmtrianglefacetsvoronoivisualizer.c"


#endif // GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSVORONOIVISUALIZER_H
