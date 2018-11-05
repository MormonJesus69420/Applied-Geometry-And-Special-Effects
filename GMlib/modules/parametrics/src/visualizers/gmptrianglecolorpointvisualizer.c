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



/*! \file gmptrianglecolorvisualizer.c
 *
 */


namespace GMlib {


  template <typename T>
  PTriangleColorPointVisualizer<T>::PTriangleColorPointVisualizer() {

    _color_mode = GM_PTRIANGLE_COLORPOINTVISUALIZER_RGB;
    glGenBuffers( 1, &_vbo_verts );
    glGenBuffers( 1, &_vbo_color );

    _c1 = GMcolor::red();
    _c2 = GMcolor::green();
    _c3 = GMcolor::blue();

    _point_size = 5.0f;

    _points.setDim(3);
    _points[0] = Point<T,3>( 1.0f, 0.0f, 0.0f );
    _points[1] = Point<T,3>( 0.0f, 1.0f, 0.0f );
    _points[2] = Point<T,3>( 0.0f, 0.0f, 1.0f );
  }

  template <typename T>
  PTriangleColorPointVisualizer<T>::~PTriangleColorPointVisualizer() {

    glDeleteBuffers( 1, &_vbo_verts );
    glDeleteBuffers( 1, &_vbo_color );
  }

  template <typename T>
  inline
  void PTriangleColorPointVisualizer<T>::display() {

//    // Push GL Attributes
//    glPushAttrib( GL_LIGHTING_BIT | GL_POINT_BIT ); {

//      // Disable lighting
//      glDisable( GL_LIGHTING );

//      // Set Point Size
//      glPointSize( _point_size );

//      // Blending
//      if( !this->_triangle->isOpaque() )
//        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//      // Bind buffers
//      glBindBuffer( GL_ARRAY_BUFFER, _vbo_verts );
//      glVertexPointer( 3, GL_FLOAT, PTRIANGLEVERTEX_SIZE, (const GLvoid*)0x0 );

//      glBindBuffer( GL_ARRAY_BUFFER, _vbo_color );
//      glColorPointer( 4, GL_FLOAT, PTRIANGLEVERTEXATTRIB_SIZE, (const GLvoid*)0x0 );

//      // Enable client states
//      glEnableClientState( GL_VERTEX_ARRAY );
//      glEnableClientState( GL_COLOR_ARRAY );

//      // Draw
//      glDrawArrays( GL_POINTS, 0, _points.getDim() );


//      // Disable client states
//      glDisableClientState( GL_VERTEX_ARRAY );
//      glDisableClientState( GL_COLOR_ARRAY );

//      // Unbind buffers
//      glBindBuffer( GL_ARRAY_BUFFER, 0x0 );

//    // Pop GL Attributes
//    } glPopAttrib();
  }

  template <typename T>
  GM_PTRIANGLE_COLORPOINTVISUALIZER_COLOR_MODE PTriangleColorPointVisualizer<T>::getColorMode() const {

    return _color_mode;
  }

  template <typename T>
  void PTriangleColorPointVisualizer<T>::getColors( Color& c1, Color& c2, Color& c3 ) {

    if(c1) c1 = _c1;
    if(c2) c2 = _c2;
    if(c3) c3 = _c3;
  }

  template <typename T>
  void PTriangleColorPointVisualizer<T>::replot(DVector<DMatrix<Vector<T, 3> > > &p, int m) {

    PTriangleVisualizer<T>::replot( p, m );

    // Allocate GPU memory
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_verts );
    glBufferData( GL_ARRAY_BUFFER, PTRIANGLEVERTEX_SIZE * _points.getDim(), 0x0, GL_DYNAMIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_color );
    glBufferData( GL_ARRAY_BUFFER, PTRIANGLEVERTEXATTRIB_SIZE * _points.getDim(), 0x0, GL_DYNAMIC_DRAW );


    // Fill GPU memory

    glBindBuffer( GL_ARRAY_BUFFER, _vbo_verts );
    PTriangleVertex *ptr_v = (PTriangleVertex*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    glBindBuffer( GL_ARRAY_BUFFER, _vbo_color );
    PTriangleVertexAttributes *ptr_c = (PTriangleVertexAttributes*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );


    if( ptr_v && ptr_c && this->_triangle ) {

      Color c1 = _c1;
      Color c2 = _c2;
      Color c3 = _c3;

      if( _color_mode == GM_PTRIANGLE_COLORPOINTVISUALIZER_HSV ) {

        c1.toHSV();
        c2.toHSV();
        c3.toHSV();
      }

      DVector< Color > colors( p.getDim() );


      for( int i = 0; i < _points.getDim(); i++ ) {

        Point<T,3> p = this->_triangle->evaluateLocal( _points[i][0], _points[i][1], 0, 0 )(0)(0);
        Color c = _points[i][0] * c1 + _points[i][1] * c2 + _points[i][2] * c3;
        if( _color_mode == GM_PTRIANGLE_COLORPOINTVISUALIZER_HSV )
          c.toRGB();

        (*ptr_v).x = p[0];
        (*ptr_v).y = p[1];
        (*ptr_v).z = p[2];

        (*ptr_c).r = c.getRedC();
        (*ptr_c).g = c.getGreenC();
        (*ptr_c).b = c.getBlueC();
        (*ptr_c).a = c.getAlphaC();

        ptr_v++;
        ptr_c++;
      }
    }

    glBindBuffer( GL_ARRAY_BUFFER, _vbo_verts );
    glUnmapBuffer( GL_ARRAY_BUFFER );
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_color );
    glUnmapBuffer( GL_ARRAY_BUFFER );
    glBindBuffer( GL_ARRAY_BUFFER, 0x0 );
  }

  template <typename T>
  void PTriangleColorPointVisualizer<T>::setColorMode(GM_PTRIANGLE_COLORPOINTVISUALIZER_COLOR_MODE color_mode) {

    _color_mode = color_mode;
  }

  template <typename T>
  void PTriangleColorPointVisualizer<T>::setColors( const Color& c1, const Color& c2, const Color& c3 ) {

    _c1 = c1;
    _c2 = c2;
    _c3 = c3;
  }

  template <typename T>
  void PTriangleColorPointVisualizer<T>::setPoints( const DVector< Point<T,3> >& points ) {

    _points = points;
  }

  template <typename T>
  void PTriangleColorPointVisualizer<T>::setTranslucency( double a ) {

    _c1.setAlpha( a );
    _c2.setAlpha( a );
    _c3.setAlpha( a );
  }



} // END namespace GMlib
