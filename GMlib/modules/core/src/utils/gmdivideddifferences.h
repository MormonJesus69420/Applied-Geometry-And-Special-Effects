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


#ifndef GM_CORE_UTILS_DIVIDEDDIFFERENCES_H
#define GM_CORE_UTILS_DIVIDEDDIFFERENCES_H

// GMlib
#include "../types/gmpoint.h"

namespace GMlib {

  namespace DD {

    template <typename T>
    void compute1D( T& p, double dt, bool closed, int d, int ed = 0 );

    template <typename T>
    void compute2D( T& p, double du, double dv, bool closed_u, bool closed_v, int d1, int d2, int ed1 = 0, int ed2 = 0 );


    template <typename T, typename G>
    void compute1D( T& p, const G& t, bool closed, int d, int ed = 0 );

    template <typename T, typename G>
    void compute2D( T& p, const G& u, const G& v, bool closed_u, bool closed_v, int d1, int d2, int ed1 = 0, int ed2 = 0 );

    /*!
     * A method for computing N dimensional recursive divided differenses on a parametric-regularized dataset.
     * The container must be a 2*N dimenional container where the first N dimensions represent the dimensions of the dataset,
     * whilest the second set of N dimensions represent the derivatives.
     * Memory must be pre-allocated.
     * The G& operator [] ( int idx ) must exist as an access operator for each dimension.
     * The data itself must have the H& operator * (double d).
     * Positional data must exist as a minimum for each data point.
     *
     * \param[in,out] p The data set
     * \param[in] sizes Size of each of the N basic dimensions of p
     * \param[in] dt The delta of two adjacent datapoints in a given dimension
     * \param[in] closed Whether the data set is ... closed in a given dimension
     * \param[in] d The number of derivatives to compute in a given dimension
     * \param[in] ed Number of existing derivitives in a given dimension
     */
    template <typename T, int n>
    void compute( T& p, const Vector<int,n>& sizes,
                  const Vector<double,n>& dt,
                  const Vector<bool,n>& closed,
                  const Vector<int,n>& d, const Vector<int,n>& ed = Vector<int,n>(0) );

    /*!
     * Computes the relation between arc length and chord length, given three points
     */
    template <typename T>
    double relationCK(const T& p1, const T& p2, const T& p3);

  } // END namespace DD
} // END namespace GMlib


// Including template definition file.
#include "gmdivideddifferences.c"

#endif // GM_CORE_UTILS_DIVIDEDDIFFERENCES_H
