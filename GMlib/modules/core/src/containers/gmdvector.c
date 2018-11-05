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





#include "gmdvector.h"


// STL includes
#include <algorithm>



namespace GMlib {


  template<typename T>
  inline
  DVector<T>::DVector(int i) {

    _p = (i>4 ? new T[i]:_init);
    _n = i;
    _private = true;
  }


  /*! \brief Standard initialize all elements to val
   *
   *  \param[in] i Number of elements
   *  \param[in] val Initial value
   */
  template<typename T>
  inline
  DVector<T>::DVector(int i, T val) {

    _p = (i>4 ? new T[i]:_init);
    _n = i;
    clear(val);
    _private = true;
  }


  /*! \brief Standard initializing from c-array */
  template<typename T>
  inline
  DVector<T>::DVector(int i, const T p[]) {
    _p = (i>4 ? new T[i]:_init);
    _n = i;
    _cpy( p );
    _private = true;
  }


  template<typename T>
  inline
  DVector<T>::DVector(const DVector<T>& v) {
    _n = 0; _p = _init;
    _cpy(v);
    _private = true;
  }


  template<typename T>
  inline
  DVector<T>::DVector( T* p, int n, int i ) {

    _p = p + i;
    _n = n;
    _private = false;
  }


  template<typename T>
  inline
  DVector<T>::~DVector() {
    if(_p != _init && _private) delete [] _p;
  }


  template <typename T>
  inline
  void  DVector<T>::_cpy(const DVector<T>& v)
  {
    if(v._n>4 && v._n>_n)
    {
      if (_p != _init) delete [] _p;
      _p = new T[v._n];
    }
    _n = v._n;
    for(int i=0; i<_n; i++) _p[i] = v._p[i];
  }


  template <typename T>
  inline
  void  DVector<T>::_cpy(const T p[]) {

    for(int i=0; i<_n; i++) _p[i] = p[i];
  }


  /*! \brief Appends v to the end of the vector.
   *
   *  \param[in] v Data to be appended
   */
  template <typename T>
  inline
  void DVector<T>::append(const DVector<T>& v) {
    if(v._n > 0)
    {
      int j = _n + v._n;
      if(j>4)
      {
        T* tmp = new T[j];
        for(j=0; j<_n; j++) tmp[j] = _p[j];
        if(_p != _init) delete [] _p;
        _p = tmp;
      }
      for(j=0; j<v._n; j++) _p[j+_n] = v(j);
      _n += v._n;
    }
  }


  /*! \brief Adds i number of elements with value val to the end of the vector.
   *
   *  \param[in] val Value of data to be added.
   *  \param[in] i Number of data elements to be added.
   */
  template <typename T>
  inline
  void DVector<T>::append(T val, int i) {
    if(i < 1) return;
    increaseDim(i, val);
  }


  /*! \brief All elements set to "v", (dimension not changed). */
  template <typename T>
  inline
  void DVector<T>::clear(T v) {
    for(int i=0; i<_n; i++) _p[i] = v;
  }


  /*! \brief Return dimension of vector. */
  template <typename T>
  inline
  int DVector<T>::getDim() const	{
    return _n;
  }


  /*! \brief Get the length of the vector. */
  template <typename T>
  inline
  T DVector<T>::getLength() const {
    T r = T(0);
    for(int i=0; i<_n; i++) r += (*this)(i)*(*this)(i);
    return sqrt(r);
  }


  template <typename T>
  inline
  T* DVector<T>::getPtr() const {
    return _p;
  }


  template <typename T>
  inline
  const DVector<T>& DVector<T>::getReversed() const {
    static DVector<T> ret;
    ret.setDim(_n);
    for(int i=0; i<_n; i++)
      ret[i] = _p[_n-1-i];
    return ret;
  }


  template <typename T>
  inline
  const DVector<T>& DVector<T>::getSubVector(int start, int end) const {
    static DVector<T> ret;
    if(start < 0)	start = 0;
    if(end > _n)		end = _n;
    if(start < end)
    {
      ret.setDim(end-start);
      int u=0;
      for(int i=start; i<end; i++)
        ret[u++] = _p[i];
    }
    else
      ret.setDim(0);
    return ret;
  }


  template <typename T>
  inline
  const T& DVector<T>::getSum() const {
    static T ret;
    ret = T(0);
    for(int i=0; i<_n; i++) ret += _p[i];
    return ret;
  }


  template <typename T>
  inline
  const T& DVector<T>::getSum(int start, int end) const {
    static T ret;
    if(start < 0)	start = 0;
    if(end   > _n)	end = _n;
    ret = T(0);
    for(int i=start; i<end; i++) ret += _p[i];
    return ret;
  }


  /*! \brief Increasing the dimention of the vector.
   *
   *  Increasing the dimention of the vector.
   *  The previous contents is intact, but if "at_end=false"
   *  then the previous contents is moved to the end of the vector.
   *  The new element is initialized by "val".
   */
  template <typename T>
  inline
  void  DVector<T>::increaseDim(int i, T val, bool at_end) {

    if(i>0)
    {
      int k,j  = _n + i;
      T* tmp = (j>4 ? new T[j] : _init);
      if(at_end)
      {
        if(j>4) for (k=0; k<_n; k++)  tmp[k] = _p[k];
        for( k=_n; k<j; k++ )         tmp[k] = val;
      }
      else
      {
        for (k=j-1; k>=i; k--) tmp[k] = _p[k-i];
        for (k=0; k<i; k++)    tmp[k] = val;
      }
      if(_p != _init) delete [] _p;
      _p = tmp;
      _n = j;
    }
  }

  /*! \brief Adds the DVector v to the beginning of the DVector.
   *
   *  \param[in] v Data.
   */
  template <typename T>
  inline
  void DVector<T>::prepend(const DVector<T>& v) {

    if(v._n>0)
    {
      int j = _n+v._n;
      if(j>4)
      {
        T* tmp = new T[j];
        for(j=0; j<_n; j++) tmp[j+v._n] = _p[j];
        if(_p != _init) delete [] _p;
        _p = tmp;
      }
      else
        for(j=_n-1; j>=0; j--) _p[j+v._n] = _p[j];
      for(j=0; j<v._n; j++) _p[j] = v(j);
      _n += v._n;
    }
  }

  /*! \brief Adds i number of elements whith value val to the beginning of the vector.
   *
   *  \param[in] val Value of data to be added.
   *  \param[in] i Number of data elements to be added.
   */
  template <typename T>
  inline
  void DVector<T>::prepend(T val, int i) {

    if(i < 1) return;
    increaseDim(i,val,false);
  }

  /*! void DVector<T>::push_back(const DVector<T>& v)
   *
   *  Equivalent to DVector::append(const DVector<T>& v). Provided for stl compatibility.
   */
  template <typename T>
  inline
  void DVector<T>::push_back(const DVector<T>& v) {

    append( v );
  }


  /*! void DVector<T>::push_back(T val, int i)
   *
   *  Equivalent to DVector::append(T val, int i). Provided for stl compatibility.
   */
  template <typename T>
  inline
  void DVector<T>::push_back(T val, int i) {

    append( val, i );
  }

  /*! void DVector<T>::push_front(const DVector<T>& v)
   *
   *  Equivalent to DVector::prepend(const DVector<T>& v). Provided for stl compatibility.
   */
  template <typename T>
  inline
  void DVector<T>::push_front(const DVector<T>& v) {

    prepend( v );
  }


  /*! void DVector<T>::push_front(T val, int i)
   *
   *  Equivalent to DVector::prepend(T val, int i). Provided for stl compatibility.
   */
  template <typename T>
  inline
  void DVector<T>::push_front(T val, int i) {

    prepend( val, i );
  }


  /*! void  DVector<T>::resetDim(int i)
   *  \brief  Keep the previous contents intact if possible.
   *
   *  Keep the previous contents intact if possible.
   *  Realocating and cleaning up the memory
   *  This function is safe but slow.
   */
  template <typename T>
  inline
  void  DVector<T>::resetDim(int i) {

    int j, k = std::min<int>(i,_n);
    T* tmp = ( i > 4 ? new T[i] : _init );

    for( j = 0; j < k; j++ )
      tmp[j] = _p[j];

    for(; j < i; j++)
      tmp[j] = T();

    if( _p != _init )
      delete [] _p;

    _n = i;
    _p = tmp;
  }


  /*! void  DVector<T>::setDim(int i)
   *  \brief In general, does not keep the previous contents intact.
   *
   *  Set dim to i, but if i is less than the previous dim then
   *  the memory is not realocated.
   *  This function is the speedy one.
   */
  template <typename T>
  inline
  void  DVector<T>::setDim(int i) {
    if(i>4 && i>_n)
    {
      if( _p != _init ) delete [] _p;
      _p = new T[i];
    }
    _n = i;
  }


  /*! Array<T>&	DVector<T>::toArray() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  const Array<T>&	DVector<T>::toArray() const {
    static Array<T> a;
    a.setSize( getDim() );
    for(int i=0; i<_n; i++) a[i] = (*this)(i);
    return a;
  }


  /*! T&  DVector<T>::operator[](int i)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  T&  DVector<T>::operator[](int i) {
  #ifdef DEBUG
    if (i<0 || i>=_n) std::cerr << "Error index " << i << " is outside(0," << _n << ")\n";
  #endif
    return _p[i];
  }


  template <typename T>
  inline
  const T&  DVector<T>::operator[](int i) const {
  #ifdef DEBUG
    if (i<0 || i>=_n) std::cerr << "Error index " << i << " is outside(0," << _n << ")\n";
  #endif
    return _p[i];
  }

  /*! const T&  DVector<T>::operator()(int i) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  const T&  DVector<T>::operator()(int i) const {
  #ifdef DEBUG
    if (i<0 || i>=_n) std::cerr << "Error index " << i << " is outside(0," << _n << ")\n";
  #endif
    return _p[i];
  }


  /*! T&  DVector<T>::front()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  T&  DVector<T>::front()  {
  #ifdef DEBUG
    if (_n==0) std::cerr << "Error index, there is no elements in the vector\n";
  #endif
    return _p[0];
  }


  /*! const T&  DVector<T>::front() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  const T&  DVector<T>::front() const {
  #ifdef DEBUG
    if (_n==0) std::cerr << "Error index, there is no elements in the vector\n";
  #endif
    return _p[0];
  }


  /*! T&  DVector<T>::back()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  T&  DVector<T>::back()  {
  #ifdef DEBUG
    if (_n==0) std::cerr << "Error index, there is no elements in the vector\n";
  #endif
    return _p[_n-1];
  }


  /*! const T&  DVector<T>::back() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  const T&  DVector<T>::back() const {
  #ifdef DEBUG
    if (_n==0) std::cerr << "Error index, there is no elements in the vector\n";
  #endif
    return _p[_n-1];
  }



  /*! bool DVector<T>::operator < (const DVector<T>& v) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  bool DVector<T>::operator < (const DVector<T>& v) const {
    return getLength() < v.getLength();
  }


  /*! DVector<T>& DVector<T>::operator+=(const DVector<T>& v)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template<typename T>
  inline
  DVector<T>& DVector<T>::operator+=(const DVector<T>& v) {
  #ifdef DEBUG
    if (_n != v.getDim())
      std::cerr << "Vector dimension error, dim=" << _n << " ,dim=" << v._getDim() << std::endl;
  #endif
    if(_n==0) {
      //This ensures that an empty DVector stil can be += with another DVector.
      //This is usefull with the * and ^ operators which utilize lokal variables that are empty.
      //In other words this makes posible a DVector<DVector<T> >.
      _cpy(v);
    }
    else
      for (int i=0; i <_n; i++) _p[i] += v._p[i];

    return *this;
  }


  /*! DVector<T>& DVector<T>::operator-=(const DVector<T>& v)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template<typename T>
  inline
  DVector<T>& DVector<T>::operator-=(const DVector<T>& v) {
  #ifdef DEBUG
    if (_n != v.getDim())
      std::cerr << "Vector dimension error, dim=" << _n << " ,dim=" << v.getDim() << std::endl;
  #endif
    for (int i=0; i <_n; i++) _p[i] -= v._p[i];
    return *this;
  }


  /*! DVector<T>	DVector<T>::operator+(const DVector<T>& a) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template<typename T>
  inline
  DVector<T>	DVector<T>::operator+(const DVector<T>& a) const {
    DVector<T> na=(*this);
    return na += a;
  }


  /*! DVector<T>	DVector<T>::operator-(const DVector<T>& a) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template<typename T>
  inline
  DVector<T>	DVector<T>::operator-(const DVector<T>& a) const {
    DVector<T> na=(*this);
    return na -= a;
  }


  /*! DVector<T>& DVector<T>::operator*=( double d )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  DVector<T>& DVector<T>::operator*=( double d ) {
    for(int i=0; i<_n; i++) _p[i] *= d;
    return *this;
  }


  /*! DVector<T>& DVector<T>::operator/=(double d)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  DVector<T>& DVector<T>::operator/=(double d) {
    for(int i=0; i<_n; i++) _p[i] /= d;
    return *this;
  }


  /*! DVector<T> DVector<T>::operator*(double d) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  DVector<T> DVector<T>::operator*(double d) const {
    DVector<T> ret = *this;
    return ret *= d;
  }


  /*! DVector<T> DVector<T>::operator / (double d) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  DVector<T> DVector<T>::operator / (double d) const {
    DVector<T> ret = *this;
    return ret /= d;
  }


  /*! DVector<T>& DVector<T>::operator=(const DVector<T>& v)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  DVector<T>& DVector<T>::operator=(const DVector<T>& v)	{
    _cpy(v); return *this;
  }


  /*! DVector<T>& DVector<T>::operator=(T p[])
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T>
  inline
  DVector<T>& DVector<T>::operator=(T p[]) {
    _cpy(p); return *this;
  }

  /*! void DVector<T>::insert(int index, const GMlib::T& val)
   *
   *  Insert a new element before the given index.
   *
   *  \param[in] index The index which the new element is inserted before
   *  \param[in] val The value given to the element
   */
  template <typename T>
  void DVector<T>::insert(int index, const T& val)  {

    // Increase dimention and "move"
    increaseDim(1);
    for( int i=_n-1; i > index; --i ) _p[i]=_p[i-1];

    // Set new value
    _p[index] = val;
  }
}
