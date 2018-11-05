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



  inline
  String::String() : std::string() {}


  inline
  String::String( const char* c ) : std::string(c) {}


  inline
  String::String( const std::string& str ) : std::string(str) {}


  inline
  String::String( int /* num */ ) : std::string() {

    // Origin implementation (.NET functions)
    //_itoa(num, _data, 10);
    //(*this) = _data;

    //std::stringstream ss;
    //ss << num;
    //(*this) = ss.str();
  }


  inline
  String::String( double /* num */ ) : std::string() {

    // Origin implementation (.NET functions)
    //_gcvt(num, 1 + int(num / 10.0) + _precision, _data);
    //(*this) = _data;

    //stringstream ss;
    //ss << setprecision(_precision) << num;
    //(*this) = ss.str();
  }


  inline
  String::String( const Point<double, 2>& p ) : std::string() {

    (*this) = String(p(0)) + " " + String(p(1));
  }


  inline
  String::String( const Point<double, 3>& p ) : std::string() {

    (*this) = String(p(0)) + " " + String(p(1)) + " " + String(p(2));
  }


  inline
  String::String( const Point<float, 2>& p ) : std::string() {

    (*this) = String(p(0)) + " " + String(p(1));
  }


  inline
  String::String( const Point<float, 3>& p ) : std::string() {

    (*this) = String(p(0)) + " " + String(p(1)) + " " + String(p(2));
  }


  inline
  String::String( const Point<int, 2>& p ) : std::string() {

    (*this) = String(p(0)) + " " + String(p(1));
  }


  inline
  String::String( const Point<int, 3>& p ) : std::string() {

    (*this) = String(p(0)) + " " + String(p(1)) + " " + String(p(2));
  }


  inline
  void String::setPrecision( int p ) {

    _precision = p;
  }

  /*
  inline
  String& String::operator << ( const char* c ) {

    (*this) += String(c);
    return (*this);
  }


  inline
  String& String::operator << ( const String& str ) {

    (*this) += String(str);
    return (*this);
  }


  inline
  String& String::operator << ( int num ) {

    (*this) += String(num);
    return (*this);
  }


  inline
  String& String::operator << ( double num ) {

    (*this) += String(num);
    return (*this);
  }


  inline
  String& String::operator << ( const Point<double, 2>& p ) {

    (*this) += String(p);
    return (*this);
  }


  inline
  String& String::operator << ( const Point<double, 3>& p ){

    (*this) += String(p);
    return (*this);
  }


  inline
  String& String::operator << ( const Point<float, 2>& p ){

    (*this) += String(p);
    return (*this);
  }


  inline
  String& String::operator << ( const Point<float, 3>& p ){

    (*this) += String(p);
    return (*this);
  }


  inline
  String& String::operator << ( const Point<int, 2>& p ){

    (*this) += String(p);
    return (*this);
  }


  inline
  String& String::operator << ( const Point<int, 3>& p ){

    (*this) += String(p);
    return (*this);
  }
  */

} // END namespace GMlib
