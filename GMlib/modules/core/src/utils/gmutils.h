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




#include "../gmglobal.h"


// stl
#include <functional>
#include <cassert>
#include <limits>
#include <cmath>


#ifndef GM_CORE_UTILS_UTILS_H
#define GM_CORE_UTILS_UTILS_H


namespace GMlib {

  namespace GMutils {

    template <typename T>
    bool compValueF(const T& lhs, const T& rhs) {
      static_assert(std::is_floating_point<T>::value, "Must be a float point value");
      return std::fabs(lhs - rhs) < std::numeric_limits<T>::epsilon();
    }

    template <typename T>
    T fact( int j )
    {
      if ( j <= 0 )
        return T(1);

      T r=1;
      for( int i = j; i > 1; i-- )
        r *= i;

      return r;
    }


    template <typename T>
    T binomial( int n, int j )
    {
           if( n < j )
             return T(0);

           return fact <T> (n) / static_cast<double>( fact <T> (j) * fact <T> (n-j) );
    }


    // Will/(might) be added in C++17
    template<class T, class Compare>
    constexpr const T& clamp( const T& v, const T& lo, const T& hi, Compare comp )
    {
        return assert( !comp(hi, lo) ),
            comp(v, lo) ? lo : comp(hi, v) ? hi : v;
    }

    // Will/(might) be added in C++17
    template<class T>
    constexpr const T& clamp( const T& v, const T& lo, const T& hi )
    {
      return clamp(v, lo, hi, std::less<>() );
    }



    //! \brief http://www.stroustrup.com/bs_faq2.html#contraints
    template<class T, class B>
    struct DerivedFrom {
      static void constraints(T* p) { B* pb = p; GM_UNUSED(pb) }
      DerivedFrom() { void(*p)(T*) = constraints; GM_UNUSED(p) }
    };

    //! \brief http://www.stroustrup.com/bs_faq2.html#contraints
    template<class T1, class T2>
    struct CanCopy {
      static void constraints(T1 a, T2 b) { T2 c = a; b = a; GM_UNUSED(c) }
      CanCopy() { void(*p)(T1,T2) = constraints; GM_UNUSED(p) }
    };

    //! \brief http://www.stroustrup.com/bs_faq2.html#contraints
    template<class T1, class T2 = T1>
    struct CanCompare {
      static void constraints(T1 a, T2 b) { a==b; a!=b; a<b; }
      CanCompare() { void(*p)(T1,T2) = constraints; GM_UNUSED(p) }
    };

    //! \brief http://www.stroustrup.com/bs_faq2.html#contraints
    template<class T1, class T2, class T3 = T1>
    struct CanMultiply {
      static void constraints(T1 a, T2 b, T3 c) { c = a*b; }
      CanMultiply() { void(*p)(T1,T2,T3) = constraints; GM_UNUSED(p) }
    };


  } // END namespace GMutils

} // END namespace GMlib


#endif // GM_CORE_UTILS_UTILS_H
