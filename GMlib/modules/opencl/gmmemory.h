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

//! \file gmmemory.h


#ifndef __GM_OPENCL_GMMEMORY_H__
#define __GM_OPENCL_GMMEMORY_H__

// local
#include "gmopencl.h"

// gmlib
#include <core/utils/gmutils.h>

namespace GMlib {
namespace CL {

  template <typename T,OpenCL::MemoryInfo::TYPE T_type>
  class Memory : public CLObject<T>, GMutils::DerivedFrom<T,cl::Memory> {
  public:
    Memory();
    Memory( const T& obj );
    Memory( const std::string& name );
    Memory( const std::string& name, const T& obj );

  }; // END class Memory





  template <typename T,OpenCL::MemoryInfo::TYPE T_type>
  Memory<T,T_type>::Memory() {}

  template <typename T,OpenCL::MemoryInfo::TYPE T_type>
  Memory<T,T_type>::Memory(const T& obj)
    : CLObject<T>(obj) {}

  template <typename T,OpenCL::MemoryInfo::TYPE T_type>
  Memory<T,T_type>::Memory( const std::string& name )
    : CLObject<T>(name, OpenCL::getInstance()->getMemory<T,T_type>(name)) {}

  template <typename T,OpenCL::MemoryInfo::TYPE T_type>
  Memory<T,T_type>::Memory(const std::string &name, const T& obj)
    : CLObject<T>(name, obj) {}


} // END namespace CL
} // END namespace GMlib

#endif // __GM_OPENCL_GMMEMORY_H__
