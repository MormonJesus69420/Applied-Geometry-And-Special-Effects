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

//! \file gmimage2dgl.cpp

#include "gmimage2dgl.h"

namespace GMlib {

namespace CL {

  Image2DGL::Image2DGL() {}

  Image2DGL::Image2DGL(cl_mem_flags flags, GLenum target, GLint miplevel, GLuint texobj)
    : Memory<cl::Image2DGL,OpenCL::MemoryInfo::IMAGE_2D_GL>(
        cl::Image2DGL( OpenCL::getInstance()->getContext(),
                       flags, target, miplevel, texobj ) ) {}

  Image2DGL::Image2DGL(const std::string &name)
    : Memory<cl::Image2DGL,OpenCL::MemoryInfo::IMAGE_2D_GL>(name) {}

  Image2DGL::Image2DGL(const std::string &name,
                       cl_mem_flags flags, GLenum target, GLint miplevel, GLuint texobj)
    : Memory<cl::Image2DGL,OpenCL::MemoryInfo::IMAGE_2D_GL>(
        name,
        cl::Image2DGL( OpenCL::getInstance()->getContext(),
                       flags, target, miplevel, texobj ) ) {}


} // END namespace CL

} // END namespace GMlib



