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



/*! \file gmpcurvecontoursvisualizer.h
 *
 *  Interface for the PCurveContoursVisualizer class.
 */


#ifndef __GMPCURVECONTOURSVISUALIZER_H__
#define __GMPCURVECONTOURSVISUALIZER_H__

#include "gmpcurvevisualizer.h"

// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmdvector.h>
#include <core/utils/gmcolor.h>


namespace GMlib {

  enum GM_PCURVE_CONTOURSVISUALIZER_MAP {
    GM_PCURVE_CONTOURSVISUALIZER_X,
    GM_PCURVE_CONTOURSVISUALIZER_Y,
    GM_PCURVE_CONTOURSVISUALIZER_Z,
    GM_PCURVE_CONTOURSVISUALIZER_T,
    GM_PCURVE_CONTOURSVISUALIZER_SPEED,
    GM_PCURVE_CONTOURSVISUALIZER_CURVATURE
  };

  template <typename T>
  class PCurveContoursVisualizer : public PCurveVisualizer<T> {
  public:
    PCurveContoursVisualizer();
    virtual ~PCurveContoursVisualizer();

    void                              display();
    const Array<Color>&               getColors() const;
    GM_PCURVE_CONTOURSVISUALIZER_MAP  getMapping() const;
    void                              replot(
      DVector< DVector< Vector<T, 3> > >& p,
      int m, int d, bool closed
    );
    void                              setColors( const Array<Color>& c );
    void                              setMapping( GM_PCURVE_CONTOURSVISUALIZER_MAP mapping );

  protected:
    Array<Color>                      _colors;
    GM_PCURVE_CONTOURSVISUALIZER_MAP  _mapping;
    DVector<Color>                    _c;

    GLuint                            _vbo_v;
    int                               _no_vertices;
    GLuint                            _vbo_c;

    Color                             getColor( T d );
    T                                 getCurvature( DVector< Vector<T, 3> >& p );
    T                                 getValue( DVector< DVector< Vector<T, 3> > >& p, int i );

  };

} // END namespace GMlib

// Include PCurveContoursVisualizer class function implementations
#include "gmpcurvecontoursvisualizer.c"



#endif // __GMPCURVECONTOURSVISUALIZER_H__
