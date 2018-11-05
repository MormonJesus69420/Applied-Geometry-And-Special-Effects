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


  template <typename T>
  int ArrayT<T>::_size_incr = 4;


  template <typename T>
  inline
  ArrayT<T>::ArrayT( int size ) {

    _no_elements = 0;

    if(size > 0)	{
      _ptr           = new T[size];
      _max_elements  = size;

    }	else {
      _ptr          = NULL;
      _max_elements = 0;
    }
  }


  template <typename T>
  inline
  ArrayT<T>::ArrayT( const ArrayT<T>& ar ) {

    _no_elements = ar._no_elements;

    if(_no_elements > 0) {
      _ptr          = new T[ar.size()];
      _max_elements = ar._no_elements;

      for(int i = 0; i < ar.size(); i++) {
        _ptr[i] = ar._ptr[i];
      }

    }	else {
      _no_elements  = _max_elements = 0;
      _ptr          = NULL;
    }
  }


  template <typename T>
  inline
  ArrayT<T>::ArrayT( const Array<T>& ar ) {

    clear();

    for(int i = 0; i < ar.size(); i++) {
      insertAlways(ar(i));
    }
  }


  template <typename T>
  inline
  ArrayT<T>::ArrayT( int size, const T* pt, bool copy ) {

    if(size > 0) {
      _max_elements = _no_elements = size;

      if(copy) {
        _ptr = new T[size];

        for(int i = 0; i < size; i++) {
          _ptr[i] = pt[i];
        }

      }	else {
        _ptr = pt;
      }

    } else {
      _no_elements  = _max_elements = 0;
      _ptr          = NULL;
    }
  }


  template <typename T>
  ArrayT<T>::~ArrayT() {

    clear();
  }


  template <typename T>
  inline
  T& ArrayT<T>::back() {

    return (*this)[_no_elements-1];
  }


  template <typename T>
  inline
  void ArrayT<T>::clear() {

    if(_max_elements > 0) {
      delete [] _ptr;
      _max_elements = _no_elements = 0;
      _ptr          = NULL;
    }
  }


  template <typename T>
  inline
  bool ArrayT<T>::empty() const {

    return _no_elements == 0;
  }


  template <typename T>
  inline
  bool ArrayT<T>::exist( const T& t ) const {

    return bool(index(t) >= 0);
  }


  template <typename T>
  inline
  T& ArrayT<T>::front() {

    return (*this)[0];
  }


  template <typename T>
  inline
  T& ArrayT<T>::getBack() {

    return back();
  }


  template <typename T>
  inline
  T& ArrayT<T>::getFront() {

    return front();
  }


  template <typename T>
  inline
  int ArrayT<T>::getIndex( const T& t ) const {

    return index(t);
  }


  template <typename T>
  inline
  int ArrayT<T>::getMaxSize() const {

    return max_size();
  }


  template <typename T>
  inline
  int ArrayT<T>::getSize() const {

    return size();
  }


  template <typename T>
  int ArrayT<T>::index( const T& t ) const {

    for(int i = 0; i < _no_elements; i++) {
      if(_ptr[i] == t) { return i; }
    }

    return -1;
  }


  template <typename T>
  inline
  bool ArrayT<T>::insert( const T& t, bool first ) {

    bool res = true;

    if(exist(t)) {
      res = false;

    } else {
      insertAlways(t, first);
    }

    return res;
  }


  template <typename T>
  inline
  bool ArrayT<T>::insert( const ArrayT<T>& ar, bool first ) {

    bool res = true;

    for(int i = 0; i < ar.size(); i++) {
      if(exist(ar(i))) {
        res = false;

      } else{
        insertAlways(ar(i), first);
      }
    }
    return res;
  }


  template <typename T>
  void ArrayT<T>::insertAlways( const T& t, bool first ) {

    if(_no_elements == _max_elements) {
      _max_elements += _size_incr;

      T* tPtr = new T[_max_elements];

      for(int i = 0; i < _no_elements; i++)	{
        tPtr[i] = _ptr[i];
      }

      delete [] _ptr;
      _ptr = tPtr;
    }

    if(first) {
      for(int i =_no_elements; i > 0; i--)	{
        _ptr[i] = _ptr[i-1];
      }
      _ptr[0] = t;

    } else {
      _ptr[_no_elements] = t;
    }

    _no_elements++;
  }


  template <typename T>
  void ArrayT<T>::insertAlways( const ArrayT<T>& ar, bool first ) {

    for(int i = 0; i < ar.size(); i++) {
      insertAlways(ar(i), first);
    }
  }


  template <typename T>
  inline
  void ArrayT<T>::insertBack( const T& t ) {

    insertAlways(t);
  }


  template <typename T>
  inline
  void ArrayT<T>::insertBack( const ArrayT<T>& ar) {

    insertAlways(ar);
  }


  template <typename T>
  inline
  void ArrayT<T>::insertFront( const T& t ) {

    insertAlways(t, true);
  }


  template <typename T>
  inline
  void ArrayT<T>::insertFront( const ArrayT<T>& ar ) {

    insertAlways(ar, true);
  }


  template <typename T>
  inline
  bool ArrayT<T>::isEmpty() const {

    return empty();
  }


  template <typename T>
  inline
  bool ArrayT<T>::isExisting( const T& t ) const {

    return exist( t );
  }


  template <typename T>
  inline
  int ArrayT<T>::max_size() const {

    return _max_elements;
  }


  template <typename T>
  inline
  void ArrayT<T>::pop_back() {

    removeBack();
  }


  template <typename T>
  inline
  void ArrayT<T>::pop_front() {

    removeFront();
  }


  template <typename T>
  inline
  void ArrayT<T>::push_back( const T& t ) {

    insertBack(t);
  }


  template <typename T>
  inline
  void ArrayT<T>::push_back( const ArrayT<T>& ar ) {

    insertBack(ar);
  }


  template <typename T>
  inline
  void ArrayT<T>::push_front( const T& t) {

    insertFront(t);
  }


  template <typename T>
  inline
  void ArrayT<T>::push_front( const ArrayT<T>& ar ) {

    insertFront(ar);
  }


  template <typename T>
  inline
  bool ArrayT<T>::remove( const T& t ) {

    return removeIndex(index(t));
  }


  template <typename T>
  inline
  bool ArrayT<T>::removeBack() {

    return removeIndex(_no_elements-1);
  }


  template <typename T>
  inline
  bool ArrayT<T>::removeFront() {

    return removeIndex(0);
  }


  template <typename T>
  bool ArrayT<T>::removeIndex( int index ) {

    if(index < 0 || index >= _no_elements) { return false; }

    // Move array forward
    _no_elements--;
    for(int i = index; i < _no_elements; i++) {
      _ptr[i] = _ptr[i+1];
    }

    return true;
  }


  template <typename T>
  inline
  void ArrayT<T>::resetSize() {

    _no_elements = 0;
  }


  template <typename T>
  inline
  void ArrayT<T>::resize( int size ) {

    setSize(size);
    strip();
  }


  template <typename T>
  void ArrayT<T>::reverse() {

    for(int i = 0, middle = _no_elements / 2; i < middle; i++) {
      swap(i, _no_elements - 1 - i);
    }
  }


  template <typename T>
  void ArrayT<T>::setMaxSize( int size ) {

    if(size <= 0) {
      clear();

    } else if (size > _max_elements) {
      _max_elements = size;
      T* tPtr       = new T[_max_elements];

      for(int i = 0; i < _no_elements; i++) {
        tPtr[i] = _ptr[i];
      }

      delete [] _ptr;
      _ptr = tPtr;
    }
  }


  template <typename T>
  void ArrayT<T>::setSize( int size ) {

    if(size <= 0) {
      clear();

    } else if (size > _max_elements) {
      _max_elements = size;
      T* tPtr       = new T[_max_elements];

      for(int i = 0; i < _no_elements; i++)	{
        tPtr[i] = _ptr[i];
      }

      delete [] _ptr;
      _ptr = tPtr;
    }

    if(size > _no_elements) {
      for (int i = _no_elements; i < size; i++) {
        _ptr[i] = T();
      }
    }

    _no_elements = size;
  }


  template <typename T>
  inline
  int ArrayT<T>::size() const {

    return _no_elements;
  }


  template <typename T>
  void ArrayT<T>::strip() {

    if(_max_elements > _no_elements) {
      _max_elements = _no_elements;
      T* tPtr       = new T[_max_elements];

      for(int i = 0; i < _no_elements; i++)	{
        tPtr[i] = _ptr[i];
      }

      delete [] _ptr;
      _ptr = tPtr;
    }
  }


  template <typename T>
  void ArrayT<T>::makeUnique() {

    for(int j, i = 0; i < _no_elements-1; i++) {
      for(j = _no_elements-1; j > i; j--) {
        if((*this)[i] == (*this)[j]) {
          removeIndex(j++);
        }
      }
    }
  }


  template <typename T>
  inline
  void ArrayT<T>::swap( int i, int j ) {

    T tmp       = (*this)[i];
    (*this)[i]  = (*this)[j];
    (*this)[j]  = tmp;
  }


  template <typename T>
  inline
  void ArrayT<T>::setIncr( int i ) {

    _size_incr = i;
  }


  template <typename T>
  inline
  T& ArrayT<T>::operator [] ( int j ) {

    #ifdef DEBUG
      if(j < 0 ||  j >= _no_elements) {
        printf("Index %d is outside (0, %d)\n", j, _no_elements);
      }
    #endif

    return _ptr[j];
  }


  template <typename T>
  inline
  const T& ArrayT<T>::operator () ( int j ) const {

    #ifdef DEBUG
      if(j < 0 ||  j >= _no_elements) {
        printf("index %d is outside (0, %d)\n", j, _no_elements);
      }
    #endif

    return _ptr[j];
  }


  template <typename T>
  ArrayT<T>& ArrayT<T>::operator =  ( const ArrayT<T>& ar ) {

    clear();

    if(ar.size() > 0) {
      _ptr = new T[ar.size()];

      for(int i = 0; i < ar.size(); i++) {
        _ptr[i] = ar._ptr[i];
      }

      _no_elements = _max_elements = ar._no_elements;
    }

    return (*this);
  }


  template <typename T>
  inline
  void ArrayT<T>::operator += ( const T& t ) {

    insertAlways(t);
  }


  template <typename T>
  inline
  void ArrayT<T>::operator += ( const ArrayT<T>& ar ) {

    insertAlways(ar);
  }


  template <typename T>
  bool ArrayT<T>::operator == ( const ArrayT<T>& ar ) const {

    if(size() != ar.size()) { return false; }

    for(int i = 0; i < size();  i++) {
      if(_ptr[i] != ar._ptr[i]) { return false; }
    }

    return true;
  }


  template <typename T>
  inline
  bool ArrayT<T>::operator != ( const ArrayT<T>& ar ) const {

    return !((*this) == ar);
  }


  // *****************************
  // IOSTREAM overloaded operators

  #ifdef GM_STREAM

  template <typename T>
  inline
  std::ostream& ArrayT<T>::_printOut( std::ostream& out ) const {

    out << size() << "\n";

    for(int i = 0; i < _no_elements; i++)
      out << (*this)(i) << ' ';

    out << std::endl;
    return out;
  }


  template <typename T>
  inline
  std::istream& ArrayT<T>::_printIn( std::istream& in ) {

    int no;
    in >> no;
    setSize(no);

    for(int i = 0; i < no; i++)
      in >> (*this)[i];

    return in;
  }


  template <typename T>
  inline
  void ArrayT<T>::print( const std::string& prompt, std::ostream& out ) const {

    out << prompt << ": " << (*this) << "\n";
  }

  #endif

} // END namespace GMlib
