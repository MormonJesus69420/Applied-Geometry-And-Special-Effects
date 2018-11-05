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

//! \file gmsampler.h


#ifndef __GM_OPENCL_GMSAMPLER_H__
#define __GM_OPENCL_GMSAMPLER_H__


// local
#include "gmopencl.h"

// stl
#include <string>

namespace GMlib {

namespace CL {


  class Sampler : public CLObject<cl::Sampler> {
  public:
    Sampler();
    Sampler( cl_bool normalized_coords, cl_addressing_mode addressing_mode,
             cl_filter_mode filter_mode );
    Sampler( const std::string& name );
    Sampler( const std::string& name, cl_bool normalized_coords,
             cl_addressing_mode addressing_mode, cl_filter_mode filter_mode);

  }; // END class Sampler



} // END namespace CL

} // END namespace GMlib


#endif // __GM_OPENCL_GMSAMPLER_H__
