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




#include "gmpcurvedefaultvisualizer.h"

// gmlib
#include <scene/render/gmdefaultrenderer.h>

namespace GMlib {

  template <typename T, int n>
  PCurveDefaultVisualizer<T,n>::PCurveDefaultVisualizer()
    : _no_vertices(0), _line_width(6.0f) {
    _prog.acquire("color");
    _vbo.create();
  }


  template <typename T, int n>
  PCurveDefaultVisualizer<T,n>::PCurveDefaultVisualizer(std::vector<DVector<Vector<T,3>>>& p)
    : PCurveVisualizer<T,n>(p), _no_vertices(0), _line_width(6.0f) {
    _prog.acquire("color");
    _vbo.create();
  }


  template <typename T, int n>
  PCurveDefaultVisualizer<T,n>::PCurveDefaultVisualizer(const PCurveDefaultVisualizer<T,n>& copy)
    : PCurveVisualizer<T,n>(copy), _no_vertices(0), _line_width(6.0f) {
    _prog.acquire("color");
    _vbo.create();
  }






  template <typename T, int n>
  inline
  void PCurveDefaultVisualizer<T,n>::render(const SceneObject* obj, const DefaultRenderer* renderer) const {

    // GL States
    ::glLineWidth( _line_width );

    _prog.bind(); {
      // Model view and projection matrices
      _prog.uniform( "u_mvpmat", obj->getModelViewProjectionMatrix(renderer->getCamera()) );
      _prog.uniform( "u_color", obj->getColor() );

      // Vertex attribute location
      GL::AttributeLocation vertice_loc = _prog.getAttributeLocation( "in_vertex" );

      // Bind and draw
      _vbo.bind();
        _vbo.enable( vertice_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertex), reinterpret_cast<const GLvoid*>(0x0) );
          glDrawArrays( GL_LINE_STRIP, 0, _no_vertices );
        _vbo.disable( vertice_loc );
      _vbo.unbind();
    } _prog.unbind();
  }



  template <typename T, int n>
  inline
  void PCurveDefaultVisualizer<T,n>::renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const {

    _prog.bind(); {
      _prog.uniform( "u_mvpmat", obj->getModelViewProjectionMatrix(renderer->getCamera()) );
      _prog.uniform( "u_color", color );
      GL::AttributeLocation vertice_loc = _prog.getAttributeLocation( "in_vertex" );
      _vbo.bind();
        _vbo.enable( vertice_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertex), reinterpret_cast<const GLvoid*>(0x0) );
          glDrawArrays( GL_LINE_STRIP, 0, _no_vertices );
        _vbo.disable( vertice_loc );
      _vbo.unbind();
    } _prog.unbind();
  }




  template <typename T, int n>
  inline
  void PCurveDefaultVisualizer<T,n>::setLineWidth(GLfloat width) {
    _line_width = width;
  }




  template <typename T, int n>
  inline
  GLfloat PCurveDefaultVisualizer<T,n>::getLineWidth() const {
    return _line_width;
  }




  template <typename T, int n>
  void PCurveDefaultVisualizer<T,n>::replot( const std::vector< DVector< Vector<T, n> > >& p,
                                             int /*m*/, int /*d*/, bool /*closed*/ ) {

    ::glLineWidth( _line_width );
    _no_vertices = (*(this->_p)).size();
    this->fillStandardVBO( _vbo, *(this->_p));
  }




  template <typename T, int n>
  void PCurveDefaultVisualizer<T,n>::update() {

    ::glLineWidth( _line_width );
    _no_vertices = (*(this->_p)).size();
    this->fillStandardVBO( _vbo, *(this->_p));
  }



} // END namespace GMlib
