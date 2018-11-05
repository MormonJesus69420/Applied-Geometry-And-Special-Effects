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



#include "gmbufferobject.h"


namespace GMlib { namespace GL {


  namespace Private {
    template <>
    typename std::list<BOInfo> GLObject<BOInfo>::_data = std::list<BOInfo>();
  }




  BufferObject::BufferObject() {}

  BufferObject::~BufferObject() { decrement(); }

  void BufferObject::create(GLenum target, GLenum binding) {

    Private::BOInfo info;
    info.target  = target;
    info.binding = binding;
    createObject(info);
  }

  void BufferObject::create(const std::string& name, GLenum target, GLenum binding) {

    Private::BOInfo info;
    info.name    = name;
    info.target  = target;
    info.binding = binding;
    createObject(info);
  }









  void BufferObject::bufferData(GLsizeiptr size, const GLvoid *data, GLenum usage) const {

    GLint id = safeBind();
    GL_CHECK(::glBufferData( getTarget(), size, data, usage ));
    safeUnbind(id);
  }

  void BufferObject::bufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) const {

    GLint id = safeBind();
    GL_CHECK(::glBufferSubData( getTarget(), offset, size, data ));
    safeUnbind(id);
  }

  void BufferObject::disableVertexArrayPointer( const GL::AttributeLocation& vert_loc ) const {

    GL_CHECK(::glDisableVertexAttribArray( vert_loc() ));
  }

  void BufferObject::doBind(GLuint id) const {

    GLenum target = getTarget();
    GL_CHECK(::glBindBuffer( target, id ));
  }

  GLuint BufferObject::doGenerate() const {

    GLuint id;
    GL_CHECK(::glGenBuffers( 1, &id ));
    return id;
  }

  void BufferObject::doDelete(GLuint id) const {

    GL_CHECK(::glDeleteBuffers( 1, &id ));
  }

  void BufferObject::enableVertexArrayPointer( const GL::AttributeLocation& vert_loc, int size, GLenum type, bool normalized, GLsizei stride, const void* offset ) const {

    GL_CHECK(::glVertexAttribPointer( vert_loc(), size, type, normalized, stride, offset ));
    GL_CHECK(::glEnableVertexAttribArray( vert_loc() ));
  }

  GLuint BufferObject::getCurrentBoundId() const {

    GLint id;
    GL_CHECK(::glGetIntegerv( getBinding(), &id ));
    return id;
  }

  void BufferObject::unmapBuffer() const {

    GLint id = safeBind();
    GL_CHECK(::glUnmapBuffer( getTarget() ));
    safeUnbind(id);
  }

}} // END namespace GMlib::GL
