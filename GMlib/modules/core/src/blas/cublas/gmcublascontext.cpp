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



/*! \file gmcublascontext.cpp
 *
 *  Implementation for the helper class for managing CuBLAS context.
 */

#include "gmcublascontext.h"

#include <cuda_runtime.h>

namespace GMlib
{

/*!  CublasContext& CublasContext::getInstance()
 *  \brief Get the singleton instance of this class
 *
 *  Creates and retains the singleton instance of this class.
 */
inline
CublasContext& CublasContext::getInstance()
{
  static CublasContext instance;
  return instance;
}

/*!  CublasContext::CublasContext()
 *  \brief Set up the CuBLAS environment
 *
 *  Constructor for the singleton,
 *  sets up the CuBLAS environment to be used.
 */
CublasContext::CublasContext()
{
  cudaGetDevice(&devID);
  cudaGetDeviceProperties(&deviceProp, devID);
  cublasCreate(&handle);
}

/*!  CublasContext::~CublasContext()
 *  \brief Tears down CuBLAS environment
 *
 *  Destructor for the singleton,
 *  tears down the CuBLAS environment.
 */
CublasContext::~CublasContext()
{
  cublasDestroy(handle);
  cudaDeviceReset();
}

/*!  const cublasHandle_t CublasContext::getHandle()
 *  \brief Gets the CuBLAS handle
 *
 *  Returns the handle to use with CuBLAS.
 */
const cublasHandle_t CublasContext::getHandle()
{
  return getInstance().handle;
}

}
