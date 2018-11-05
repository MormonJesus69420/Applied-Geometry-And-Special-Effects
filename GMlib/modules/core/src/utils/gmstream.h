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




#ifndef GM_CORE_UTILS_STREAM_H
#define GM_CORE_UTILS_STREAM_H



#ifdef GM_STREAM
#include <iostream>



namespace GMlib {








  /*! \class  Separator gmStream.h <gmStream>
   *  \brief  Interface for the Separator class.
   *
   *  Separator is a class containing a character defining
   *  the read/write separation between elements/objects etc.
   *
   *  FOR FURTHER DEVELOPMENT !!
   *  The invariant (stats) for Separator is:
   *  _binary is default false.
   */
  class Separator {
  public:
    Separator(char s = ' ', bool b = false);
    Separator( const Separator &s );

    bool            getBinary() const;
    char            getSeparator() const;

    void            setBinary(bool b);
    void            setSeparator(char s);


  private:
    bool            _binary;      // If true, don't send the Separator to the stream
    char            _separator;
//    unsigned int    _separator;


  }; // END Separator


  /*! Separator(char s, bool b)
   *  \brief  Default Constructor
   *
   *  Initialize _separator and _binary before creating the constructor.
   */
  inline
  Separator::Separator( char s, bool b ) : _binary(b), _separator(s) { }


  /*! Separator( const Separator &s )
   *  \brief  Default Copy Constructor
   *
   *  Initialize _separator and _binary before creating the constructor.
   */
  inline
  Separator::Separator( const Separator &s ) : _binary(s._binary), _separator(s._separator) { }


  /*! bool Separator::getBinary() const
   *  \brief  Get value of binary
   *
   *  \return TRUE/FALSE
   */
  inline
  bool Separator::getBinary() const {

    return _binary;
  }


  /*! char Separator::getSeparator() const
   *  \brief  Get value of separator
   *
   *  \return Char which indicated the separation value
   */
  inline
  char Separator::getSeparator() const {

    return _separator;
  }


  /*! \brief  Set the value of binary */
  inline
  void Separator::setBinary( bool b  ) {

    _binary = b;
  }


  /*! \brief  Set the value of separator
   */
  inline
  void Separator::setSeparator( char s ) {

    _separator = s;
  }


  // Predefinitions of the basic separators used throughout the code
  namespace GMseparator {

    const Separator& element();
    const Separator& object();
    const Separator& group();
  }

  // *****************************
  // IOSTREAM overloaded operators

  /*!
   *  If Separator's flag for a binary stream has been set
   *  then nothing will be sendt to the stream. The stram
   *  will alsp be left untouch if the Separator is '' <- nothing.
   *
   *  \return T_Stream
   */
  template< typename T_Stream >
  T_Stream& operator << ( T_Stream &out, const Separator &s ) {

    if( !s.getBinary())
      out << s.getSeparator();
    return out;
  }


  /*!
   *  If Separator's flag for a binary stream has been set
   *  then nothing will be retrieved from the stream.
   *
   *  \return T_Stream
   */
  template< typename T_Stream >
  T_Stream& operator >> ( T_Stream &in, Separator &s ) {

    if( s.getBinary() )
      return in;

    if( s.getSeparator() != ' ' && s.getSeparator() != '\t' && s.getSeparator() != '\n') {

      unsigned int tmp;

      in >> tmp;
      s.setSeparator(static_cast<char>(tmp));
    }
    return in;
  }

} // END namespace

#endif // GM_STREAM


#endif // GM_CORE_UTILS_STREAM_H
