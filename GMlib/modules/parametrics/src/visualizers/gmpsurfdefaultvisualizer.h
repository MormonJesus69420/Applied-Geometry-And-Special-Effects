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





#ifndef GM_PARAMETRICS_VISUALIZERS_PSURFDEFAULTVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PSURFDEFAULTVISUALIZER_H

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
  class PSurfDefaultVisualizer : public PSurfVisualizer<T,n> {
    GM_VISUALIZER(PSurfDefaultVisualizer)
  public:
    PSurfDefaultVisualizer();
    PSurfDefaultVisualizer( const PSurfDefaultVisualizer<T,n>& copy );

    void    render( const SceneObject* obj, const DefaultRenderer* renderer ) const override;
    void    renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const override;

    void    replot( const DMatrix< DMatrix< Vector<T, n> > >& p, const DMatrix< Vector<float, 3> >& normals,
                                            int m1, int m2, int d1, int d2, bool closed_u, bool closed_v ) override;

  protected:
    GL::Program                 _prog;
    GL::Program                 _color_prog;

    GL::VertexBufferObject      _vbo;
    GL::IndexBufferObject       _ibo;
    GL::Texture                 _nmap;

    GLuint                      _no_strips;
    GLuint                      _no_strip_indices;
    GLsizei                     _strip_size;

    GLenum                      _mode;

    virtual void                draw() const;

    void                        initShaderProgram();

    void                        _init();

  }; // END class PSurfDefaultVisualizer

} // END namespace GMlib

// Include PSurfDefaultVisualizer class function implementations
#include "gmpsurfdefaultvisualizer.c"


#endif // GM_PARAMETRICS_VISUALIZERS_PSURFDEFAULTVISUALIZER_H
