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

namespace GMlib {
namespace Wavelet {

  /*!
   * \brief Filter<T>::Filter
   * \param dim - Dimension
   * \param coefs - The raw coefficients for a low-pass filter (not normalized)
   */
  template<typename T>
  Filter<T>::Filter() {
  }

  template<typename T>
  void Filter<T>::setFilterCoefs(const DVector<T> &coefs) {
    calcFilters(coefs);
  }

  template<typename T>
  const DVector<T>&
  Filter<T>::getDecomposeHP() const {
    return _d_hp;
  }

  template<typename T>
  const DVector<T>&
  Filter<T>::getDecomposeLP() const {
    return _d_lp;
  }

  template<typename T>
  const DVector<T>&
  Filter<T>::getReconstructHP() const {
    return _r_hp;
  }

  template<typename T>
  const DVector<T>&
  Filter<T>::getReconstructLP() const {
    return _r_lp;
  }

  /*!
   * \brief Filter<T>::qmf - Computes a quadrature mirror filter
   * \param f - Original filter
   * \param v - Resulting filter
   * \param p - Even or odd number
   *
   *  Changes the sign of the even index entries of the reversed
   *  vector filter coefficients f if p is even. If p is odd the
   *  same holds for odd index entries.
   *
   */
  template<typename T>
  void
  Filter<T>::qmf(const DVector<T> &f, DVector<T> &v, int p) const {
    v = f.getReversed();
    for (int i=1-p%2; i < v.getDim(); i+=2) {
      v[i] = -v[i];
    }
  }

} // END namespace Wavelet
} // END namespace GMlib
