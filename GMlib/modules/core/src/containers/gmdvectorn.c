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


  template <typename T, int n, class K>
  DVectorN<T,n,K>::DVectorN( int i ) {

    this->_pt = ( i ? new DVectorN<T,n-1>[i] : 0 );
    _n = i;
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>::DVectorN( const DVectorN<T,n,K>& v ) {
    this->_pt = 0x0;
    _copy(v);
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>::DVectorN( const T p[] ) {

    this->_pt = 0x0;
    _copy(p);
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>::~DVectorN() {

    if(this->_pt)
      delete [] this->_pt;
  }

  template <typename T, int n, class K>
  void DVectorN<T,n,K>::_copy( const T p[] ) {

    for( int i = 0; i < _n; i++ )
      this->_pt[i] = p[i];
  }

  template <typename T, int n, class K>
  void DVectorN<T,n,K>::_copy( const DVectorN<T,n,K>& v ) {

    if( this->_pt ) delete []this->_pt;

    if ( v._n > 0 ) {

      this->_pt = new DVectorN<T,n-1>[v._n];
      for( int i = 0; i < v._n; i++ )
        this->_pt[i] = v._pt[i];
    }
    else   this->_pt = 0x0;
    _n = v._n;
  }

  template <typename T, int n, class K>
  void DVectorN<T,n,K>::_indexOK( int /*i*/ ) const {

//#if defined(IOSTREAMH) || defined(__IOSTREAM_H) || defined(__STD_FSTREAM__)
//    if (i<0 || i>=_n) std::cout << "Index error in vector!" << std::endl;
//#endif
  }

  /************************************************************
  * Function dim() returns the sizes of the dimensions in
  * reverse order, for use with setDim.
  ************************************************************/
  template <typename T, int n, class K>
  Vector<int,n>& DVectorN<T,n,K>::getDimRev() const {

    static Vector<int,n> r;

    if(_n)
      for( int j = 0; j < (n-1); j++ )
        r[j] = this->_pt[0].getDimRev()(j);

    r[n-1] = _n;

    return r;
  }

//  template <typename T, int n, class K>
//  DVector<T>* DVectorN<T,n,K>::flat() {

//    static Vector<int,n> vec = getDim();

//    int dim = 1;
//    for( int i = 0; i < n; i++ ) dim *= vec[i];

//    static DVector<T>* flat_vec = new DVector<T>(dim,1);
//    for( int i = vec[n-1]-1; i >= 0; i-- )
//      flat_vec->putFirst( (DVector<T>) *(this->_pt[i].flat()) );

//    return flat_vec;
//  }

  /************************************************************
  * Function getDim() returns the sizes of the dimensions.
  ************************************************************/
  template <typename T, int n, class K>
  Vector<int,n>& DVectorN<T,n,K>::getDim() const {

    static Vector<int,n> res1,res2;
    res1 = getDimRev();

    for( int i = 0; i < n; i++ )
      res2[(n-1)-i] = res1[i];

    return res2;
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K> DVectorN<T,n,K>::getTransposed() const {

    static Vector<int,n> _dim_ = getDimRev();
    static Vector<int,n> dims;

    //Transpose and revert axis/dimensions.
    for( int i = 0; i < (n-1); i++ )
      dims[i] = _dim_[i+1];
    dims[(n-1)]=_dim_[0];

    DVectorN<T,n,K> res;
    res.setDim(dims);

    int dd;
    int ddd = dd = 1;
    for( int i = 1; i < n; i++ ) dd *= _dim_[i];
    ddd = dd * _dim_[0];

    int j=0;
    int k=0;
    T temp;
    for(int i=0; i<ddd; i++) {

      temp = getValue(i);
      res.setValue( j+(k*dd), temp );
      k++;

      if( (k*dd) >= ddd ) { k=0; j++; }
    }

    return res;
  }

  template <typename T, int n, class K>
  T DVectorN<T,n,K>::getValue( int idx_flat ) const {

    Vector<int,n> _dim_ = getDim();

    int ddd = 1;
    for(int i=1; i<n; i++) ddd*=_dim_[i];

    T result = T(-1);
    for( int j = 1; j <= _dim_[0]; j++ ) {

      if( (j*ddd) > idx_flat ) {

        idx_flat -= (j-1) * ddd;
        return result = this->_pt[j-1].getValue( idx_flat );
      }
    }

    return result;
  }

  template <typename T, int n, class K>
  unsigned long int DVectorN<T,n,K>::makeIdx1D( DVector<unsigned long int> k, int res ) {

    unsigned long int tmp=0;
      for(unsigned long int i=0; i<k.getDim(); i++)
        tmp+=k[i]*std::pow(float(res),float(k.getDim()-1-i));
    return tmp;
  }

  template <typename T, int n, class K>
  DVector<unsigned long int> DVectorN<T,n,K>::makeIdx2D( unsigned long int k, int res ) {

    DVector<unsigned long int> r(2);
    r[0]= static_cast<unsigned long int>(k/res);
    unsigned long int tmp=k-r[0]*res;
    r[1]= static_cast<unsigned long int>(tmp);

    return r;
  }

  template <typename T, int n, class K>
  DVector<unsigned long int> DVectorN<T,n,K>::makeIdxND(unsigned long int idx, int res, int dim) {

//    DVector<unsigned long int> r(dim);
//    int m=dim-1;
//    int i=0;
//    r[i]= (unsigned long int) k/std::pow((float)res,m); // 2^3

//    while(i<dim) {
//     i++;
//     unsigned long int tmp=k-r[i-1]*std::pow((float)res,m-i);
//     r[i]=(unsigned long int) tmp/std::pow((float)res,m-i-1);
//    }
//    return r;


    unsigned long int v = idx;
//    int mu = res;
    int m = dim -1;


    float fact = std::pow( float(res), m);

    DVector<unsigned long int> k(dim);
    k[0] = static_cast<unsigned long int>(v / fact); //std::pow( 2.0f, (float)res * ( m - 1) );

//    std::cout << "DVector<unsigned long int> DVectorN<T,n,K>::makeIdxND(unsigned long int idx, int res, int dim)" << std::endl;
//    std::cout << " -- mu: " << mu << std::endl;
//    std::cout << " -- m: " << m << std::endl;

//    std::cout << "k_1: " << k[0];
//    std::cout << ", v: " << v;
//    std::cout << ", fact: " << fact << std::endl;

    unsigned long int v_n = v - k[0] * fact;
    for( int i = 1; i < dim; i++ ) {

      fact = std::pow( float(res), m-i);
      k[i] = static_cast<unsigned long int>( v_n / fact );//std::pow( 2.0f, (float)res * ( m - i) );

      v_n = v_n - k[i] * fact;//std::pow( 2.0f, (float)res * ( m - i) );

//      std::cout << "k_" << (i+1) << ": " << k[i];
//      std::cout << ", v_" << (i+1) << ": " << v_n;
//      std::cout << ", fact: " << fact << std::endl;
    }

    return k;
  }

  template <typename T, int n, class K>
  void DVectorN<T,n,K>::setDim( const Vector<int,n>& dims ) {

    if( dims(0) != _n ) {

      if( this->_pt ) delete []this->_pt;

      _n = dims(0);

      if(_n>0) this->_pt = new DVectorN<T,n-1>[_n];
      else     this->_pt = 0x0;
    }

    Vector<int,n-1> sub_dims;
    for( int i = 0; i < n-1; ++i ) sub_dims[i] = dims(i+1);
    for( int i = 0; i < _n; ++i )  this->_pt[i].setDim(sub_dims);
  }

  template <typename T, int n, class K>
  void DVectorN<T,n,K>::setValue(int idx_flat, T val ) {

    Vector<int,n> _dim_ = getDim();

    int ddd = 1;
    for( int i = 1; i < n; i++ )
      ddd *= _dim_[i];

    for(int j=1;j<=_dim_[0];j++) {

      if( (j*ddd) > idx_flat) {

        idx_flat -= (j-1)*ddd;
        this->_pt[j-1].setValue( idx_flat, val );
        return;
      }
    }
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>& DVectorN<T,n,K>::operator = ( const DVectorN<T,n,K>& v ) {

    _copy(v);
    return *this;
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>& DVectorN<T,n,K>::operator = ( T p[]) {

    _copy(p);
    return *this;
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>	DVectorN<T,n,K>::operator + (const DVectorN<T,n,K>& a)	const {

    DVectorN<T,n,K> na = *this;
    na += a;
    return na;
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>	DVectorN<T,n,K>::operator - (const DVectorN<T,n,K>& a)	const {

    DVectorN<T,n,K> na = *this;
    na -= a;
    return na;
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>	DVectorN<T,n,K>::operator % (const DVectorN<T,n,K>& v)	const {

    DVectorN<T,n,K> na = *this ;
    na %= v;
    return na;
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>	DVectorN<T,n,K>::operator * (double d) const {

    DVectorN<T,n,K> na = *this;
    na *= d;
    return na;
  }

  template <typename T, int n, class K>
  DVectorN<T,n,K>	DVectorN<T,n,K>::operator / (double d) const {

    DVectorN<T,n,K> na = *this;
    na /= d;
    return na;
  }

  /***************************************************************************************************
  * Function operator*: Multiplying an n dimensional DVectorN with a one dimensional DVector returning
  * a DVectorN of one dimension lower (n-1), containing values of type T. Since a DVectorN<T,1> is
  * essentially a DVector<T> these are interchangeable.
  ****************************************************************************************************/
  template <typename T, int n, class K>
  inline
  DVectorN<T,n-1>& DVectorN<T,n,K>::operator * ( const DVector<K>& v ) {

    static Vector<int,n> dims = getDimRev();
    static Vector<int,n-1> _dim_(n-1);
    for( int i = 0; i < (n-1); i++ )
      _dim_[i] = dims[i];

    static DVectorN<T,n-1> result;
    result.setDim(_dim_);

    for(int i = 0; i < _n; i++)
      result[i] = this->_pt[i] * v;

    return result;
  }

  /***************************************************************************************************
  * Function operator^: Multiplying an n dimensional DVectorN with a one dimensional DVector returning
  * a DVectorN of one dimension lower (n-1), containing values of type K. Since a DVectorN<T,1> is
  * essentially a DVector<T> these are interchangeable.
  ****************************************************************************************************/
  template<typename T, int n, class K>
  inline
  DVectorN<T,n-1>& DVectorN<T,n,K>::operator ^ ( const DVector<K>& v ) {

    //static Vector<int,n> dims = dim(); // dim() newer gets called for some reason.
    static Vector<int,n-1> _dim_(n-1);
    for( int i = 0; i < (n-1); i++ )
      _dim_[i] = getDimRev()[i]; //Solved it like this, but gives more function calls.

    static DVectorN<T,n-1> result;
    result.setDim(_dim_);

    for( int i = 0; i < _n; i++ )
      result[i] = this->_pt[i] ^ v;

    return result;
  }


  /***************************************************************************************************
  * Function operator*: A recursive template operator multiplying an n dimensional DVectorN with a two
  * dimensional matrix. Returning a DVectotN of the same dimension as the original DVectorN.
  ****************************************************************************************************/
  template <typename T, int n, class K>
  inline
  DVectorN<T,n,K>& DVectorN<T,n,K>::operator*(const DMatrix<K>& m) {

    static DVectorN<T,n,K> result;
    result.setDim(getDim());

    for( int i = 0; i < _n; i++ )
      result[i] = this->_pt[i] * m;

    return result;
  }

  template <class T, int n, class K>
  DVectorN<T,n-1>& DVectorN<T,n,K>::operator[](int i) {

    _indexOK(i);
    return this->_pt[i];
  }

  template <class T, int n, class K>
  const DVectorN<T,n-1>& DVectorN<T,n,K>::operator()(int i) const {

    _indexOK(i);
    return this->_pt[i];
  }

  template <class T, int n, class K>
  DVectorN<T,n,K>& DVectorN<T,n,K>::operator+=(const DVectorN<T,n,K>& v) {

    int j = std::min<int>(_n,v._n);
    for( int i = 0; i < j; i++ ) this->_pt[i] += v._pt[i];
    return *this;
  }

  template <class T, int n, class K>
  DVectorN<T,n,K>& DVectorN<T,n,K>::operator-=(const DVectorN<T,n,K>& v) {

    int j = std::min<int>( _n, v._n );
    for( int i = 0; i < j; i++ ) this->_pt[i] -= v._pt[i];
    return *this;
  }

  template <class T, int n, class K>
  DVectorN<T,n,K>& DVectorN<T,n,K>::operator%=(const DVectorN<T,n,K>& v) {

    int j = std::min<int>(_n,v._n);
    for( int i = 0; i < j; i++ ) this->_pt[i] %= v._pt[i];
    return *this;
  }

  template <class T, int n, class K>
  DVectorN<T,n,K>& DVectorN<T,n,K>::operator*=(double d) {

    for( int i = 0; i < _n; i++ ) this->_pt[i] *= d;
    return *this;
  }

  template <class T, int n, class K>
  DVectorN<T,n,K>& DVectorN<T,n,K>::operator/=(double d) {

    for( int i = 0; i < _n; i++ ) this->_pt[i] /= d;
    return *this;
  }










  template <typename T, class K>
  DVectorN<T,1,K>::DVectorN( int i ) : DVector<T>(i) {}

  template <typename T, class K>
  DVectorN<T,1,K>::DVectorN( const DVector<T>& v ) : DVector<T>(v) {}

  template <typename T, class K>
  DVectorN<T,1,K>::DVectorN( const T p[] ) : DVector<T>(p) {}

  template <typename T, class K>
  void DVectorN<T,1,K>::_copy( const DVectorN<T,1>& v ) {

    if (this->_n == v._n) {

      for(int i=0;i<v._n;i++) this->_pt[i] = v._pt[i];
    }
    else {

      if(this->_pt) delete []this->_pt;
      if (v._n > 0) {

        this->_pt = new DVectorN<T,1>[v._n]; // this->_pt = new DVectorN<T,n-1>[v._n];
        for( int i = 0; i < v._n; i++ )
          this->_pt[i]=v._pt[i];
      }
      else
        this->_pt = 0x0;

      this->_n = v._n;
    }
  }

  template <typename T, class K>
  Vector<int,1>	DVectorN<T,1,K>::getDimRev() const {

    Vector<int,1> r( DVector<T>::getDim() );
    return r;
  }

//  template <typename T, class K>
//  DVector<T>*	DVectorN<T,1,K>::flat() {

//    static DVector<T>* flat_vec = new DVector<T>( DVector<T>::getDim(), 1 );

//    for( int i = DVector<T>::getDim()-1; i >= 0; i-- )
//      (*flat_vec)[i]= (*this)(i);

//    return flat_vec;
//  }

  template <typename T, class K>
  Vector<int,1>&	DVectorN<T,1,K>::getDim() const {

    return getDimRev();
  }

  template <typename T, class K>
  T DVectorN<T,1,K>::getValue(int i) {

    return this->getPtr()[i];
  }

  template <typename T, class K>
  void DVectorN<T,1,K>::setDim(const Vector<int,1>& i) {

    DVector<T>::setDim( i(0) );
  }

  template <typename T, class K>
  void DVectorN<T,1,K>::setValue(int i, T val) {

    this->getPtr()[i]= val;
  }

  /***************************************************************************************************
  * Stoping condition for function operator* for matrix multiplication.
  ****************************************************************************************************/
  template<typename T, class K>
  inline
  DVectorN<T,1>& DVectorN<T,1,K>::operator * ( const DMatrix<K>& m ) {

    static DVectorN<T,1> _vect;

    if( m.dim1() == getDimRev()[0] ) {

      _vect.setDim( m.dim1() );
      for( int j = 0; j < getDimRev()[0]; j++ ) {

        _vect[j] = T(0.0);
        for( int i = 0; i < getDimRev()[0]; i++ )
          _vect[j] += (*this)(i) * m(i)(j);
      }
    }

    return _vect;
  }


} // END namespace GMlib
