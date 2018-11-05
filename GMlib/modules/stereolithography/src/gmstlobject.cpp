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





#include "gmstlobject.h"

// gmlib
#include <core/utils/gmstream.h>
#include <parametrics/surfaces/gmpsphere.h>

namespace GMlib {

  StlObject::StlObject(float r) {
    _header = std::string( "STL place holder" );
    _sphere = new PSphere<float>( r );
    _sphere->replot();
    insert( _sphere );

    // update bounding sphere
    Point<float,3> pos(0,0,0);
    Sphere<float,3> s(pos,r);

    setSurroundingSphere(s);

    _init();
  }


  StlObject::StlObject( PSurf<float,3> *obj, int m1, int m2, GM_STL_VISUALIZATION gsv ) {

    // Resample DSurf
    DMatrix< DMatrix< Vector<float, 3> > > p;
    obj->resample( p, m1, m2, 1, 1,
                   obj->getParStartU(),
                   obj->getParStartV(),
                   obj->getParEndU(),
                   obj->getParEndV() );

    // Set Array Max Size (speedup)
    _normals.setMaxSize( (p.getDim1()-1)*(p.getDim2()-1)*2 );
    _vertices.setMaxSize( _normals.getMaxSize()*3 );

    for( int i = 0; i < p.getDim1() - 1; i++ ) {
      for( int j = 1; j < p.getDim2(); j++ ) {

        Vector<float,3> &v1 = p[i][j-1][0][0];
        Vector<float,3> &v2 = p[i+1][j-1][0][0];
        Vector<float,3> &v3 = p[i][j][0][0];
        Vector<float,3> &v4 = p[i+1][j-1][0][0];
        Vector<float,3> &v5 = p[i+1][j][0][0];
        Vector<float,3> &v6 = p[i][j][0][0];

        Vector<float,3> n1 = Vector<float,3>(v2 - v1) ^ (v3 - v1);
        Vector<float,3> n2 = Vector<float,3>(v5 - v4) ^ (v6 - v4);

        _normals.insertAlways( n1 );
        _vertices.insertAlways( v1 );
        _vertices.insertAlways( v2 );
        _vertices.insertAlways( v3 );

        _normals.insertAlways( n2 );
        _vertices.insertAlways( v4 );
        _vertices.insertAlways( v5 );
        _vertices.insertAlways( v6 );
      }
    }

    _init();
    replot( gsv );
  }


  StlObject::StlObject( const std::string& filename, const Color& color, int /*flip*/ ) {

    _dlist  = 0;
    _color  = color;
    _header = filename;

    _readStlBinary( filename );
    _init();

    replot();
  }


  StlObject::StlObject( std::ifstream& stream, bool binary, const Color& color ) {

    _dlist = 0;
    _color = color;

    load( stream, binary );
    _init();

    replot();
  }



  StlObject::~StlObject() {

    glDeleteLists( _dlist, 2 );
  }


  unsigned long StlObject::_getUli() {

    unsigned char byte1 = static_cast<unsigned char>(fgetc( _stl_file ));
    unsigned char byte2 = static_cast<unsigned char>(fgetc( _stl_file ));
    unsigned char byte3 = static_cast<unsigned char>(fgetc( _stl_file ));
    unsigned char byte4 = static_cast<unsigned char>(fgetc( _stl_file ));
    unsigned long int number =  static_cast<unsigned long int>(byte1)
                             + (static_cast<unsigned long int>(byte2<<8))
                             + (static_cast<unsigned long int>(byte3<<16))
                             + (static_cast<unsigned long int>(byte4<<24));

    return( number );
  }


  float StlObject::_getFloat() {

    float number;
    unsigned char stream[5];
    // used to be 3,2,1,0
    stream[0]=static_cast<unsigned char>(fgetc( _stl_file ));
    stream[1]=static_cast<unsigned char>(fgetc( _stl_file ));
    stream[2]=static_cast<unsigned char>(fgetc( _stl_file ));
    stream[3]=static_cast<unsigned char>(fgetc( _stl_file ));
    stream[4] = '\0';

    memcpy( &number, &stream, 4 );

    return( number );
  }


  unsigned int StlObject::_getUint() {

    unsigned char byte1 = static_cast<unsigned char>(fgetc( _stl_file ));
    unsigned char byte2 = static_cast<unsigned char>(fgetc( _stl_file ));
    unsigned int number = static_cast<unsigned int>(byte1) + (static_cast<unsigned int>(byte2<<8));

    return( number );
  }


  void StlObject::_init() {

    _point_size = 5.0;
  }


  void StlObject::_makeList( GLenum /*e*/ ) {



//    if( _dlist ) {

//      glDeleteLists( _dlist, 2 );
//      _dlist = 0;
//    }


//    // make display lists
//    _dlist = glGenLists( 2 );

//    glPushAttrib( GL_POINT_BIT );

//    glNewList( _dlist, GL_COMPILE ); {

//      if( e == GL_POINTS )
//        glPointSize( _point_size );

//      glBegin( e ); {

//        for( int i = 0; i < _normals.getSize(); i++) {
//          glNormal( _normals[i] );                // STL file only carries one normal
//          for( int j = 0; j < 3; j++ )            // for each triangle, makes a facet shading,
//            glPoint( _vertices[ 3*i+j ] );        // they must be averaged for true smooth surfaces.
//        }
//      } glEnd();
//    } glEndList();

//    // Build a new list for selection
//    glNewList( _dlist+1, GL_COMPILE ); {
//      glBegin( GL_TRIANGLES ); {

//        for( int i = 0; i < _normals.getSize(); i++ ) {
//          for( int j = 0;j < 3; j++ )
//            glPoint( _vertices[ 3*i+j ] );
//        }
//      } glEnd();
//    } glEndList();

//    glPopAttrib();
  }


  int StlObject::_readStlBinary(const std::string& filename) {

    if( ( _stl_file = fopen( filename.c_str(), "rb" ) ) == NULL )
      return(-1);

    unsigned long int t;

    // Skipping comment line
    for( t=0; t<80 ; t++ )
      fgetc( _stl_file );

    // Get number for triangles
    unsigned long int n_triangles = _getUli();
    float x,y,z;
    _normals.setMaxSize( n_triangles+1 );
    _vertices.setMaxSize( (n_triangles*3)+1 );

    // Read all the triangles
    for( t=0; t<n_triangles; t++ ) {

      // and put in array
      x=_getFloat(); y=_getFloat(); z=_getFloat();
      _normals.insertAlways( UnitVector<float,3>(x,y,z) );


      x=_getFloat(); y=_getFloat(); z=_getFloat();
      _vertices.insertAlways( Point<float,3> (x,y,z) );

      x=_getFloat(); y=_getFloat(); z=_getFloat();
      _vertices.insertAlways( Point<float,3> (x,y,z) );

      x=_getFloat(); y=_getFloat(); z=_getFloat();
      _vertices.insertAlways( Point<float,3> (x,y,z) );

      // Get number of attribute bytes (is always zero and can be ignored)
      _getUint();
    }

    fclose( _stl_file );
    return 1;
  }


  void StlObject::_updateBounding() {

    // update bounding box
    _bbox.reset( _vertices[0] );
    for( int i = 0; i < _vertices.getSize()-1; i++ )
      _bbox += _vertices[i];

    // update surrouding sphere
    Point<float,3> pos( _bbox.getPointCenter() );  // which can be far from origo
    Sphere<float,3> s( pos, _bbox.getPointDelta().getLength() * 0.5 );

    setSurroundingSphere( s );
  }


  float StlObject::getPointSize() {

    return _point_size;
  }


  void StlObject::replot( GM_STL_VISUALIZATION gsv ) {

    if( gsv == GM_STL_POINT )
      _makeList( GL_POINTS );
    else
      _makeList( GL_TRIANGLES );

    _updateBounding();
  }


  void StlObject::load( std::ifstream& stream, bool binary ) {

    // Binary
    if( binary ) {

      // Read header
      char hbuff[80];
      stream.read( hbuff, 80 );

      _header = hbuff;

      unsigned int facets;
      stream.read( reinterpret_cast<char*>(&facets), sizeof( unsigned int ) );


      // Allocate memory
      _normals.setSize(facets);
      _vertices.setSize(_normals.getMaxSize()*3);

      // Read Normal and Vertices and Attribute bit of each face
      for( unsigned int i = 0; i < facets; i++ ) {

        // Normal
        stream.read( reinterpret_cast<char*>(_normals[i].getPtr()), 3 * sizeof( unsigned int ) );

        // Vertices
        stream.read( reinterpret_cast<char*>(_vertices[i*3].getPtr()), 9 * sizeof( unsigned int ) );

        // Attribute
        unsigned short attrib = 0;
        stream.read( reinterpret_cast<char*>(&attrib), sizeof( unsigned short ) );
      }
    }
  }


  void StlObject::save( std::ofstream& /*stream*/, bool /*binary*/ ) {

    assert(false);
//    typedef std::stringstream std_ss;

//    // Binary
//    if( binary ) {


//      std_ss header;
//      header << std_ss("GMlib STL: ") << std_ss(this->getIdentity());

//      char hbuff[80]; for( int i = 0; i < 80; i++ ) hbuff[i] = ' ';
//      memcpy( hbuff, header.str().c_str(), header.str().length() );
//      stream.write( hbuff, 80 );


//      unsigned int facets = _normals.getSize();
//      stream.write( (char*)&facets, sizeof( unsigned int ) );

//      for( int i = 0; i < _normals.getSize(); i++ ) {

//        // Normal
//        stream.write( (char*)&_normals[i], 3 * sizeof( unsigned int ) );

//        // Vertices
//        stream.write( (char*)&_vertices[i*3], 9 * sizeof( unsigned int ) );

//        // Attribute
//        unsigned short attrib = 0;
//        stream.write( (char*)&attrib, sizeof( unsigned short ) );
//      }
//    }
//    else {

//      std_ss content;
//      content << std_ss("solid ") << std_ss(this->getIdentity()) << std::endl;

//      for( int i = 0; i < _normals.getSize(); i++ ) {

//        const Vector<float,3> &v0 = _vertices(i*3);
//        const Vector<float,3> &v1 = _vertices(i*3+1);
//        const Vector<float,3> &v2 = _vertices(i*3+2);

//        // Normal
//        const Vector<float,3> &n = _normals(i);

//        content << std_ss("  facet normal ") << n(0) << std_ss(" ") << n(1) << std_ss(" ") << n(2) << std::endl;

//        content << std_ss("    outer loop") << std::endl;

//        // Vertices
//        content << std_ss("      vertex ") << v0(0) << std_ss(" ") << v0(1) <<
//                   std_ss(" ") << v0(2) << std::endl;
//        content << std_ss("      vertex ") << v1(0) << std_ss(" ") << v1(1) <<
//                   std_ss(" ") << v1(2) << std::endl;
//        content << std_ss("      vertex ") << v2(0) << std_ss(" ") << v2(1) <<
//                   std_ss(" ") << v2(2) << std::endl;

//        content << std_ss("    endloop") << std::endl;

//        content << std_ss("  endfacet") << std::endl;
//      }

//      content << std_ss("endsolid ") << std_ss(this->getIdentity()) << std::endl;

//      stream.write( content.str().c_str(), content.str().length() * sizeof( char ) );
//    }
  }


  void StlObject::setPointSize( float s ) {

    _point_size = s;
  }


}
