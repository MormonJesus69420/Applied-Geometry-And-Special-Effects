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

//! \file gmbufferrendergl.h


#ifndef __GM_OPENCL_MEMORY_GMBUFFERRENDERGL_H__
#define __GM_OPENCL_MEMORY_GMBUFFERRENDERGL_H__


#include "../gmmemory.h"

// stl
#include <string>

namespace GMlib {

namespace CL {


  class BufferRenderGL : public Memory<cl::BufferRenderGL,OpenCL::MemoryInfo::BUFFER_RENDER_GL> {
  public:
    BufferRenderGL();
    BufferRenderGL( cl_mem_flags flags, GLuint bufobj );
    BufferRenderGL( const std::string& name );
    BufferRenderGL( const std::string& name, cl_mem_flags flags, GLuint bufobj );


  }; // END class BufferRenderGL



} // END namespace CL

} // END namespace GMlib


#endif // __GM_OPENCL_MEMORY_GMBUFFERRENDERGL_H__
