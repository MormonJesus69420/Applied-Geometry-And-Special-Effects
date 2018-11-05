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



#include "gmrenderbufferobject.h"

namespace GMlib { namespace GL {

  namespace Private {

    template <>
    typename std::list<RBOInfo> GLObject<RBOInfo>::_data = std::list<RBOInfo>();
  }




  RenderbufferObject::RenderbufferObject() {}

  RenderbufferObject::~RenderbufferObject() { decrement(); }

  void RenderbufferObject::create() {

    Private::RBOInfo info;
    createObject(info);
  }

  void RenderbufferObject::create(const std::string& name) {

    Private::RBOInfo info;
    info.name = name;
    createObject(info);
  }






  void RenderbufferObject::renderbufferStorage(GLenum internal_format, GLsizei width, GLsizei height) const {

    GLuint id = safeBind();
    GL_CHECK(::glRenderbufferStorage( GL_RENDERBUFFER, internal_format, width, height ));
    safeUnbind(id);
  }

  GLuint RenderbufferObject::getCurrentBoundId() const {

    GLint id;
    GL_CHECK(::glGetIntegerv( GL_RENDERBUFFER_BINDING, &id ));
    return id;
  }

  void RenderbufferObject::doBind(GLuint id) const {

    GL_CHECK(::glBindRenderbuffer( GL_RENDERBUFFER, id ));
  }

  GLuint RenderbufferObject::doGenerate() const {

    GLuint id;
    GL_CHECK(::glGenRenderbuffers( 1, &id ));
    return id;
  }

  void RenderbufferObject::doDelete(GLuint id) const {

    GL_CHECK(::glDeleteRenderbuffers( 1, &id ));
  }


}} // END namespace GMlib::GL
