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

//! \file gmsampler.cpp

#include "gmsampler.h"

namespace GMlib {

namespace CL {

  Sampler::Sampler() {}

  Sampler::Sampler(cl_bool normalized_coords, cl_addressing_mode addressing_mode, cl_filter_mode filter_mode)
    : CLObject<cl::Sampler>( cl::Sampler(OpenCL::getInstance()->getContext(), normalized_coords, addressing_mode, filter_mode) ) {}

  Sampler::Sampler(const std::string &name)
    : CLObject<cl::Sampler>( name, OpenCL::getInstance()->getSampler(name) ) {}

  Sampler::Sampler(const std::string &name, cl_bool normalized_coords,
                   cl_addressing_mode addressing_mode, cl_filter_mode filter_mode)
    : CLObject<cl::Sampler>( name, cl::Sampler(OpenCL::getInstance()->getContext(), normalized_coords, addressing_mode, filter_mode) ) {}


} // END namespace CL

} // END namespace GMlib



