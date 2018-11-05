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



/*! \file gmpsurfcontoursvisualizer.c
 *
 *  Implementation of the PSurfContoursVisualizer template class.
 */


namespace GMlib {


  template <typename T>
  PSurfContoursVisualizer<T>::PSurfContoursVisualizer() {

    this->setRenderProgram( GL::GLProgram("psurf_contours") );

    _mapping = GM_PSURF_CONTOURSVISUALIZER_X;

    // Set default colors
    _colors += GMcolor::red();
    _colors += GMcolor::blue();

    // Set default interpolation method
    _method = GM_PSURF_CONTOURSVISUALIZER_LINEAR;

    glGenBuffers( 1, &_ibo );
    glGenBuffers( 1, &_vbo );
  }

  template <typename T>
  PSurfContoursVisualizer<T>::~PSurfContoursVisualizer() {

    glDeleteBuffers( 1, &_ibo );
    glDeleteBuffers( 1, &_vbo );
  }

  template <typename T>
  inline
  void PSurfContoursVisualizer<T>::display() {

    const GL::GLProgram &prog = this->getRenderProgram();
    prog.setUniform( "u_selected", false );

    GLuint vert_loc = prog.getAttributeLocation( "in_vertex" );
    GLuint color_loc = prog.getAttributeLocation( "in_color" );

    GLsizei stride = sizeof( Vertex );
    glBindBuffer( GL_ARRAY_BUFFER, _vbo );

    glVertexAttribPointer( vert_loc, 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)0x0 );
    glEnableVertexAttribArray( vert_loc );

    glVertexAttribPointer( color_loc, 4, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(3*sizeof(GLfloat)) );
    glEnableVertexAttribArray( color_loc );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _ibo );
    for( int i = 0; i < _tri_strips; i++ )
      glDrawElements( GL_TRIANGLE_STRIP, _indices_per_tri_strip, GL_UNSIGNED_SHORT, (const GLvoid*)( i*_tri_strip_offset ) );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0x0 );

    glDisableVertexAttribArray( color_loc );
    glDisableVertexAttribArray( vert_loc );

    glBindBuffer( GL_ARRAY_BUFFER, 0x0 );

  }

  template <typename T>
  inline
  Color PSurfContoursVisualizer<T>::getColor( T d ) {

    // Find Index
    int idx;
    idx = d * ( _colors.getSize()-1 );
    if( idx == _colors.getSize()-1 ) idx--;
    if( (idx < 0) || (idx > _colors.getSize()-1) ) idx = 0;

    return _colors[idx];
  }

  template <typename T>
  inline
  Color PSurfContoursVisualizer<T>::getColorInterpolated( T d ) {

    // Find Index
    int idx;
    idx = d * ( _colors.getSize()-1 );
    if( idx == _colors.getSize()-1 ) idx--;
    if( (idx < 0) || (idx > _colors.getSize()-1) ) idx = 0;


    double local_d = (double( _colors.getSize()-1 ) * d) - idx;
    Color ret = _colors[idx].getInterpolatedHSV( local_d, _colors[idx+1] );

    return ret;
  }

  template <typename T>
  const Array<Color>& PSurfContoursVisualizer<T>::getColors() const {

    return _colors;
  }

  template <typename T>
  inline
  T PSurfContoursVisualizer<T>::getCurvatureGauss( DMatrix< Vector<T,3> >& p ) {

    UnitVector<T,3>   N   = p[1][0]^p[0][1];
    Vector<T,3>		  du  = p[1][0];
    Vector<T,3>		  dv  = p[0][1];
    Vector<T,3>		  duu = p[2][0];
    Vector<T,3>		  duv = p[1][1];
    Vector<T,3>		  dvv = p[0][2];

    T E = du  * du;
    T F = du  * dv;
    T G = dv  * dv;
    T e = N   * duu;
    T f = N   * duv;
    T g = N   * dvv;

    return (e*g - f*f) / (E*G - F*F);
  }

  template <typename T>
  inline
  T PSurfContoursVisualizer<T>::getCurvatureMean( DMatrix< Vector<T,3> >& p ) {

    UnitVector<T,3>   N   = p[1][0]^p[0][1];
    Vector<T,3>		  du  = p[1][0];
    Vector<T,3>		  dv  = p[0][1];
    Vector<T,3>		  duu = p[2][0];
    Vector<T,3>		  duv = p[1][1];
    Vector<T,3>		  dvv = p[0][2];

    T E = du  * du;
    T F = du  * dv;
    T G = dv  * dv;
    T e = N   * duu;
    T f = N   * duv;
    T g = N   * dvv;

    return 0.5 * (e*G - 2 * (f*F) + g*E) / (E*G - F*F);
  }

  template <typename T>
  inline
  T PSurfContoursVisualizer<T>::getCurvaturePrincipalMax( DMatrix< Vector<T,3> >& p ) {

    T K = getCurvatureGauss( p );
    T H = getCurvatureMean( p );

    return H + sqrt( H*H - K );
  }

  template <typename T>
  inline
  T PSurfContoursVisualizer<T>::getCurvaturePrincipalMin( DMatrix< Vector<T,3> >& p ) {

    T K = getCurvatureGauss( p );
    T H = getCurvatureMean( p );

    return H - sqrt( H*H - K );
  }

  template <typename T>
  GM_PSURF_CONTOURSVISUALIZER_INTERPOLATION_METHOD PSurfContoursVisualizer<T>::getInterpolationMethod() const {

    return _method;
  }

  template <typename T>
  GM_PSURF_CONTOURSVISUALIZER_MAP PSurfContoursVisualizer<T>::getMapping() const {

    return _mapping;
  }

  template <typename T>
  inline
  void PSurfContoursVisualizer<T>::replot(
    DMatrix< DMatrix< Vector<T, 3> > >& p,
    DMatrix< Vector<T, 3> >& normals,
    int m1, int m2, int d1, int d2,
    bool /*closed_u*/, bool /*closed_v*/
  ) {

    _tri_strips = PSurfVisualizer<T>::getNoTriangleStrips( p.getDim1(), p.getDim2() );
    _indices_per_tri_strip = PSurfVisualizer<T>::getNoIndicesPerTriangleStrip( p.getDim1(), p.getDim2() );
    _tri_strip_offset =  sizeof(GLushort) * _indices_per_tri_strip;

    PSurfVisualizer<T>::fillTriangleStripIBO( _ibo, p.getDim1(), p.getDim2() );


    // Color/Material Countours
    DMatrix<double> cmap;

    T min, max;
    T C;
    cmap.setDim( p.getDim1(), p.getDim2() );
    switch( _mapping ) {

    case GM_PSURF_CONTOURSVISUALIZER_U:
      {
        for( int i = 0; i < p.getDim1(); i++ )
          for( int j = 0; j < p.getDim2(); j++ )
            cmap[i][j] = double(i) / double(p.getDim1()-1);
      }
      break;
    case GM_PSURF_CONTOURSVISUALIZER_V:
      {
        for( int i = 0; i < p.getDim1(); i++ )
          for( int j = 0; j < p.getDim2(); j++ )
            cmap[i][j] = double(j) / double(p.getDim2()-1);
      }
      break;

    default:
      {
        // Init min/max
        min = max = getValue(p, 0, 0);

        // Extract all Gaussian Curvature data
        for( int i = 0; i < p.getDim1(); i++ ) {
          for( int j = 0; j < p.getDim2(); j++ ) {

            const T curvature = getValue(p, i, j);
            if( curvature < min )
              min = curvature;
            if( curvature > max )
              max = curvature;
          }
        }

        // Correct interval
        if( (max-min) < 1e-5 ) {

          for( int i = 0; i < p.getDim1(); i++ )
            for( int j = 0; j < p.getDim2(); j++ )
              cmap[i][j] = 0.0;
        }
        else {
          C = 1.0f / (max-min);
          min /= (max-min);

          // Compute interpolated map values
          for( int i = 0; i < p.getDim1(); i++ ) {
            for( int j = 0; j < p.getDim2(); j++ ) {

              const T curvature = getValue(p, i, j);
              cmap[i][j] = ( C * curvature ) - min;
            }
          }
        }
      }
      break;
    }


    glBindBuffer( GL_ARRAY_BUFFER, _vbo );
    Vertex data[p.getDim1()*p.getDim2()];

    // Fill vertex point data.
    for( int i = 0; i < p.getDim1(); i++ ) {
      for( int j = 0; j < p.getDim2(); j++ ) {

        const int idx = i * p.getDim2() + j;
        data[idx].x = p[i][j][0][0][0];
        data[idx].y = p[i][j][0][0][1];
        data[idx].z = p[i][j][0][0][2];
      }
    }


    // Fill vertex color data
    switch( _method ) {
    case GM_PSURF_CONTOURSVISUALIZER_NO_INTERPOLATION:
      for( int i = 0; i < p.getDim1(); i++ ) {
        for( int j = 0; j < p.getDim2(); j++ ) {

          const int idx = i * p.getDim2() + j;
          const Color c = getColor( cmap[i][j] );
          data[idx].r = c.getRedC();
          data[idx].g = c.getGreenC();
          data[idx].b = c.getBlueC();
          data[idx].a = c.getAlphaC();
        }
      }
      break;

    case GM_PSURF_CONTOURSVISUALIZER_LINEAR:
    default:
      for( int i = 0; i < p.getDim1(); i++ ) {
        for( int j = 0; j < p.getDim2(); j++ ) {

          const int idx = i * p.getDim2() + j;
          const Color c = getColorInterpolated( cmap[i][j] );
          data[idx].r = c.getRedC();
          data[idx].g = c.getGreenC();
          data[idx].b = c.getBlueC();
          data[idx].a = c.getAlphaC();
        }
      }
      break;
    }

    glBindBuffer( GL_ARRAY_BUFFER, _vbo );
    glBufferData( GL_ARRAY_BUFFER, p.getDim1() * p.getDim2() * sizeof(Vertex), data, GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0x0 );
  }

  template <typename T>
  inline
  T PSurfContoursVisualizer<T>::getValue( DMatrix< DMatrix< Vector<T, 3> > >& p, int i, int j ) {

    switch( _mapping ) {
    case GM_PSURF_CONTOURSVISUALIZER_X:
      return p[i][j][0][0][0];
    case GM_PSURF_CONTOURSVISUALIZER_Y:
      return p[i][j][0][0][1];
    case GM_PSURF_CONTOURSVISUALIZER_Z:
      return p[i][j][0][0][2];

    default: break;
    }


    // Values requireing 1st derivatives
    if( p[i][j].getDim1() < 2 || p[i][j].getDim2() < 2 )
      return T(0);

    switch( _mapping ) {
    case GM_PSURF_CONTOURSVISUALIZER_U:
      return p[i][j][1][0].getLength();
    case GM_PSURF_CONTOURSVISUALIZER_V:
      return p[i][j][0][1].getLength();

    default: break;
    }


    // Values requireing 2nd derivatives
    if( p[i][j].getDim1() < 3 || p[i][j].getDim2() < 3 )
      return T(0);

    switch( _mapping ) {
    case GM_PSURF_CONTOURSVISUALIZER_CURVATURE_GAUSS:
      return getCurvatureGauss(p[i][j]);

    case GM_PSURF_CONTOURSVISUALIZER_CURVATURE_MEAN:
      return getCurvatureMean(p[i][j]);

    case GM_PSURF_CONTOURSVISUALIZER_CURVATURE_PRINCIPAL_MAX:
      return getCurvaturePrincipalMax( p[i][j] );

    case GM_PSURF_CONTOURSVISUALIZER_CURVATURE_PRINCIPAL_MIN:
      return getCurvaturePrincipalMin( p[i][j] );

    default: break;
    }

    return T(0);
  }

  template <typename T>
  void PSurfContoursVisualizer<T>::setColors( const Array<Color>& c ) {

    _colors = c;
  }

  template <typename T>
  void PSurfContoursVisualizer<T>::setInterpolationMethod( GM_PSURF_CONTOURSVISUALIZER_INTERPOLATION_METHOD method ) {

    _method = method;
  }

  template <typename T>
  void PSurfContoursVisualizer<T>::setMapping( GM_PSURF_CONTOURSVISUALIZER_MAP mapping ) {

    _mapping = mapping;
  }



} // END namespace GMlib
