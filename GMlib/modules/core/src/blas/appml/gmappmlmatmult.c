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



/*! \file gmappmlmatmult.c
 *
 *  Implementation of APPML-specific matrix multiplication operators.
 */

#include "gmappmlcontext.h"

#include <clAmdBlas.h>

namespace GMlib
{

/*!  const DMatrix<float>&  operator*(const DMatrix<float>& m, const DMatrix<float>& b)
 *  \brief Multiply two matrices
 *
 *  APPML-specific implementation of the multiplication of two matrices
 *  of data type float.
 */
inline
const DMatrix<float>&  operator*(const DMatrix<float>& m, const DMatrix<float>& b)
{
  static DMatrix<float> r;
  r.setDim(m.getDim1(), b.getDim2());

  static Array<float> work;
  work.setSize(m.getDim1() * b.getDim2());

  cl_mem d_A = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, m.getDim1() * m.getDim2() * sizeof(float), NULL, &AppmlContext::getErr());
  cl_mem d_B = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, b.getDim1() * b.getDim2() * sizeof(float), NULL, &AppmlContext::getErr());
  cl_mem d_C = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_WRITE, work.getSize() * sizeof(float), NULL, &AppmlContext::getErr());

  for(int i=0; i<m.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_A, CL_TRUE, i*m.getDim2()*sizeof(float), m(i).getDim()*sizeof(float), &m(i)(0), 0, NULL, NULL);
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_B, CL_TRUE, i*b.getDim2()*sizeof(float), b(i).getDim()*sizeof(float), &b(i)(0), 0, NULL, NULL);
  }

  AppmlContext::getErr() = clAmdBlasSgemm(clAmdBlasRowMajor, clAmdBlasNoTrans, clAmdBlasNoTrans, m.getDim1(), b.getDim2(), m.getDim2(), 1.0f,
    d_A, m.getDim2(), d_B, b.getDim2(), 0.0f, d_C, m.getDim2(), 1, &AppmlContext::getQueue(), 0, NULL, &AppmlContext::getEvent());

  if(AppmlContext::getErr() == CL_SUCCESS)
  {
    AppmlContext::getErr() = clWaitForEvents(1, &AppmlContext::getEvent());
    AppmlContext::getErr() = clEnqueueReadBuffer(AppmlContext::getQueue(), d_C, CL_TRUE, 0, work.getSize()*sizeof(float), work.getPtr(), 0, NULL, NULL);
  }

  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_A);


  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(float));

  return r;
}

/*!  const DMatrix<double>&  operator*(const DMatrix<double>& m, const DMatrix<double>& b)
 *  \brief Multiply two matrices
 *
 *  APPML-specific implementation of the multiplication of two matrices
 *  of data type double.
 */
inline
const DMatrix<double>&  operator*(const DMatrix<double>& m, const DMatrix<double>& b)
{
  static DMatrix<double> r;
  r.setDim(m.getDim1(), b.getDim2());

  static Array<double> work;
  work.setSize(m.getDim1() * b.getDim2());

  cl_mem d_A = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, m.getDim1() * m.getDim2() * sizeof(double), NULL, &AppmlContext::getErr());
  cl_mem d_B = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, b.getDim1() * b.getDim2() * sizeof(double), NULL, &AppmlContext::getErr());
  cl_mem d_C = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_WRITE, work.getSize() * sizeof(double), NULL, &AppmlContext::getErr());

  for(int i=0; i<m.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_A, CL_TRUE, i*m.getDim2()*sizeof(double), m(i).getDim()*sizeof(double), &m(i)(0), 0, NULL, NULL);
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_B, CL_TRUE, i*b.getDim2()*sizeof(double), b(i).getDim()*sizeof(double), &b(i)(0), 0, NULL, NULL);
  }

  AppmlContext::getErr() = clAmdBlasDgemm(clAmdBlasRowMajor, clAmdBlasNoTrans, clAmdBlasNoTrans, m.getDim1(), b.getDim2(), m.getDim2(), 1.0,
    d_A, m.getDim2(), d_B, b.getDim2(), 0.0, d_C, m.getDim2(), 1, &AppmlContext::getQueue(), 0, NULL, &AppmlContext::getEvent());

  if(AppmlContext::getErr() == CL_SUCCESS)
  {
    AppmlContext::getErr() = clWaitForEvents(1, &AppmlContext::getEvent());
    AppmlContext::getErr() = clEnqueueReadBuffer(AppmlContext::getQueue(), d_C, CL_TRUE, 0, work.getSize()*sizeof(float), work.getPtr(), 0, NULL, NULL);
  }

  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_A);

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(double));

  return r;
}

/*!  const DMatrix<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DMatrix<std::complex<float> >& b)
 *  \brief Multiply two matrices
 *
 *  APPML-specific implementation of the multiplication of two matrices
 *  of data type std::complex<float>.
 */
inline
const DMatrix<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DMatrix<std::complex<float> >& b)
{
  static DMatrix<std::complex<float> > r;
  int work_size = m.getDim1() * b.getDim2();

  r.setDim(m.getDim1(), b.getDim2());
  std::complex<float>* work = new std::complex<float>[work_size];

  cl_mem d_A = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, m.getDim1() * m.getDim2() * sizeof(std::complex<float>), NULL, &AppmlContext::getErr());
  cl_mem d_B = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, b.getDim1() * b.getDim2() * sizeof(std::complex<float>), NULL, &AppmlContext::getErr());
  cl_mem d_C = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_WRITE, work_size * sizeof(std::complex<float>), NULL, &AppmlContext::getErr());

  for(int i=0; i<m.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_A, CL_TRUE, i*m.getDim2()*sizeof(std::complex<float>), m(i).getDim()*sizeof(std::complex<float>), &m(i)(0), 0, NULL, NULL);
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_B, CL_TRUE, i*b.getDim2()*sizeof(std::complex<float>), b(i).getDim()*sizeof(std::complex<float>), &b(i)(0), 0, NULL, NULL);
  }

  static const FloatComplex alpha = floatComplex(1.0f, 0.0f);
  static const FloatComplex beta = floatComplex(0.0f, 0.0f);

  AppmlContext::getErr() = clAmdBlasCgemm(clAmdBlasRowMajor, clAmdBlasNoTrans, clAmdBlasNoTrans, m.getDim1(), b.getDim2(), m.getDim2(), alpha,
    d_A, m.getDim2(), d_B, b.getDim2(), beta, d_C, m.getDim2(), 1, &AppmlContext::getQueue(), 0, NULL, &AppmlContext::getEvent());

  if(AppmlContext::getErr() == CL_SUCCESS)
  {
    AppmlContext::getErr() = clWaitForEvents(1, &AppmlContext::getEvent());
    AppmlContext::getErr() = clEnqueueReadBuffer(AppmlContext::getQueue(), d_C, CL_TRUE, 0, work_size*sizeof(std::complex<float>), work, 0, NULL, NULL);
  }

  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_A);

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(std::complex<float>));

  delete[] work;

  return r;
}

/*!  const DMatrix<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DMatrix<std::complex<double> >& b)
 *  \brief Multiply two matrices
 *
 *  APPML-specific implementation of the multiplication of two matrices
 *  of data type std::complex<double>.
 */
inline
const DMatrix<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DMatrix<std::complex<double> >& b)
{
  static DMatrix<std::complex<double> > r;
  int work_size = m.getDim1() * b.getDim2();

  r.setDim(m.getDim1(), b.getDim2());
  std::complex<double>* work = new std::complex<double>[work_size];

  cl_mem d_A = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, m.getDim1() * m.getDim2() * sizeof(std::complex<double>), NULL, &AppmlContext::getErr());
  cl_mem d_B = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, b.getDim1() * b.getDim2() * sizeof(std::complex<double>), NULL, &AppmlContext::getErr());
  cl_mem d_C = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_WRITE, work_size * sizeof(std::complex<double>), NULL, &AppmlContext::getErr());

  for(int i=0; i<m.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_A, CL_TRUE, i*m.getDim2()*sizeof(std::complex<double>), m(i).getDim()*sizeof(std::complex<double>), &m(i)(0), 0, NULL, NULL);
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_B, CL_TRUE, i*b.getDim2()*sizeof(std::complex<double>), b(i).getDim()*sizeof(std::complex<double>), &b(i)(0), 0, NULL, NULL);
  }

  static const DoubleComplex alpha = doubleComplex(1.0, 0.0);
  static const DoubleComplex beta = doubleComplex(0.0, 0.0);

  AppmlContext::getErr() = clAmdBlasZgemm(clAmdBlasRowMajor, clAmdBlasNoTrans, clAmdBlasNoTrans, m.getDim1(), b.getDim2(), m.getDim2(), alpha,
    d_A, m.getDim2(), d_B, b.getDim2(), beta, d_C, m.getDim2(), 1, &AppmlContext::getQueue(), 0, NULL, &AppmlContext::getEvent());

  if(AppmlContext::getErr() == CL_SUCCESS)
  {
    AppmlContext::getErr() = clWaitForEvents(1, &AppmlContext::getEvent());
    AppmlContext::getErr() = clEnqueueReadBuffer(AppmlContext::getQueue(), d_C, CL_TRUE, 0, work_size*sizeof(std::complex<double>), work, 0, NULL, NULL);
  }

  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_A);

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(std::complex<double>));

  delete[] work;

  return r;
}

/*!  const DVector<float>&  operator*(const DMatrix<float>& m, const DVector<float>& b)
 *  \brief Multiply a matrix with a vector
 *
 *  APPML-specific implementation of the multiplication of a matrix
 *  with a vector of data type float.
 */
inline
const DVector<float>&  operator*(const DMatrix<float>& m, const DVector<float>& b)
{
  static DVector<float> r;
  r.setDim(m.getDim1());

  cl_mem d_A = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, m.getDim1() * m.getDim2() * sizeof(float), NULL, &AppmlContext::getErr());
  cl_mem d_B = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, b.getDim() * sizeof(float), NULL, &AppmlContext::getErr());
  cl_mem d_C = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_WRITE, r.getDim() * sizeof(float), NULL, &AppmlContext::getErr());

  for(int i=0; i<m.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_A, CL_TRUE, i*m.getDim2()*sizeof(float), m(i).getDim()*sizeof(float), &m(i)(0), 0, NULL, NULL);
  }

  AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_B, CL_TRUE, 0, b.getDim()*sizeof(float), &b(0), 0, NULL, NULL);

  AppmlContext::getErr() = clAmdBlasSgemv(clAmdBlasRowMajor, clAmdBlasNoTrans, m.getDim2(), b.getDim(), 1.0f, d_A, m.getDim1(), d_B, 0,
    1, 0.0f, d_C, 0, 1, 1, &AppmlContext::getQueue(), 0, NULL, &AppmlContext::getEvent());

  if(AppmlContext::getErr() == CL_SUCCESS)
  {
    AppmlContext::getErr() = clWaitForEvents(1, &AppmlContext::getEvent());
    AppmlContext::getErr() = clEnqueueReadBuffer(AppmlContext::getQueue(), d_C, CL_TRUE, 0, r.getDim()*sizeof(float), &r[0], 0, NULL, NULL);
  }

  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_A);

  return r;
}

/*!  const DVector<double>&  operator*(const DMatrix<double>& m, const DVector<double>& b)
 *  \brief Multiply a matrix with a vector
 *
 *  APPML-specific implementation of the multiplication of a matrix
 *  with a vector of data type double.
 */
inline
const DVector<double>&  operator*(const DMatrix<double>& m, const DVector<double>& b)
{
  static DVector<double> r;
  r.setDim(m.getDim1());

  cl_mem d_A = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, m.getDim1() * m.getDim2() * sizeof(double), NULL, &AppmlContext::getErr());
  cl_mem d_B = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, b.getDim() * sizeof(double), NULL, &AppmlContext::getErr());
  cl_mem d_C = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_WRITE, r.getDim() * sizeof(double), NULL, &AppmlContext::getErr());

  for(int i=0; i<m.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_A, CL_TRUE, i*m.getDim2()*sizeof(double), m(i).getDim()*sizeof(double), &m(i)(0), 0, NULL, NULL);
  }

  AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_B, CL_TRUE, 0, b.getDim()*sizeof(double), &b(0), 0, NULL, NULL);

  AppmlContext::getErr() = clAmdBlasDgemv(clAmdBlasRowMajor, clAmdBlasNoTrans, m.getDim2(), b.getDim(), 1.0, d_A, m.getDim1(), d_B, 0,
    1, 0.0, d_C, 0, 1, 1, &AppmlContext::getQueue(), 0, NULL, &AppmlContext::getEvent());

  if(AppmlContext::getErr() == CL_SUCCESS)
  {
    AppmlContext::getErr() = clWaitForEvents(1, &AppmlContext::getEvent());
    AppmlContext::getErr() = clEnqueueReadBuffer(AppmlContext::getQueue(), d_C, CL_TRUE, 0, r.getDim()*sizeof(double), &r[0], 0, NULL, NULL);
  }

  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_A);

  return r;
}

/*!  const DVector<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DVector<std::complex<float> >& b)
 *  \brief Multiply a matrix with a vector
 *
 *  APPML-specific implementation of the multiplication of a matrix
 *  with a vector of data type std::complex<float>.
 */
inline
const DVector<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DVector<std::complex<float> >& b)
{
  static DVector<std::complex<float> > r;
  r.setDim(m.getDim1());

  cl_mem d_A = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, m.getDim1() * m.getDim2() * sizeof(std::complex<float>), NULL, &AppmlContext::getErr());
  cl_mem d_B = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, b.getDim() * sizeof(std::complex<float>), NULL, &AppmlContext::getErr());
  cl_mem d_C = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_WRITE, r.getDim() * sizeof(std::complex<float>), NULL, &AppmlContext::getErr());

  for(int i=0; i<m.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_A, CL_TRUE, i*m.getDim2()*sizeof(std::complex<float>), m(i).getDim()*sizeof(std::complex<float>), &m(i)(0), 0, NULL, NULL);
  }

  AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_B, CL_TRUE, 0, b.getDim()*sizeof(std::complex<float>), &b(0), 0, NULL, NULL);

  static const FloatComplex alpha = floatComplex(1.0f, 0.0f);
  static const FloatComplex beta = floatComplex(0.0f, 0.0f);

  AppmlContext::getErr() = clAmdBlasCgemv(clAmdBlasRowMajor, clAmdBlasNoTrans, m.getDim2(), b.getDim(), alpha, d_A, m.getDim1(), d_B, 0,
    1, beta, d_C, 0, 1, 1, &AppmlContext::getQueue(), 0, NULL, &AppmlContext::getEvent());

  if(AppmlContext::getErr() == CL_SUCCESS)
  {
    AppmlContext::getErr() = clWaitForEvents(1, &AppmlContext::getEvent());
    AppmlContext::getErr() = clEnqueueReadBuffer(AppmlContext::getQueue(), d_C, CL_TRUE, 0, r.getDim()*sizeof(std::complex<float>), &r[0], 0, NULL, NULL);
  }

  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_A);

  return r;
}

/*!  const DVector<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DVector<std::complex<double> >& b)
 *  \brief Multiply a matrix with a vector
 *
 *  APPML-specific implementation of the multiplication of a matrix
 *  with a vector of data type std::complex<double>.
 */
inline
const DVector<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DVector<std::complex<double> >& b)
{
  static DVector<std::complex<double> > r;
  r.setDim(m.getDim1());

  cl_mem d_A = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, m.getDim1() * m.getDim2() * sizeof(std::complex<double>), NULL, &AppmlContext::getErr());
  cl_mem d_B = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_ONLY, b.getDim() * sizeof(std::complex<double>), NULL, &AppmlContext::getErr());
  cl_mem d_C = clCreateBuffer(AppmlContext::getContext(), CL_MEM_READ_WRITE, r.getDim() * sizeof(std::complex<double>), NULL, &AppmlContext::getErr());

  for(int i=0; i<m.getDim1(); i++)
  {
    AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_A, CL_TRUE, i*m.getDim2()*sizeof(std::complex<double>), m(i).getDim()*sizeof(std::complex<double>), &m(i)(0), 0, NULL, NULL);
  }

  AppmlContext::getErr() = clEnqueueWriteBuffer(AppmlContext::getQueue(), d_B, CL_TRUE, 0, b.getDim()*sizeof(std::complex<double>), &b(0), 0, NULL, NULL);

  static const DoubleComplex alpha = doubleComplex(1.0, 0.0);
  static const DoubleComplex beta = doubleComplex(0.0, 0.0);

  AppmlContext::getErr() = clAmdBlasZgemv(clAmdBlasRowMajor, clAmdBlasNoTrans, m.getDim2(), b.getDim(), alpha, d_A, m.getDim1(), d_B, 0,
    1, beta, d_C, 0, 1, 1, &AppmlContext::getQueue(), 0, NULL, &AppmlContext::getEvent());

  if(AppmlContext::getErr() == CL_SUCCESS)
  {
    AppmlContext::getErr() = clWaitForEvents(1, &AppmlContext::getEvent());
    AppmlContext::getErr() = clEnqueueReadBuffer(AppmlContext::getQueue(), d_C, CL_TRUE, 0, r.getDim()*sizeof(std::complex<double>), &r[0], 0, NULL, NULL);
  }

  clReleaseMemObject(d_C);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_A);

  return r;
}

} // namespace GMlib
