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




#ifndef GM_CORE_UTILS_STRING_H
#define GM_CORE_UTILS_STRING_H


#include "../types/gmpoint.h"

// stl
#include <string>

namespace GMlib {

  /*! \class  String gmstring.h <gmString>
   *  \brief  This is a String class
   *
   *  This is a String class
   */
  class String : public std::string {
  public:
    String();
    String( const char* c );
    String( const std::string& str );
    String( int num );
    String( double num );
    String( const Point<double, 2>& p );
    String( const Point<double, 3>& p );
    String( const Point<float, 2>& p );
    String( const Point<float, 3>& p );
    String( const Point<int, 2>& p );
    String( const Point<int, 3>& p );

    void  setPrecision( int p = 2 );

    /*
    String& operator << ( const char* c );
    String& operator << ( const String& str );
    String& operator << ( int num );
    String& operator << ( double num );
    String& operator << ( const Point<double, 2>& p );
    String& operator << ( const Point<double, 3>& p );
    String& operator << ( const Point<float, 2>& p );
    String& operator << ( const Point<float, 3>& p );
    String& operator << ( const Point<int, 2>& p );
    String& operator << ( const Point<int, 3>& p );
    */


  private:
    static char   _data;
    static int    _precision;


  }; // END class String


  #ifdef GM_STREAM

    //***************************************
    // Template iostream operators for String

    template <typename T_Stream>
    T_Stream& operator << ( T_Stream& out, const String& str ) {

      out << std::string(str);
      return out;
    }


    template <typename T_Stream>
    T_Stream& operator >> ( T_Stream& in, String& str ) {

      in >> std::string(str);
      return in;
    }

  #endif

} // END namespace

// Inlcude inline String class implementations
#include "gmstring.c"



#endif // GM_CORE_UTILS_STRING_H

