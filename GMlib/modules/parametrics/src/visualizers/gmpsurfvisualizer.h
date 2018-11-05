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





#ifndef GM_PARAMETRICS_VISUALIZERS_PSURFVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PSURFVISUALIZER_H


// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmdmatrix.h>
#include <opengl/gmtexture.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>
#include <scene/gmvisualizer.h>


namespace GMlib {

  template <typename T, int n>
  class PSurf;

  template <typename T, int n>
  class PSurfVisualizer : public Visualizer {
  public:
    PSurfVisualizer();
    PSurfVisualizer( const PSurfVisualizer<T,n>& copy );

    virtual void  replot( const DMatrix< DMatrix< Vector<T, n> > >& p, const DMatrix< Vector<float,3> >& normals,
                                                            int m1, int m2, int d1, int d2, bool closed_u, bool closed_v );

    virtual void  replot( const DVector<DVector<Vector<T, n> > >& p, const DMatrix< Vector<float,3> >& normals, int m, bool closed_u, bool closed_v );


    static void   fillStandardVBO(GL::VertexBufferObject &vbo, const DMatrix< DMatrix< Vector<T,n> > >& p );
    static void   fillStandardVBO(GL::VertexBufferObject &vbo, const DVector<DVector<Vector<T,n> > >& p );

    static void   fillTriangleStripIBO(GL::IndexBufferObject& ibo, int m1, int m2, GLuint& no_strips, GLuint& no_strip_indices, GLsizei& strip_size );
    static void   fillNMap( GL::Texture& nmap, const DMatrix< Vector<float, 3> >& normals, bool closed_u, bool closed_v);
    static void   compTriangleStripProperties( int m1, int m2, GLuint& no_strips, GLuint& no_strip_indices, GLsizei& strip_size );

    static void   fillMap( GL::Texture& map, const DMatrix< DMatrix< Vector<T,n> > >& p, int d1, int d2, bool closed_u, bool closed_v );
    static void   fillStandardIBO( GLuint vbo_id, int m1, int m2 );
    static void   fillTriangleStripTexVBO( GLuint vbo_id, int m1, int m2 );
    static void   fillTriangleStripNormalVBO( GLuint vbo_id, DMatrix< Vector<float,3> >& normals );
    static void   fillTriangleStripVBO( GLuint vbo_id, DMatrix< DMatrix< Vector<T,n> > >& p, int d1 = 0, int d2 = 0 );
    static void   getTriangleStripDataInfo( const DMatrix< DMatrix< Vector<T,n> > >& p, int& no_dp, int& no_strips, int& no_verts_per_strips );

  }; // END class PSurfVisualizer

} // END namespace GMlib

// Include PSurfVisualizer class function implementations
#include "gmpsurfvisualizer.c"



#endif // GM_PARAMETRICS_VISUALIZERS_PSURFVISUALIZER_H
