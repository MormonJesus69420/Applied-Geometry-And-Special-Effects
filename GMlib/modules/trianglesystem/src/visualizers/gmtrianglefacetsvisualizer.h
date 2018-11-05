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





#ifndef GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSVISUALIZER_H
#define GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSVISUALIZER_H


// gmlib
#include <scene/gmvisualizer.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>


namespace GMlib {

  template <typename T>
  class TriangleFacets;

  template <typename T>
  class TriangleFacetsVisualizer : public Visualizer {
  public:
    TriangleFacetsVisualizer();
    ~TriangleFacetsVisualizer();
    virtual void  replot( const TriangleFacets<T>* tf ) const;
    void          set( SceneObject* obj );


    static void   fillStandardVBO( GL::VertexBufferObject& vbo,
                                   const TriangleFacets<T>* tf );
    static void   fillStandardIBO( GL::IndexBufferObject& ibo,
                                   const TriangleFacets<T>* tf );

  }; // END class TriangleFacetsVisualizer

} // END namespace GMlib

// Include TriangleFacetsVisualizer class function implementations
#include "gmtrianglefacetsvisualizer.c"



#endif // GM_TRIANGLESYSTEM_VISUALIZERS_TRIANGLEFACETSVISUALIZER_H
