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


#ifndef GM_OPENGL_PROGRAMPIPELINE_H
#define GM_OPENGL_PROGRAMPIPELINE_H


#include "gmopengl.h"
#ifdef GL_VERSION_4_1

  #include "gmglobject.h"

  //local
  #include "gmtexture.h"
  #include "gmshader.h"
  #include "bufferobjects/gmuniformbufferobject.h"
  #include "gmprogram.h"


namespace GMlib {

namespace GL {



  namespace Private {
    struct ProgramPipelineInfo : public GLObjectInfo {};
  }


  class ProgramPipeline : public Private::GLObject<Private::ProgramPipelineInfo> {
  public:
    explicit ProgramPipeline();
    ~ProgramPipeline();

    void                      create();
    void                      create( const std::string& name );

    void                      useProgramStages( GLbitfield stages, const Program& program );


  private:

    /* pure-virtual functions from Object */
    GLuint                    getCurrentBoundId() const override;
    void                      doBind( GLuint id ) const override;
    GLuint                    doGenerate() const override;
    void                      doDelete(GLuint id) const override;




  }; // END class Program



} // END namespace GL

} // END namespace GMlib



#endif // GL_VERSION_4_1

#endif // GM_OPENGL_PROGRAMPIPELINE_H
