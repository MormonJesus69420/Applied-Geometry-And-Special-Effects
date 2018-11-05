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

//! \file gmprogram.cpp

#include "gmprogram.h"

namespace GMlib {

namespace CL {

  Program::Program() {}

  Program::Program(const cl::Program::Sources &sources)
    : CLObject<cl::Program>( cl::Program(OpenCL::getInstance()->getContext(),sources) ) {}

  Program::Program(const std::string &name)
    : CLObject<cl::Program>(name, OpenCL::getInstance()->getProgram(name)) {}

  Program::Program(const std::string &name,const cl::Program::Sources &sources)
    : CLObject<cl::Program>(name, OpenCL::getInstance()->createProgram(name,sources)) {}

  cl_int Program::build(const std::vector<cl::Device> &devices, const char *options,
                        void (*notifyFptr)(cl_program, void *), void *data) const {

    return obj().build( devices, options, notifyFptr, data );
  }

  Kernel Program::getKernel(const std::string &name) const {

    if( isManaged() )
      return Kernel( getName(), name );
    else
      return Kernel( *this, name );
  }

} // END namespace CL

} // END namespace GMlib



