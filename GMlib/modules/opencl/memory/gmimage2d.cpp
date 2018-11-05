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

//! \file gmimage2d.cpp

#include "gmimage2d.h"

namespace GMlib {

namespace CL {

  Image2D::Image2D() {}

  Image2D::Image2D(cl_mem_flags flags, cl::ImageFormat format, size_t width,
                   size_t height, size_t row_pitch, void *host_ptr)
    : Memory<cl::Image2D,OpenCL::MemoryInfo::IMAGE_2D>(
        cl::Image2D( OpenCL::getInstance()->getContext(), flags, format, width,
                     height, row_pitch, host_ptr ) ) {}

  Image2D::Image2D(const std::string &name)
    : Memory<cl::Image2D,OpenCL::MemoryInfo::IMAGE_2D>(name) {}

  Image2D::Image2D(const std::string &name, cl_mem_flags flags,
                   cl::ImageFormat format, size_t width, size_t height,
                   size_t row_pitch, void *host_ptr)
    : Memory<cl::Image2D,OpenCL::MemoryInfo::IMAGE_2D>(
        name,
        cl::Image2D( OpenCL::getInstance()->getContext(), flags, format, width,
                     height, row_pitch, host_ptr ) ) {}


} // END namespace CL

} // END namespace GMlib



