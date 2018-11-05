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





#include "gmpsurfnormalsvisualizer.h"

#include <opengl/gmopengl.h>


namespace GMlib {

  template <typename T, int n>
  PSurfNormalsVisualizer<T,n>::PSurfNormalsVisualizer()
    : _no_elements(0),_color( GMcolor::black() ),
      _size(1.0), _mode(GM_SURF_NORMALSVISUALIZER_ALL) {

    _prog.acquire("color");
    _vbo.create();
  }

  template <typename T, int n>
  PSurfNormalsVisualizer<T,n>::PSurfNormalsVisualizer(const PSurfNormalsVisualizer<T,n>& copy)
    : PSurfVisualizer<T,n>(copy),
      _no_elements(0),
      _color(copy._color), _size(copy._size), _mode(copy._mode) {

    _prog.acquire("color");
    _vbo.create();
  }

  template <typename T, int n>
  inline
  void PSurfNormalsVisualizer<T,n>::render( const SceneObject* obj, const DefaultRenderer *renderer) const {

    const HqMatrix<float,3> &mvpmat = obj->getModelViewProjectionMatrix(renderer->getCamera());

    _prog.bind(); {

      _prog.uniform( "u_mvpmat", mvpmat );
      _prog.uniform( "u_color", _color );

      GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );

      _vbo.bind();
      _vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, 0, static_cast<const GLvoid*>(0x0) );

      // Draw
      GL_CHECK(glDrawArrays( GL_LINES, 0, _no_elements ));

      _vbo.disable( vert_loc );
      _vbo.unbind();

    } _prog.unbind();
  }

  template <typename T, int n>
  const Color& PSurfNormalsVisualizer<T,n>::getColor() const {

    return _color;
  }

  /*! GM_SURF_NORMALSVISUALIZER_MODE PSurfNormalsVisualizer<T,n>::getMode() const
   *
   *  Returns the displaymode of the visualizer.
   *  All       - Shows all normals.
   *  Boundary  - Shows only the normals on the boundary.
   *  Interior  - Shows only the interior normals.
   *
   *  \return Visualizer normals mode.
   */
  template <typename T, int n>
  GM_SURF_NORMALSVISUALIZER_MODE PSurfNormalsVisualizer<T,n>::getMode() const {

    return _mode;
  }

  template <typename T, int n>
  double PSurfNormalsVisualizer<T,n>::getSize() const {

    return _size;
  }

  /*! void PSurfNormalsVisualizer<T,n>::makePlotAll( DMatrix< DMatrix< Vector<T, 3> > >& p, DMatrix< Vector<float, 3> >& normals )
   *
   *  Generates the plot data for all normals.
   *
   *  \param[in]  p         Evaluated position data.
   *  \param[in]  normals   Evaluated Normal data.
   */
  template <typename T, int n>
  void PSurfNormalsVisualizer<T,n>::makePlotAll( const DMatrix< DMatrix< Vector<T, 3> > >& p, const DMatrix< Vector<float, 3> >& normals ) {

    int no_normals = p.getDim1() * p.getDim2();
    _no_elements = no_normals * 2;

    _vbo.bind();
    _vbo.bufferData( no_normals * 2 * sizeof(GL::GLVertex), 0x0, GL_STATIC_DRAW );

    GL::GLVertex *ptr = _vbo.mapBuffer<GL::GLVertex>();
    if( ptr ) {

      for( int i = 0; i < p.getDim1(); i++ ) {
        for( int j = 0; j < p.getDim2(); j++ ) {

          const Point<T,3> &pos = p(i)(j)(0)(0);
          (*ptr).x = float(pos(0));
          (*ptr).y = float(pos(1));
          (*ptr).z = float(pos(2));
          ptr++;

          const Vector<T,3> N = normals(i)(j).getNormalized() * _size;
          (*ptr).x = float(pos(0)) + N(0);
          (*ptr).y = float(pos(1)) + N(1);
          (*ptr).z = float(pos(2)) + N(2);
          ptr++;
        }
      }
    }

    _vbo.unmapBuffer();
    _vbo.unbind();
  }

  /*! void PSurfNormalsVisualizer<T,n>::makePlotInterior( DMatrix< DMatrix< Vector<T, 3> > >& p, DMatrix< Vector<float, 3> >& normals )
   *
   *  Generates the plot data for all interior normals.
   *
   *  \param[in]  p         Evaluated position data.
   *  \param[in]  normals   Evaluated Normal data.
   */
  template <typename T, int n>
  void PSurfNormalsVisualizer<T,n>::makePlotInterior( const DMatrix< DMatrix< Vector<T, 3> > >& p, const DMatrix< Vector<float, 3> >& normals ) {

    int no_normals = ( p.getDim1() - 2 ) * ( p.getDim2() - 2 );
    _no_elements = no_normals * 2;

    _vbo.bufferData( no_normals * 2 * sizeof(GL::GLVertex), 0x0, GL_STATIC_DRAW );

    GL::GLVertex *ptr = _vbo.mapBuffer<GL::GLVertex>();
    if( ptr ) {

      for( int i = 1; i < p.getDim1()-1; i++ ) {
        for( int j = 1; j < p.getDim2()-1; j++ ) {

          const Point<T,3> &pos = p(i)(j)(0)(0);
          (*ptr).x = float(pos(0));
          (*ptr).y = float(pos(1));
          (*ptr).z = float(pos(2));
          ptr++;

          const Vector<T,3> N = normals(i)(j).getNormalized() * _size;
          (*ptr).x = float(pos(0)) + N(0);
          (*ptr).y = float(pos(1)) + N(1);
          (*ptr).z = float(pos(2)) + N(2);
          ptr++;
        }
      }
    }

    _vbo.unmapBuffer();
  }

  /*! void PSurfNormalsVisualizer<T,n>::makePlotBoundary( DMatrix< DMatrix< Vector<T, 3> > >& p, DMatrix< Vector<float, 3> >& normals )
   *
   *  Generates the plot data for all boundary normals.
   *
   *  \param[in]  p         Evaluated position data.
   *  \param[in]  normals   Evaluated Normal data.
   */
  template <typename T, int n>
  void PSurfNormalsVisualizer<T,n>::makePlotBoundary( const DMatrix< DMatrix< Vector<T, 3> > >& p, const DMatrix< Vector<float, 3> >& normals ) {

    int no_normals = ( p.getDim1() + p.getDim2() ) * 2 - 4;
    _no_elements = no_normals * 2;

    _vbo.bind();
    _vbo.bufferData( no_normals * 2 * sizeof(GL::GLVertex), 0x0, GL_STATIC_DRAW );

    GL::GLVertex *ptr = _vbo.mapBuffer<GL::GLVertex>();
    if( ptr ) {

      for( int i = 0, j; i < p.getDim1(); i++ ) {

        // j = 0
        j = 0;

        const Point<T,3> &pos = p(i)(j)(0)(0);
        (*ptr).x = float(pos(0));
        (*ptr).y = float(pos(1));
        (*ptr).z = float(pos(2));
        ptr++;

        const Vector<T,3> N1 = normals(i)(j).getNormalized() * _size;
        (*ptr).x = float(pos(0)) + N1(0);
        (*ptr).y = float(pos(1)) + N1(1);
        (*ptr).z = float(pos(2)) + N1(2);
        ptr++;

        // j = p.getDim2() -1
        j = p.getDim2() - 1;

        const Point<T,3> &pos2 = p(i)(j)(0)(0);
        (*ptr).x = float(pos2(0));
        (*ptr).y = float(pos2(1));
        (*ptr).z = float(pos2(2));
        ptr++;

        const Vector<T,3> N2 = normals(i)(j).getNormalized() * _size;
        (*ptr).x = float(pos2(0)) + N2(0);
        (*ptr).y = float(pos2(1)) + N2(1);
        (*ptr).z = float(pos2(2)) + N2(2);
        ptr++;
      }

      for( int i, j = 1; j < p.getDim2()-1; j++ ) {

        // i = 0
        i = 0;

        const Point<T,3> &pos = p(i)(j)(0)(0);
        (*ptr).x = float(pos(0));
        (*ptr).y = float(pos(1));
        (*ptr).z = float(pos(2));
        ptr++;

        const Vector<T,3> N1 = normals(i)(j).getNormalized() * _size;
        (*ptr).x = float(pos(0)) + N1(0);
        (*ptr).y = float(pos(1)) + N1(1);
        (*ptr).z = float(pos(2)) + N1(2);
        ptr++;

        // j = p.getDim1() -1
        i = p.getDim1() - 1;

        const Point<T,3> &pos2 = p(i)(j)(0)(0);
        (*ptr).x = float(pos2(0));
        (*ptr).y = float(pos2(1));
        (*ptr).z = float(pos2(2));
        ptr++;

        const Vector<T,3> N2 = normals(i)(j).getNormalized() * _size;
        (*ptr).x = float(pos2(0)) + N2(0);
        (*ptr).y = float(pos2(1)) + N2(1);
        (*ptr).z = float(pos2(2)) + N2(2);
        ptr++;
      }
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
    glBindBuffer( GL_ARRAY_BUFFER, 0x0 );
  }

  template <typename T, int n>
  void PSurfNormalsVisualizer<T,n>::replot(
    const DMatrix< DMatrix< Vector<T, n> > >& p,
    const DMatrix< Vector<float, n> >& normals,
    int /*m1*/, int /*m2*/, int /*d1*/, int /*d2*/,
    bool /*closed_u*/, bool /*closed_v*/
  ) {

    switch( _mode ) {

    case GM_SURF_NORMALSVISUALIZER_INTERIOR:
      makePlotInterior( p, normals );
      break;

    case GM_SURF_NORMALSVISUALIZER_BOUNDARY:
      makePlotBoundary( p, normals );
      break;

    case GM_SURF_NORMALSVISUALIZER_ALL:
    default:
      makePlotAll( p, normals );
      break;
    }
  }

  template <typename T, int n>
  void PSurfNormalsVisualizer<T,n>::setColor( const Color& color ) {

    _color = color;
  }

  /*! void PSurfNormalsVisualizer<T,n>::setMode( GM_SURF_NORMALSVISUALIZER_MODE mode )
   *
   *  Sets the normals mode for the visualizer.
   *
   *  All       - Shows all normals.
   *  Boundary  - Shows only the normals on the boundary.
   *  Interior  - Shows only the interior normals.
   *
   *  \param[in]  mode  The normals mode.
   *
   *  \see PSurfNormalsVisualizer<T,n>::GM_SURF_NORMALSVISUALIZER_MODE
   */
  template <typename T, int n>
  void PSurfNormalsVisualizer<T,n>::setMode( GM_SURF_NORMALSVISUALIZER_MODE mode ) {

    _mode = mode;
  }

  template <typename T, int n>
  void PSurfNormalsVisualizer<T,n>::setSize( double size ) {

    _size = size;
  }



} // END namespace GMlib
