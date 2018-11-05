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





#include "gmtexture.h"

// gmlib
#include <opengl/gmopengl.h>


namespace GMlib {


  std::map<unsigned int, int> Texture::_texture_id_map = std::map<unsigned int, int>();


  Texture::Texture() {

    _data = NULL;
    _texture_id = 0;
    _texture_dimension = GL::GM_TEXTURE_2D;
    _texture_gen_mode = GL::GM_OBJECT_LINEAR;
  }


  Texture::Texture( unsigned char* data, unsigned int width, unsigned int height, Texture::DEPTH depth, bool gen_texture ) {

    _texture_id = 0;
    _texture_dimension = GL::GM_TEXTURE_2D;
    _texture_gen_mode = GL::GM_OBJECT_LINEAR;
    set( data, width, height, depth, gen_texture );
  }


  Texture::Texture( const Texture& texture ) {

    operator = ( texture );
  }


  Texture::~Texture() {

    if( _texture_id ) {

      _texture_id_map[_texture_id]--;

      if( _texture_id_map[_texture_id] < 1 )
        glDeleteTextures( 1, &_texture_id );
    }
  }


  bool Texture::genTexture() {

    // Check if Data != 0
    if( _data == 0 )
      return false;

    // Gen Texture ID
    if( _texture_id ) {

      _texture_id_map[_texture_id]--;

      if( _texture_id_map[_texture_id] < 1 )
        glDeleteTextures( 1, &_texture_id );
    }

    glGenTextures( 1, &_texture_id );


    // Bind Texture ID
    glBindTexture( _texture_dimension, _texture_id );


    // Texture parameters;
    // Minifying and Magnifying Policy
    glTexParameteri( _texture_dimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( _texture_dimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


    // S/T/R Wrap Policy
    if( _texture_dimension == GL::GM_TEXTURE_1D || _texture_dimension == GL::GM_TEXTURE_2D )
      glTexParameteri( _texture_dimension, GL_TEXTURE_WRAP_S, GL_REPEAT );

    if( _texture_dimension == GL::GM_TEXTURE_2D)
      glTexParameteri( _texture_dimension, GL_TEXTURE_WRAP_T, GL_REPEAT );


    // S/T/R Texture Gen Mode
    if( _texture_dimension == GL::GM_TEXTURE_1D || _texture_dimension == GL::GM_TEXTURE_2D )
      glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, _texture_gen_mode );

    if( _texture_dimension == GL::GM_TEXTURE_2D )
      glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, _texture_gen_mode );


    // Texture Coordinate Generation
    if( _texture_dimension == GL::GM_TEXTURE_1D ) {

      if ( _depth == GM_TEXTURE_DEPTH_24 )
        gluBuild1DMipmaps( _texture_dimension, 3, _width, GL_RGB, GL_UNSIGNED_BYTE, _data );
      else if( _depth == GM_TEXTURE_DEPTH_32 )
        gluBuild1DMipmaps( _texture_dimension, 3, _width, GL_RGBA, GL_UNSIGNED_BYTE, _data );
    }
    else if ( _texture_dimension == GL::GM_TEXTURE_2D ) {

      if ( _depth == GM_TEXTURE_DEPTH_24 )
        gluBuild2DMipmaps( _texture_dimension, 3, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, _data );
      else if( _depth == GM_TEXTURE_DEPTH_32 )
        gluBuild2DMipmaps( _texture_dimension, 3, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _data );
    }

    // "Release" Texture ID
    glBindTexture( _texture_dimension, 0 );

    // Add Texture ID to Texture ID Map
    _texture_id_map[_texture_id] = 1;

    // Return Success
    return true;
  }


  unsigned short Texture::getDepth() const {

    return _depth;
  }


  unsigned int Texture::getDimension() const {

    return _texture_dimension;
  }


  unsigned int Texture::getHeight() const {

    return _height;
  }


  const unsigned char* Texture::getPtr() const {

    return _data;
  }


  unsigned int Texture::getTextureId() const {

    return _texture_id;
  }


  unsigned int Texture::getWidth() const {

    return _width;
  }


  bool Texture::isValid() const {

    return _texture_id;
  }


  bool Texture::set( unsigned char* data, unsigned int width, unsigned int height, Texture::DEPTH depth, bool gen_texture ) {

    _data   = data;
    _width  = width;
    _height = height;
    _depth  = depth;

    if( gen_texture )
      return genTexture();
    else
      return false;
  }


  void Texture::setDimension( unsigned int texture_dimension ) {

    _texture_dimension = texture_dimension;
  }


  void Texture::setGenMode( unsigned int texture_gen_mode ) {

    _texture_gen_mode = texture_gen_mode;
  }


  Texture& Texture::operator = ( const Texture& texture ) {

    // Keep old tex id
    unsigned int old_texture_id = _texture_id;

    // Copy data
    _texture_id = texture._texture_id;
    _texture_dimension = texture._texture_dimension;
    _texture_gen_mode = texture._texture_gen_mode;

    _data = texture._data;
    _width = texture._width;
    _height = texture._height;
    _depth = texture._depth;

    // Iterate current tex id
    if( _texture_id )
      _texture_id_map[ _texture_id ]++;

    // Clean up old tex id
    if( old_texture_id && (old_texture_id != _texture_id ) ) {

      _texture_id_map[old_texture_id]--;

      if( _texture_id_map[old_texture_id] < 1 )
        glDeleteTextures( 1, &old_texture_id );
    }

    return *this;
  }


  const Color& Texture::operator()(int i, int j) const {

      Color* color = reinterpret_cast<Color*>(_data);
      return color[ (j * _width) + i];
  }


  bool Texture::operator == ( const Texture& texture ) const {

    return _texture_id == texture._texture_id;
  }

}
