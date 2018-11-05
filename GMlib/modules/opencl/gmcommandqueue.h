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

//! \file gmcommandqueue.h


#ifndef __GM_OPENCL_GMCOMMANDQUEUE_H__
#define __GM_OPENCL_GMCOMMANDQUEUE_H__


// local
#include "gmopencl.h"
#include "gmkernel.h"
#include "memory/gmbuffer.h"
#include "memory/gmimage2d.h"


// gmlib
#include <core/containers/gmdvector.h>
#include <core/containers/gmdmatrix.h>
#include <core/types/gmpoint.h>

// stl
#include <string>

namespace GMlib {

namespace CL {

  class CommandQueue : public CLObject<cl::CommandQueue> {
  public:
    CommandQueue();
    CommandQueue( const cl::Device& device,
                  cl_command_queue_properties properties = 0);
    CommandQueue( const std::string& name );
    CommandQueue( const std::string& name, const cl::Device& device,
                  cl_command_queue_properties properties = 0);



    // Copy buffer operation
    cl_int    enqueueCopyBuffer( const Buffer& src, const Buffer& dst,
                                 ::size_t src_offset, ::size_t dst_offset,
                                 ::size_t size,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0) const;

    cl_int    enqueueCopyBufferRect(const Buffer &src, const Buffer &dst,
                                    const cl::size_t<3> src_origin, const cl::size_t<3> dst_origin,
                                    const cl::size_t<3> region,
                                    ::size_t src_row_pitch, ::size_t src_row_slice,
                                    ::size_t dst_row_pitch, ::size_t dst_row_slice,
                                    const std::vector<cl::Event> *events,
                                    cl::Event *event) const;

    // DVector Buffer R/W operations
    template <typename T>
    cl_int    enqueueReadBuffer( const Buffer& buffer, cl_bool blocking,
                                 DVector<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;
    template <typename T>
    cl_int    enqueueReadBuffer( const Buffer& buffer, cl_bool blocking,
                                 ::size_t offset, DVector<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;
    template <typename T>
    cl_int    enqueueWriteBuffer( const Buffer& buffer, cl_bool blocking,
                                  const DVector<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;
    template <typename T>
    cl_int    enqueueWriteBuffer( const Buffer& buffer, cl_bool blocking,
                                  ::size_t offset, const DVector<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;

    // DMatrix Buffer R/W operations
    template <typename T>
    cl_int    enqueueReadBuffer( const Buffer& buffer, cl_bool blocking,
                                 DMatrix<T>& data ) const;
    template <typename T>
    cl_int    enqueueReadBuffer( const Buffer& buffer, cl_bool blocking,
                                 ::size_t offset, DMatrix<T>& data) const;
    template <typename T>
    cl_int    enqueueWriteBuffer( const Buffer& buffer, cl_bool blocking,
                                  const DMatrix<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;
    template <typename T>
    cl_int    enqueueWriteBuffer( const Buffer& buffer, cl_bool blocking,
                                  ::size_t offset, const DMatrix<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;

    // DMatrix Image R/W operations
    template <typename T>
    cl_int    enqueueReadImage( const Image2D& image, cl_bool blocking,
                                DMatrix<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;

    template <typename T>
    cl_int    enqueueReadImage( const Image2D& image, cl_bool blocking,
                                const Vector<size_t,2>& origin,
                                DMatrix<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;
    template <typename T>
    cl_int    enqueueWriteImage( const Image2D& image, cl_bool blocking,
                                 const DMatrix<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;

    template <typename T>
    cl_int    enqueueWriteImage( const Image2D& image, cl_bool blocking,
                                 const Vector<size_t,2>& origin,
                                 const DMatrix<T>& data,
                                 const VECTOR_CLASS<cl::Event>* events = 0x0,
                                 cl::Event* event = 0x0 ) const;

    // Kernel execution
    cl_int    enqueueNDRangeKernel( const Kernel& kernel,
                                    const cl::NDRange& offset,
                                    const cl::NDRange& global,
                                    const cl::NDRange& local,
                                    const VECTOR_CLASS<cl::Event>* events = 0x0,
                                    cl::Event* event = 0x0 ) const;

  }; // END class CommandQueue















  template <typename T>
  inline
  cl_int
  CommandQueue::enqueueReadBuffer(const Buffer &buffer, cl_bool blocking,
                                  DMatrix<T>& data) const {

    return enqueueReadBuffer( buffer, blocking, 0, data );
  }

  template <typename T>
  inline
  cl_int
  CommandQueue::enqueueReadBuffer(const Buffer &buffer, cl_bool blocking,
                                  ::size_t offset, DMatrix<T>& data) const {

    DVector<T> vec(data.getDim1() * data.getDim2());

    cl_int res;
    cl::Event event;
    res = enqueueReadBuffer( buffer, blocking, offset, vec, 0x0, &event );

    event.wait();
    data = vec.getPtr();

    return res;
  }

  template <typename T>
  inline
  cl_int
  CommandQueue::enqueueReadBuffer(const Buffer &buffer, cl_bool blocking,
                                  DVector<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event) const {

    return enqueueReadBuffer( buffer, blocking, 0, data, events, event );
  }

  template <typename T>
  inline
  cl_int
  CommandQueue::enqueueReadBuffer(const Buffer &buffer, cl_bool blocking,
                                  ::size_t offset, DVector<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event) const {

    return obj().enqueueReadBuffer( buffer(), blocking, offset,
                                        data.getDim() * sizeof(T),
                                        data.getPtr(), events, event );
  }

  template <typename T>
  cl_int
  CommandQueue::enqueueReadImage( const Image2D& image, cl_bool blocking,
                                  DMatrix<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event ) const {

    return enqueueReadImage( image, blocking, Vector<size_t,2>(size_t(0)), data,
                             events, event );
  }

  template <typename T>
  cl_int
  CommandQueue::enqueueReadImage( const Image2D& image, cl_bool blocking,
                                  const Vector<size_t,2>& origin,
                                  DMatrix<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event ) const {


    DVector<T> vec(data.getDim1() * data.getDim2());

    const ::size_t T_size = sizeof(T);

    cl::size_t<3> o;
    o[0] = origin(0) * T_size;
    o[1] = origin(1) * T_size;
    o[2] = 0;

    cl::size_t<3> r;
    r[0] = data.getDim1() * T_size;
    r[1] = data.getDim2() * T_size;
    r[2] = 0;

    const ::size_t row_pitch = data.getDim1() * T_size;

    cl_int res;
    res = obj().enqueueReadImage( image(), blocking, o, r, row_pitch, 0, vec.getPtr(),
                                  events, event );
    data = vec.getPtr();

    return res;
  }

  template <typename T>
  inline
  cl_int
  CommandQueue::enqueueWriteBuffer(const Buffer &buffer, cl_bool blocking,
                                   const DMatrix<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event) const {

    return enqueueWriteBuffer(buffer, blocking, 0, data, events, event );
  }

  template <typename T>
  inline
  cl_int
  CommandQueue::enqueueWriteBuffer(const Buffer &buffer, cl_bool blocking,
                                   ::size_t offset, const DMatrix<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event) const {

    DVector<T> vec = data.toDVector();
    return enqueueWriteBuffer( buffer, blocking, offset, vec, events, event );
  }

  template <typename T>
  inline
  cl_int
  CommandQueue::enqueueWriteBuffer(const Buffer &buffer, cl_bool blocking,
                                   const DVector<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event) const {

    return enqueueWriteBuffer( buffer, blocking, 0, data, events, event );
  }

  template <typename T>
  inline
  cl_int
  CommandQueue::enqueueWriteBuffer(const Buffer &buffer, cl_bool blocking,
                                   ::size_t offset, const DVector<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event) const {

    return obj().enqueueWriteBuffer( buffer(), blocking, offset,
                                         data.getDim() * sizeof(T),
                                         data.getPtr(), events, event );
  }

  template <typename T>
  cl_int
  CommandQueue::enqueueWriteImage( const Image2D& image, cl_bool blocking,
                                   const DMatrix<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event ) const {

    return enqueueWriteImage( image, blocking, Vector<size_t,2>(size_t(0)), data, events, event );
  }

  template <typename T>
  cl_int
  CommandQueue::enqueueWriteImage( const Image2D& image, cl_bool blocking,
                                   const Vector<size_t,2>& origin,
                                   const DMatrix<T>& data,
                                     const VECTOR_CLASS<cl::Event>* events,
                                     cl::Event* event ) const {


    DVector<T> &vec = data.toDVector();

    const ::size_t T_size = sizeof(T);

    cl::size_t<3> o;
    o[0] = origin(0) * T_size;
    o[1] = origin(1) * T_size;
    o[2] = 0;

    cl::size_t<3> r;
    r[0] = data.getDim1() * T_size;
    r[1] = data.getDim2() * T_size;
    r[2] = 0;

    const ::size_t row_pitch = data.getDim1() * T_size;

    return obj().enqueueWriteImage( image(), blocking, o, r, row_pitch, 0, vec.getPtr(),
                                   events, event );
  }



} // END namespace CL

} // END namespace GMlib


#endif // __GM_OPENCL_GMCOMMANDQUEUE_H__
