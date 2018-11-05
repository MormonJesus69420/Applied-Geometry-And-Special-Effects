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



#include "gmshader.h"

#include "gmprogram.h"


namespace GMlib { namespace GL {

  namespace Private {

    template <>
    typename std::list<ShaderInfo> GLObject<ShaderInfo>::_data = std::list<ShaderInfo>();
  }




  Shader::Shader() {}

  Shader::~Shader() { decrement(); }

  void Shader::create(GLenum type) {


    Private::ShaderInfo info;
    info.type = _create_type = type;
    createObject(info);
  }

  void Shader::create(const std::string &name, GLenum type) {

    Private::ShaderInfo info;
    info.name = name;
    info.type = _create_type = type;
    createObject(info);
  }

  bool Shader::compile() {

    GL_CHECK(::glCompileShader( getId() ));

    int param;
    GL_CHECK(::glGetShaderiv( getId(), GL_COMPILE_STATUS, &param ));

    updateCompilerLog();

    return param == GL_TRUE;
  }

  std::string Shader::getSource() const {

    const GLuint id = getId();

    int length;
    GL_CHECK(::glGetShaderiv( id, GL_SHADER_SOURCE_LENGTH, &length ));
    length++;

    int read;
    DVector<char> buff(length);
    GL_CHECK(::glGetShaderSource( id, length, &read, buff.getPtr() ));

    return std::string( buff.getPtr() );
  }

  void Shader::setSource(const std::string& source) const {

    //! \todo check whether this is necessary
    const char* src = source.c_str();
    GL_CHECK(::glShaderSource( getId(), 1, &src, 0x0 ));
  }

  GLenum Shader::getType() const {

    return getInfoIter()->type;
  }

  std::string Shader::getCompilerLog() const {

    return getInfoIter()->compiler_log;
  }

  GLuint Shader::getCurrentBoundId() const { return 0; }

  void Shader::doBind(GLuint /*id*/) const {}

  GLuint Shader::doGenerate() const {

    GLuint id;
    GL_CHECK(id = ::glCreateShader( _create_type ));
    return id;
  }

  void Shader::doDelete(GLuint id) const {

    // Detach shader from all programs it is attached to
    const std::list<GMlib::GL::Private::ProgramInfo> &programs = GMlib::GL::Program::getData();
    std::list<GMlib::GL::Private::ProgramInfo>::const_iterator p_itr;
    for( p_itr = programs.begin(); p_itr != programs.end(); ++p_itr ) {

      GLint no_as;
      GL_CHECK(::glGetProgramiv( p_itr->id, GL_ATTACHED_SHADERS, &no_as ));

      std::vector<GLuint> shader_ids(no_as);
      GL_CHECK(::glGetAttachedShaders( p_itr->id, no_as, 0x0, &shader_ids[0] ) );

      std::vector<GLuint>::const_iterator s_itr =
          std::find( shader_ids.begin(), shader_ids.end(), id );
      if( s_itr != shader_ids.end() )
        GL_CHECK(::glDetachShader( p_itr->id, *s_itr ));
    }

    // delete shader
    GL_CHECK(::glDeleteShader( id ));
  }

  void Shader::updateCompilerLog() {

    InfoIter itr = getInfoIter();
    itr->compiler_log.clear();

    //! \todo Check if this can be written directly to the compile log std::string

    int len = 0;
    int written = 0;
    char *log;

    GL_CHECK(::glGetShaderiv( getId(), GL_INFO_LOG_LENGTH, &len ));

    if( len > 0 ) {

      len = len+1;
      log = new char[len];

      GL_CHECK(::glGetShaderInfoLog( getId(), len, &written, log ));

      itr->compiler_log.append( log, len );
      delete log;
    }
  }

}} // END namespace GMlib::GL
