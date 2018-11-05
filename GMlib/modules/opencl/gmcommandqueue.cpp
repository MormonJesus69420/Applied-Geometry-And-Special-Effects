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

//! \file gmcommandqueue.cpp

#include "gmcommandqueue.h"

namespace GMlib {

namespace CL {

  CommandQueue::CommandQueue() {}

  CommandQueue::CommandQueue(const cl::Device &device, cl_command_queue_properties properties)
    : CLObject<cl::CommandQueue>( cl::CommandQueue(OpenCL::getInstance()->getContext(),device,properties) ) {}

  CommandQueue::CommandQueue(const std::string &name)
    : CLObject<cl::CommandQueue>(name, OpenCL::getInstance()->getCommandQueue(name)) {}

  CommandQueue::CommandQueue(const std::string &name, const cl::Device &device,
                             cl_command_queue_properties properties)
    : CLObject<cl::CommandQueue>(name, OpenCL::getInstance()->createCommandQueue(name,device,properties)) {
  }

  cl_int
  CommandQueue::enqueueNDRangeKernel(const Kernel &kernel,
                                     const cl::NDRange &offset,
                                     const cl::NDRange &global,
                                     const cl::NDRange &local,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event) const {

    return obj().enqueueNDRangeKernel( kernel(), offset, global, local,
                                       events, event );
  }

  cl_int
  CommandQueue::enqueueCopyBuffer(const Buffer &src, const Buffer &dst,
                                  size_t src_offset, size_t dst_offset,
                                  size_t size,
                                  const std::vector<cl::Event> *events,
                                  cl::Event *event) const {

    return obj().enqueueCopyBuffer(src(), dst(), src_offset, dst_offset, size,
                                   events, event );
  }

  cl_int
  CommandQueue::enqueueCopyBufferRect(const Buffer &src, const Buffer &dst,
                                      const cl::size_t<3> src_origin,
                                      const cl::size_t<3> dst_origin,
                                      const cl::size_t<3> region,
                                      size_t src_row_pitch, size_t src_row_slice,
                                      size_t dst_row_pitch, size_t dst_row_slice,
                                      const std::vector<cl::Event> *events,
                                      cl::Event *event) const {

    return obj().enqueueCopyBufferRect( src(), dst(), src_origin, dst_origin, region,
                                        src_row_pitch, src_row_slice,
                                        dst_row_pitch, dst_row_slice,
                                        events, event );
  }

} // END namespace CL

} // END namespace GMlib



