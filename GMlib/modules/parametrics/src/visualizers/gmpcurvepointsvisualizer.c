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




#include "gmpcurvepointsvisualizer.h"


namespace GMlib {

  template <typename T, int n>
  PCurvePointsVisualizer<T,n>::PCurvePointsVisualizer(float size)
    : _no_vertices{0}, _size{size}, _color{GMcolor::red()} {
    _prog.acquire("color");
    _vbo.create();
  }



  template <typename T, int n>
  PCurvePointsVisualizer<T,n>::PCurvePointsVisualizer(std::vector<DVector<Vector<T,3>>>& p)
    : PCurveVisualizer<T,n>(p), _no_vertices{0}, _size{T(4)}, _color{GMcolor::blue()} {
    _prog.acquire("color");
    _vbo.create();
  }



  template <typename T, int n>
  PCurvePointsVisualizer<T,n>::PCurvePointsVisualizer(const PCurvePointsVisualizer<T,n>& copy)
    : PCurveVisualizer<T,n>(copy), _no_vertices{0}, _size{T(4)}, _color{GMcolor::blue()} {
    _prog.acquire("color");
    _vbo.create();
  }



  template <typename T, int n>
  PCurvePointsVisualizer<T,n>::~PCurvePointsVisualizer() {}






  template <typename T, int n>
  inline
  void PCurvePointsVisualizer<T,n>::render(const SceneObject* obj, const DefaultRenderer* renderer) const {

    // GL States
    glPointSize( _size);

    _prog.bind(); {
      // Model view and projection matrices
      _prog.uniform( "u_mvpmat", obj->getModelViewProjectionMatrix(renderer->getCamera()) );
      _prog.uniform( "u_color", _color );

      // Vertex attribute location
      GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );

      // Bind and draw
      _vbo.bind();
        _vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertex), reinterpret_cast<const GLvoid*>(0x0) );
          glDrawArrays( GL_POINTS, 0, _no_vertices );
        _vbo.disable( vert_loc );
      _vbo.unbind();
    } _prog.unbind();
  }



  template <typename T, int n>
  inline
  void PCurvePointsVisualizer<T,n>::renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const {

    _prog.bind(); {
      _prog.uniform( "u_mvpmat", obj->getModelViewProjectionMatrix(renderer->getCamera()) );
      _prog.uniform( "u_color", color );
      GL::AttributeLocation vertice_loc = _prog.getAttributeLocation( "in_vertex" );
      _vbo.bind();
        _vbo.enable( vertice_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertex), reinterpret_cast<const GLvoid*>(0x0) );
          glDrawArrays( GL_POINTS, 0, _no_vertices );
        _vbo.disable( vertice_loc );
      _vbo.unbind();
    } _prog.unbind();
  }




  template <typename T, int n>
  inline
  void PCurvePointsVisualizer<T,n>::setColor( const Color& color ) {
    _color = color;
  }



  template <typename T, int n>
  inline
  void PCurvePointsVisualizer<T,n>::setSize( float size ) {
    _size = size;
  }



  template <typename T, int n>
  inline
  const Color& PCurvePointsVisualizer<T,n>::getColor() const {
    return _color;
  }



  template <typename T, int n>
  inline
  float PCurvePointsVisualizer<T,n>::getSize() const {
    return _size;
  }



  template <typename T, int n>
  inline
  void PCurvePointsVisualizer<T,n>::replot( const std::vector< DVector< Vector<T, n> > >& p,
                                            int /*m*/, int /*d*/, bool /*closed*/ ) {
     _no_vertices = (*(this->_p)).size();
     this->fillStandardVBO( _vbo, *(this->_p));
   }



  template <typename T, int n>
  void PCurvePointsVisualizer<T,n>::update() {

    _no_vertices = (*(this->_p)).size();
    this->fillStandardVBO( _vbo, *(this->_p));
  }



} // END namespace GMlib
