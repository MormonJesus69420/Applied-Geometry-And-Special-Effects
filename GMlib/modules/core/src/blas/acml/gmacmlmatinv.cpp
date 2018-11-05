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



/*! \file gmacmlmatinv.cpp
 *
 *  Implementation of ACML-specific matrix inversion operators.
 */

#include "gmacmlmatinv.h"

#include <acml.h>


namespace GMlib
{

  /*!  DMatrix<float>&  DMatrix<float>::invert()
   *  \brief Invert the matrix
   *
   *  ACML-acceleration of matrix inversion
   *  specific to data type float.
   */
  template<>
  DMatrix<float>& DMatrix<float>::invert() {
    if(getDim1()==getDim2() && getDim1()>1)
    {
      int nk=getDim1();
      Array<float> aa;
      aa.setSize(nk*nk);
      for(int i=0; i<nk; i++)
        for(int j=0; j<nk; j++) aa[i+nk*j] = (float) (*this)[i][j];

      Array<int> ipiv; ipiv.setSize(nk);    // pivot table (result), size max(1,mmm,nnn)
      int info=0;                // error message, i=info>0 means that a[i][i]=0 ): singular,

      sgetrf(nk, nk, aa.ptr(), nk, ipiv.ptr(), &info);
      sgetri(nk, aa.getPtr(), nk, ipiv.getPtr(), &info);

      for(int i=0; i<nk; i++)
        for(int j=0; j<nk; j++) (*this)[i][j]=(float) aa[i+j*nk];
    }
    return (*this);
  }

  /*!  DMatrix<double>&  DMatrix<double>::invert()
   *  \brief Invert the matrix
   *
   *  ACML-acceleration of matrix inversion
   *  specific to data type double.
   */
  template<>
  DMatrix<double>& DMatrix<double>::invert() {
    if(getDim1()==getDim2() && getDim1()>1)
    {
      int nk=getDim1();
      Array<double> aa;
      aa.setSize(nk*nk);
      for(int i=0; i<nk; i++)
        for(int j=0; j<nk; j++) aa[i+nk*j] = (double) (*this)[i][j];

      Array<int> ipiv; ipiv.setSize(nk);    // pivot table (result), size max(1,mmm,nnn)
      int info=0;                // error message, i=info>0 means that a[i][i]=0 ): singular,

      dgetrf(nk, nk, aa.ptr(), nk, ipiv.ptr(), &info);
      dgetri(nk, aa.getPtr(), nk, ipiv.getPtr(), &info);

      for(int i=0; i<nk; i++)
        for(int j=0; j<nk; j++) (*this)[i][j]=(double) aa[i+j*nk];
    }
    return (*this);
  }
} // namespace GMlib
