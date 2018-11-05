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

#ifndef __GM_WAVELET_FILTER_H__
#define __GM_WAVELET_FILTER_H__

// gmlib
#include <core/containers/gmdvector.h>

namespace GMlib {
namespace Wavelet {

  /*!
   * \brief The Filter class
   *
   *  Contains wavelet filters to decompose and reconstruct a signal; both
   *  hi-pass and lo-pass filters - 4 in total.
   */
  template<typename T>
  class Filter {
  public:
    Filter();

    void setFilterCoefs(const DVector<T> &coefs);

    const DVector<T>& getDecomposeHP() const;
    const DVector<T>& getDecomposeLP() const;
    const DVector<T>& getReconstructHP() const;
    const DVector<T>& getReconstructLP() const;

  protected:
    //- Re-implement this to generate filters from input coefficients
    virtual void calcFilters(const DVector<T>& coefs) = 0;

    //- Compute quadrature mirror filter
    void qmf(const DVector<T>& f, DVector<T>& v, int p=0) const;

    //- Filters [size = number of filter elements]
    DVector<T> _d_hp;  // decompose hi-pass filter
    DVector<T> _d_lp;  // decompose low-pass filter
    DVector<T> _r_hp;  // reconstruct hi-pass filter
    DVector<T> _r_lp;  // reconstruct lo-pass filter

  }; // END class Filter

} // END namespace Wavelet
} // END namespace GMlib


#include "gmfilter.c"

#endif // __GM_WAVELET_FILTER_H__
