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




#ifndef GM_PARAMETRICS_UTILS_PUTILS_H__
#define GM_PARAMETRICS_UTILS_PUTILS_H__


namespace GMlib {

  //- Forward declarations
  template <typename T, int n> class PCurve;

  namespace GMPutils {

    template <typename T, int n>
    void printErrors(PCurve<T, n>& f, PCurve<T, n>& g);

    template <typename T, int n>
    T getG2(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, double eps);

    template <typename T, int n>
    T getGInf(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, int samples=1000);

    template <typename T, int n>
    T getL2(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, double eps);

    template <typename T, int n>
    T getLInf(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, int samples=1000);

    template <typename T, int n>
    T getDiffCp(PCurve<T, n>& f, PCurve<T, n>& g, T t);

    template <typename T, int n>
    T getDiffSqr(PCurve<T, n>& f, PCurve<T, n>& g, T t);

    template <typename T, int n>
    T getIntegralClosestPointSqr(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, double eps);

    template <typename T, int n>
    T getIntegralDerivativeLength(PCurve<T, n>& f, T a, T b, double eps);

    template <typename T, int n>
    T getIntegralDiffSqr(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, double eps);


  } // END namespace GMPutils

} // END namespace GMlib

#include "gmputils.c"

#endif // GM_PARAMETRICS_UTILS_PUTILS_H__
