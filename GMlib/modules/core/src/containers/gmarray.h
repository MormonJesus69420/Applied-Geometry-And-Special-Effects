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


#ifndef GM_CORE_CONTAINERS_ARRAY_H
#define GM_CORE_CONTAINERS_ARRAY_H

// GMlib
#include "../utils/gmstream.h"




namespace GMlib {


  template <typename T>
  class Array;

  template <typename T>
  class ArrayT;


  template <typename T>
  class Array {
  public:
    Array( int size = 6 );
    Array( int size, T t );
    Array( int size, const T* t );
    Array( const Array<T>& ar );
    Array( const ArrayT<T>& ar );
    virtual ~Array();

    T&              back();
    const T&        back() const;
    virtual void    clear();
    bool            empty() const;
    bool            exist( const T& t ) const;
    T&              front();
    const T&        front() const;
    T&              getBack();                    // Alias: back()
    T&              getFront();                   // Alias: front()
    int             getIndex( const T& t ) const; // Alias: index(const T& t)
    int             getMaxSize() const;           // Alias: max_size()
    T*              getPtr();                     // Alias: ptr()
    int             getSize() const;              // Alias: size()
    Array<int>      getSortedIndex() const;
    bool            getStreamMode() const;
    int             index( const T& t ) const;
    void            initialize( const T& t );
    bool            insert( const T& t, bool first = false );
    bool            insert( const Array<T>& ar, bool first = false );
    bool            insert( const ArrayT<T>& ar, bool first = false );
    void            insertAlways( const T& t, bool first = false );
    void            insertAlways( const Array<T>& ar, bool first = false );
    void            insertAlways( const ArrayT<T>& ar, bool first = false );
    void            insertBack( const T& t );
    void            insertBack( const Array<T>& ar );
    void            insertBack( const ArrayT<T>& ar );
    void            insertFront( const T& t );
    void            insertFront( const Array<T>& ar );
    void            insertFront( const ArrayT<T>& ar );
    bool            isEmpty() const;                  // Alias: empty()
    bool            isExisting( const T& t ) const;   // Alias: exist(const T& t)
    bool            isSorted() const;
    void            makeUnique();
    void            makeUniqueD();
    int             max_size() const;
    void            pop();
    void            pop_back();                       // Alias: removeBack()
    void            pop_front();                      // Alias: removeFront()
    T*              ptr();
    void            push();
    void            push_back( const T& t );          // Alias: insertBack(const T&)
    void            push_back( const Array<T>& ar );  // Alias: insertBack(const Array<T>&)
    void            push_back( const ArrayT<T>& ar ); // Alias: insertBack(const ArrayT<T>&)
    void            push_front( const T& t );         // Alias: insertFront(const T&)
    void            push_front( const Array<T>& ar ); // Alias: insertFront(const Array<T>&)
    void            push_front( const ArrayT<T>& ar );// Alias: insertFront(const ArrayT<T>&)
    bool            remove( const T& t );
    bool            removeBack();
    bool            removeFront();
    bool            removeIndex( int index );
    void            resetSize();
    void            resize( int size );
    void            reverse();
    int             size() const;
    void            setMaxSize( int size );
    void            setSize( int size );
    void            setSize( int size, T t );
    void            setSorted( bool sorted = true );
    void            setStreamMode( bool mode = false );
    virtual void    sort();
    void            strip();
    void            swap( int i, int j );


    const T&        operator  [] ( int i ) const;
    T&              operator  [] ( int i );
    const T&        operator  () ( int i ) const;

    Array<T>&       operator  = ( const Array<T>& ar );
    Array<T>&       operator  = ( const ArrayT<T>& ar );

    void            operator  += ( const T& t );
    void            operator  += ( const Array<T>& ar );

    bool            operator  == ( const Array<T>& ar ) const;
    bool            operator  != ( const Array<T>& ar ) const;


  protected:
    void            expand();

    T               _data[6];
    T               *_data_ptr;
    int             _no_elements;
    int             _max_elements;

    bool            _sorted;

    // True:  iostream writing and reading the number (default)
    // False: iostream writing and reading only array,
    //        depending on no_element
    bool            _numb;


  public:

    #ifdef GM_STREAM
      // *****************************
      // IOSTREAM overloaded operators

      template <typename T_Stream>
      friend
      T_Stream& operator << ( T_Stream& out, const Array<T>& ar ) {

        if(ar.getStreamMode()) {
          out << ar.getSize() << GMseparator::element();
        }

        for(int i = 0; i < ar.size(); i++) {
          out << ar(i) << GMseparator::object();
        }
        return out;
      }



      template <typename T_Stream>
      friend
      T_Stream& operator >> ( T_Stream& in, Array<T>& ar) {

        int i;
        static Separator es(GMseparator::element());
        static Separator os(GMseparator::object());
        static Separator gs(GMseparator::group());

        if(ar.getStreamMode()) {
          in >> i >> es;
          ar.setSize(i);
        }

        for(i = 0; i < ar.size(); i++) {
          in >> ar(i) >> os;
        }
        return in;
      }

    #endif

  }; // END Array

} // END namespace


// Including template definition file.
#include "gmarray.c"

#endif // GM_CORE_CONTAINERS_ARRAY_H
