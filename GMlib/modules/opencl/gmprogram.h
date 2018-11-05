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

//! \file gmprogram.h


#ifndef __GM_OPENCL_GMPROGRAM_H__
#define __GM_OPENCL_GMPROGRAM_H__


// local
#include "gmopencl.h"
#include "gmkernel.h"

// stl
#include <string>

namespace GMlib {

namespace CL {

  class Kernel;

  class Program : public CLObject<cl::Program> {
  public:
    Program();
    Program( const cl::Program::Sources& sources );
    Program( const std::string& name );
    Program( const std::string& name, const cl::Program::Sources& sources );

    cl_int        build( const VECTOR_CLASS<cl::Device>& devices,
                         const char* options = NULL,
                         void (CL_CALLBACK * notifyFptr)(cl_program, void *) = NULL,
                         void* data = NULL) const;

    Kernel        getKernel( const std::string& name ) const;

  }; // END class Program



} // END namespace CL

} // END namespace GMlib


#endif // __GM_OPENCL_GMPROGRAM_H__

