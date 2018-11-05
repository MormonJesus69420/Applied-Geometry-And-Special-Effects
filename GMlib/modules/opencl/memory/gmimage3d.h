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

//! \file gmimage3d.h


#ifndef __GM_OPENCL_MEMORY_GMIMAGE3D_H__
#define __GM_OPENCL_MEMORY_GMIMAGE3D_H__


#include "../gmmemory.h"

// stl
#include <string>

namespace GMlib {

namespace CL {


  class Image3D : public Memory<cl::Image3D,OpenCL::MemoryInfo::IMAGE_3D> {
  public:
    Image3D();
    Image3D( cl_mem_flags flags, cl::ImageFormat format, ::size_t width,
             ::size_t height, ::size_t depth, ::size_t row_pitch = 0,
             ::size_t slice_pitch = 0, void* host_ptr = 0x0 );
    Image3D( const std::string& name );
    Image3D( const std::string& name,
             cl_mem_flags flags, cl::ImageFormat format, ::size_t width,
             ::size_t height, ::size_t depth, ::size_t row_pitch = 0,
             ::size_t slice_pitch = 0, void* host_ptr = 0x0 );


  }; // END class Image3D



} // END namespace CL

} // END namespace GMlib


#endif // __GM_OPENCL_MEMORY_GMIMAGE3D_H__
