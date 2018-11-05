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

#ifndef GM_PARAMETRICS_VISUALIZERS_PSURFPARAMLINESVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PSURFPARAMLINESVISUALIZER_H

#include "gmpsurfvisualizer.h"

// gmlib
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <opengl/bufferobjects/gmuniformbufferobject.h>
#include <opengl/gmtexture.h>



namespace GMlib {

  template <typename T, int n>
  class PSurfParamLinesVisualizer : public PSurfVisualizer<T,n> {
    GM_VISUALIZER(PSurfParamLinesVisualizer)
  public:
    PSurfParamLinesVisualizer();
    PSurfParamLinesVisualizer( const PSurfParamLinesVisualizer<T,n>& copy );

    void          render( const SceneObject* obj, const DefaultRenderer *cam ) const override;

    virtual void  replot( const DMatrix< DMatrix< Vector<T, n> > >& p,
                          const DMatrix< Vector<T, 3> >& normals,
                          int m1, int m2, int d1, int d2,
                          bool closed_u, bool closed_v
    ) override;

  private:
    GL::Program                 _prog;

    GL::VertexBufferObject      _vbo;
    GL::IndexBufferObject       _ibo;
    GL::Texture                 _nmap;
    GL::Texture                 _ptex_u, _ptex_v;

    GLuint                      _no_strips;
    GLuint                      _no_strip_indices;
    GLsizei                     _strip_size;

    Material                    _mat;

    void                        draw() const;
    void                        generatePTex( int m1, int m2, int s1, int s2, bool closed_u, bool closed_v );

    void                        initShaderProgram();

  }; // END class PSurfParamLinesVisualizer

} // END namespace GMlib

// Include PSurfParamLinesVisualizer class function implementations
#include "gmpsurfparamlinesvisualizer.c"


#endif // GM_PARAMETRICS_VISUALIZERS_PSURFPARAMLINESVISUALIZER_H
