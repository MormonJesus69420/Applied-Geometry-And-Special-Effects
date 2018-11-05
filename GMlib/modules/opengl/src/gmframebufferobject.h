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


#ifndef GM_OPENGL_FRAMEBUFFEROBJECT_H
#define GM_OPENGL_FRAMEBUFFEROBJECT_H


#include "gmglobject.h"
#include "gmrenderbufferobject.h"
#include "gmtexture.h"


namespace GMlib {

namespace GL {

  namespace Private {
    struct FBOInfo : GLObjectInfo {};
  }

  class FramebufferObject : public Private::GLObject<Private::FBOInfo> {
  public:
    explicit FramebufferObject();
    ~FramebufferObject();

    void                    create();
    void                    create( const std::string& name );

    void                    bindRead() const;
    void                    unbindRead() const;

    void                    bindDraw() const;
    void                    unbindDraw() const;

    void                    clear(GLbitfield mask) const;
    void                    clearColorBuffer(const Color& c) const;

    void                    attachRenderbuffer( const RenderbufferObject& rbo,  GLenum attachment );

    void                    attachTexture1D(const Texture& tex, GLenum target, GLenum attachment, GLenum textarget = GL_TEXTURE_1D, GLint level = 0);
    void                    attachTexture2D(const Texture& tex, GLenum target, GLenum attachment, GLenum textarget = GL_TEXTURE_2D, GLint level = 0);
    void                    attachTexture3D(const Texture& tex, GLenum target, GLenum attachment, GLenum textarget = GL_TEXTURE_3D, GLint level = 0, GLint layer = 0 );

    void                    blitTo( const FramebufferObject& dest_fbo,
                                    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
                                    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
                                    GLbitfield mask, GLenum filter ) const;

    void                    blitTo( GLuint dest_id,
                                    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
                                    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
                                    GLbitfield mask, GLenum filter ) const;

    /* pure virtual functions */
    GLuint                  getCurrentBoundId() const override;
    void                    doBind( GLuint id ) const override;
    GLuint                  doGenerate() const override;
    void                    doDelete(GLuint id) const override;

    void                    privateBind( GLenum target, GLuint id ) const;


  }; // END class FramebufferObject

  inline
  void FramebufferObject::privateBind( GLenum target, GLuint id ) const {

    GL_CHECK(::glBindFramebuffer( target, id ));
  }

  inline
  void FramebufferObject::bindRead() const {

    privateBind( GL_READ_FRAMEBUFFER, getId() );
  }

  inline
  void FramebufferObject::unbindRead() const {

    privateBind( GL_READ_FRAMEBUFFER, 0 );
  }

  inline
  void FramebufferObject::bindDraw() const {

    privateBind( GL_DRAW_FRAMEBUFFER, getId() );
  }

  inline
  void FramebufferObject::unbindDraw() const {

    privateBind( GL_DRAW_FRAMEBUFFER, 0 );
  }

  inline
  void FramebufferObject::clear(GLbitfield mask) const {

    GLint id = safeBind();
    GL_CHECK(::glClear( mask ));
    safeUnbind(id);
  }

  inline
  void FramebufferObject::clearColorBuffer(const Color &c ) const {

    static float cc[4];
    GL_CHECK(::glGetFloatv( GL_COLOR_CLEAR_VALUE, cc ));
    glClearColor( c );

    GLint id = safeBind();
    GL_CHECK(::glClear( GL_COLOR_BUFFER_BIT ));
    safeUnbind(id);

    GL_CHECK(::glClearColor( GLclampf(cc[0]), GLclampf(cc[1]), GLclampf(cc[2]), GLclampf(cc[3]) ));
  }


} // END namespace GL

} // END namespace GMlib


#endif // GM_OPENGL_FRAMEBUFFEROBJECT_H
