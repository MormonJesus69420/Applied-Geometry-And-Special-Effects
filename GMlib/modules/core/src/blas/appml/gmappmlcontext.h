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



/*! \file gmappmlcontext.h
 *
 *  Interface for the helper class for managing APPML OpenCL context.
 */

#ifndef __gmAPPMLCONTEXT_H__
#define __gmAPPMLCONTEXT_H__

#include <clAmdBlas.h>


namespace GMlib
{

/*!  \class AppmlContext gmappmlcontext.h gmAppmlContext
 *  \brief Helper class for managing APPML OpenCL context
 *
 *  Singleton that holds the OpenCL context used by APPML.
 */
class AppmlContext
{
public:
  static const cl_context& getContext();
  static cl_int& getErr();
  static cl_command_queue& getQueue();
  static cl_event& getEvent();

private:
  AppmlContext();
  ~AppmlContext();

  inline
  static AppmlContext& getInstance();

  cl_int err;
  cl_platform_id platform;
  cl_device_id device;
  cl_context_properties props[3];
  cl_context ctx;
  cl_command_queue queue;
  cl_event event;
  int ret;

    AppmlContext(AppmlContext const&);    // Don't Implement
    void operator=(AppmlContext const&);  // Don't implement
};

}

#endif //__gmAPPMLCONTEXT_H__
