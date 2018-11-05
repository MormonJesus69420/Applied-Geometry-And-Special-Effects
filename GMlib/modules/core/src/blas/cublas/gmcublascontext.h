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



/*! \file gmcublascontext.h
 *
 *  Interface for the helper class for managing CuBLAS context.
 */

#ifndef __gmCUBLASCONTEXT_H__
#define __gmCUBLASCONTEXT_H__

#include <cublas_v2.h>


namespace GMlib
{

/*!  \class CublasContext gmcublascontext.h gmCublasContext
 *  \brief Helper class for managing CuBLAS context
 *
 *  Singleton that maintains the context used by CuBLAS.
 */
class CublasContext
{
public:
  static const cublasHandle_t getHandle();

private:
  CublasContext();
  ~CublasContext();

  inline
  static CublasContext& getInstance();
  int devID;
  cudaDeviceProp deviceProp;
  cublasHandle_t handle;

    CublasContext(CublasContext const&);  // Don't Implement
    void operator=(CublasContext const&);  // Don't implement
};

}

#endif //__gmCUBLASCONTEXT_H__
