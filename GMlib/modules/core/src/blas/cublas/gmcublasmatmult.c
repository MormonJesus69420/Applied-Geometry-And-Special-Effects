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



/*! \file gmcublasmatmult.c
 *
 *  Implementation of CuBLAS-specific matrix multiplication operators.
 */

#include <cuda_runtime.h>
#include <cublas_v2.h>
#include "gmcublascontext.h"

namespace GMlib
{

/*!  const DMatrix<float>&  operator*(const DMatrix<float>& m, const DMatrix<float>& b)
 *  \brief Multiply two matrices
 *
 *  CuBLAS-specific implementation of the multiplication of two matrices
 *  of data type float.
 */
inline
const DMatrix<float>&  operator*(const DMatrix<float>& m, const DMatrix<float>& b)
{
  static DMatrix<float> r;
  r.setDim(m.getDim1(), b.getDim2());

  float* d_A;
  float* d_B;
  float* d_C;

  cudaMalloc((void**)&d_A, sizeof(float) * m.getDim1() * m.getDim2());
  cudaMalloc((void**)&d_B, sizeof(float) * b.getDim1() * b.getDim2());
  cudaMalloc((void**)&d_C, sizeof(float) * m.getDim1() * b.getDim2());

  for(int i=0; i<m.getDim1(); i++)
  {
    cudaMemcpy(&d_A[i*m.getDim2()], &m(i)(0), sizeof(float)*m.getDim2(), cudaMemcpyHostToDevice);
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    cudaMemcpy(&d_B[i*b.getDim2()], &b(i)(0), sizeof(float)*b.getDim2(), cudaMemcpyHostToDevice);
  }

  const float alpha = 1.0f;
  const float beta = 0.0f;

  cublasSgemm(CublasContext::getHandle(), CUBLAS_OP_N, CUBLAS_OP_N, b.getDim2(), m.getDim1(), m.getDim2(), &alpha, d_B, b.getDim2(), d_A, m.getDim2(), &beta, d_C, b.getDim1());

  cudaEvent_t stop;
  cudaEventCreate(&stop);
  cudaEventRecord(stop, NULL);
  cudaEventSynchronize(stop);

  static Array<float> work;
  work.setSize(m.getDim1() * b.getDim2());
  cudaMemcpy(work.getPtr(), d_C, sizeof(float)*work.getSize(), cudaMemcpyDeviceToHost);

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(float));

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  return r;
}

/*!  const DMatrix<double>&  operator*(const DMatrix<double>& m, const DMatrix<double>& b)
 *  \brief Multiply two matrices
 *
 *  CuBLAS-specific implementation of the multiplication of two matrices
 *  of data type double.
 */
inline
const DMatrix<double>&  operator*(const DMatrix<double>& m, const DMatrix<double>& b)
{
  static DMatrix<double> r;
  r.setDim(m.getDim1(), b.getDim2());

  double* d_A;
  double* d_B;
  double* d_C;

  cudaMalloc((void**)&d_A, sizeof(double) * m.getDim1() * m.getDim2());
  cudaMalloc((void**)&d_B, sizeof(double) * b.getDim1() * b.getDim2());
  cudaMalloc((void**)&d_C, sizeof(double) * m.getDim1() * b.getDim2());

  for(int i=0; i<m.getDim1(); i++)
  {
    cudaMemcpy(&d_A[i*m.getDim2()], &m(i)(0), sizeof(double)*m.getDim2(), cudaMemcpyHostToDevice);
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    cudaMemcpy(&d_B[i*b.getDim2()], &b(i)(0), sizeof(double)*b.getDim2(), cudaMemcpyHostToDevice);
  }

  const double alpha = 1.0;
  const double beta = 0.0;

  cublasDgemm(CublasContext::getHandle(), CUBLAS_OP_N, CUBLAS_OP_N, b.getDim2(), m.getDim1(), m.getDim2(), &alpha, d_B, b.getDim2(), d_A, m.getDim2(), &beta, d_C, b.getDim1());

  cudaEvent_t stop;
  cudaEventCreate(&stop);
  cudaEventRecord(stop, NULL);
  cudaEventSynchronize(stop);

  static Array<double> work;
  work.setSize(m.getDim1() * b.getDim2());
  cudaMemcpy(work.getPtr(), d_C, sizeof(double)*work.getSize(), cudaMemcpyDeviceToHost);

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(double));

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  return r;
}

/*!  const DMatrix<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DMatrix<std::complex<float> >& b)
 *  \brief Multiply two matrices
 *
 *  CuBLAS-specific implementation of the multiplication of two matrices
 *  of data type std::complex<float>.
 */
inline
const DMatrix<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DMatrix<std::complex<float> >& b)
{
    static DMatrix<std::complex<float> > r;
  r.setDim(m.getDim1(), b.getDim2());

  int work_size = m.getDim1() * b.getDim2();

  cuComplex* d_A;
  cuComplex* d_B;
  cuComplex* d_C;

  cudaMalloc((void**)&d_A, sizeof(std::complex<float>) * m.getDim1() * m.getDim2());
  cudaMalloc((void**)&d_B, sizeof(std::complex<float>) * b.getDim1() * b.getDim2());
  cudaMalloc((void**)&d_C, sizeof(std::complex<float>) * work_size);

  for(int i=0; i<m.getDim1(); i++)
  {
    cudaMemcpy(&d_A[i*m.getDim2()], &m(i)(0), sizeof(std::complex<float>)*m.getDim2(), cudaMemcpyHostToDevice);
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    cudaMemcpy(&d_B[i*b.getDim2()], &b(i)(0), sizeof(std::complex<float>)*b.getDim2(), cudaMemcpyHostToDevice);
  }

  static const cuComplex alpha = make_cuFloatComplex(1.0f, 0.0f);
  static const cuComplex beta = make_cuFloatComplex(0.0f, 0.0f);

  cublasCgemm(CublasContext::getHandle(), CUBLAS_OP_N, CUBLAS_OP_N, b.getDim2(), m.getDim1(), m.getDim2(), &alpha, d_B, b.getDim2(), d_A, m.getDim2(), &beta, d_C, b.getDim1());

  cudaEvent_t stop;
  cudaEventCreate(&stop);
  cudaEventRecord(stop, NULL);
  cudaEventSynchronize(stop);

  std::complex<float>* work = new std::complex<float>[work_size];
  cudaMemcpy(work, d_C, sizeof(std::complex<float>)*work_size, cudaMemcpyDeviceToHost);

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(std::complex<float>));
  delete[] work;

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  return r;
}

/*!  const DMatrix<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DMatrix<std::complex<double> >& b)
 *  \brief Multiply two matrices
 *
 *  CuBLAS-specific implementation of the multiplication of two matrices
 *  of data type std::complex<double>.
 */
inline
const DMatrix<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DMatrix<std::complex<double> >& b)
{
    static DMatrix<std::complex<double> > r;
  r.setDim(m.getDim1(), b.getDim2());

  int work_size = m.getDim1() * b.getDim2();

  cuDoubleComplex* d_A;
  cuDoubleComplex* d_B;
  cuDoubleComplex* d_C;

  cudaMalloc((void**)&d_A, sizeof(std::complex<double>) * m.getDim1() * m.getDim2());
  cudaMalloc((void**)&d_B, sizeof(std::complex<double>) * b.getDim1() * b.getDim2());
  cudaMalloc((void**)&d_C, sizeof(std::complex<double>) * work_size);

  for(int i=0; i<m.getDim1(); i++)
  {
    cudaMemcpy(&d_A[i*m.getDim2()], &m(i)(0), sizeof(std::complex<double>)*m.getDim2(), cudaMemcpyHostToDevice);
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    cudaMemcpy(&d_B[i*b.getDim2()], &b(i)(0), sizeof(std::complex<double>)*b.getDim2(), cudaMemcpyHostToDevice);
  }

  static const cuDoubleComplex alpha = make_cuDoubleComplex(1.0, 0.0);
  static const cuDoubleComplex beta = make_cuDoubleComplex(0.0, 0.0);

  cublasZgemm(CublasContext::getHandle(), CUBLAS_OP_N, CUBLAS_OP_N, b.getDim2(), m.getDim1(), m.getDim2(), &alpha, d_B, b.getDim2(), d_A, m.getDim2(), &beta, d_C, b.getDim1());

  cudaEvent_t stop;
  cudaEventCreate(&stop);
  cudaEventRecord(stop, NULL);
  cudaEventSynchronize(stop);

  std::complex<double>* work = new std::complex<double>[work_size];
  cudaMemcpy(work, d_C, sizeof(std::complex<double>)*work_size, cudaMemcpyDeviceToHost);

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(std::complex<double>));
  delete[] work;

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  return r;
}

/*!  const DVector<float>&  operator*(const DMatrix<float>& m, const DVector<float>& b)
 *  \brief Multiply a matrix with a vector
 *
 *  CuBLAS-specific implementation of the multiplication of a matrix
 *  with a vector of data type float.
 */
inline
const DVector<float>&  operator*(const DMatrix<float>& m, const DVector<float>& b)
{
  static DVector<float> r;
  r.setDim(m.getDim1());

  float* d_A;
  float* d_B;
  float* d_C;

  cudaMalloc((void**)&d_A, sizeof(float) * m.getDim1() * m.getDim2());
  cudaMalloc((void**)&d_B, sizeof(float) * b.getDim());
  cudaMalloc((void**)&d_C, sizeof(float) * r.getDim());

  for(int i=0; i<m.getDim1(); i++)
  {
    cudaMemcpy(&d_A[i*m.getDim2()], &m(i)(0), sizeof(float)*m.getDim2(), cudaMemcpyHostToDevice);
  }
  cudaMemcpy(d_B, &b(0), sizeof(float)*b.getDim(), cudaMemcpyHostToDevice);

  const float alpha = 1.0f;
  const float beta = 0.0f;

  cublasSgemv(CublasContext::getHandle(), CUBLAS_OP_T, m.getDim1(), m.getDim2(), &alpha, d_A, m.getDim1(), d_B, 1, &beta, d_C, 1);

  cudaEvent_t stop;
  cudaEventCreate(&stop);
  cudaEventRecord(stop, NULL);
  cudaEventSynchronize(stop);

  cudaMemcpy(&r[0], d_C, sizeof(float)*r.getDim(), cudaMemcpyDeviceToHost);

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  return r;
}

/*!  const DVector<double>&  operator*(const DMatrix<double>& m, const DVector<double>& b)
 *  \brief Multiply a matrix with a vector
 *
 *  CuBLAS-specific implementation of the multiplication of a matrix
 *  with a vector of data type double.
 */
inline
const DVector<double>&  operator*(const DMatrix<double>& m, const DVector<double>& b)
{
  static DVector<double> r;
  r.setDim(m.getDim1());

  double* d_A;
  double* d_B;
  double* d_C;

  cudaMalloc((void**)&d_A, sizeof(double) * m.getDim1() * m.getDim2());
  cudaMalloc((void**)&d_B, sizeof(double) * b.getDim());
  cudaMalloc((void**)&d_C, sizeof(double) * r.getDim());

  for(int i=0; i<m.getDim1(); i++)
  {
    cudaMemcpy(&d_A[i*m.getDim2()], &m(i)(0), sizeof(double)*m.getDim2(), cudaMemcpyHostToDevice);
  }
  cudaMemcpy(d_B, &b(0), sizeof(double)*b.getDim(), cudaMemcpyHostToDevice);

  const double alpha = 1.0;
  const double beta = 0.0;

  cublasDgemv(CublasContext::getHandle(), CUBLAS_OP_T, m.getDim1(), m.getDim2(), &alpha, d_A, m.getDim1(), d_B, 1, &beta, d_C, 1);

  cudaEvent_t stop;
  cudaEventCreate(&stop);
  cudaEventRecord(stop, NULL);
  cudaEventSynchronize(stop);

  cudaMemcpy(&r[0], d_C, sizeof(double)*r.getDim(), cudaMemcpyDeviceToHost);

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  return r;
}

/*!  const DVector<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DVector<std::complex<float> >& b)
 *  \brief Multiply a matrix with a vector
 *
 *  CuBLAS-specific implementation of the multiplication of a matrix
 *  with a vector of data type std::complex<float>.
 */
inline
const DVector<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DVector<std::complex<float> >& b)
{
  static DVector<std::complex<float> > r;
  r.setDim(m.getDim1());

  cuComplex* d_A;
  cuComplex* d_B;
  cuComplex* d_C;

  cudaMalloc((void**)&d_A, sizeof(std::complex<float>) * m.getDim1() * m.getDim2());
  cudaMalloc((void**)&d_B, sizeof(std::complex<float>) * b.getDim());
  cudaMalloc((void**)&d_C, sizeof(std::complex<float>) * r.getDim());

  for(int i=0; i<m.getDim1(); i++)
  {
    cudaMemcpy(&d_A[i*m.getDim2()], &m(i)(0), sizeof(std::complex<float>)*m.getDim2(), cudaMemcpyHostToDevice);
  }
  cudaMemcpy(d_B, &b(0), sizeof(std::complex<float>)*b.getDim(), cudaMemcpyHostToDevice);

  const cuComplex alpha = make_cuComplex(1.0f, 0.0f);
  const cuComplex beta = make_cuComplex(0.0f, 0.0f);

  cublasCgemv(CublasContext::getHandle(), CUBLAS_OP_T, m.getDim1(), m.getDim2(), &alpha, d_A, m.getDim1(), d_B, 1, &beta, d_C, 1);

  cudaEvent_t stop;
  cudaEventCreate(&stop);
  cudaEventRecord(stop, NULL);
  cudaEventSynchronize(stop);

  cudaMemcpy(&r[0], d_C, sizeof(std::complex<float>)*r.getDim(), cudaMemcpyDeviceToHost);

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  return r;
}

/*!  const DVector<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DVector<std::complex<double> >& b)
 *  \brief Multiply a matrix with a vector
 *
 *  CuBLAS-specific implementation of the multiplication of a matrix
 *  with a vector of data type std::complex<double>.
 */
inline
const DVector<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DVector<std::complex<double> >& b)
{
  static DVector<std::complex<double> > r;
  r.setDim(m.getDim1());

  cuDoubleComplex* d_A;
  cuDoubleComplex* d_B;
  cuDoubleComplex* d_C;

  cudaMalloc((void**)&d_A, sizeof(std::complex<double>) * m.getDim1() * m.getDim2());
  cudaMalloc((void**)&d_B, sizeof(std::complex<double>) * b.getDim());
  cudaMalloc((void**)&d_C, sizeof(std::complex<double>) * r.getDim());

  for(int i=0; i<m.getDim1(); i++)
  {
    cudaMemcpy(&d_A[i*m.getDim2()], &m(i)(0), sizeof(std::complex<double>)*m.getDim2(), cudaMemcpyHostToDevice);
  }
  cudaMemcpy(d_B, &b(0), sizeof(std::complex<double>)*b.getDim(), cudaMemcpyHostToDevice);

  const cuDoubleComplex alpha = make_cuDoubleComplex(1.0, 0.0);
  const cuDoubleComplex beta = make_cuDoubleComplex(0.0, 0.0);

  cublasZgemv(CublasContext::getHandle(), CUBLAS_OP_T, m.getDim1(), m.getDim2(), &alpha, d_A, m.getDim1(), d_B, 1, &beta, d_C, 1);

  cudaEvent_t stop;
  cudaEventCreate(&stop);
  cudaEventRecord(stop, NULL);
  cudaEventSynchronize(stop);

  cudaMemcpy(&r[0], d_C, sizeof(std::complex<double>)*r.getDim(), cudaMemcpyDeviceToHost);

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  return r;
}

} // namespace GMlib
