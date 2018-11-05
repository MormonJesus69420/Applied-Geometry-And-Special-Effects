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



#include "gmframebufferobject.h"


namespace GMlib { namespace GL {


  namespace Private {

    template <>
    std::list<FBOInfo> GLObject<FBOInfo>::_data = std::list<FBOInfo>();
  }




  FramebufferObject::FramebufferObject() {}



  FramebufferObject::~FramebufferObject() { decrement(); }

  void FramebufferObject::create() {

    Private::FBOInfo info;
    createObject(info);
  }

  void FramebufferObject::create(const std::string& name) {

    Private::FBOInfo info;
    info.name = name;
    createObject(info);
  }

  GLuint FramebufferObject::getCurrentBoundId() const{

    GLint id;
    GL_CHECK(::glGetIntegerv( GL_FRAMEBUFFER_BINDING, &id ));
    return id;
  }

  void FramebufferObject::doBind(GLuint id) const {

    privateBind( GL_FRAMEBUFFER, id );
  }

  GLuint FramebufferObject::doGenerate() const {

    GLuint id;
    GL_CHECK(::glGenFramebuffers( 1, &id ));
    return id;
  }

  void FramebufferObject::doDelete(GLuint id) const {

    GL_CHECK(::glDeleteFramebuffers( 1, &id ));
  }



  void FramebufferObject::attachRenderbuffer(const RenderbufferObject &rbo, GLenum attachment) {

    GLint id = safeBind();
    rbo.bind();
    GL_CHECK(::glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.getId() ));
    rbo.unbind();
    safeUnbind(id);
  }

  void FramebufferObject::attachTexture1D(const Texture &tex, GLenum target, GLenum attachment, GLenum textarget, GLint level ) {

    GLint id = safeBind();
    GL_CHECK(::glFramebufferTexture1D( target, attachment, textarget, tex.getId(), level ));
    safeUnbind(id);
  }

  void FramebufferObject::attachTexture2D(const Texture &tex, GLenum target, GLenum attachment, GLenum textarget, GLint level ) {

    GLint id = safeBind();
    GL_CHECK(::glFramebufferTexture2D( target, attachment, textarget, tex.getId(), level ));
    safeUnbind(id);
  }

  void FramebufferObject::attachTexture3D(const Texture &tex, GLenum target, GLenum attachment, GLenum textarget, GLint level, GLint layer ) {

    GLint id = safeBind();
    GL_CHECK(::glFramebufferTexture3D( target, attachment, textarget, tex.getId(), level, layer ));
    safeUnbind(id);
  }

  void FramebufferObject::blitTo(const FramebufferObject& dest_fbo, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) const {

    bindRead();
    dest_fbo.bindDraw();
    GL_CHECK(::glBlitFramebuffer( srcX0,srcY0,srcX1,srcY1,dstX0,dstY0,dstX1,dstY1,mask,filter ));
    dest_fbo.unbindDraw();
    unbindRead();
  }

  void FramebufferObject::blitTo(GLuint dest_id, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) const {

    bindRead();
    GL_CHECK(::glBindFramebuffer( GL_DRAW_FRAMEBUFFER, dest_id ));
    GL_CHECK(::glBlitFramebuffer( srcX0,srcY0,srcX1,srcY1,dstX0,dstY0,dstX1,dstY1,mask,filter ));
    GL_CHECK(::glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0x0 ));
    unbindRead();
  }

}} // END namespace GMlib::GL
