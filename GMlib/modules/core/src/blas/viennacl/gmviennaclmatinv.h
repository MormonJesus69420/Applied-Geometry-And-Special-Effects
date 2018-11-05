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



/*! \file gmviennaclmatinv.h
 *
 *  Interface for ViennaCL-specific matrix inversion operators.
 */

#ifndef __gmMATINV_H__
#define __gmMATINV_H__

#include "../../containers/gmdmatrix.h"


namespace GMlib
{
  template<>
  DMatrix<float>& DMatrix<float>::invert();

  template<>
  DMatrix<double>& DMatrix<double>::invert();

} // namespace GMlib


#endif // __gmMATINV_H__
