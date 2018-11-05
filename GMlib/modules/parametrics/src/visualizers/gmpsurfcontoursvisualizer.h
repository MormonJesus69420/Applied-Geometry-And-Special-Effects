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



/*! \file gmpsurfcontoursvisualizer.h
 *
 *  Interface for the PSurfContoursVisualizer class.
 */


#ifndef __GMPSURFCONTOURSVISUALIZER_H__
#define __GMPSURFCONTOURSVISUALIZER_H__


#include "gmpsurfvisualizer.h"


// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmdvector.h>
#include <core/containers/gmdmatrix.h>
#include <core/utils/gmcolor.h>


namespace GMlib {


  enum GM_PSURF_CONTOURSVISUALIZER_MAP {
    GM_PSURF_CONTOURSVISUALIZER_X,
    GM_PSURF_CONTOURSVISUALIZER_Y,
    GM_PSURF_CONTOURSVISUALIZER_Z,
    GM_PSURF_CONTOURSVISUALIZER_U,
    GM_PSURF_CONTOURSVISUALIZER_V,
    GM_PSURF_CONTOURSVISUALIZER_SPEED_U,
    GM_PSURF_CONTOURSVISUALIZER_SPEED_V,
    GM_PSURF_CONTOURSVISUALIZER_CURVATURE_GAUSS,
    GM_PSURF_CONTOURSVISUALIZER_CURVATURE_MEAN,
    GM_PSURF_CONTOURSVISUALIZER_CURVATURE_PRINCIPAL_MAX,
    GM_PSURF_CONTOURSVISUALIZER_CURVATURE_PRINCIPAL_MIN
  };

  enum GM_PSURF_CONTOURSVISUALIZER_INTERPOLATION_METHOD {
    GM_PSURF_CONTOURSVISUALIZER_NO_INTERPOLATION,
    GM_PSURF_CONTOURSVISUALIZER_LINEAR
  };


  template <typename T>
  class PSurfContoursVisualizer : public PSurfVisualizer<T> {
  public:
    struct Vertex {
      GLfloat   x, y, z;
      GLfloat   r, g, b, a;
      GLfloat   padding;
    }; // Size 8 * sizeof( GLfloat )

    PSurfContoursVisualizer();
    virtual ~PSurfContoursVisualizer();

    void                              display();
    const Array<Color>&               getColors() const;
    GM_PSURF_CONTOURSVISUALIZER_INTERPOLATION_METHOD   getInterpolationMethod() const;
    GM_PSURF_CONTOURSVISUALIZER_MAP   getMapping() const;
    void                              replot(
      DMatrix< DMatrix< Vector<T, 3> > >& p,
      DMatrix< Vector<T, 3> >& normals,
      int m1, int m2, int d1, int d2,
      bool closed_u, bool closed_v
    );
    void                              setColors( const Array<Color>& c );
    void                              setInterpolationMethod( GM_PSURF_CONTOURSVISUALIZER_INTERPOLATION_METHOD method );
    void                              setMapping( GM_PSURF_CONTOURSVISUALIZER_MAP mapping );

  protected:
    Array<Color>                      _colors;
    GM_PSURF_CONTOURSVISUALIZER_MAP   _mapping;
    GM_PSURF_CONTOURSVISUALIZER_INTERPOLATION_METHOD   _method;

    GLuint                            _ibo;
    GLuint                            _vbo;

    int                               _tri_strips;
    GLsizei                           _tri_strip_offset;
    int                               _indices_per_tri_strip;



    Color                             getColor( T d );
    Color                             getColorInterpolated( T d );
    T                                 getCurvatureGauss( DMatrix< Vector<T,3> >& p );
    T                                 getCurvatureMean( DMatrix< Vector<T,3> >& p );
    T                                 getCurvaturePrincipalMax( DMatrix< Vector<T,3> >& p );
    T                                 getCurvaturePrincipalMin( DMatrix< Vector<T,3> >& p );
    T                                 getValue( DMatrix< DMatrix< Vector<T, 3> > >& p, int i, int j );

  };

} // END namespace GMlib

// Include PSurfContoursVisualizer class function implementations
#include "gmpsurfcontoursvisualizer.c"


#endif // __GMPSURFCONTOURSVISUALIZER_H__
