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


#ifndef GM_OPENGL_TEXTURE_H
#define GM_OPENGL_TEXTURE_H


#include "gmglobject.h"


namespace GMlib {

namespace GL {

  namespace Private {
    struct TextureInfo : GLObjectInfo { GLenum target; };
  }


  class Texture : public Private::GLObject<Private::TextureInfo> {
  public:
    explicit Texture();
    ~Texture();

    void                    create( GLenum target );
    void                    create( const std::string& name, GLenum target );

    GLenum                  getTarget() const;

    /* OpenGL API */
    void                    texImage1D( GLint level, GLint internal_format, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *data );
    void                    texImage2D( GLint level, GLint internal_format, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data );
    void                    texImage3D( GLint level, GLint internal_format, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *data );

    void                    texSubImage1D( GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* data );
    void                    texSubImage2D( GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data );
    void                    texSubImage3D( GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* data );

    void                    getTexImage( GLint level, GLenum format, GLenum type, GLvoid* pixels );

    void                    texParameterf(GLenum pname, GLfloat param );
    void                    texParameteri(GLenum pname, GLint param );


  private:

    /* pure-virtual functions from Object */
    GLuint                  getCurrentBoundId() const override;
    void                    doBind( GLuint id ) const override;
    GLuint                  doGenerate() const override;
    void                    doDelete(GLuint id) const override;


  }; // END class Texture

  inline
  GLenum Texture::getTarget() const { return getInfoIter()->target; }


} // END namespace GL

} // END namespace GMlib


#endif // GM_OPENGL_TEXTURE_H
