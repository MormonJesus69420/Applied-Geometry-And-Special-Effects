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

//! \file gmopencl.h

#include "gmopencl.h"


// stl
#include <iostream>

namespace GMlib {
namespace CL {


  OpenCL* OpenCL::_instance = 0x0;



  OpenCL::OpenCL(cl_device_type device_type) {

    std::vector<cl::Platform> platforms = getPlatforms();
    if( platforms.size() ) {

      cl_context_properties properties[] =
         { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
      _context = cl::Context( device_type,properties );
    }
  }

  OpenCL::~OpenCL() {}

  OpenCL*
  OpenCL::getInstance() {

    return _instance;
  }

  bool
  OpenCL::initialize( cl_device_type dev_type ) {

    if( _instance )
      return false;

    _instance = new OpenCL(dev_type);

    return getPlatforms().size() && _instance->getDevices().size();
  }

  std::vector<cl::Platform>
  OpenCL::getPlatforms() {

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    return platforms;
  }

  const cl::Context&
  OpenCL::getContext() const {

    return _context;
  }

  std::vector<cl::Device>
  OpenCL::getDevices() const {

    return _context.getInfo<CL_CONTEXT_DEVICES>();
  }


  cl::CommandQueue
  OpenCL::createCommandQueue( const std::string &name,
                              const cl::Device &device,
                              cl_command_queue_properties properties) {

    if( name.empty() )
      return cl::CommandQueue();

    if( _command_queues.find(name) != _command_queues.end() )
      return cl::CommandQueue();

    cl_int error;
    cl::CommandQueue cc(_context,device,properties,&error);
    if( error != CL_SUCCESS )
      return cl::CommandQueue();

    return _command_queues[name] = cc;
  }

  void
  OpenCL::deleteCommandQueue(const std::string &name) {

    _command_queues.erase(name);
  }

  cl::CommandQueue
  OpenCL::getCommandQueue(const std::string &name) const {

    CommandQueueMap::const_iterator itr = _command_queues.find(name);
    if( itr == _command_queues.end() )
      return cl::CommandQueue();

    return (*itr).second;
  }

  cl::Program
  OpenCL::createProgram( const std::string &name,
                         const cl::Program::Sources &sources) {

    if( name.empty() )
      return cl::Program();

    if( _programs.find(name) != _programs.end() )
      return cl::Program();

    cl_int error;
    cl::Program program(_context,sources,&error);
    if( error != CL_SUCCESS )
      return cl::Program();

    return _programs[name] =  program;
  }

  cl::Program
  OpenCL::getProgram(const std::string &name) const {

    ProgramMap::const_iterator itr = _programs.find(name);
    if( itr == _programs.end() )
      return cl::Program();

    return (*itr).second;
  }

  void
  OpenCL::deleteProgram(const std::string &name) {

    _programs.erase(name);
  }

  cl::Kernel
  OpenCL::getKernel(const std::string &program_name,
                                const std::string &kernel_name) const {

    ProgramMap::const_iterator itr = _programs.find(program_name);
    if( itr == _programs.end() )
      return cl::Kernel();

    cl_int error;
    cl::Kernel kernel( (*itr).second, kernel_name.c_str(), &error );
    if( error != CL_SUCCESS )
      return cl::Kernel();

    return kernel;
  }

  cl::Event
  OpenCL::createEvent(const std::string &name) {

    if( name.empty() )
      return cl::Event();

    if( _events.find(name) != _events.end() )
      return cl::Event();

    cl::Event event = cl::Event();
    _events[name].event = event;
    _events[name].type  = EventInfo::EVENT;

    return event;
  }

  cl::UserEvent
  OpenCL::createUserEvent(const std::string &name) {

    if( name.empty() )
      return cl::UserEvent();

    if( _events.find(name) != _events.end() )
      return cl::UserEvent();

    cl::UserEvent event = cl::UserEvent();
    _events[name].event = event;
    _events[name].type  = EventInfo::USER_EVENT;

    return event;
  }

  void
  OpenCL::deleteEvent(const std::string &name) {

    _events.erase(name);
  }

  OpenCL::EventInfo::TYPE
  OpenCL::getEventType(const std::string &name) {

    EventMap::iterator itr = _events.find(name);
    if( itr == _events.end() )
      return EventInfo::INVALID;

    return (*itr).second.type;
  }

  cl::Buffer
  OpenCL::createBuffer(const std::string &name, cl_mem_flags flags,
                            size_t size, void *host_ptr) {

    if( name.empty() )
      return cl::Buffer();

    if( _memory_objs.find(name) != _memory_objs.end() )
      return cl::Buffer();

    cl_int error;
    cl::Buffer buffer(_context,flags,size,host_ptr,&error);
    if( error != CL_SUCCESS )
      return cl::Buffer();

    _memory_objs[name].memory = buffer;
    _memory_objs[name].type   = MemoryInfo::BUFFER;

    return buffer;
  }

  cl::BufferGL
  OpenCL::createBufferGL(const std::string &name, cl_mem_flags flags,
                              GLuint bufobj) {

    if( name.empty() )
      return cl::BufferGL();

    if( _memory_objs.find(name) != _memory_objs.end() )
      return cl::BufferGL();

    cl_int error;
    cl::BufferGL buffer(_context,flags,bufobj,&error);
    if( error != CL_SUCCESS )
      return cl::BufferGL();

    _memory_objs[name].memory = buffer;
    _memory_objs[name].type   = MemoryInfo::BUFFER_GL;

    return buffer;
  }

  cl::BufferRenderGL
  OpenCL::createBufferRenderGL(const std::string &name, cl_mem_flags flags,
                                    GLuint bufobj) {

    if( name.empty() )
      return cl::BufferRenderGL();

    if( _memory_objs.find(name) != _memory_objs.end() )
      return cl::BufferRenderGL();

    cl_int error;
    cl::BufferRenderGL buffer(_context,flags,bufobj,&error);
    if( error != CL_SUCCESS )
      return cl::BufferRenderGL();

    _memory_objs[name].memory = buffer;
    _memory_objs[name].type   = MemoryInfo::BUFFER_RENDER_GL;

    return buffer;
  }

  cl::Image2D
  OpenCL::createImage2D(const std::string &name, cl_mem_flags flags,
                             cl::ImageFormat format, ::size_t width, ::size_t height,
                             size_t row_pitch, void *host_ptr) {

    if( name.empty() )
      return cl::Image2D();

    if( _memory_objs.find(name) != _memory_objs.end() )
      return cl::Image2D();

    cl_int error;
    cl::Image2D img(_context, flags,format, width,height, row_pitch, host_ptr, &error);
    if( error != CL_SUCCESS )
      return cl::Image2D();

    _memory_objs[name].memory = img;
    _memory_objs[name].type   = MemoryInfo::IMAGE_2D;

    return img;
  }

  cl::Image2DGL
  OpenCL::createImage2DGL(const std::string &name, cl_mem_flags flags,
                               GLenum target, GLint miplevel, GLuint texobj) {

    if( name.empty() )
      return cl::Image2DGL();

    if( _memory_objs.find(name) != _memory_objs.end() )
      return cl::Image2DGL();

    cl_int error;
    cl::Image2DGL img(_context, flags,target, miplevel, texobj, &error);
    if( error != CL_SUCCESS )
      return cl::Image2DGL();

    _memory_objs[name].memory = img;
    _memory_objs[name].type   = MemoryInfo::IMAGE_2D_GL;

    return img;
  }

  cl::Image3D
  OpenCL::createImage3D(const std::string &name, cl_mem_flags flags,
                             cl::ImageFormat format,
                             ::size_t width, ::size_t height, ::size_t depth,
                             ::size_t row_pitch, ::size_t slice_pitch,
                             void *host_ptr) {

    if( name.empty() )
      return cl::Image3D();

    if( _memory_objs.find(name) != _memory_objs.end() )
      return cl::Image3D();

    cl_int error;
    cl::Image3D img(_context, flags,format, width,height,depth,
                    row_pitch,slice_pitch, host_ptr, &error);
    if( error != CL_SUCCESS )
      return cl::Image3D();

    _memory_objs[name].memory = img;
    _memory_objs[name].type   = MemoryInfo::IMAGE_3D;

    return img;
  }

  cl::Image3DGL
  OpenCL::createImage3DGL(const std::string &name, cl_mem_flags flags,
                               GLenum target, GLint miplevel, GLuint texobj) {

    if( name.empty() )
      return cl::Image3DGL();

    if( _memory_objs.find(name) != _memory_objs.end() )
      return cl::Image3DGL();

    cl_int error;
    cl::Image3DGL img(_context, flags,target, miplevel, texobj, &error);
    if( error == CL_SUCCESS )
      return cl::Image3DGL();

    _memory_objs[name].memory = img;
    _memory_objs[name].type = MemoryInfo::IMAGE_3D_GL;

    return img;
  }

  void
  OpenCL::deleteMemory(const std::string &name) {

    _memory_objs.erase(name);
  }

  OpenCL::MemoryInfo::TYPE
  OpenCL::getMemoryType(const std::string &name) {

    MemoryMap::iterator itr = _memory_objs.find(name);
    if( itr == _memory_objs.end() )
      return MemoryInfo::INVALID;

    return (*itr).second.type;
  }

  cl::Sampler
  OpenCL::createSampler( const std::string &name,
                         cl_bool normalized_coords,
                         cl_addressing_mode addressing_mode,
                         cl_filter_mode filter_mode) {

    if( name.empty() )
      return cl::Sampler();

    if( _samplers.find(name) != _samplers.end() )
      return cl::Sampler();

    cl_int error;
    cl::Sampler sampler(_context, normalized_coords, addressing_mode, filter_mode, &error);
    if( error != CL_SUCCESS )
      return cl::Sampler();

    return _samplers[name] = sampler;
  }

  void
  OpenCL::deleteSampler(const std::string &name) {

    _samplers.erase(name);
  }

  cl::Sampler OpenCL::getSampler(const std::string &name) const {

    SamplerMap::const_iterator itr = _samplers.find(name);
    if( itr == _samplers.end() )
      return cl::Sampler();

    return (*itr).second;
  }





} // END namespace CL

} // END namespace GMlib
