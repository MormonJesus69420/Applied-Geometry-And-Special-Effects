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

#ifndef __GM_WAVELET_ORTHOFILTER_H__
#define __GM_WAVELET_ORTHOFILTER_H__

#include "gmfilter.h"

namespace GMlib {
namespace Wavelet {

  /*!
   * \brief The OrthoFilter class
   *
   *  A class for orthogonal wavelet filters.
   *
   */
  template<typename T>
  class OrthoFilter : public Filter<T> {
  public:
    OrthoFilter();

  protected:
    //- Inherited from Filter<T>
    void calcFilters(const DVector<T>& coefs);

  }; // END class OrthoFilter

} // END namespace Wavelet
} // END namespace GMlib


#include "gmorthofilter.c"

#endif // __GM_WAVELET_ORTHOFILTER_H__
