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




#ifndef GM_SCENE_UTILS_TEXTURE_H
#define GM_SCENE_UTILS_TEXTURE_H

// gmlib
#include <core/utils/gmcolor.h>

// stl
#include <map>


namespace GMlib {


  class Texture {
  public:

    enum DEPTH {
      GM_TEXTURE_DEPTH_24 = 24,
      GM_TEXTURE_DEPTH_32 = 32
    };

    Texture();
    Texture( unsigned char* data, unsigned int width, unsigned int height,
             Texture::DEPTH depth = Texture::GM_TEXTURE_DEPTH_24,
             bool gen_texture = false );
    Texture( const Texture& copy );
   ~Texture();

    bool                     genTexture();
    unsigned short           getDepth() const;
    unsigned int             getDimension() const;
    unsigned int             getHeight() const;
    const unsigned char*     getPtr() const;
    unsigned int             getTextureId() const;
    unsigned int             getWidth() const;
    bool                     isValid() const;
    bool                     set( unsigned char* data, unsigned int width, unsigned int height,
                                  Texture::DEPTH depth = Texture::GM_TEXTURE_DEPTH_24,
                                  bool     gen_texture = false );
    void                     setDimension( unsigned int texture_dimension );
    void                     setGenMode( unsigned int texture_gen_mode );

    Texture&                 operator =  ( const Texture& texture );
    const Color&             operator () (int i, int j) const;
    bool                     operator == ( const Texture& texture ) const;


  private:
    static std::map<unsigned int, int>   _texture_id_map;

    unsigned char*           _data;
    unsigned int             _height;
    unsigned int             _width;
    unsigned short           _depth;

    unsigned int             _texture_id;
    unsigned int             _texture_dimension;
    unsigned int             _texture_gen_mode;

  }; // END class Texture

} // END namespace GMlib


#endif // GM_SCENE_UTILS_TEXTURE_H
