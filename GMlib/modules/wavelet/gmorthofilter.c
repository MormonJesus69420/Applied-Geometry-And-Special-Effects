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

#include "gmorthofilter.h"

namespace GMlib {
namespace Wavelet {

  template<typename T>
  OrthoFilter<T>::OrthoFilter() :
    Filter<T>() {
  }

  template<typename T>
  void
  OrthoFilter<T>::calcFilters(const DVector<T> &coefs) {
    //- Normalize filter sum and store as decompose low-pass filter
    this->_r_lp = coefs * ( T(1) / coefs.getDim() ) * sqrt(T(coefs.getDim()));

    //- Compute hi-pass from low-pass
    this->qmf(this->_r_lp, this->_r_hp);

    //- Reconstruct filters are simply decompose filters reversed
    this->_d_lp = this->_r_lp.getReversed();
    this->_d_hp = this->_r_hp.getReversed();
  }

} // END namespace Wavelet
} // END namespace GMlib
