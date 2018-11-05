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



/*! \file gmacmlmatmult.c
 *
 *  Implementation of ACML-specific matrix multiplication operators.
 */
#include <acml.h>

namespace GMlib
{

/*!  const DMatrix<float>&  operator*(const DMatrix<float>& m, const DMatrix<float>& b)
 *  \brief Multiply two matrices
 *
 *  ACML-specific implementation of the multiplication of two matrices
 *  of data type float.
 */
inline
const DMatrix<float>&  operator*(const DMatrix<float>& m, const DMatrix<float>& b)
{
  static const char transpose = 'T';
  static const char noTranspose = 'N';
  static DMatrix<float> r;
  static Array<float> cA;
  static Array<float> cB;
  r.setDim(m.getDim1(), b.getDim2());
  cA.setSize(m.getDim1() * m.getDim2());
  cB.setSize(b.getDim1() * b.getDim2());

  for(int i=0; i<m.getDim1(); i++)
  {
    memcpy(&cA[i*m.getDim2()], &m(i)(0), m.getDim2()*sizeof(float));
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    memcpy(&cB[i*b.getDim2()], &b(i)(0), b.getDim2()*sizeof(float));
  }

  static Array<float> work;
  work.setSize(m.getDim1() * b.getDim2());

  sgemm(noTranspose, noTranspose, b.getDim2(), m.getDim1(), m.getDim2(), 1.0f, cB.getPtr(), b.getDim2(), cA.getPtr(), m.getDim2(), 0.0f, work.getPtr(), b.getDim1());

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(float));

  return r;
}

/*!  const DMatrix<double>&  operator*(const DMatrix<double>& m, const DMatrix<double>& b)
 *  \brief Multiply two matrices
 *
 *  ACML-specific implementation of the multiplication of two matrices
 *  of data type double.
 */
inline
const DMatrix<double>&  operator*(const DMatrix<double>& m, const DMatrix<double>& b)
{
  static const char transpose = 'T';
  static const char noTranspose = 'N';
  static DMatrix<double> r;
  static Array<double> cA;
  static Array<double> cB;
  r.setDim(m.getDim1(), b.getDim2());
  cA.setSize(m.getDim1() * m.getDim2());
  cB.setSize(b.getDim1() * b.getDim2());

  for(int i=0; i<m.getDim1(); i++)
  {
    memcpy(&cA[i*m.getDim2()], &m(i)(0), m.getDim2()*sizeof(double));
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    memcpy(&cB[i*b.getDim2()], &b(i)(0), b.getDim2()*sizeof(double));
  }

  static Array<double> work;
  work.setSize(m.getDim1() * b.getDim2());

  dgemm(noTranspose, noTranspose, b.getDim2(), m.getDim1(), m.getDim2(), 1.0, cB.getPtr(), b.getDim2(), cA.getPtr(), m.getDim2(), 0.0, work.getPtr(), b.getDim1());

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(double));

  return r;
}

/*!  const DVector<float>&  operator*(const DMatrix<float>& m, const DVector<float>& b)
 *  \brief Multiply a matrix with a vector
 *
 *  ACML-specific implementation of the multiplication of a matrix
 *  with a vector of data type float.
 */
inline
const DVector<float>&  operator*(const DMatrix<float>& m, const DVector<float>& b) {
  static DVector<float> r;

  if(m.getDim2() != b.getDim()) return r;

  r.setDim(m.getDim1());
  int n = m.getDim2();
  float* vec = const_cast<float*>(&b(0));
  for(int i=0;i<m.getDim1();i++)
  {
    r[i] = sdot(m.getDim2(), const_cast<float*>(&m(i)(0)), 1, vec, 1);
  }
  return r;
}

/*!  const DVector<double>&  operator*(const DMatrix<double>& m, const DVector<double>& b)
 *  \brief Multiply a matrix with a vector
 *
 *  ACML-specific implementation of the multiplication of a matrix
 *  with a vector of data type double.
 */
inline
const DVector<double>&  operator*(const DMatrix<double>& m, const DVector<double>& b) {
  static DVector<double> r;

  if(m.getDim2() != b.getDim()) return r;

  r.setDim(m.getDim1());
  int n = m.getDim2();
  double* vec = const_cast<double*>(&b(0));
  for(int i=0;i<m.getDim1();i++)
  {
    r[i] = ddot(m.getDim2(), const_cast<double*>(&m(i)(0)), 1, vec, 1);
  }
  return r;
}

/*!  const DVector<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DVector<std::complex<float> >& b)
 *  \brief Multiply a matrix with a vector
 *
 *  ACML-specific implementation of the multiplication of a matrix
 *  with a vector of data type std::complex<float>.
 */
inline
const DVector<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DVector<std::complex<float> >& b) {
  static DVector<std::complex<float> > r;

  if(m.getDim2() != b.getDim()) return r;

  r.setDim(m.getDim1());
  int n = m.getDim2();
  complex* vec = (complex*)(&b(0));
  for(int i=0;i<m.getDim1();i++)
  {
        static complex tmp;
        tmp = cdotu(m.getDim2(), (complex*)(&m(i)(0)), 1, vec, 1);
        r[i] = *reinterpret_cast<std::complex<float>*>(&tmp);
  }
  return r;
}

/*!  const DVector<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DVector<std::complex<double> >& b)
 *  \brief Multiply a matrix with a vector
 *
 *  ACML-specific implementation of the multiplication of a matrix
 *  with a vector of data type std::complex<double>.
 */
inline
const DVector<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DVector<std::complex<double> >& b) {
  static DVector<std::complex<double> > r;

  if(m.getDim2() != b.getDim()) return r;

  r.setDim(m.getDim1());
  int n = m.getDim2();
  doublecomplex* vec = (doublecomplex*)(&b(0));
  for(int i=0;i<m.getDim1();i++)
  {
        static doublecomplex tmp;
        tmp = zdotu(m.getDim2(), (doublecomplex*)(&m(i)(0)), 1, vec, 1);
        r[i] = *reinterpret_cast<std::complex<double>*>(&tmp);
  }
  return r;
}

/*!  const DMatrix<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DMatrix<std::complex<float> >& b)
 *  \brief Multiply two matrices
 *
 *  ACML-specific implementation of the multiplication of two matrices
 *  of data type std::complex<float>.
 */
inline
const DMatrix<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DMatrix<std::complex<float> >& b)
{
  static const char transpose = 'T';
  static const char noTranspose = 'N';
  static DMatrix<std::complex<float> > r;
  std::complex<float>* cA = new std::complex<float>[m.getDim1() * m.getDim2()];
  std::complex<float>* cB = new std::complex<float>[b.getDim1() * b.getDim2()];
  r.setDim(m.getDim1(), b.getDim2());

  for(int i=0; i<m.getDim1(); i++)
  {
    memcpy(&cA[i*m.getDim2()], &m(i)(0), m.getDim2()*sizeof(std::complex<float>));
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    memcpy(&cB[i*b.getDim2()], &b(i)(0), b.getDim2()*sizeof(std::complex<float>));
  }

  std::complex<float>* work = new std::complex<float>[m.getDim1() * b.getDim2()];
  complex alpha = {1.0f, 0.0f};
  complex beta = {0.0f, 0.0f};

  cgemm(noTranspose, noTranspose, b.getDim2(), m.getDim1(), m.getDim2(), &alpha, reinterpret_cast<complex*>(cB), b.getDim2(),
    reinterpret_cast<complex*>(cA), m.getDim2(), &beta, reinterpret_cast<complex*>(work), b.getDim1());

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(std::complex<float>));

  delete[] cA;
  delete[] cB;
  delete[] work;
  return r;
}

/*!  const DMatrix<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DMatrix<std::complex<double> >& b)
 *  \brief Multiply two matrices
 *
 *  ACML-specific implementation of the multiplication of two matrices
 *  of data type std::complex<double>.
 */
inline
const DMatrix<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DMatrix<std::complex<double> >& b)
{
  static const char transpose = 'T';
  static const char noTranspose = 'N';
  static DMatrix<std::complex<double> > r;
  std::complex<double>* cA = new std::complex<double>[m.getDim1() * m.getDim2()];
  std::complex<double>* cB = new std::complex<double>[b.getDim1() * b.getDim2()];
  r.setDim(m.getDim1(), b.getDim2());

  for(int i=0; i<m.getDim1(); i++)
  {
    memcpy(&cA[i*m.getDim2()], &m(i)(0), m.getDim2()*sizeof(std::complex<double>));
  }
  for(int i=0; i<b.getDim1(); i++)
  {
    memcpy(&cB[i*b.getDim2()], &b(i)(0), b.getDim2()*sizeof(std::complex<double>));
  }

  std::complex<double>* work = new std::complex<double>[m.getDim1() * b.getDim2()];
  doublecomplex alpha = {1.0f, 0.0f};
  doublecomplex beta = {0.0f, 0.0f};

  zgemm(noTranspose, noTranspose, b.getDim2(), m.getDim1(), m.getDim2(), &alpha, reinterpret_cast<doublecomplex*>(cB), b.getDim2(),
    reinterpret_cast<doublecomplex*>(cA), m.getDim2(), &beta, reinterpret_cast<doublecomplex*>(work), b.getDim1());

  for(int i=0; i<r.getDim1(); i++)
    memcpy(&r[i][0], &work[i*r.getDim2()], r.getDim2()*sizeof(std::complex<double>));

  delete[] cA;
  delete[] cB;
  delete[] work;
  return r;
}

} // namespace GMlib
