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





#ifndef GM_PARAMETRICS_VISUALIZERS_PSURFNORMALSVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PSURFNORMALSVISUALIZER_H

#include "gmpsurfvisualizer.h"

// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmdvector.h>
#include <core/containers/gmdmatrix.h>
#include <core/utils/gmcolor.h>
#include <opengl/gmprogram.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>


namespace GMlib {

  enum GM_SURF_NORMALSVISUALIZER_MODE {
    GM_SURF_NORMALSVISUALIZER_ALL,
    GM_SURF_NORMALSVISUALIZER_INTERIOR,
    GM_SURF_NORMALSVISUALIZER_BOUNDARY
  };

  template <typename T, int n>
  class PSurfNormalsVisualizer : public PSurfVisualizer<T,n> {
    GM_VISUALIZER(PSurfNormalsVisualizer)
  public:
    PSurfNormalsVisualizer();
    PSurfNormalsVisualizer( const PSurfNormalsVisualizer<T,n>& copy );

    void                              render( const SceneObject* obj, const DefaultRenderer* renderer) const override;
    void                              replot( const DMatrix< DMatrix< Vector<T, n> > >& p,
                                              const DMatrix< Vector<float, n> >& normals,
                                              int m1, int m2, int d1, int d2,
                                              bool closed_u, bool closed_v
                                              ) override;

    const Color&                      getColor() const;
    void                              setColor( const Color& color );

    GM_SURF_NORMALSVISUALIZER_MODE    getMode() const;
    void                              setMode( GM_SURF_NORMALSVISUALIZER_MODE mode );

    double                            getSize() const;
    void                              setSize( double size = 1.0 );

  protected:
    GL::Program                       _prog;

    GL::VertexBufferObject            _vbo;
    int                               _no_elements;

    Color                             _color;
    double                            _size;

    GM_SURF_NORMALSVISUALIZER_MODE    _mode;

    void                              makePlotAll( const DMatrix< DMatrix< Vector<T, 3> > >& p, const DMatrix< Vector<float, 3> >& normals );
    void                              makePlotInterior( const DMatrix< DMatrix< Vector<T, 3> > >& p, const DMatrix< Vector<float, 3> >& normals );
    void                              makePlotBoundary( const DMatrix< DMatrix< Vector<T, 3> > >& p, const DMatrix< Vector<float, 3> >& normals );


  };

} // END namespace GMlib

// Include PSurfNormalsVisualizer class function implementations
#include "gmpsurfnormalsvisualizer.c"





#endif // GM_PARAMETRICS_VISUALIZERS_PSURFNORMALSVISUALIZER_H
