/**********************************************************************************
**
** Copyright (C) 2013 Narvik University College
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



/*! \file gmappmlcontext.cpp
 *
 *  Implementation for the helper class for managing APPML OpenCL context.
 */

#include "gmappmlcontext.h"

#include <cstdio>

namespace GMlib
{

/*!  AppmlContext&  AppmlContext::getInstance()
 *  \brief Get the singleton instance of this class
 *
 *  Creates and retains the singleton instance of this class.
 */
inline
AppmlContext& AppmlContext::getInstance()
{
  static AppmlContext instance;
  return instance;
}

/*!  AppmlContext::AppmlContext()
 *  \brief Set up the OpenCL environment
 *
 *  Constructor for the singleton,
 *  sets up the OpenCL environment used by APPML.
 */
AppmlContext::AppmlContext()
{
  platform = 0;
  device = 0;
  props[0] = CL_CONTEXT_PLATFORM;
  props[1] = 0;
  props[2] = 0;
  ctx = 0;
  queue = 0;
  event = NULL;
  ret = 0;

  /* Setup OpenCL environment. */
  err = clGetPlatformIDs(1, &platform, NULL);
  if (err != CL_SUCCESS) {
    printf( "clGetPlatformIDs() failed with %d\n", err );
    return;
  }

  err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
  if (err != CL_SUCCESS) {
    printf( "clGetDeviceIDs() failed with %d\n", err );
    return;
  }

  props[1] = (cl_context_properties)platform;
  ctx = clCreateContext(props, 1, &device, NULL, NULL, &err);
  if (err != CL_SUCCESS) {
    printf( "clCreateContext() failed with %d\n", err );
    return;
  }

  queue = clCreateCommandQueue(ctx, device, 0, &err);
  if (err != CL_SUCCESS) {
    printf( "clCreateCommandQueue() failed with %d\n", err );
    clReleaseContext(ctx);
    return;
  }

  /* Setup clAmdBlas. */
  err = clAmdBlasSetup();
  if (err != CL_SUCCESS) {
    printf("clAmdBlasSetup() failed with %d\n", err);
    clReleaseCommandQueue(queue);
    clReleaseContext(ctx);
    return;
  }
}

/*!  AppmlContext::~AppmlContext()
 *  \brief Tears down OpenCL environment
 *
 *  Destructor for the singleton,
 *  tears down the OpenCL environment used by APPML.
 */
AppmlContext::~AppmlContext()
{
  clAmdBlasTeardown();

  clReleaseCommandQueue(queue);
  clReleaseContext(ctx);
}

/*!  const cl_context& AppmlContext::getContext()
 *  \brief Get the OpenCL context
 *
 *  Gets the OpenCL context of the singleton.
 */
const cl_context& AppmlContext::getContext()
{
  return AppmlContext::getInstance().ctx;
}

/*!  cl_int& AppmlContext::getErr()
 *  \brief Get the OpenCL error
 *
 *  Gets the OpenCL error of the singleton.
 */
cl_int& AppmlContext::getErr()
{
  return AppmlContext::getInstance().err;
}

/*!  cl_command_queue& AppmlContext::getQueue()
 *  \brief Get the OpenCL queue
 *
 *  Gets the OpenCL commend queue of the singleton.
 */
cl_command_queue& AppmlContext::getQueue()
{
  return AppmlContext::getInstance().queue;
}

/*!  cl_event& AppmlContext::getEvent()
 *  \brief Get the OpenCL event
 *
 *  Gets the OpenCL event of the singleton.
 */
cl_event& AppmlContext::getEvent()
{
  return AppmlContext::getInstance().event;
}

}
