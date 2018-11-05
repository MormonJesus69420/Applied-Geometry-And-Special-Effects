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





// gmlib
//#include <opengl/glsl/gmglprogram.h>



namespace GMlib {

  template <typename T>
  TriangleFacetsVoronoiVisualizer<T>::TriangleFacetsVoronoiVisualizer() {

    glGenBuffers( 1, &_ibo );
    glGenBuffers( 1, &_vbo );
  }

  template <typename T>
  TriangleFacetsVoronoiVisualizer<T>::~TriangleFacetsVoronoiVisualizer() {

    glDeleteBuffers( 1, &_vbo );
    glDeleteBuffers( 1, &_ibo );
  }

  template <typename T>
  inline
  void TriangleFacetsVoronoiVisualizer<T>::display() {

    this->glSetDisplayMode();

//    Array< Light* > lights = this->_obj->getScene()->getLights();
//    std::cout << "No. Lights: " << lights.getSize() << std::endl;
/*
    const GL::GLProgram &prog = this->getRenderProgram();

    prog.uniform( "u_color", this->_obj->getColor() );
    prog.uniform( "u_selected", this->_obj->isSelected() );
    prog.uniform( "u_lighted", this->_obj->isLighted() );
    prog.uniform( "u_mat_dif", this->_obj->getMaterial().getDif() );
    prog.uniform( "u_light_dif", Color( 1.0f, 1.0f, 1.0f ) );//lights[0]->getDiffuse() );
    prog.uniform( "u_light_pos", Point<float,3>( 0.0f, 10.0f, 0.0f ) );

    // Get Material Data
    const Material &m = this->_obj->getMaterial();
    prog.uniform( "u_amb", m.getAmb() );
    prog.uniform( "u_dif", m.getDif() );
    prog.uniform( "u_spc", m.getSpc() );
    prog.uniform( "u_shin", m.getShininess() );

    GLuint vert_loc = prog.getAttributeLocation( "in_vertex" );
    GLuint normal_loc = prog.getAttributeLocation( "in_normal" );
*/

//    const GLsizei v_size = sizeof(GLVertex);
//    glBindBuffer( GL_ARRAY_BUFFER, _vbo );
//    glVertexAttribPointer( vert_loc, 3, GL_FLOAT, GL_FALSE, v_size, (GLvoid*)getGLVertexPointOffset() );
//    glEnableVertexAttribArray( vert_loc );

//    glVertexAttribPointer( normal_loc, 3, GL_FLOAT, GL_TRUE, v_size, (GLvoid*)getGLVertexNormalOffset() );
//    glEnableVertexAttribArray( normal_loc );

//    glDrawArrays( GL_LINES, 0, this->_tf->getVoronoiEdges().getSize() * 2 );

////        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _ibo );
////        glDrawElements( GL_TRIANGLES, this->_tf->getNoTriangles() * 3, GL_UNSIGNED_SHORT, (const GLvoid*)0x0 );
////        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0x0 );

//    glDisableVertexAttribArray( normal_loc );
//    glDisableVertexAttribArray( vert_loc );

//    glBindBuffer( GL_ARRAY_BUFFER, 0x0 );
  }

  template <typename T>
  inline
  void TriangleFacetsVoronoiVisualizer<T>::replot() const {

//    this->_tf->createVoronoi();


//    //    Array<Point<T,2> > &vorpnts = this->_tf->getVoronoiPoints();
//    const Array<TSVEdge<T> > &voredges = this->_tf->getVoronoiEdges();

//    // Fill the VBO
//    int no_vertices = voredges.getSize() * 2;
//    GLVertex vertices[no_vertices];

//    for( int i = 0; i < no_vertices / 2; i++ ) {

//      for( int j = 0; j < 2; j++ ) {

//        const int idx = i*2+j;
//        const Point<T,2>  &eep = voredges(i)(j); // Edge end pos
//        vertices[idx].x = eep(0);
//        vertices[idx].y = eep(1);
//        vertices[idx].z = 0;
//      }
//    }

//    glBindBuffer( GL_ARRAY_BUFFER, _vbo );
//    glBufferData( GL_ARRAY_BUFFER, no_vertices * sizeof(GLVertex), vertices, GL_STATIC_DRAW );
//    glBindBuffer( GL_ARRAY_BUFFER, 0x0 );

////    int no_indices = this->_tf->getNoTriangles() * 3;
////    GLushort indices[no_indices];
////    GLushort *iptr = indices;

////    for( int i = 0; i < this->_tf->getNoTriangles(); i++ ) {

////      Array< TSVertex<T>* > tri_verts = this->_tf->getTriangle(i)->getVertices();
////      for( int j = 0; j < tri_verts.getSize(); j++ )
////        for( int k = 0; k < this->_tf->getSize(); k++ )
////          if( tri_verts[j] == this->_tf->getVertex(k) )
////            *iptr++ = k;
////    }

////    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _ibo );
////    glBufferData( GL_ELEMENT_ARRAY_BUFFER, no_indices * sizeof(GLushort), indices, GL_STATIC_DRAW );
////    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0x0 );
  }



} // END namespace GMlib
