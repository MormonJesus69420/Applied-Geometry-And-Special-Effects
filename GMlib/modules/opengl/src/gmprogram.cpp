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



#include "gmprogram.h"

#include <functional>

namespace GMlib { namespace GL {

  namespace Private {

    template <>
    typename std::list<ProgramInfo> GLObject<ProgramInfo>::_data = std::list<ProgramInfo>();
  }




  Program::Program() {}

  Program::~Program() { decrement(); }

  void Program::create() {

    Private::ProgramInfo info;
    createObject(info);
  }

  void Program::create(const std::string &name) {

    Private::ProgramInfo info;
    info.name = name;
    createObject(info);
  }

  bool Program::link() {

    // Link program
    GL_CHECK(::glLinkProgram( getId() ));

    // get link status
    int param;
    GL_CHECK(::glGetProgramiv( getId(), GL_LINK_STATUS, &param ));

    updateLinkerLog();

    return param == GL_TRUE;
  }

  const std::string&Program::getLinkerLog() const {

    return getInfoIter()->linker_log;
  }

  GLuint Program::getCurrentBoundId() const {

    GLint id;
    GL_CHECK(::glGetIntegerv( GL_CURRENT_PROGRAM, &id ));
    return id;
  }

  void Program::doBind(GLuint id) const {

    GL_CHECK(::glUseProgram( id ));
  }

  GLuint Program::doGenerate() const {

    GLuint id;
    GL_CHECK(id = ::glCreateProgram());
    return id;
  }

  void Program::doDelete(GLuint id) const {

    // Detach shaders
    std::vector<GLuint> as = getAttachedShaders();
    std::for_each( as.begin(), as.end(), std::bind1st( std::mem_fun(&Program::detachShaderInternal), this ) );

    // delete
    GL_CHECK(::glDeleteProgram( id ));
  }

  void Program::updateLinkerLog() {

    InfoIter itr = getInfoIter();
    itr->linker_log.clear();

    //! \todo Check if this can be written directly to the compile log std::string

    int len = 0;
    int written = 0;
    char *log;

    GL_CHECK(::glGetProgramiv( getId(), GL_INFO_LOG_LENGTH, &len ));

    if( len > 0 ) {

      len = len+1;
      log = new char[len];

      GL_CHECK(::glGetProgramInfoLog( getId(), len, &written, log ));

      itr->linker_log.append( log, len );
      delete log;
    }
  }

  std::vector<GLuint> Program::getAttachedShaders() const {

    if(!isValid())
      return std::vector<GLuint>();

    GLint no_as;
    GL_CHECK(::glGetProgramiv( getId(), GL_ATTACHED_SHADERS, &no_as ));

    std::vector<GLuint> shader_ids(no_as);
    GL_CHECK(::glGetAttachedShaders( getId(), no_as, 0x0, &shader_ids[0] ) );

    return shader_ids;
  }

  void Program::attachShader(const Shader& shader) const {

    attachShaderInternal( shader.getId() );
  }

  void Program::detachShader(const Shader& shader) const {

    detachShaderInternal( shader.getId() );
  }

  void Program::attachShaderInternal(GLuint id) const {

    GL_CHECK(::glAttachShader( getId(), id ));
  }

  void Program::detachShaderInternal(GLuint id) const {

    GL_CHECK(::glDetachShader( getId(), id ));
  }

  void Program::disableAttributeArray( const std::string& name ) const {

    disableAttributeArray( getAttributeLocation(name) );
  }

  void Program::disableAttributeArray( const AttributeLocation& loc ) const {

    GL_CHECK(::glDisableVertexAttribArray( loc() ));
  }

  void Program::enableAttributeArray( const std::string& name ) const {

    enableAttributeArray( getAttributeLocation( name ) );
  }

  void Program::enableAttributeArray( const AttributeLocation& loc ) const {

    GL_CHECK(::glEnableVertexAttribArray( loc() ));
  }

  AttributeLocation Program::getAttributeLocation(const std::string& name) const {

    GL::AttributeLocation loc;
    GL_CHECK(loc = ::glGetAttribLocation( getId(), name.c_str() ));
    return loc;
  }

  UniformBlockIndex Program::getUniformBlockIndex(const std::string &name) const {

    GL::UniformBlockIndex block_index;
    GL_CHECK(block_index = ::glGetUniformBlockIndex( getId(), name.c_str() ));
    return block_index;
  }

  UniformLocation Program::getUniformLocation(const std::string& name) const {

    GL::UniformLocation uniform_loc;
    GL_CHECK(uniform_loc = ::glGetUniformLocation( getId(), name.c_str() ));
    return uniform_loc;
  }

  void Program::uniform(const std::string &name, bool b) const {

    GL_CHECK(::glUniform1i( getUniformLocation(name)(), b ));
  }

  void Program::uniform(const std::string& name, const Color &c) const {

    GL_CHECK(::glUniform4f(
        getUniformLocation( name )(),
        c.getRedC(), c.getGreenC(), c.getBlueC(), c.getAlphaC()
        ));
  }

  void Program::uniform(const std::string& name, const Matrix<float,3,3>& matrix, bool transpose ) const {

    GL_CHECK(::glUniformMatrix3fv(
        getUniformLocation( name )(),
        1, transpose, matrix.getPtr()
        ));
  }

  void Program::uniform(const std::string& name, const Matrix<float,4,4>& matrix, bool transpose ) const {

    GL_CHECK(::glUniformMatrix4fv(
        getUniformLocation( name )(),
        1, transpose, matrix.getPtr()
        ));
  }

  void Program::uniform(const std::string &name, const Point<int, 2> &p) const {

    GL_CHECK(::glUniform2iv( getUniformLocation(name)(), 1, p.getPtr() ));
  }

  void Program::uniform(const std::string &name, const Point<float, 2> &p) const {

    GL_CHECK(::glUniform2fv( getUniformLocation(name)(), 1, p.getPtr() ));
  }

  void Program::uniform(const std::string &name, const Point<float, 3> &p) const {

    GL_CHECK(::glUniform3fv( getUniformLocation(name)(), 1, p.getPtr() ));
  }

  void Program::uniform(const std::string &name, const Point<float, 4> &p) const {

    GL_CHECK(::glUniform4fv( getUniformLocation(name)(), 1, p.getPtr() ));
  }

  void Program::uniform(const std::string &name, const Texture& tex, GLenum tex_unit, GLuint tex_nr ) const {

    GL_CHECK(::glActiveTexture( tex_unit ));
    GL_CHECK(::glBindTexture( tex.getTarget(), tex.getId() ));
    GL_CHECK(::glUniform1i( getUniformLocation( name )(), tex_nr ));
  }

  void Program::uniform(const std::string &name, float f) const {

    GL_CHECK(::glUniform1f( getUniformLocation( name )(), f ));
  }

  void Program::uniform( const std::string& name, int i ) const {

    GL_CHECK(::glUniform1i( getUniformLocation( name )(), i ));
  }

  void Program::bindBufferBase(const std::string &name, const UniformBufferObject &ubo, GLuint binding_point) const {

    GL_CHECK(::glUniformBlockBinding( getId(), getUniformBlockIndex( name)(), binding_point ));
    GL_CHECK(::glBindBufferBase( GL_UNIFORM_BUFFER, binding_point, ubo.getId() ));
  }

}} // END namespace GMlib::GL




#ifdef GL_VERSION_4_1

namespace GMlib { namespace GL {

  void Program::programUniform(const std::string &name, bool b) const {

    GL_CHECK(::glProgramUniform1i( getId(), getUniformLocation(name)(), b ));
  }

  void Program::programUniform(const std::string& name, unsigned int ui) const
  {
    GL_CHECK(::glProgramUniform1ui( getId(), getUniformLocation(name)(), ui ));
  }

  void Program::programUniform(const std::string &name, float f) const {

    GL_CHECK(::glProgramUniform1f( getId(), getUniformLocation( name )(), f ));
  }

  void Program::programUniform( const std::string& name, int i ) const {

    GL_CHECK(::glProgramUniform1i( getId(), getUniformLocation( name )(), i ));
  }


  void Program::programUniform(const std::string& name, const GMlib::Color& c) const {
    GL_CHECK(::glProgramUniform4f(
        getId(),
        getUniformLocation( name )(),
        c.getRedC(), c.getGreenC(), c.getBlueC(), c.getAlphaC()
        ));
  }

  void Program::programUniform(const std::string &name, const Point<int, 2> &p) const {

    GL_CHECK(::glProgramUniform2iv( getId(), getUniformLocation(name)(), 1, p.getPtr() ));
  }

  void Program::programUniform(const std::string &name, const Point<int, 3> &p) const {

    GL_CHECK(::glProgramUniform3iv( getId(), getUniformLocation(name)(), 1, p.getPtr() ));
  }


  void Program::programUniform(const std::string &name, const Point<float, 2> &p) const {

    GL_CHECK(::glProgramUniform2fv( getId(), getUniformLocation(name)(), 1, p.getPtr() ));
  }

  void Program::programUniform(const std::string &name, const Point<float, 3> &p) const {

    GL_CHECK(::glProgramUniform3fv( getId(), getUniformLocation(name)(), 1, p.getPtr() ));
  }

  void Program::programUniform(const std::string &name, const Point<float, 4> &p) const {

    GL_CHECK(::glProgramUniform4fv( getId(), getUniformLocation(name)(), 1, p.getPtr() ));
  }

  void Program::programUniform(const std::string &name, const Texture& tex, GLenum tex_unit, GLuint tex_nr ) const {

    GL_CHECK(::glActiveTexture( tex_unit ));
    GL_CHECK(::glBindTexture( tex.getTarget(), tex.getId() ));
    GL_CHECK(::glProgramUniform1i( getId(), getUniformLocation( name )(), tex_nr ));
  }


  void Program::programUniformMatrix(const std::string& name, const Matrix<float,3,3>& matrix, bool transpose ) const {

    GL_CHECK(::glProgramUniformMatrix3fv(
        getId(),
        getUniformLocation( name )(),
        1, transpose, matrix.getPtr()
        ));
  }

  void Program::programUniformMatrix(const std::string& name, const Matrix<float,4,4>& matrix, bool transpose ) const {

    GL_CHECK(::glProgramUniformMatrix4fv(
        getId(),
        getUniformLocation( name )(),
        1, transpose, matrix.getPtr()
        ));
  }

}} // END namespace GMlib::GL

#endif // GL_VERSION_4_1




