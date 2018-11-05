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


#ifndef GM_PARAMETRICS_VISUALIZERS_PSURFTEXVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PSURFTEXVISUALIZER_H

#include "gmpsurfvisualizer.h"

// gmlib
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/bufferobjects/gmuniformbufferobject.h>
#include <opengl/gmtexture.h>
#include <opengl/gmprogram.h>
#include <opengl/shaders/gmvertexshader.h>
#include <opengl/shaders/gmfragmentshader.h>



namespace GMlib {

  template <typename T, int n>
  class PSurfTexVisualizer : public PSurfVisualizer<T,n> {
    GM_VISUALIZER(PSurfTexVisualizer)
  public:
    PSurfTexVisualizer();
    PSurfTexVisualizer( const PSurfTexVisualizer<T,n>& copy );

    void          setTexture( const GL::Texture& tex );

    void          render( const SceneObject* obj, const DefaultRenderer* renderer ) const;
    void          renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const;

    virtual void  replot( const DMatrix< DMatrix< Vector<T, n> > >& p,
                          const DMatrix< Vector<T, 3> >& normals,
                          int m1, int m2, int d1, int d2,
                          bool closed_u, bool closed_v
    );

  private:
    GL::Program                 _prog;
    GL::Program                 _color_prog;

    GL::VertexBufferObject      _vbo;
    GL::IndexBufferObject       _ibo;
    GL::Texture                 _nmap;
    GL::Texture                 _tex;

    GLuint                      _no_strips;
    GLuint                      _no_strip_indices;
    GLsizei                     _strip_size;

    void                        draw() const;

    void                        initShaderProgram();

  }; // END class PSurfTexVisualizer

} // END namespace GMlib

// Include PSurfTexVisualizer class function implementations
#include "gmpsurftexvisualizer.c"


#endif // GM_PARAMETRICS_VISUALIZERS_PSURFTEXVISUALIZER_H
