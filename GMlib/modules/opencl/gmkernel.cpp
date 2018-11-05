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

//! \file gmkernel.cpp

#include "gmkernel.h"

namespace GMlib {

namespace CL {

  Kernel::Kernel() {}

  Kernel::Kernel(const Program &program, const std::string &kernel_name)
    : CLObject<cl::Kernel>( cl::Kernel( program(), kernel_name.c_str() ) ) {}

  Kernel::Kernel(const std::string& prog_name, const std::string& kernel_name)
    : CLObject<cl::Kernel>(kernel_name, OpenCL::getInstance()->getKernel(prog_name,kernel_name)) {}

} // END namespace CL

} // END namespace GMlib



