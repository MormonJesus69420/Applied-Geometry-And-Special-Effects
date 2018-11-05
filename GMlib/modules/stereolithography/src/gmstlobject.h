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




#ifndef GM_STEREOLITHOGRAPHY_STLOBJECT_H
#define GM_STEREOLITHOGRAPHY_STLOBJECT_H


// gmlib
#include <core/containers/gmarraylx.h>
#include <scene/gmsceneobject.h>
#include <trianglesystem/gmtrianglesystem.h>

// stl
#include <string>


namespace GMlib {

  template <typename T, int n>  class PSurf;
  template <typename T>         class PSphere;

  template <typename T>         class TSVertex;


  enum GM_STL_VISUALIZATION {
    GM_STL_POINT,
    GM_STL_TRIANGLE
  };


  class StlObject : public SceneObject {
    GM_SCENEOBJECT(StlObject)
  public:
    StlObject( const std::string& filename, const Color& color = 5, int flip = 1 ); // From file, in given color
    StlObject( std::ifstream& stream, bool binary = true, const Color& color = GMcolor::aqua() );

    StlObject( PSurf<float,3> *obj, int m1 = 20, int m2 = 20, GM_STL_VISUALIZATION gsv = GM_STL_TRIANGLE );
    StlObject( float r = 10 );       // Makes a Sphere, just for debugging
    ~StlObject();

    unsigned int                      getNoNormals() const;
    unsigned int                      getNoPoints() const;
    Array< Vector<float,3> >          getNormals();
    const Array< Vector<float,3> >&   getNormals() const;
    Array<Point<float,3> >            getPoints();
    const Array<Point<float,3> >&     getPoints() const;
    float                             getPointSize();
    ArrayLX< TSVertex<float> >        getVertices();

    void                              load( std::ifstream& stream, bool binary = true );
    void                              replot( GM_STL_VISUALIZATION gsv = GM_STL_TRIANGLE );
    void                              save( std::ofstream& stream, bool binary = true );
    void                              setPointSize( float s = 5.0 );

  protected:
    void                              localDisplay(const DefaultRenderer *) const override;
    void                              localSelect(const Renderer *, const Color &) const override;

  private:
    float                             _getFloat();
    unsigned int                      _getUint();
    unsigned long                     _getUli();
    int                               _readStlBinary( const std::string& filename );
    void                              _updateBounding();
    void                              _makeList( GLenum e = GL_TRIANGLES );
    void                              _init();

    unsigned int                      _dlist;
    PSphere<float>                   *_sphere;      // Debug

    std::string                       _header;      // I put the filename in here,
    FILE                             *_stl_file;

    Array<Point<float,3> >            _vertices;    // storage, each three makes a triangle
    Array<Vector<float,3> >           _normals;     // with one normal for each triangle
    Box<float,3>                      _bbox;        // Bounding box, should be an options
    float                             _point_size;

    // binary file utility functions to render it.

  }; // END class StlObject






  inline
  unsigned int StlObject::getNoNormals() const {

    return _normals.getSize();
  }


  inline
  unsigned int StlObject::getNoPoints() const {

    return _vertices.getSize();
  }


  inline
  Array< Vector<float,3> > StlObject::getNormals() {

    return _normals;
  }


  inline
  const Array< Vector<float,3> >& StlObject::getNormals() const {

    return _normals;
  }


  inline
  Array< Point<float,3> > StlObject::getPoints() {

//    Array< Point<float,3> > v;         // returns copy, no cast between Point<float,3> and Point<float,3>
//    v.setSize( _vertices.getSize() );
//    for( int i = 0; i < _vertices.getSize(); i++ )
//      v[i] = _vertices[i];

//    return v;
      return _vertices;
  }


  inline
  const Array< Point<float,3> >& StlObject::getPoints() const {

    return _vertices;
  }


  inline
  ArrayLX< TSVertex<float> > StlObject::getVertices() {

    ArrayLX< TSVertex<float> > v;
    v.setSize( _vertices.getSize() );
    for( int i = 0; i < _vertices.getSize(); i++ )
      v[i] = TSVertex<float>( _vertices[i], _normals[i/3] );

    return v;
  }


  inline
  void StlObject::localDisplay(const DefaultRenderer*) const {

//    glPushAttrib( GL_LIGHTING_BIT );

//    if( this->isLighted() )
//    {
//      this->_material.glSet();
//    }
//    else
//    {
//      glDisable( GL_LIGHTING );
//      glColor(this->_color);
//    }

//    if( _dlist )
//      glCallList( _dlist );

//    glPopAttrib();
  }


  inline
  void StlObject::localSelect(const Renderer *, const Color &) const {

    glCallList( _dlist+1 );
  }

} // END namespace GMlib


#endif // GM_STEREOLITHOGRAPHY_STLOBJECT_H


