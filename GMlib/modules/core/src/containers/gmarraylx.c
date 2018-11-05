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



  /*! \brief Default Constructor
   *
   *  \param[in] n Initial number of elements
   *  \param[in] inc Initial increment size.
   */
  template<typename T>
  inline
  ArrayLX<T>::ArrayLX(int n, int inc) {

    _max_elements = _no_elements = _no_ptrs = _no1 = _no2 = 0;
    _size_incr = inc;
    _ptr = 0x0;
      if (n > 0)	_newRow(n);
  }


  /*! \brief Copy Constructor
   *
   *  \param[in] array The array to create this from.
   *  \param[in] inc Incremental size. If none is provided the incremental size of the template array will be used.
   */
  template<typename T>
  inline
  ArrayLX<T>::ArrayLX(const ArrayLX<T>& array, int inc)
  {
    _max_elements = _no_elements = _no_ptrs = _no1 = _no2 = 0;
    if(inc == 0)	_size_incr = array._size_incr;
    else			_size_incr = inc;
    _ptr = 0x0;

    if( array.size() > 0)
    {
      _newRow( array.size() );
      for(int i=0; i<array.size(); i++) _ptr[0].ptr[i] = array(i);
      _no_elements = array.size();
      _no1 = 1;
      _no2 = 0;
    }
  }


  /*! \brief Standard Constructor
   *
   *  \param[in] n The initial number of elements
   *  \param[in] p Initial data.
   *  \param[in] inc The initial incremental size.
   *  \param[in] copy Whether to copy the initial data, or just append the refference pointer.
   */
  template<typename T>
  inline
  ArrayLX<T>::ArrayLX( int n, const T* p, int inc, bool copy )
  {
    _max_elements = _no_elements = _no_ptrs = _no1 = _no2 = 0;
    _size_incr = inc;
    _ptr = 0x0;

    if (n>0)
    {
      if (copy)
      {
        _newRow(n);
        for(int i=0; i<n; i++) _ptr[0].ptr[i] = p[i];
      }
      else
        _newRow(n,p);
      _no_elements = n;
      _no1 = 1;
      _no2 = 0;
    }
  }


  template<typename T>
  ArrayLX<T>::~ArrayLX() {
    for(int i=0; i<_no_ptrs; i++)
      if(_ptr[i].size > 0) {delete [] _ptr[i].ptr; _ptr[i].ptr=0x0; }
    if(_no_ptrs > 0) delete [] _ptr;
  }


  /*! \brief Return the last(back) element of the array
   *
   *  \return reference to element
   */
  template <typename T>
  inline
  T& ArrayLX<T>::back() {
    return (*this)[_no_elements-1];
  }


  /*! \brief Clear the Array
   *
   *  Clear/Reset/Truncate Array.
   *
   *  \param[in] inc The new size increament value
   */
  template<typename T>
  inline
  void  ArrayLX<T>::clear( int inc ) {
    for(int i=0; i<_no_ptrs; i++)
      if(_ptr[i].size > 0) {delete [] _ptr[i].ptr; _ptr[i].ptr=0x0; }

    if(_no_ptrs > 0 && _ptr != 0x0) delete _ptr;
    _max_elements = _no_elements = _no_ptrs = _no1 = _no2 =0;
    _ptr = 0x0;
    if( inc >0 )	_size_incr = inc;
  }


  /*! \brief Function which returns wheter or not the array is empty.
   *
   *  \return True if Array has no elements.
   */
  template <typename T>
  inline
  bool	ArrayLX<T>::empty() const {

    return _no_elements == 0;
  }


  /*! \brief Returns whether or not an element exists in the array
   *
   *  \param[in] obj The object to search for
   *  \return True if Element exists.
   */
  template <typename T>
  inline
  bool ArrayLX<T>::exist(const T& obj) const {			// True if present.

    return bool(index(obj) >= 0);
  }


  /*! \brief Return the first(front) element of the array
   *
   *  \return reference to element
   */
  template <typename T>
  inline
  T& ArrayLX<T>::front() {
    return (*this)[0];
  }


  /*! \brief Return the last(back) element of the array
   *
   *  Return the last(back) element of the array
   *  Alias: int back() function
   *  \see back()
   *
   *  \return reference to element
   */
  template <typename T>
  inline
  T& ArrayLX<T>::getBack() {
    return back();
  }


  /*! \brief Get Element by index
   *
   *  \param[in] i The index of the element to get.
   *  \return reference to element
   */
  template<typename T>
  inline
  T&  ArrayLX<T>::getElement( int i ) const {

  #ifdef DEBUG
      if (i < 0 ||  i >= _no_elements)
      printf("index %d is outside (0, %d)\n", i, _no_elements);
  #endif

    if(_no_ptrs == 1)	return _ptr[0].ptr[i];
    int j;
    for(j=0; j<_no_ptrs; j++)
      if(i < _ptr[j].size)	break;
      else			i -= _ptr[j].size;

    return _ptr[j].ptr[i];
  }


  /*! \brief Return the first(front) element of the array
   *
   *  Return the first(front) element of the array
   *  Alias: front() function
   *  \see front()
   *
   *  \return reference to element
   */
  template <typename T>
  inline
  T& ArrayLX<T>::getFront() {
    return front();
  }


  /*! \brief Gets the index of a spesific object
   *
   *  Gets the index of a spesific object
   *  Alias: int index( const T& obj ) const
   *  \see int ArrayLX<T>::index( const T& obj ) const
   *
   *  \param[in] obj The object to finde the index of.
   *  \return The index of the object provided. If the object does not exist -1 will be returned.
   */
  template<typename T>
  inline
  int ArrayLX<T>::getIndex( const T& obj ) const {
    return index( obj );
  }


  /*! \brief Returns the max number of elements of the Array.
   *
   *  Returns the max number of elements of the Array.
   *  This can be extended.
   *  Alias: int max_size() const
   *  \see int ArrayLX<T>::max_size() const
   *
   *  \return The max number of elements.
   */
  template <typename T>
  inline
  int ArrayLX<T>::getMaxSize() const {
    return max_size();
  }


  /*! \brief Returns the size of the Array.
   *
   *  Returns the number of elements in the array.
   *  Alias: int size() const
   *  \see int ArrayLX<T>::size() const
   *
   *  \return The number of elements in the array.
   */
  template <typename T>
  inline
  int ArrayLX<T>::getSize() const {
      return size();
  }


  /*! \brief Gets the index of a spesific object
   *
   *  Gets the index of a spesific object
   *
   *  \param[in] obj The object to finde the index of.
   *  \return The index of the object provided. If the object does not exist -1 will be returned.
   */
  template<typename T>
  int	ArrayLX<T>::index( const T& obj ) const
  {
    for (int i=0; i<_no_elements; i++)
      if ((*this)(i) == obj)		return i;

    return -1;
  }


  /*! \brief Insert an ArrayLX into the Array
   *
   *  Insert an ArrayLX into the array
   *
   *  \param[in] array The ArrayLX to be inserted
   *  \param[in] front Whether to insert the ArrayLX at the front (or back)
   *  \return Whether all objects where inserted or not; it will not insert the elements which allready exists.
   */
  template<typename T>
  inline
  bool ArrayLX<T>::insert( const ArrayLX<T>& array, bool front ) {
    bool ret = true;

    for(int i=0; i<array.size(); i++) {
      if (exist(array(i)))  ret = false;
      else			alwaysInsert( array(i), front );
    }

    return ret;
  }


  /*! \brief Insert into Array
   *
   *  Insert an object into the array
   *
   *  \param[in] obj The object to be inserted
   *  \param[in] front Whether to insert the object at the front (or back)
   *  \return Whether the object where inserted or not; it will not be inserted if it already exists.
   */
  template<typename T>
  inline
  bool ArrayLX<T>::insert( const T& obj, bool front ) {

    if ( exist(obj) )
      return false;

    insertAlways(obj,front);
    return true;
  }


  /*! \brief Insert Objects into the Array (Always)
   *
   *  Insert Objects into the Array (Always)
   *
   *  \param[in] array The ArrayLX of objects to insert. (Merge with)
   *  \param[in] front Whether to insert the objects at the front (or the back)
   */
  template<typename T>
  void ArrayLX<T>::insertAlways( const ArrayLX<T>& array, bool front ) {
    for (int i=0; i< array.size(); i++)
      insertAlways(array(i),front);
  }


  /*! \brief Insert Objects into the Array (Always)
   *
   *  Insert Objects into the Array (Always)
   *
   *  \param[in] obj The object to insert
   *  \param[in] front Whether to insert the object at the front (or the back)
   */
  template<typename T>
  void ArrayLX<T>::insertAlways( const T& obj, bool front ) {
    if (_no_elements == _max_elements) _newRow(_size_incr);	 // Is the LX_Array full?

    if(front)
    {
      for(int i=_no_elements; i>0; i--)	(*this)[i] = (*this)[i-1];
      _ptr[0].ptr[0] = obj;
    }
    else
      _ptr[_no1].ptr[_no2] = obj;

    _indexIncr();
  }


  /*!int ArrayLX<T>::max_size() const
   *  \brief Returns the max number of elements of the Array.
   *
   *  Returns the max number of elements of the Array.
   *  This can be extended.
   *
   *  \return The max number of elements.
   */
  template <typename T>
  inline
  int ArrayLX<T>::max_size() const {					// Number of max possible elements.

    return _max_elements;
  }


  /*! \brief Remove the given object
   *
   *  Remove the given object from the array.
   *
   *  \param[in] obj The object requested removal.
   *  \return Whether or not the element has been removed.
   */
  template< typename T>
  inline
  bool ArrayLX<T>::remove(const T& obj)	{		// True if removed (False not existing)
    return removeIndex(index(obj));
  }


  /*! \brief Remove the back element of the array.
   *
   *  Remove the back element of the array.
   *
   *  \return Whether or not the element has been removed.
   */
  template< typename T>
  inline
  bool ArrayLX<T>::removeBack() { // Remove last object in array
    return removeIndex(_no_elements-1);
  }


  /*! \brief Remove the front element of the array
   *
   *  Remove the front element of the array
   *
   *  \return Whether or not the element has been removed.
   */
  template< typename T>
  inline
  bool ArrayLX<T>::removeFront() { // Remove first object in array

    return removeIndex(0);
  }




  /*! \brief Remove element with a given index
   *
   *  Remove element with a given index
   *
   *  \param[in] index The index of the parameter to be removed.
   *  \return Whether the element where removed or not
   */
  template<typename T>
  bool ArrayLX<T>::removeIndex(int	index) {
      // Check index.
    if (index < 0 || index >= _no_elements)		return false;

      // Move LX_Array forward.
    _indexDecr();

    (*this)[index] = _ptr[_no1].ptr[_no2];
    return true;
  }


  /*! \brief Resizes the Array.
   *
   *  Resizes the array. Sets the array and strips excessive elements \see strip()
   *
   *  \param[in] n The new ArrayXL size.
   */
  template <typename T>
  inline
  void ArrayLX<T>::resize( int n ) {
      setSize(n);
      strip();
  }


  /*! \brief Reverse the Array
   *
   *  Reverse the Array
   */
  template<typename T>
  void	ArrayLX<T>::reverse() {
    for( int i = 0, mid = _no_elements / 2; i < mid; i++ )
      swap(i, _no_elements - 1 - i);
  }


  /*! \brief set size of potential numbers
   *
   *  set size of potential numbers
   *  \param[in] n The new max size of the Array.
   *  \param[in] inc The new incremental size of the array. If none is provided the old will be unchanged.
   */
  template<typename T>
  void ArrayLX<T>::setMaxSize( int n , int inc ) {
    if (n == 0)		clear();
    else if (_max_elements < n)
    {
      int idx = n - _max_elements;

      _no_ptrs++;
      LX_ptr<T>*  	newptr = new LX_ptr<T>[_no_ptrs];
      int i;
      for(i=0; i<_no_ptrs-1; i++)
      {
        newptr[i]  = _ptr[i];
        _ptr[i].ptr = 0x0;
        _ptr[i].size = 0;
      }
      newptr[i].ptr = new T[idx];
      newptr[i].size = idx;

      if(_no_ptrs > 1) delete [] _ptr;
      _ptr = newptr;

      _max_elements += idx;
    }

    if(inc>0)	_size_incr = inc;
  }


  /*! \brief set size of active numbers not potential
   *
   *  set size of active numbers not potential
   *  \param[in] n The new array size.
   */
  template<typename T>
  void	ArrayLX<T>::setSize( int n ) {
    if (n <= 0)		clear();
    else if (n > _max_elements) {

      int idx = n - _max_elements;
      if( idx < _size_incr )
        idx = _size_incr;

      _newRow(idx);
    }

    if (n < _no_elements)
    {
      int idx = _no_elements - n;
      while(idx>0)
        if(_no2 == 0)
        {
          _no1--;
          _no2 = _ptr[_no1].size-1;
          idx--;
        }
        else if(_no2 <= idx)
        {
          idx -= _no2;
          _no2 = 0;
        }
        else
        {
          _no2 -= idx;
          idx = 0;
        }

      _no_elements = n;
    }
    else if (n > _no_elements)
    {
      int idx = n - _no_elements;
      while(idx>0)
        if(_ptr[_no1].size - _no2 <= idx)
        {
          idx -= _ptr[_no1].size - _no2;
          _no1++;
          _no2 = 0;
        }
        else
        {
          _no2 += idx;
          idx = 0;
        }

      for (int i = _no_elements; i < n; i++)
        (*this)[i] = T();
      _no_elements = n;
    }
  }


  /*! \brief Returns the size of the Array.
   *
   *  Returns the number of elements in the array.
   *
   *  \return The number of elements in the array.
   */
  template <typename T>
  inline
  int ArrayLX<T>::size() const {					// Number of elements.

    return _no_elements;
  }


  /*! \brief Strip all the excess elements, only actual numbers
   *
   *  Strip all the excess elements, only actual numers
   */
  template<typename T>
  void	ArrayLX<T>::strip() {

    if( _max_elements > _no_elements ) {

      LX_ptr<T>*  	newptr  = new LX_ptr<T>[1];
      newptr[0].ptr         = new T[_no_elements];
      newptr[0].size        = _no_elements;

      // Copy elements
      for( int i = 0; i < _no_elements; i++ )
        newptr[0].ptr[i] = (*this)[i];

      clear();
      _max_elements = _no_elements = newptr[0].size;
      _no_ptrs  = 1;
      _ptr      = newptr;
      _no1      = 1;
      _no2      = 0;
    }
  }


  /*! \brief Strip all the excess elements, no free elements.
   *
   *   Strip all the excess elements, no free elements.
   */
  template<typename T>
  void ArrayLX<T>::makeUnique() {

    // Remove surplus elements.
    for (int j,i=0; i<_no_elements-1; i++)
      for (j=_no_elements-1; j>i; j--)
        if((*this)[i] == (*this)[j])	removeIndex(j++);
  }


  /*! \brief Swap two elements in the array.
   *
   *   Swap two elements in the array.
   *
   *  \param[in] i Index of the first element.
   *  \param[in] j Index of the first element.
   */
  template <typename T>
  inline
  void ArrayLX<T>::swap(int i, int j)	{			// Swap two elements.

    T tmp = (*this)[i];
    (*this)[i] = (*this)[j];
    (*this)[j] = tmp;
  }


  /*! \brief Keeps track of the next write index.
   *
   *  Keeps track of the next write index.
   *  Decreases the element counter by one, and moves the index
   *  to the correct location.
   */
  template<typename T>
  inline
  void ArrayLX<T>::_indexDecr() {
    if(_no2 == 0) {
      _no1--;
      _no2 = _ptr[_no1].size-1;
    }
    else _no2--;
    _no_elements--;
  }


  /*! \brief Keeps track of the next write index.
   *
   *  Keeps track of the next write index.
   *  Increases the element counter by one, and moves the index
   *  to the correct location.
   */
  template<typename T>
  inline
  void ArrayLX<T>::_indexIncr() {
    if(_no2 == (_ptr[_no1].size-1)) {
      _no1++;
      _no2 = 0;
    }
    else	_no2++;
    _no_elements++;

  }


  /*! \brief Extends the Array.
   *
   *  Extends the Array.
   *  Extends by size n if no data ptr is given.
   *
   *  \param[in] n The size of the new row.
   *  \param[in] ptr Pointer to data.
   */
  template<typename T>
  void ArrayLX<T>::_newRow(int n, T* ptr)
  {
    if(n<=0) return;

    _no_ptrs++;
    LX_ptr<T>*  	newptr = new LX_ptr<T>[_no_ptrs];
    int i;
    for(i=0; i<_no_ptrs-1; i++)
    {
      newptr[i]  = _ptr[i];
      _ptr[i].ptr = 0x0;
      _ptr[i].size = 0;
    }
    if(ptr)	newptr[i].ptr    = ptr;
    else	newptr[i].ptr = new T[n];


    newptr[i].size = n;

    if(_no_ptrs > 1) delete [] _ptr;
    _ptr = newptr;

    _max_elements += n;
  }


  /*! \brief Returns a referance to the given element at index i
   *
   *   Returns a referance to the given element at index i
   *
   *  \param[in] i The index of the element to retrieve
   *  \return The element at index i
   */
  template<typename T>
  inline
  T&  ArrayLX<T>::operator[](int i) {
  #ifdef DEBUG
      if (i < 0 ||  i >= _no_elements)
      printf("index %d is outside (0, %d)\n", i, _no_elements);
  #endif

    if(_no_ptrs == 1)	return _ptr[0].ptr[i];
    int j;
    for(j=0; j<_no_ptrs; j++)
      if(i < _ptr[j].size)	break;
      else			i -= _ptr[j].size;

    return _ptr[j].ptr[i];
  }


  /*! \brief Returns a constant referance to the given element at index i
   *
   *   Returns a constant referance to the given element at index i
   *
   *  \param[in] i The index of the element to retrieve
   *  \return The element at index i
   */
  template <typename T>
  inline
  const T&   ArrayLX<T>::operator()(int i) const {
  #ifdef DEBUG
      if (i < 0 ||  i >= _no_elements)
      printf("index %d is outside (0, %d)\n", i, _no_elements);
  #endif
    if(_no_ptrs == 1)	return _ptr[0].ptr[i];

    int j;
    for(j=0; j<_no_ptrs; j++)
      if(i < _ptr[j].size)	break;
      else			i -= _ptr[j].size;

    return _ptr[j].ptr[i];
  }


  /*! \brief Assign the given array to this array.
   *
   *  Assign the given array to this array, all data in this data will be lost.
   *
   *  \param[in] array The array to assign to this.
   *  \return Returns a referance to this array.
   */
  template <typename T>
  ArrayLX<T>& ArrayLX<T>::operator=( const	ArrayLX<T>&	array ) {
    // BBang 27.06.00 - to avoid slicing, check for assignment to self
    if( this == &array )
      return *this;

    clear();

    if( array.size() > 0) {
      _newRow(array.size());
      for(int i=0; i<array.size(); i++) _ptr[0].ptr[i] = array(i);
      _no1 = 1;
      _no2 = 0;
      _no_elements = array.size();
    }

    return *this;
  }


  /*! \brief Append an ArrayLX to this array.
   *
   *  Append an ArrayLX to this array. Expands this to make room for the array.
   *
   *  \param[in] array The ArrayLX to append.
   */
  template <typename T>
  inline
  void ArrayLX<T>::operator += ( const ArrayLX<T>& array ) {
    insertAlways( array );
  }


  /*! \brief Appends an object to this array
   *
   *  Appends an object to this array.
   *
   *  \param[in] obj The object to append.
   */
  template <typename T>
  inline
  void ArrayLX<T>::operator += ( const T& obj ) {
    insertAlways( obj );
  }



  /*! \brief Equality test for the whole Array.
   *
   *  Equality test for the whole Array.
   *  Every element of the two Arrays is paired against each other.
   *  If the size is different the function will result false,
   *  whether every element is the same or not.
   *
   *  \param[in] array The Array which to compare with.
   *  \return Whether or not the Array is considered equal to this. True if it is.
   */
  template <typename T>
  bool ArrayLX<T>::operator == ( const ArrayLX<T>& array ) const {
    if (size() != array.size())
      return false;

    for (int i=0; i < _no_elements;  i++)
      if ((*this)(i) != array(i))
        return false;

    return true;
  }


  /*! \brief Equality (not) test for the whole Array.
   *
   *  Equality (not) test for the whole Array
   *  \see operator==( const ArrayLX<T>& array ).
   *
   *  \param[in] array The Array which to compare with.
   *  \return Whether or not the Array is considered equal to this. True if not.
   */
  template <typename T>
  inline
  bool ArrayLX<T>::operator != ( const ArrayLX<T>& array ) const {	// NOT EQUAL?
    return !((*this) == array);
  }

} // END namespace GMlib
