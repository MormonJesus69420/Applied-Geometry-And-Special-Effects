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

//! \file gmbuffergl.cpp

#include "gmbuffergl.h"

namespace GMlib {

namespace CL {

  BufferGL::BufferGL() {}

  BufferGL::BufferGL(cl_mem_flags flags, GLuint bufobj)
    : Memory<cl::BufferGL,OpenCL::MemoryInfo::BUFFER_GL>(
        cl::BufferGL( OpenCL::getInstance()->getContext(),
                      flags, bufobj ) ) {}

  BufferGL::BufferGL(const std::string &name)
    : Memory<cl::BufferGL,OpenCL::MemoryInfo::BUFFER_GL>(name) {}

  BufferGL::BufferGL(const std::string &name, cl_mem_flags flags, GLuint bufobj)
    : Memory<cl::BufferGL,OpenCL::MemoryInfo::BUFFER_GL>(
        name,
        cl::BufferGL( OpenCL::getInstance()->getContext(),
                      flags, bufobj ) ) {}

} // END namespace CL

} // END namespace GMlib



