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
 *  Implementation of the PTriangleColorVisualizer template class.
 */

namespace GMlib {


  template <typename T>
  PTriangleColorVisualizer<T>::PTriangleColorVisualizer() {

    _color_mode = GM_PTRIANGLE_COLORVISUALIZER_RGB;
    glGenBuffers( 1, &_vbo_color );

    _c1 = GMcolor::red();
    _c2 = GMcolor::green();
    _c3 = GMcolor::blue();
  }

  template <typename T>
  PTriangleColorVisualizer<T>::~PTriangleColorVisualizer() {

    glDeleteBuffers( 1, &_vbo_color );
  }

  template <typename T>
  inline
  void PTriangleColorVisualizer<T>::display() {

//    // Push GL Attributes
//    glPushAttrib( GL_LIGHTING_BIT | GL_LINE_BIT | GL_TEXTURE_BIT ); {

//      // Disable lighting
//      glDisable( GL_LIGHTING );

//      // Blending
//      if( !this->_triangle->isOpaque() )
//        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//      // Bind buffers
//      glBindBuffer( GL_ARRAY_BUFFER, this->_vbo );
//      glVertexPointer( 3, GL_FLOAT, PTRIANGLEVERTEX_SIZE, (const GLvoid*)0x0 );

//      glBindBuffer( GL_ARRAY_BUFFER, _vbo_color );
//      glColorPointer( 4, GL_FLOAT, PTRIANGLEVERTEXATTRIB_SIZE, (const GLvoid*)0x0 );

//      // Enable client states
//      glEnableClientState( GL_VERTEX_ARRAY );
//      glEnableClientState( GL_COLOR_ARRAY );

//      // Draw
//      for( int i = 0; i < this->_m; i++ )
//        glDrawArrays( GL_TRIANGLE_STRIP, i*(i+2), 2*i+3 );


//      // Disable client states
//      glDisableClientState( GL_VERTEX_ARRAY );
//      glDisableClientState( GL_COLOR_ARRAY );

//      // Unbind buffers
//      glBindBuffer( GL_ARRAY_BUFFER, 0x0 );

//    // Pop GL Attributes
//    } glPopAttrib();
  }

  template <typename T>
  GM_PTRIANGLE_COLORVISUALIZER_COLOR_MODE PTriangleColorVisualizer<T>::getColorMode() const {

    return _color_mode;
  }

  template <typename T>
  void PTriangleColorVisualizer<T>::getColors( Color& c1, Color& c2, Color& c3 ) {

    if(c1) c1 = _c1;
    if(c2) c2 = _c2;
    if(c3) c3 = _c3;
  }

  template <typename T>
  void PTriangleColorVisualizer<T>::replot(DVector<DMatrix<Vector<T, 3> > > &p, int m) {

    PTriangleVisualizer<T>::replot( p, m );

    // Allocate GPU memory
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_color );
    glBufferData( GL_ARRAY_BUFFER, PTRIANGLEVERTEXATTRIB_SIZE * this->_no_vertices, 0x0, GL_DYNAMIC_DRAW );


    // Fill GPU memory
    PTriangleVertexAttributes *ptr = (PTriangleVertexAttributes*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );


    if(ptr) {

      DVector< Color > colors( p.getDim() ); {

        Color c1 = _c1;
        Color c2 = _c2;
        Color c3 = _c3;
        if( _color_mode == GM_PTRIANGLE_COLORVISUALIZER_HSV ) {

          c1.toHSV();
          c2.toHSV();
          c3.toHSV();
        }

        float u, v, w, du = 1.0f / (m-1);
        int i,j,k;
        for(k=0,i=0; i<m; i++) {
          for(j=0;j<=i;j++) {
            v = j*du;
            u = (i-j)*du;
            w = 1.0f-u-v;

            Color c = u * c1 + v * c2 + w * c3;
            if( _color_mode == GM_PTRIANGLE_COLORVISUALIZER_HSV )
              c.toRGB();
            colors[k++] = c;
          }
        }
      }

      for( int i = 0; i < this->_m; i++ ) {

        int o1 = 0.5 *  i    * (i+1);
        int o2 = 0.5 * (i+1) * (i+2);
        int o = i*(i+2);
        int is = 2 * i + 3;

        for( int j = 0; j < is/2; j++ ) {

          int idx1 = (o+j*2);
          int idxp1 = o2 +j;

          ptr[idx1].r   = colors[idxp1].getRedC();
          ptr[idx1].g   = colors[idxp1].getGreenC();
          ptr[idx1].b   = colors[idxp1].getBlueC();
          ptr[idx1].a   = colors[idxp1].getAlphaC();


          int idx2 = (o+j*2+1);
          int idxp2 = o1 +j;

          ptr[idx2].r   = colors[idxp2].getRedC();
          ptr[idx2].g   = colors[idxp2].getGreenC();
          ptr[idx2].b   = colors[idxp2].getBlueC();
          ptr[idx2].a   = colors[idxp2].getAlphaC();
        }

        ptr[o+is-1].r   = colors[o1+is-1].getRedC();
        ptr[o+is-1].g   = colors[o1+is-1].getGreenC();
        ptr[o+is-1].b   = colors[o1+is-1].getBlueC();
        ptr[o+is-1].a   = colors[o1+is-1].getAlphaC();
      }

    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
    glBindBuffer( GL_ARRAY_BUFFER, 0x0 );
  }

  template <typename T>
  void PTriangleColorVisualizer<T>::setColorMode(GM_PTRIANGLE_COLORVISUALIZER_COLOR_MODE color_mode) {

    _color_mode = color_mode;
  }

  template <typename T>
  void PTriangleColorVisualizer<T>::setColors( const Color& c1, const Color& c2, const Color& c3 ) {

    _c1 = c1;
    _c2 = c2;
    _c3 = c3;
  }

  template <typename T>
  void PTriangleColorVisualizer<T>::setTranslucency( double a ) {

    _c1.setAlpha( a );
    _c2.setAlpha( a );
    _c3.setAlpha( a );
  }

} // END namespace GMlib





