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





#ifndef GM_PARAMETRICS_VISUALIZERS_PSURFPOINTSVISUALIZER_H
#define GM_PARAMETRICS_VISUALIZERS_PSURFPOINTSVISUALIZER_H

#include "gmpsurfvisualizer.h"

// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmdvector.h>
#include <core/containers/gmdmatrix.h>
#include <core/utils/gmcolor.h>


namespace GMlib {


  template <typename T, int n>
  class PSurfPointsVisualizer : public PSurfVisualizer<T,n> {
    GM_VISUALIZER(PSurfPointsVisualizer)
  public:
    PSurfPointsVisualizer();
    PSurfPointsVisualizer( const PSurfPointsVisualizer<T,n>& copy );

    void              render( const SceneObject* obj, const DefaultRenderer* renderer ) const;
    void              replot( const DMatrix< DMatrix< Vector<T, n> > >& p,
                              const DMatrix< Vector<T, n> >& normals,
                              int m1, int m2, int d1, int d2,
                              bool closed_u, bool closed_v );

    const Color&      getColor() const;
    float             getSize() const;
    void              setColor( const Color& color );
    void              setSize( float size );

  protected:

    float                     _size;
    Color                     _color;

    int                       _no_points;
    GL::Program               _prog;
    GL::VertexBufferObject    _vbo;
  }; // END class PSurfPointsVisualizer



} // END namespace GMlib

// Include PSurfPointsVisualizer class function implementations
#include "gmpsurfpointsvisualizer.c"


#endif // GM_PARAMETRICS_VISUALIZERS_PSURFPOINTSVISUALIZER_H
