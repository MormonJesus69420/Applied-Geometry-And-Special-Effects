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





#ifndef GM_SCENE_VISUALIZERS_SELECTORGRIDVISUALIZER_H
#define GM_SCENE_VISUALIZERS_SELECTORGRIDVISUALIZER_H


#include "../gmvisualizer.h"

// gmlib
#include <core/containers/gmdmatrix.h>
#include <opengl/gmprogram.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>


namespace GMlib {

  template <typename T, int n>
  class Selector;

  template <typename T>
  class SelectorGridVisualizer : public Visualizer {
    GM_VISUALIZER(SelectorGridVisualizer)
  public:
    SelectorGridVisualizer();

    void                          render(const SceneObject *obj, const DefaultRenderer *renderer) const override;
    void                          reset();
    void                          setColor( const Color& col );
    void                          setLineWidth( float line_width = 1.0f );
    void                          setSelectors( DVector< Point<T,3> >& c, int tp=0, bool closed = false );
    void                          setSelectors( DVector< Vector<T,3> >& c, int tp=0, bool closed = false );
    void                          setSelectors( DMatrix< Point<T,3> >& c, bool closed_u = false, bool closed_v = false );
    void                          setSelectors( DMatrix< Vector<T,3> >& c, bool closed_u = false, bool closed_v = false);
    void                          setScale(const Vector<T,3>& s);

    void                          update() override;
  protected:


    int                           _no_indices;
    float                         _line_width;
    DVector< Point<T,3>* >        _c;
    bool                          _scaled;
    Vector<T,3>                   _scale;

    GL::Program                   _prog;

    GL::VertexBufferObject        _vbo;
    GL::IndexBufferObject         _ibo;

    Color                         _color;

  private:
    void                          _fillVBO();

    void                          _makeLines( bool closed );
    void                          _makeTriangs();
    void                          _makeQuads( int m1, int m2, bool closed_u, bool closed_v );


    void                          storeSelectorValuePointers( DVector<Point<T,3> >& c );
    void                          storeSelectorValuePointers( DVector<Vector<T,3> >& c );
    void                          storeSelectorValuePointers( DMatrix<Point<T,3> >& c );
    void                          storeSelectorValuePointers( DMatrix<Vector<T,3> >& c );

  }; // END class SelectorGridVisualizer

} // END namespace GMlib

// Implementations
#include "gmselectorgridvisualizer.c"


#endif // GM_SCENE_VISUALIZERS_SELECTORGRIDVISUALIZER_H
