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



/*! \file   gmtestutils.h 
 *
 *  Utils used when testing the library.
 */


#ifndef __gmTESTUTILS_H__
#define __gmTESTUTILS_H__

#include <gtest/gtest.h>

namespace GMlib {
namespace GMtest {

  template <typename T, int n>
  ::testing::AssertionResult assertArrays( const T* arr1, const T* arr2 ) {

    for( int i = 0; i < n; ++i )
      if( arr1[i] != arr2[i] )
        return ::testing::AssertionFailure() << "array[i]: " << i << arr1[i] << " != " << arr2[i];

    return ::testing::AssertionSuccess();
  }




  template <typename T>
  void createDestroyObject() {

    T *obj = new T;
    delete obj;
  }




} // END namespace GMtest
} // END namespace GMlib



#endif // __gmTESTUTILS_H__


