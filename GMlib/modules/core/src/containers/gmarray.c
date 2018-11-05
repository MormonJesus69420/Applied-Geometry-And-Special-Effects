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

// gmlib
#include "gmarrayt.h"

// system
#include <memory.h>



namespace GMlib {

  template <typename T>
  inline
  Array<T>::Array( int size ) {

    _sorted       = false;
    _numb         = false;
    _no_elements  = 0;
    _max_elements = (size < 6 ? 6 : size );

    if(_max_elements > 6) { _data_ptr = new T[_max_elements]; }
    else { _data_ptr = _data; }
  }


  template <typename T>
  inline
  Array<T>::Array( int size, T t ) {

    _sorted       = false;
    _no_elements  = size;
    _max_elements = (size < 6 ? 6 : size);

    if(_max_elements > 6) { _data_ptr = new T[_max_elements]; }
    else { _data_ptr = _data; }

    for(int i = 0; i < size; i++) { _data_ptr[i] = t; }
  }


  template <typename T>
  inline
  Array<T>::Array( int size, const T* t ) {

    _sorted       = false;
    _numb         = true;
    _no_elements  = size;
    _max_elements  = (size < 6 ? 6 : size);

    if(_max_elements > 6) { _data_ptr = new T[_max_elements]; }
    else { _data_ptr = _data; }

    memcpy(_data_ptr, t, size * sizeof(T));
    //for(int i = 0; i < size; i++) { _data_ptr[i] = t[i]; }
  }


  template <typename T>
  inline
  Array<T>::Array( const Array<T>& ar ) {

    _sorted       = false;
    _no_elements  = 0;
    _max_elements = 6;
    _data_ptr     = _data;
    (*this)       = ar;
  }


  template <typename T>
  Array<T>::Array( const ArrayT<T>& ar ) {

    _sorted       = false;
    _no_elements  = 0;
    _max_elements = 6;
    _data_ptr     = _data;
    (*this)       = ar;
  }


  template <typename T>
  Array<T>::~Array() {

    if(_max_elements > 6) { delete [] _data_ptr; }
  }


  template <typename T>
  inline
  T& Array<T>::back() {

    return (*this)[_no_elements-1];
  }


  template <typename T>
  inline
  const T& Array<T>::back() const {

    return (*this)(_no_elements-1);
  }


  template <typename T>
  inline
  void Array<T>::clear() {

    if(_max_elements > 6) { delete [] _data_ptr; }
    _no_elements  = 0;
    _max_elements = 6;
    _data_ptr     = _data;
    _numb         = true;
    _sorted       = false;
  }


  template <typename T>
  inline
  bool Array<T>::empty() const {

    return _no_elements <= 0;
  }


  template <typename T>
  inline
  bool Array<T>::exist( const T& t ) const {

    return bool(index(t) >= 0);
  }

  template <typename T>
  inline
  const T& Array<T>::front() const {

    return (*this)(0);
  }

  template <typename T>
  inline
  T& Array<T>::front() {

    return (*this)[0];
  }


  template <typename T>
  inline
  T& Array<T>::getBack() {

    return back();
  }


  template <typename T>
  inline
  T& Array<T>::getFront() {

    return front();
  }


  template <typename T>
  inline
  int Array<T>::getIndex( const T& t ) const {

    return index(t);
  }


  template <typename T>
  inline
  int Array<T>::getMaxSize() const {

    return max_size();
  }


  template <typename T>
  inline
  T* Array<T>::getPtr() {

    return ptr();
  }


  template <typename T>
  inline
  int Array<T>::getSize() const {

    return size();
  }

  template <typename T>
  Array<int> Array<T>::getSortedIndex() const {

    if( this->_no_elements == 0) { return Array<int>(0); }

    int i, j, k;
    Array<int> ar;

    ar.setSize( this->_no_elements );

    for(i = 0; i < this->_no_elements; i++)
      ar[i] = i;

    for(i = 0; i < this->_no_elements - 1; i++) {
      for(k = i, j = i+1; j < this->_no_elements; j++)
        if( this->_data_ptr[ar[j]] < this->_data_ptr[ar[k]] )  k = j;

      ar.swap(i, k);
    }
    return ar;
  }

  template <typename T>
  inline
  bool Array<T>::getStreamMode() const {

    return _numb;
  }


  template <typename T>
  int Array<T>::index( const T& t ) const {

    for(int i = 0; i < _no_elements; i++)
      if(_data_ptr[i] == t)
        return i;

    return -1;
  }


  template <typename T>
  inline
  void Array<T>::initialize( const T& t ) {

    for(int i = 0; i < size(); i++)
      (*this)[i] = t;
  }


  template <typename T>
  inline
  bool Array<T>::insert( const T& t, bool first ) {

    bool res = true;

    if(exist(t)) res = false;
    else         insertAlways(t, first);

    return res;
  }


  template <typename T>
  inline
  bool Array<T>::insert( const Array<T>& ar, bool first ) {

    bool res = true;

    for(int i = 0; i < ar.size(); i++)
      if(exist(ar._data_ptr[i]))
        res = false;
      else
        insertAlways(ar._data_ptr[i], first);

    return res;
  }


  template <typename T>
  inline
  bool Array<T>::insert( const ArrayT<T>& ar, bool first ) {

    bool res = true;
    for(int i = 0; i < ar.size(); i++)
      if(exist(ar(i))) res = false;
      else             insert(ar(i), first);

    return res;
  }


  template <typename T>
  void Array<T>::insertAlways( const T& t, bool first ) {

    int i;

    // Expand the array if nessesary
    this->expand();

    if( _sorted ) {
      for (i = this->_no_elements; i > 0 && t < this->_data_ptr[i-1]; --i)
        this->_data_ptr[i] = this->_data_ptr[i-1];

      this->_data_ptr[i] = t;

    } else if(first) {
      for(i = this->_no_elements; i > 0; i--)
        this->_data_ptr[i] = this->_data_ptr[i-1];

      this->_data_ptr[0] = t;

    } else
      this->_data_ptr[this->_no_elements] = t;

    this->_no_elements++;
  }


  template <typename T>
  void Array<T>::insertAlways( const Array<T>& ar, bool first ) {

    for(int i = 0; i < ar.size(); i++)
      insertAlways(ar._data_ptr[i], first);
  }


  template <typename T>
  void Array<T>::insertAlways( const ArrayT<T>& ar, bool first ) {

    for(int i = 0; i < ar.size(); i++)
        insertAlways(ar(i), first);
  }


  template <typename T>
  inline
  void Array<T>::insertBack( const T& t ) {

    insertAlways(t);
  }


  template <typename T>
  inline
  void Array<T>::insertBack( const Array<T>& ar ) {

    insertAlways(ar);
  }


  template <typename T>
  inline
  void Array<T>::insertBack( const ArrayT<T>& ar ) {

    insertAlways(ar);
  }


  template <typename T>
  inline
  void Array<T>::insertFront( const T& t ) {

    insertAlways(t, true);
  }


  template <typename T>
  inline
  void Array<T>::insertFront( const Array<T>& ar ) {

    insertAlways(ar, true);
  }


  template <typename T>
  inline
  void Array<T>::insertFront( const ArrayT<T>& ar ) {

    insertAlways(ar, true);
  }


  template <typename T>
  inline
  void Array<T>::expand() {

    if(_no_elements == _max_elements) {
      // Allocate new array
      _max_elements += 10;
      T *tmp = new T[_max_elements];

      // Copy old array
      // memcpy(tmp, _data_ptr, _no_elements * sizeof(T));
      for(int i = 0; i < _no_elements; i++) {
        tmp[i] = _data_ptr[i];
      }

      // Delete old array and use the new
      if(_data_ptr != _data) {
        delete [] _data_ptr;
      }

      _data_ptr = tmp;
    }
  }


  template <typename T>
  inline
  bool Array<T>::isEmpty() const {

    return empty();
  }

  /*
  template <typename T>
  inline
  bool Array<T>::exist( const T& t ) const {

    return exist(t);
  }
  */

  template <typename T>
  inline
  int Array<T>::max_size() const {

    return _max_elements;
  }


  template <typename T>
  inline
  void Array<T>::pop() {

    _no_elements--;
  }


  template <typename T>
  inline
  void Array<T>::pop_back() {

    removeBack();
  }


  template <typename T>
  inline
  void Array<T>::pop_front() {

    removeFront();
  }


  template <typename T>
  inline
  T* Array<T>::ptr() {

    return &_data_ptr[0];
  }


  template <typename T>
  inline
  void Array<T>::push() {

    (*this) += back();
  }


  template <typename T>
  inline
  void Array<T>::push_back( const T& t ) {

    insertBack(t);
  }


  template <typename T>
  inline
  void Array<T>::push_back( const Array<T>& ar ) {

    insertBack(ar);
  }


  template <typename T>
  inline
  void Array<T>::push_back( const ArrayT<T>& ar ) {

    insertBack(ar);
  }


  template <typename T>
  inline
  void Array<T>::push_front( const T& t ) {

    insertFront(t);
  }


  template <typename T>
  inline
  void Array<T>::push_front( const Array<T>& ar ) {

    insertFront(ar);
  }


  template <typename T>
  inline
  void Array<T>::push_front( const ArrayT<T>& ar ) {

    insertFront(ar);
  }


  template <typename T>
  inline
  bool Array<T>::remove( const T& t ) {

    return removeIndex(index(t));
  }


  template <typename T>
  inline
  bool Array<T>::removeBack() {

    return removeIndex(_no_elements-1);
  }


  template <typename T>
  inline
  bool Array<T>::removeFront() {

    return removeIndex(0);
  }


  template <typename T>
  bool Array<T>::removeIndex(int index) {

    if(index < 0 || index >= _no_elements) {
      return false;
    }

    // Move array forward
    this->_no_elements--;
    if(_sorted) {
      for(int j = index; j < this->_no_elements; j++) {

        this->_data_ptr[j] = this->_data_ptr[j+1];
      }

    } else if(index != this->_no_elements) {
      this->_data_ptr[index] = this->_data_ptr[this->_no_elements];
    }
    return true;
  }


  template <typename T>
  inline
  void Array<T>::resetSize() {

    _no_elements = 0;
  }


  template <typename T>
  inline
  void Array<T>::resize( int size ) {

    setSize(size);
    strip();
  }


  template <typename T>
  void Array<T>::reverse() {

    for(int i = 0, middle = _no_elements / 2; i < middle; i++) {
      swap(i, _no_elements - 1 - i);
    }
  }


  template <typename T>
  inline
  int Array<T>::size() const {

    return _no_elements;
  }


  template <typename T>
  void Array<T>::setMaxSize( int size ) {

    if(size > _max_elements && size > 6) {
      int old_max       = _max_elements;
      T*  old_data_ptr  = _data_ptr;

      _max_elements = size;
      _data_ptr     = new T[_max_elements];

      //memcpy(_data_ptr, old_data_ptr, _no_elements * sizeof(T));
      for(int i = 0; i < _no_elements; i++) {
        _data_ptr[i] = old_data_ptr[i];
      }

      if(old_max > 6) {
        delete [] old_data_ptr;
      }
    }
  }


  template <typename T>
  void Array<T>::setSize( int size ) {

    int old_max = _max_elements;
    int limit   = (_no_elements < size ? _no_elements : size);

    if(_max_elements < size || (_no_elements - size) > 10 || size == 0) {

      T* old_data_ptr = _data_ptr;
      _no_elements    = size;
      _max_elements   = (_no_elements > 6 ? _no_elements : 6);

      if (_max_elements > 6) {
        _data_ptr = new T[_max_elements];

      }	else {
        _data_ptr = _data;
      }

      for(int i = 0; i < limit; i++) {
        _data_ptr[i] = old_data_ptr[i];
      }

      if (old_max > 6) {
        delete [] old_data_ptr;
      }

    }	else {
      _no_elements = size;
    }

    for(int i = limit; i < _no_elements; i++) {
      _data_ptr[i] = T();
    }
  }


  template <typename T>
  void Array<T>::setSize( int size, T t ) {

    setSize(size);

    for(int i = 0; i < size; i++) {
      _data_ptr[i] = t;
    }
  }


  template <typename T>
  inline
  void Array<T>::setSorted( bool sorted ) {

    _sorted = sorted;
  }


  template <typename T>
  inline
  void Array<T>::setStreamMode( bool mode ) {

    _numb = mode;
  }


  template <typename T>
  void Array<T>::sort() {

    int	i,j,k;

    _sorted = true;

    for(i = 0; i < this->_no_elements - 1; i++) {
      for(k = i, j = i+1; j < this->_no_elements; j++) {
        if(this->_data_ptr[j] < this->_data_ptr[k]) { k = j; }
      }

      if(i != k) { this->swap(i, k); }
    }
  }


  template <typename T>
  void Array<T>::strip() {

    if(_max_elements > _no_elements && _max_elements > 6) {

      T* old_data_ptr = _data_ptr;

      if(_no_elements > 6) {
        _data_ptr     = new T[_no_elements];
        _max_elements = _no_elements;

      } else {
        _data_ptr     = _data;
        _max_elements = 6;
      }

      //memcpy(_data_ptr, old_data_ptr, _no_elements * sizeof(T));
      for(int i=0; i < _no_elements; i++) {
        _data_ptr[i] = old_data_ptr[i];
      }

      if(old_data_ptr != _data) {
        delete [] old_data_ptr;
      }
    }
  }


  template <typename T>
  void Array<T>::makeUnique() {

    for(int i = 0; i < _no_elements-1; i++) {
      for(int j = _no_elements-1; j > i; j--) {
        if(_data_ptr[i] == _data_ptr[j]) {
          removeIndex(j);
        }
      }
    }
  }


  template <typename T>
  void Array<T>::makeUniqueD() {

    for(int i = 0; i < _no_elements-1; i++) {
      for(int j = _no_elements-1; j > i; j--) {
        if( (*_data_ptr[i]) == _data_ptr[j]) {
          removeIndex(j);
        }
      }
    }
  }


  template <typename T>
  inline
  void Array<T>::swap( int i, int j ) {

    T tmp         = _data_ptr[i];
    _data_ptr[i]  = _data_ptr[j];
    _data_ptr[j]  = tmp;
  }


  template <typename T>
  inline
  T& Array<T>::operator [] ( int i ) {

    #ifdef DEBUG
      if(i < 0 ||  i >= _no_elements) {
        printf("Index %d is outside (0, %d)\n", i, _no_elements);
      }
    #endif
    return _data_ptr[i];
  }


  template <typename T>
  inline
  const T& Array<T>::operator [] ( int i ) const {

    #ifdef DEBUG
      if(i < 0 ||  i >= _no_elements) {
        printf("Index %d is outside (0, %d)\n", i, _no_elements);
      }
    #endif
    return _data_ptr[i];
  }

  template <typename T>
  inline
  const T& Array<T>::operator () ( int i ) const {

    #ifdef DEBUG
      if(i < 0 ||  i >= _no_elements) {
        printf("Index %d is outside (0, %d)\n", i, _no_elements);
      }
    #endif
    return _data_ptr[i];
  }


  template <typename T>
  Array<T>& Array<T>::operator = ( const Array<T>& ar ) {

    if(_max_elements < ar.size() || (_max_elements - ar.size()) > 10) {

        _no_elements = ar.size();

        if(_max_elements > 6) {
          delete [] _data_ptr;
        }

        _max_elements = (_no_elements > 6 ? _no_elements : 6);

        if (_max_elements > 6) {
          _data_ptr = new T[_max_elements];

        } else {
          _data_ptr = _data;
        }

      } else {
        _no_elements = ar.size();
      }

    _numb	= ar._numb;
    _sorted = ar._sorted;

    //memcpy(_data_ptr, ar._data_ptr, _no_elements * sizeof(T));
    for(int i = 0; i < _no_elements; i++) {
        _data_ptr[i] = ar._data_ptr[i];
    }

    return (*this);
  }


  template <typename T>
  Array<T>& Array<T>::operator = ( const ArrayT<T>& ar ) {

    clear();
    for( int i = 0; i < ar.size(); i++ )
      insertAlways( ar(i) );

    return (*this);
  }


  template <typename T>
  inline
  void Array<T>::operator += ( const T& t ) {

    insertAlways(t);
  }


  template <typename T>
  inline
  void Array<T>::operator += ( const Array<T>& ar ) {

    insertAlways(ar);
  }


  template <typename T>
  bool Array<T>::operator == ( const Array<T>& ar ) const {

    if(size() != ar.size()) {
      return false;
    }

    for(int i = 0; i < _no_elements;  i++) {
      if(_data_ptr[i] != ar._data_ptr[i]) {
        return false;
      }
    }
    return true;
  }


  template <typename T>
  inline
  bool Array<T>::operator != ( const Array<T>& ar ) const {

    return !((*this) == ar);
  }

  template <typename T>
  inline
  bool Array<T>::isSorted() const {
    return _sorted;
  }

} // END namespace GMlib
