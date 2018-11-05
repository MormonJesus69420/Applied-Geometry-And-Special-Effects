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



#include "gmprogrampipeline.h"

#ifdef GL_VERSION_4_1

  #include <functional>


namespace GMlib { namespace GL {

  namespace Private {

    template <>
    typename std::list<ProgramPipelineInfo> GLObject<ProgramPipelineInfo>::_data = std::list<ProgramPipelineInfo>();
  }




  ProgramPipeline::ProgramPipeline() {}

  ProgramPipeline::~ProgramPipeline() { decrement(); }

  void ProgramPipeline::create() {

    Private::ProgramPipelineInfo info;
    createObject(info);
  }

  void ProgramPipeline::create(const std::string &name) {

    Private::ProgramPipelineInfo info;
    info.name = name;
    createObject(info);
  }

  void ProgramPipeline::useProgramStages(GLbitfield stages, const Program& program) {

    GL_CHECK(::glUseProgramStages(getId(),stages,program.getId()));
  }

  GLuint ProgramPipeline::getCurrentBoundId() const {

    GLint id;
    GL_CHECK(::glGetIntegerv( GL_CURRENT_PROGRAM, &id ));
    return id;
  }

  void ProgramPipeline::doBind(GLuint id) const {

    GL_CHECK(::glBindProgramPipeline( id ));
  }

  GLuint ProgramPipeline::doGenerate() const {

    GLuint id;
    GL_CHECK(::glGenProgramPipelines(1,&id));
    return id;
  }

  void ProgramPipeline::doDelete(GLuint id) const {

    // delete
    GL_CHECK(::glDeleteProgramPipelines(1,&id));
  }



}} // END namespace GMlib::GL


#endif // GL_VERSION_4_1

