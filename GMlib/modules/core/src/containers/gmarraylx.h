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





#ifndef GM_CORE_CONTAINERS_ARRAYLX_H
#define GM_CORE_CONTAINERS_ARRAYLX_H


// GMlib includes
#include "../utils/gmstream.h"


namespace GMlib{


  /*! \brief LX Array (auto pointer)
   *
   *  LX Array (auto pointer)
   */
  template <typename T>
  struct LX_ptr {
    T     *ptr;
    int   size;
  };


  /*! \brief  This is a gmArrayLX, Array Large eXpandable
   *
   *  This is a gmArrayLX, Array Large eXpandable.
   *  gmArrayLX is function-compatiple with STL containers.
   */
  template <typename T>
  class ArrayLX {
  public:
    ArrayLX( int n = 0, int incr = 100 );
    ArrayLX( const ArrayLX<T>& array, int incr = 0 );
    ArrayLX( int n, const T* p, int incr = 100, bool copy = true );
    ~ArrayLX();

    T&            back();
    void          clear( int incr = 0 );
    bool          empty() const;
    bool          exist( const T& obj ) const;
    T&            front();
    T&            getBack();                                                    // Alias: back()
    T&            getFront();                                                   // Alias: front()
    int           getIndex( const T& obj ) const;                               // Alias: index( const T& obj )
    int           getMaxSize() const;                                           // Alias: max_size()
    int           getSize() const;                                              // Alias: size()
    int           index( const T& obj ) const;
    bool          insert( const ArrayLX<T>& array, bool front = false );
    bool          insert( const T& obj, bool front = false );
    void          insertAlways( const ArrayLX<T>& array, bool front = false );
    void          insertAlways( const T& obj, bool front = false );
    void          makeUnique();
    int           max_size() const;
    bool          remove( const T& obj );
    bool          removeBack();
    bool          removeFront();
    bool          removeIndex( int i );
    void          resize( int n );
    void          reverse();
    void          setMaxSize( int n , int incr = 0 );
    void          setSize( int n );
    int           size() const;
    void          strip();
    void          swap( int i, int j );

    T&            operator  []  ( int i );
    const T&      operator  ()  ( int i ) const;

    ArrayLX<T>&   operator  =   ( const ArrayLX<T>& array );

    void          operator  +=  ( const T& obj );
    void          operator  +=  ( const ArrayLX<T>& array );

    bool          operator  ==  ( const ArrayLX<T>& array ) const;
    bool          operator  !=  ( const ArrayLX<T>& array ) const;


  protected:
    T&            getElement( int i ) const;


  private:
    LX_ptr<T>  	  *_ptr;
    int           _no_ptrs;
    int           _no1, _no2;

    int	          _no_elements;
    int	          _max_elements;
    int           _size_incr;


    void          _indexDecr();
    void          _indexIncr();
    void          _newRow( int size, T* ptr = 0x0 );


  public:

    #ifdef GM_STREAM

    // *****************************
    // IOSTREAM overloaded operators
    template <typename T_Stream>
    friend
    T_Stream& operator << ( T_Stream& out, const ArrayLX<T>& ar ) {

      out << "Size: " << ar.size() << "\n";
      out << "Contains:\n";
      for( int i = 0; i < ar.size(); i++ )
        out << ar(i) << GMseparator::element();
      return out;
    }


    template <typename T_Stream>
    friend
    T_Stream& operator >> ( T_Stream& in,  ArrayLX<T>& ar ) {

      int elements;
      in >> elements;
      ar.setSize( elements );
      for( int i = 0; i < ar.size(); i++ )
        in >> ar[i];
      return in;
    }

    #endif


  }; // END ArrayLX


} // END namespace GMlib


#include "gmarraylx.c"

#endif // GM_CORE_CONTAINERS_ARRAYLX_H

