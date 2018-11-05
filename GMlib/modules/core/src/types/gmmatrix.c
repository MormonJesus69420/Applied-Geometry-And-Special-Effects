/**********************************************************************************
**
** Copyright (C) 1994 - 2017 University of Tromsø - The Arctic University of Norway
** Contact: GMlib Online Portal at https://source.uit.no/gmlib/gmlib/wikis/home
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





#include <cmath>

namespace GMlib {


  template <typename T, int n, int m>
  Vector<T,n>	Matrix<T,n,m>::_c;

  template <typename T, int n, int m>
  M_I_<T,n,m>	Matrix<T,n,m>::_I;




  /*! const Vector<T,n>& Matrix<T,n,m>::col(int j)const
   *  \brief  col
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  const Vector<T,n>& Matrix<T,n,m>::getStatCol(int j)const {

    GM_Static2_<T,n,m>::eq1(_c.getPtr(),getPtr()+j); return _c;
  }




  /*! const Vector<T,m>& Matrix<T,n,m>::row(int i)const
   *  \brief  row
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  const Vector<T,m>& Matrix<T,n,m>::getStatRow(int i)const {

    return (*this)(i);
  }




  /*! void Matrix<T,n,m>::cpy()
   *  \brief  cpy
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::cpy() {

    memcpy(getPtr(),_I.getPtr(),sizeof(Matrix<T,n,m>));
  }




  /*! void Matrix<T,n,m>::cpy(const T* v)
   *  \brief  cpy
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::cpy(const T* v) {

    memcpy(getPtr(), v, sizeof(Matrix<T,n,m>));
  }




  /*! void Matrix<T,n,m>::cpy(const Vector<Vector<T,m>,n>& v)
   *  \brief  cpy
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::cpy(const Vector<Vector<T,m>,n>& v)	{

    memcpy(getPtr(), v.getPtr(),sizeof(Matrix<T,n,m>));
  }




  /*! void Matrix<T,n,m>::cpy_t(const T* v)
   *  \brief  cpy
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::cpy_t(const T* v) {

    GM_Static_P_<T,n,m>::eq_t(getPtrP(),v);
  }




  /*! void Matrix<T,n,m>::cpy_t(const Vector<Vector<T,n>,m>& v)
   *  \brief  cpy
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::cpy_t(const Vector<Vector<T,n>,m>& v)	{

    GM_Static_P_<T,n,m>::eq_t( getPtrP(), static_cast<T*>(v.getPtr()) );
  }




  /*! void Matrix<T,n,m>::cpy_r(const Point<T,m>& p,int i)
   *  \brief  cpy
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::cpy_r(const Point<T,m>& p,int i) {

    (*this)[i] = p;
  }




  /*! void Matrix<T,n,m>::cpy_c(const Point<T,n>& p,int j)
   *  \brief  cpy
   *
   *  Detailed description of
   *  the function
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::cpy_c(const Point<T,n>& p,int j) {

    GM_Static2_<T,n,m>::eq2(p.getPtr(),getPtr()+j);
  }




  /*! Matrix<T,n,m>::Matrix()
   *  \brief  Default Constructor
   *
   *  The constructer gives an I-matrix
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m>::Matrix() {

    cpy();
  }




  /*! Matrix<T,n,m>::Matrix(bool i)
   *  \brief  The fast constructor
   *
   *  The constructer gives random contents, there will be no initializing
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m>::Matrix(bool /*i*/)	{
  }




  /*! Matrix<T,n,m>::Matrix(const Point<T,m>& r,const Point<T,n>& c)
   *  \brief constructor
   *
   *  The tensor (outer) product constructor
   *  The constructer gives an tensor (outer) product of the two
   *  vectors that is the parameters
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m>::Matrix(const Point<T,m>& r,const Point<T,n>& c) {

    GM_Static_P_<T,n,m>::vv_ox(getPtrP(), &r, c.getPtr());
  }




  /*! Matrix<T,n,m>::Matrix(const Vector<T,m>& u)
   *  \brief constructor
   *
   *  A constructor generating an orthonormal set of rows
   *	The constructer gives an arbitary orthonormal set of rows
   *	constructed from one vector that is the parameter
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m>::Matrix(const Vector<T,m>& u) {

    setOrtho(u);
  }




  /*! Matrix<T,n,m>::Matrix(const Vector<T,m>& u)
   *  \brief constructor
   *
   *  A constructor generating an orthonormal set of rows
   *	The constructer gives an arbitary orthonormal set of rows
   *	constructed from two vectors that is the parameters
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m>::Matrix(const Vector<T,m>& u, const Vector<T,m>& v)	{

    setOrtho(u,v);
  }




  /*! Matrix<T,n,m>::Matrix(const Vector<T,m>& u)
   *  \brief The copy constructor
   *
   *  The constructer gives a copy of the input Vector<Vector> i.e. matrix
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m>::Matrix( const Vector<Vector<T,m>,n>& v )	{

    cpy(v);
  }




  /*! Matrix<T,n,m>::Matrix(const Vector<T,m>& u)
   *  \brief The transpose copy constructor
   *
   *  The constructer gives a transpoded copy of the input Vector<Vector>, i.e. matrix
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m>::Matrix(const Vector<Vector<T,n>,m>& v, bool trans)	{

    if(trans) cpy_t(v); else (n == m?cpy(v):cpy_t(v));
  }




  /*! Matrix<T,n,m>::Matrix(const Vector<T,m>& u)
   *  \brief The transpose constructor
   *
   *  The constructer gives a transpoded copy of the input classical vector, i.e. matrix
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m>::Matrix(const T* v, bool trans)	{

    if(trans) cpy_t(v); else cpy(v);
  }




  /*! void Matrix<T,n,m>::reset()
   *  \brief Reset to "I-matrix"
   *
   *  The matrix is not nessesarly a square matrix
   *	but the biggest possible square submatrix is set to an I-matrix
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::reset() {
    cpy();
  }




  /*! \brief Gives a T* pointer
   *  The matrix can be treated as a classic n*m-dim array of T
   */
  template <typename T, int n, int m>
  inline
  T* Matrix<T,n,m>::getPtr() {

    return reinterpret_cast<T*>(this);
  }




  /*! \brief Gives a const T* pointer
   *  The matrix can be treated as a classic n*m-dim array of T
   */
  template <typename T, int n, int m>
  inline
  const T* Matrix<T,n,m>::getPtr() const	{

    return reinterpret_cast<const T*>(this);
  }




  /*! \brief Gives a Point<T,m>* pointer into the matrix
   *
   *  The matrix can be treated as a classic n-dim of
   *  Point<T,n> *
   */
  template <typename T, int n, int m>
  inline
  Point<T,m>* Matrix<T,n,m>::getPtrP() {

    return reinterpret_cast<Point<T,m>*>(this);
  }




  /*! \brief Gives a Point<T,m>* const-pointer into the matrix
   *
   *  The matrix can be treated as a classic n-dim of
   *  Point<T,n> *
   */
  template <typename T, int n, int m>
  inline
  const Point<T,m>* Matrix<T,n,m>::getPtrP() const	{

    return reinterpret_cast<const Point<T,m>*>(this);
  }




  /*! Matrix<T,n,m>&	Matrix<T,n,m>::operator=(const Vector<Vector<T,m>,n>& m)
   *  \brief The operator = m
   *
   *  The operator =, i.e. it makes a copy of m. It also retur a referens to itself
   *  that is to use a=b=m.
   */
  template <typename T, int n, int m>
  inline
  const Matrix<T,n,m>&	Matrix<T,n,m>::operator=(const Vector<Vector<T,m>,n>& mat)	{

    cpy(mat);   return(*this);
  }




  /*! Matrix<T,n,m>&	Matrix<T,n,m>::setTransposed(const Vector<Vector<T,m>,n>& m)
   *  \brief Set this matrix equal the transposed of matrix m
   *
   *  Set this matrix equal the transposed of matrix m
   *	Transpose this matrix. It also retur a referens to itself
   *	that is tu usa a=b=c.
   */
  template <typename T, int n, int m>
  inline
  const Matrix<T,n,m>&	Matrix<T,n,m>::setTransposed(const Vector<Vector<T,m>,n>& mat) {

    cpy_t(mat); return(*this);
  }




  /*! Matrix<T,n,m>	Matrix<T,n,m>::transposed() const
   *  \brief Makes a transposed copy
   *
   *  It does not change itself.
   *  \return Returns a transposed copy
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,m,n>	Matrix<T,n,m>::getTransposed() const	{

    return Matrix<T,m,n>(*this,true);
  }




  /*! void Matrix<T,n,m>::setCol(const Point<T,n>& c, int i)
   *  \brief  setCol
   *
   *  Put vector c at column number i
   *	Column number i is set equal to vector c, it makes a transposed copy.
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::setCol(const Point<T,n>& c, int i) {

    cpy_c(c,i);
  }




  /*! void Matrix<T,n,m>::setRow(const Point<T,m>& r, int j)
   *  \brief  setRow
   *
   *  Put vector r at row number j
   *	Row number j is set equal to vector r, it makes a copy.
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::setRow(const Point<T,m>& r, int j) {

    cpy_r(r,j);
  }




  /*! void Matrix<T,n,m>::initCol(int i, int j)
   *  \brief  initCol
   *
   *  Initiate column number i
   *	Column number i is initated to zero at all elements except for
   *	element number j that is set to 1.
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::initCol(int i, int j)	{

    GM_Static2_<T,n,m>::eq3(this+i, _I+j);
  }




  /*! void Matrix<T,n,m>::initRow(int i, int j)
   *  \brief  initRow
   *
   *  Initiate row number i
   *	Row number i is initated to zero at all elements except for
   *	element number j that is set to 1.
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::initRow(int i, int j)	{

    memcpy((*this)[i].getPtr(), _I.getPtr()+j*m, sizeof(Vector<T,m>));
  }




  /*! void Matrix<T,n,m>::initRow(int i, int j)
   *  \brief  getCol
   *
   *  Returns column number i
   *	Column number i is returned, but is set to a constant
   *	so it is not possible to change.
   */
  template <typename T, int n, int m>
  inline
  Point<T,n>const& Matrix<T,n,m>::getCol(int i) const	{

    return getStatCol(i);
  }




  /*! Point<T,m>const& Matrix<T,n,m>::getRow(int j) const
   *  \brief  getRow
   *
   *  Returns Row number j
   *	Row number j is returned, but is set to a constant
   *	so it is not possible to change.
   */
  template <typename T, int n, int m>
  inline
  Point<T,m>const& Matrix<T,n,m>::getRow(int j) const	{

    return getStatRow(j);
  }




  /*! void Matrix<T,n,m>::scale(const Point<T,n>& s)
   *  \brief  scale
   *
   *  Scaling the matrix row by row
   *  Row number i is scaled with s[i].
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::scale(const Point<T,n>& s) {

    GM_Static_P_<T,n,m>::sc(getPtrP(),s.getPtr());
  }




  /*! Matrix<T,n,m>& Matrix<T,n,m>::operator+=(const Matrix<T,n,m>& m)
   *  \brief
   *
   *  K = T += M
   *  This matrix (T) is updated by summing with matrix M, it is also
   *  possible to do: K = T += M, where K is another matrix,
   *  and which gives K = T and T = old T + M.
   */
  template <typename T, int n, int m>
  inline
  const Matrix<T,n,m>& Matrix<T,n,m>::operator+=(const Matrix<T,n,m>& mat) {

    return static_cast<const Matrix<T,n,m>&>(Vector<Vector<T,m>,n>::operator +=(mat));
  }




  /*! Matrix<T,n,m>& Matrix<T,n,m>::operator-=(const Matrix<T,n,m>& m)
   *  \brief
   *
   *  K = T -= M
   *  This matrix (T) is updated by subtract with matrix M, it is also
   *	possible to do: K = T -= M, where K is another matrix,
   *	and which gives K = T and T = old T - M.
   */
  template <typename T, int n, int m>
  inline
  const Matrix<T,n,m>& Matrix<T,n,m>::operator-=(const Matrix<T,n,m>& mat) {

    return static_cast<const Matrix<T,n,m>&>(Vector<Vector<T,m>,n>::operator -=(mat));
  }




  /*! Matrix<T,n,m> Matrix<T,n,m>::operator+(const Matrix<T,n,m>& mat) const
   *  \brief
   *
   *  K = T + M
   *  Summing this matrix (T) and matrix M, this matrix is not changed. It is
   *  to do: K = T + M, where K is another matrix.
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m> Matrix<T,n,m>::operator+(const Matrix<T,n,m>& mat) const	{

    return static_cast< Matrix<T,n,m> >(Vector<Vector<T,m>,n>::operator +(mat));
  }




  /*! Matrix<T,n,m>& Matrix<T,n,m>::operator-(const Matrix<T,n,m>& m) const
   *  \brief
   *
   *  K = T - M
   *	Subtract matrix M from this matrix (T), this matrix is not changed. It is
   *	to do: K = T - M, where K is another matrix.
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m> Matrix<T,n,m>::operator-(const Matrix<T,n,m>& mat) const	{

    return static_cast< Matrix<T,n,m> >(Vector<Vector<T,m>,n>::operator -(mat));
  }




  /*! Matrix<T,n,m>& Matrix<T,n,m>::operator-() const
   *  \brief  operator -
   *
   *  K = -T
   *	Returning this matrix (T) with changed sign. NB, this matrix is not changed. It is
   *	to do: K = -T, where K is another matrix.
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m> Matrix<T,n,m>::operator-() const	{

    return static_cast< Matrix<T,n,m> >(Vector<Vector<T,m>,n>::operator -());
  }




  /*! Point<T,n>&  Matrix<T,n,m>::operator*(const Point<T,m>& v) const
   *  \brief  operator *
   *
   *  p =  T * q
   *	Returning a point: p =  T * q, where p is a point in R^n and q is a point i R^m
   *	and T is this matrix.
   */
  template <typename T, int n, int m>
  inline
  const Point<T,n>&  Matrix<T,n,m>::operator*(const Point<T,m>& v) const {

    GM_Static_P_<T,n,m>::mv_x(_c.getPtr(),getPtrP(),v); return _c;
  }




  /*! Matrix<T,n,m>& Matrix<T,n,m>::operator*=(const double d)
   *  \brief  operator *=
   *
   *  K = T *= d
   *	Scaling this matrix, T *= d. It is also returning a referens to itself
   *			so we can do the following: K = T *= d.
   */
  template <typename T, int n, int m>
  inline
  const Matrix<T,n,m>& Matrix<T,n,m>::operator*=(const double d) {

    return static_cast<const Matrix<T,n,m>&>(Vector<Vector<T,m>,n>::operator *=(d));
  }




  /*! Matrix<T,n,m>& Matrix<T,n,m>::operator*(const double d) const
   *  \brief  operator *
   *
   *	K = S * d
   *  Returns a scaled matrix, K = S * d. NB, this matrix (S) is not scaled.
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m> Matrix<T,n,m>::operator*(const double d) const {

    return static_cast< Matrix<T,n,m> >(Vector<Vector<T,m>,n>::operator *(d));
  }




  /*! Matrix<T,n,m>& Matrix<T,n,m>::operator/=(double d)
   *  \brief  operator /=
   *
   *	T /= d is equal T *= 1/d
   *  Scaling this matrix, T /= d. It is also returning a referens to itself
   *  so we can do the following: K = T /= d.
   */
  template <typename T, int n, int m>
  inline
  const Matrix<T,n,m>& Matrix<T,n,m>::operator/=(double d) {

    return static_cast<const Matrix<T,n,m>&>(Vector<Vector<T,m>,n>::operator /=(d));
  }




  /*! Matrix<T,n,m>& Matrix<T,n,m>::operator/(double d) const
   *  \brief  operator /
   *
   *	K = T/d is equal to K = T * 1/d
   *  Returns a scaled matrix, K = T/d. NB, this matrix (T) is not scaled.
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,m> Matrix<T,n,m>::operator/(double d) const {

    return static_cast< Matrix<T,n,m> >(Vector<Vector<T,m>,n>::operator /(d));
  }




  /*! bool Matrix<T,n,m>::operator < ( const Matrix<T,n,m>& )
   *
   *  Dummy comparison operator for Array
   */
  template <typename T, int n, int m>
  inline
  bool Matrix<T,n,m>::operator < ( const Matrix<T,n,m>& /*m*/ ) const {

    return true;
  }




  /*! void Matrix<T,n,m>::swap(int i,int j)
   *  \brief  swap
   *
   *	Exchange two elements in this matrix, m[i][j] is swapped with  m[j][i]
   *	m[i][j] is swapped with  m[j][i], that is a diagonal swap.
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::swap(int i,int j)	{

    T tmp = (*this)[i][j]; (*this)[i][j] = (*this)[j][i]; (*this)[j][i] = tmp;
  }




  /*! void Matrix<T,n,m>::swap(int i,int j,int a,int b)
   *  \brief  swap
   *
   *	Exchange two elements in this matrix, m[i][j] is swapped with  m[a][b]
   *	m[i][j] is swapped with  m[a][b], that is a general swap.
   *  Also the previous diagonal swap(i,j) is equal to the general swap(i,j,j,i)
   */
  template <typename T, int n, int m>
  inline
  void Matrix<T,n,m>::swap(int i,int j,int a,int b)	{

    T tmp = (*this)[i][j]; (*this)[i][j] = (*this)[a][b]; (*this)[a][b] = tmp;
  }




  /*! Matrix<T,n,n> Matrix<T,n,m>::getMultTrans()
   *  \brief Matrix member multiplication: (*this) * (*this)T
   *
   *  Matrix member multiplication: (*this) * (*this)T
   *
   *  \return Matrix Multiplied with the transposed of itself
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,n,n> Matrix<T,n,m>::getMultTrans() {

    Matrix<T,n,n> r;
    GM_Static_P2_<T,n,m,n>::mm_xT(r.getPtrP(),getPtrP(),getPtr());
    return r;
  }




  /*! Matrix<T,m,m> Matrix<T,n,m>::getTransMult()
   *  \brief Matrix member multiplication: (*this)T * (*this)
   *
   *  Matrix member multiplication: (*this)T * (*this)
   *
   *  \return The transposed matrix multiplied with itself
   */
  template <typename T, int n, int m>
  inline
  Matrix<T,m,m> Matrix<T,n,m>::getTransMult() {

    Matrix<T,m,m> r;
    GM_Static_P2_<T,m,n,m>::mm_xTT(r.getPtrP(),getPtr(),getPtr());
    return r;
  }




  /*! void Matrix<T,n,m>::setOrtho(const Vector<T,m>& u)
   *  \brief Set the matrix orthonormal using a Vector
   *
   *  Make an orthonormal set of basis-vectors using one vector u  as a start.
   *
   *  \param[in] u The start Vector
   */
  template <class T, int n, int m>
  void Matrix<T,n,m>::setOrtho(const Vector<T,m>& u) {

    int i, j, k, ku = u.getMaxAbsIndex();

    GM_Static_<T,m>::sc_r(getPtr(), u.getPtr(), (1/u.getLength()));

    for(k=0,i=1;i<n;i++,k++)
    {
      if(k==ku) k++;
      initRow(i, k);
    }

    for(i=1;i<n;i++)
    {
      for(j=0; j<i; j++)
      {
        T tmp = GM_Static_<T,m>::dpr((*this)[i].getPtr(),(*this)[j].getPtr());
        GM_Static_<T,m>::decomp((*this)[i].getPtr(),(*this)[j].getPtr(),tmp);
      }
      (*this)[i].normalize();
    }
  }




  /*! void Matrix<T,n,m>::makeMeOrtho(const Vector<T,m>& u, const Vector<T,m>& v)
   *  \brief Set the matrix orthonormal using two Vector
   *
   *  Make an orthonormal set of basis-vectors using two vectors u  and v as a start.
   *
   *  \param[in] u The first start Vector
   *  \param[in] v The second start Vector
   */
  template <class T, int n, int m>
  void Matrix<T,n,m>::setOrtho(const Vector<T,m>& u, const Vector<T,m>& v) {

    int i, j, k, ku, kv;

    GM_Static_<T,m>::sc_r(getPtr(), u.getPtr(), (1/u.getLength()));
    GM_Std_<T,m>::ortoNormal((*this)[1].getPtr(), v.getPtr(), getPtr());
    ku = (*this)[0].getMaxAbsIndex();
    kv = (*this)[1].getMaxAbsIndex();


    for(k=0,i=2;i<n;i++,k++)
    {
      if(k==ku) k++;
      if(k==kv)
      {
        k++;
        if(k==ku) k++;
      }
      initRow(i, k);
    }

    for(i=2;i<n;i++)
    {
      for(j=0; j<i; j++)
      {
        T tmp = GM_Static_<T,m>::dpr((*this)[i].getPtr(),(*this)[j].getPtr());
        GM_Static_<T,m>::decomp((*this)[i].getPtr(),(*this)[j].getPtr(),tmp);
      }
      (*this)[i].normalize();
    }
  }




  template <class T, int n, int m>
  template <typename G>
  Matrix<T,n,m>::operator Matrix<G,n,m> () const {
    Matrix<G,n,m> v;
    GM_Static1_<G,T,n*m>::eq( v.getPtr(), getPtr() );
    return v;
  }




  template <class T, int n, int m>
  template <typename G>
  Matrix<G,n,m> Matrix<T,n,m>::toType() const {
    Matrix<G,n,m> v;
    GM_Static1_<G,T,n*m>::eq( v.getPtr(), getPtr() );
    return v;
  }




  /*! SqMatrix<T, n>::nSqMatrix(): Matrix<T,n,n>()
   *  \brief The default constructor
   *
   *  The constructer gives an I-matrix
   */
  template <typename T, int n>
  inline
  SqMatrix<T, n>::SqMatrix() : Matrix<T,n,n>() {
  }


  /*! SqMatrix<T, n>::SqMatrix(bool i): Matrix<T,n,n>(i)
   *  \brief The fast constructor
   *
   *  The constructer gives random contents, there will be no initializing
   */
  template <typename T, int n>
  inline
  SqMatrix<T, n>::SqMatrix(bool i): Matrix<T,n,n>(i) {
  }




  /*! SqMatrix<T, n>::SqMatrix(const Point<T,n>& r, const Point<T,n>& c): Matrix<T,n,n>(r,c)
   *  \brief The tensor (outer) product constructor
   *
   *  The constructer gives an tensor (outer) product of the two vectors that is the parameters
   */
  template <typename T, int n>
  inline
  SqMatrix<T, n>::SqMatrix(const Point<T,n>& r, const Point<T,n>& c): Matrix<T,n,n>(r,c) {
  }




  /*! SqMatrix<T, n>::SqMatrix(const Vector<Vector<T,n>,n>& m): Matrix<T,n,n>(m)
   *  \brief The copy constructor
   *
   *  The constructer gives a copy of the input Vector<Vector> i.e. matrix
   */
  template <typename T, int n>
  inline
  SqMatrix<T, n>::SqMatrix(const Vector<Vector<T,n>,n>& m): Matrix<T,n,n>(m) {
  }




  /*! SqMatrix<T, n>::SqMatrix(const Vector<Vector<T,n>,n>& m, bool trans): Matrix<T,n,n>(m,trans)
   *  \brief The transpose copy constructor
   *
   *  The constructer gives a transposed copy of the input matrix if "trans" is true.
   */
  template <typename T, int n>
  inline
  SqMatrix<T, n>::SqMatrix(const Vector<Vector<T,n>,n>& m, bool trans): Matrix<T,n,n>(m,trans) {
  }




  /*! SqMatrix<T, n>::SqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v)
   *  \brief The copy or transpose constructor
   *
   *  If true, the constructer gives a transpoded copy of the input
   *  classical vector, i.e. matrix, else it only makes a
   *  matrix copy of the classical vactor.
   *
   *  Make a rotation matrix in the plane span by vectors u and v.
   */
  template <typename T, int n>
  inline
  SqMatrix<T, n>::SqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v){
    makeOrtho(u, v, *this);
    Matrix<T,n,n> x(*this);
    rotateXY(a);
    basisChange(x);
  }




  /*! SqMatrix<T, n>::SqMatrix(const T* m, bool trans): Matrix<T,n,n>(m,trans)
   *  \brief The rotation constructor
   *
   *  The constructer gives a rotation matrix rotating in one
   *  of the main planes (x,y),
   *  i.e. the default (0,1) - is the xy-plane. Remark the for example (0,1) and (1,0)
   *  is rotation in opposit direction.
   */
  template <typename T, int n>
  inline
  SqMatrix<T, n>::SqMatrix(const T* m, bool trans): Matrix<T,n,n>(m,trans) {}




  /*! SqMatrix<T, n>::SqMatrix(Angle a, int x = 0, int y=1)
   *  \brief The general rotation constructor
   *
   *  The constructer gives a rotation matrix rotating in the plane
   *  spanned by the two vectors u and v.
   *
   *  Make a rotation matrix in one of the main planes (x,y) (0,1)-is xy-plane.
   *  Remark the for example (0,1) and (1,0) is rotation in opposit direction.
   */
  template <typename T, int n>
  inline
  SqMatrix<T, n>::SqMatrix(Angle a, int x, int y) {
    T sina = T(sin(a.getRad())), cosa = T(cos(a.getRad()));
    (*this)[x][x]=cosa; (*this)[y][x]=-sina; (*this)[x][y]=sina; (*this)[y][y]=cosa;
  }




  template <typename T, int n>
  inline
  Matrix<T,n,n> const& SqMatrix<T, n>::transpose() {
    GM_Static3_<T,n-1,n>::trn(( this->getPtrP()+1)->getPtr(), this->getPtr()+1);
    return(*this);
  }//Matrix<T,n,n> v(*this,true); *this = v; return(*this);}


  /*! Matrix<T,n,n> const& SqMatrix<T, n>::transposeMult(const Matrix<T,n,n>& m) const
   *  \brief Mutiplicate transpose of this matrix to matrix m: (*this) = T(*this) *  m
   *
   *  Mutiplicate transpose of this matrix to matrix m: (*this) = T(*this) *  m
   */
  template <typename T, int n>
  inline
  Matrix<T,n,n> const& SqMatrix<T, n>::transposeMult(const Matrix<T,n,n>& m) const {	// Not changing this: a = this->transpose * m
    static Matrix<T,n,n> r;
    GM_Static_P_<T,n,n>::mc_x(r.getPtr(), this->getPtr(),m.getPtr());
    return r;
  }




  /*! Matrix<T,n,n> const& SqMatrix<T, n>::reverseMult(const Matrix<T,n,n>& m)
   *  \brief Mutiplicate matrix m to this matrix in reverce order : (*this) = m * (*this)
   *
   *  Mutiplicate matrix m to this matrix in reverce order : (*this) = m * (*this)
   */
  template <typename T, int n>
  inline
  Matrix<T,n,n> const& SqMatrix<T, n>::reverseMult(const Matrix<T,n,n>& m) {		// Changing this ( is a kind of *= operator): *this = m * *this
    static Matrix<T,n,n> r;
    GM_Static_P2_<T,n,n,n>::mm_x(r.getPtrP(), m.getPtrP(), this->getPtr());
    return *this = r;
  }




  template <typename T, int n>
  inline
  void SqMatrix<T, n>::setDiagonal(const Point<T,n>& r) {
    cpy_d(r);
  }

  template <typename T, int n>
  inline
  Point<T,n> const&  SqMatrix<T, n>::getDiagonal(int i) const	{
    return getStatDiagonal(i);
  }




  /*! void SqMatrix<T, n>::invert()
   *  \brief The general inversion of a matrix using full pivoting.
   *
   *  The general inversion of a matrix using full pivoting.
   *  Ordinary LU-decomp used.
   */
  template <typename T, int n>
  inline
  void SqMatrix<T, n>::invert() {
    SqMatrix<T,n> a(*this);
    Vector<int,n> indx;
    Vector<double,n> vv;

    const double TINY=1.0e-20;
    int i,j,imax,k;
    double big,dum,sum,temp;

    double d=1.0;
    for (i=0;i<n;i++)
    {
      big=0.0;
      for (j=0;j<n;j++) if ((temp = std::fabs(a[i][j])) > big) big=temp;
    //	if (big == 0.0)  nrerror("Singular matrix in routine ludcmp");
      vv[i] = 1.0/big;
    }
    imax = 0;
    for (j=0;j<n;j++)
    {
      big=0.0;
      for (i=0;i<j;i++)
      {
        sum = a[i][j];
        for (k=0;k<i;k++) sum -= a[i][k]*a[k][j];
        a[i][j] = sum;
      }
      for (i=j;i<n;i++)
      {
        sum = a[i][j];
        for (k=0;k<j;k++) sum -= a[i][k]*a[k][j];
        a[i][j]=sum;
        if ((dum=vv[i]*std::fabs(sum)) >= big) { big=dum; imax=i; }
      }
      if (j != imax)
      {
        for (k=0;k<n;k++) a.swap(imax,k,j,k);
        d = -d;
        vv[imax]=vv[j];
      }
      indx[j] = imax;
      if (a[j][j] == 0.0) a[j][j] = TINY;
      if (j != n-1)
      {
        dum=1.0/(a[j][j]);
        for (i=j+1;i<n;i++) a[i][j] *= dum;
      }
    }									// LU-decomp. finished, stored in a[][]

    for(int cols=0; cols<n; cols++)		// LU-back subst. begins
    {
      int ii=0, ip;
      vv = T(0.0); vv[cols] = T(1.0);
      for (i=0;i<n;i++)
      {
        ip = indx[i];
        T sum2 = vv[ip];
        vv[ip] = vv[i];
        if (ii != 0)		 for (j=ii-1;j<i;j++) sum2 -= a[i][j]*vv[j];
        else if (sum2 != 0.0) ii = i+1;
        vv[i] = sum2;
      }
      for (i=n-1;i>=0;i--)
      {
        T sum2=vv[i];
        for (j=i+1;j<n;j++)  sum2 -= a[i][j]*vv[j];
        vv[i]=sum2/a[i][i];
      }
      this->setCol(vv,cols);		// inverse stored in this, a and vv is disappearing?
    }									// LU-back subst. finished,
  }




  /*! void SqMatrix<T, n>::invert2()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void SqMatrix<T, n>::invertOrthoNormal() {
    transpose();
  }	// Use only for orthonormal matrices (rotation matrices)


  /*! void SqMatrix<T, n>::rotateXY(Angle a, int x=0, int y=1)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,x,y)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,x,y)* (*this)
   */
  template <typename T, int n>
  inline
  void SqMatrix<T, n>::rotateXY(Angle a, int x, int y) {
    GM_Static_<T,n>::rot_xy(this->getPtr()+x*n,this->getPtr()+y*n, T(sin(a.getRad())), T(cos(a.getRad())));
  }




  /*! void SqMatrix<T, n>::rotation(Angle a, const Vector<T,n>& u, const Vector<T,n>& v)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   */
  template <typename T, int n>
  inline
  void SqMatrix<T, n>::rotate(Angle a, const Vector<T,n>& u, const Vector<T,n>& v) {
    Matrix<T,n,n> x;
    makeOrtho(u,v,x);
    basisChangeInv(x);
    rotateXY(a);
    basisChange(x);
  }




  /*! void SqMatrix<T, n>::basisChange(const Matrix<T,n,n>& x)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void SqMatrix<T, n>::basisChange(const Matrix<T,n,n>& x)	{
    (*this) = transposeMult(x);
  }


  /*! void SqMatrix<T, n>::basisChangeInv(const Matrix<T,n,n>& x)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void SqMatrix<T, n>::basisChangeInv(const Matrix<T,n,n>& x)	{
    (*this) = x*(*this);
  }




  // **************************
  // Protected Member Functions


  /*! const Point<T,n>& SqMatrix<T, n>::getStatDiagonal(int i) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  const Point<T,n>& SqMatrix<T, n>::getStatDiagonal(int /*i*/) const {

      GM_Static3_<T,n,n>::eq1( this->_c.getPtr(), this->getPtr() );
      return this->_c;
  }




  /*! void  SqMatrix<T, n>::cpy_d(const Point<T,n>& p)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void  SqMatrix<T, n>::cpy_d(const Point<T,n>& p) {

    GM_Static3_<T,n,n>::eq2( p.getPtr(), this->getPtr() );
  }




  /*! void SqMatrix<T, n>::makeOrtho(const Vector<T,n>& u, const Vector<T,n>& v, Matrix<T,n,n>& x)
   *  \brief To make an orthonormal set of basis-vectors
   *
   *  To make an orthonormal set of basis-vectors using
   *  vector u and vector v as a start.
   */
  template <typename T, int n>
  void SqMatrix<T, n>::makeOrtho(const Vector<T,n>& u, const Vector<T,n>& v, Matrix<T,n,n>& x) {
    int i,j,k,ku,kv;
    GM_Static_<T,n>::sc_r(x.getPtr(), u.getPtr(), (1/u.getLength()));
    GM_Std_<T,n>::ortoNormal(x[1].getPtr(), v.getPtr(), x.getPtr());
    ku = u.getMaxAbsIndex();
    kv = x[1].getMaxAbsIndex();

    for(k=0,i=2;i<n;i++,k++)
    {
      if(k==ku) k++;
      if(k==kv)
      {
        k++;
        if(k==ku) k++;
      }
      x.initRow(i, k);
    }

    for(i=2;i<n;i++)
    {
      for(j=0; j<i; j++)
      {
        T tmp = GM_Static_<T,n>::dpr(x[i].getPtr(),x[j].getPtr());
        GM_Static_<T,n>::decomp(x[i].getPtr(),x[j].getPtr(),tmp);
      }
      x[i].normalize();
    }
  }




  template <typename T, int n>
  template <typename G>
  SqMatrix<T,n>::operator SqMatrix<G,n> () const {

      SqMatrix<G,n> v;
      GM_Static1_<G,T,n*n>::eq( v.getPtr(), this->getPtr() );
      return v;
    }



//***********************************************************
//              HqMatrix                            *********
//***********************************************************


  /*! HqMatrix<T, n>::HqMatrix():SqMatrix<T,n+1>()
   *  \brief The default constructor
   *
   *  The constructer gives an I-matrix
   */
#define Hq_constr1(n) inline\
  HqMatrix<T, n>::HqMatrix():SqMatrix<T,n+1>() {}

  template <typename T, int n>
  Hq_constr1(n)

  template <typename T>
  Hq_constr1(3)




  /*! HqMatrix<T, n>::HqMatrix(bool i):SqMatrix<T,n+1>(i)
   *  \brief The fast constructor
   *
   *  The constructer gives random contents, there will be no initializing
   */
#define Hq_constr2(n) inline\
  HqMatrix<T, n>::HqMatrix(bool i):SqMatrix<T,n+1>(i) {}

  template <typename T, int n>
  Hq_constr2(n)

  template <typename T>
  Hq_constr2(3)




  /*! HqMatrix<T, n>::HqMatrix(const Matrix<T,n+1,n+1>& v):SqMatrix<T,n+1>(v)
   *  \brief The copy constructor
   *
   *  The constructer gives a copy of the input (homogenious) matrix
   */
#define Hq_constr3(n) inline\
  HqMatrix<T, n>::HqMatrix(const Matrix<T,n+1,n+1>& v):SqMatrix<T,n+1>(v)	{}

  template <typename T, int n>
  Hq_constr3(n)

  template <typename T>
  Hq_constr3(3)




  /*! HqMatrix<T, n>::HqMatrix(const Matrix<T,n+1,n+1>& v, bool trans):SqMatrix<T,n+1>(v,trans)
   *  \brief The transpose copy constructor
   *
   *  The constructer gives a transposed copy of the input (homogenious) matrix if "trans" is true.
   */
#define Hq_constr4(n) inline\
  HqMatrix<T, n>::HqMatrix(const Matrix<T,n+1,n+1>& v, bool trans):SqMatrix<T,n+1>(v,trans)	{}

  template <typename T, int n>
  Hq_constr4(n)

  template <typename T>
  Hq_constr4(3)



  /*! HqMatrix<T, n>::HqMatrix(const Matrix<T,n,n>& v, bool trans=false):SqMatrix<T,n+1>(false)
   *  \brief The  copy or transpose copy constructor from a sub matrix.
   *
   *  The constructer gives a copy (or transposed copy if "trans" is true) of the
   *  input matrix in the upper left corner.
   *  The rightmost and lowermost vectors are set to 0 exept
   *  for the right lower corner value that is 1.
   */
#define Hq_constr5(n) inline\
  HqMatrix<T, n>::HqMatrix( const Matrix<T,n,n>& v, bool trans) : SqMatrix<T,n+1>(v,trans)	{}

  template <typename T, int n>
  Hq_constr5(n)

  template <typename T>
  Hq_constr5(3)





  /*! HqMatrix<T, n>::HqMatrix(const Vector<T,n>& d)
   *  \brief The translation constructor
   *
   *  The constructer gives an homogenious translation matrix.
   */
#define Hq_constr6(n) inline\
    HqMatrix<T, n>::HqMatrix( const Vector<T,n>& d  ){\
      GM_Static2_<T,n,n+1>::eq2( d.getPtr(), this->getPtr()+n );\
    }

  template <typename T, int n>
  Hq_constr6(n)

  template <typename T>
  Hq_constr6(3)




  /*! HqMatrix<T, n>::HqMatrix(Angle a,  int x, int y)
   *  \brief The clean rotation constructor
   *
   *  The constructer gives a clean rotation matrix rotating in one
   *  of the main planes (x,y) around origin,
   *  i.e. the default (0,1) - is the xy-plane. Remark the for example (0,1) and (1,0)
   *  is rotation in opposit direction.
   */
#define Hq_constr7(n) inline\
    HqMatrix<T, n>::HqMatrix(Angle a,  int x, int y){\
      T sina = T(sin(a.getRad())), cosa = T(cos(a.getRad()));\
      (*this)[x][x]=cosa; (*this)[y][x]=-sina; (*this)[x][y]=sina; (*this)[y][y]=cosa;\
    }

  template <typename T, int n>
  Hq_constr7(n)

  template <typename T>
  Hq_constr7(3)




  /*! HqMatrix<T, n>::HqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v)
   *  \brief The clean general rotation constructor
   *
   *  The constructer gives a rotation matrix rotating in the plane
   *  spanned by the two vectors u and v, but located in origin.
   */
#define Hq_constr8(n) inline\
    HqMatrix<T, n>::HqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v){\
        makeOrtho(u, v, *this);\
        Matrix<T,n+1,n+1> x(*this);\
        rotateXY(a);\
        this->basisChange(x);\
      }

  template <typename T, int n>
  Hq_constr8(n)

  template <typename T>
  Hq_constr8(3)




  /*! HqMatrix<T, n>::HqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p)
   *  \brief The general rotation constructor
   *
   *  The constructer gives a rotation matrix rotating in the plane
   *  spanned by the two vectors u and v and that is located in the point p.
   */
#define Hq_constr9(n) inline\
    HqMatrix<T, n>::HqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p) {\
      Vector<T,n> r;\
      T sina = sin(a.getRad());\
      T cosa = cos(a.getRad());\
      makeOrtho(u, v, *this);\
      SqMatrix<T,n+1> x(*this);\
      GM_Static_<T,n>::rot_xy( this->getPtr(), this->getPtr()+(n+1), sina, cosa);\
      this->basisChange(x);\
      T x0 = GM_Static_<T,n>::dpr(p.getPtr(),x.getPtr());\
      T x1 = GM_Static_<T,n>::dpr(p.getPtr(),x[1].getPtr());\
      r[0] = (x0*cosa-x1*sina);\
      r[1] = (x0*sina+x1*cosa);\
      GM_Static_P_<T,n-2,n>::hq_2x(r.getPtr()+2,x[2].getPtr(),p);\
      GM_Static_P_<T,n,n>::hq_3x(this->getPtr()+n,x.getPtr(),r,p.getPtr());\
    }

  template <typename T, int n>
  Hq_constr9(n)

  template <typename T>
  Hq_constr9(3)





#define Hq_set_transp(n) inline\
    Matrix<T,n+1,n+1>& HqMatrix<T, n>::setTranspose(const Matrix<T,n+1,n+1>& v) {\
      cpy_t(v); return(*this);\
    }

  template <typename T, int n>
  Hq_set_transp(n)

  template <typename T>
  Hq_set_transp(3)




  /*! void HqMatrix<T, n>::rotateXY(Angle a, int x=0, int y=1)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,x,y)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,x,y)* (*this)
   */
#define Hq_rotateXY(n) inline\
    void HqMatrix<T, n>::rotateXY(Angle a, int x , int y ) {\
      GM_Static_<T,n>::rot_xy(this->getPtr()+x*(n+1),this->getPtr()+y*(n+1), T(sin(a.getRad())), T(cos(a.getRad())));\
    }

  template <typename T, int n>
  Hq_rotateXY(n)

  template <typename T>
  Hq_rotateXY(3)




  /*! void HqMatrix<T, n>::rotate(Angle a, const Vector<T,n>& u, const Vector<T,n>& v)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   */
#define Hq_rotate1(n) inline\
    void HqMatrix<T, n>::rotate(Angle a, const Vector<T,n>& u, const Vector<T,n>& v){\
      HqMatrix<T,n> x(a,u,v);\
      *this = *this * x;\
    }

  template <typename T, int n>
  Hq_rotate1(n)

  template <typename T>
  Hq_rotate1(3)




  /*! HqMatrix<T, n>::rotate(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   */
#define Hq_rotate2(n) inline\
    void HqMatrix<T, n>::rotate(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p){\
      HqMatrix<T,n> x(a,u,v,p);\
      *this = *this * x;\
    }

  template <typename T, int n>
  Hq_rotate2(n)

  template <typename T>
  Hq_rotate2(3)




  /*! void HqMatrix<T, n>::rotateGlobal(Angle a, const Vector<T,n>& u, const Vector<T,n>& v)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   */
#define Hq_rotateGlobal(n) inline\
    void HqMatrix<T, n>::rotateGlobal(Angle a, const Vector<T,n>& u, const Vector<T,n>& v){\
      HqMatrix<T,n> x(a,u,v);\
      this->reverseMult( x );\
    }

  template <typename T, int n>
  Hq_rotateGlobal(n)

  template <typename T>
  Hq_rotateGlobal(3)




  /*! HqMatrix<T, n>::rotateGlobal(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   */
#define Hq_rotateGlobal2(n) inline\
    void HqMatrix<T, n>::rotateGlobal(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p){\
      HqMatrix<T,n> x(a,u,v,p);\
      this->reverseMult( x );\
    }

  template <typename T, int n>
  Hq_rotateGlobal2(n)

  template <typename T>
  Hq_rotateGlobal2(3)





  /*! void Matrix<T,n,m>::scale(const Point<T,n>& s)
   *  \brief  scale
   *
   *  Scaling the matrix row by row
   *  Row number i is scaled with s[i].
   */
#define Hq_scale(n) inline\
    void HqMatrix<T,n>::scale(const Point<T,n>& s) {\
      GM_Static_P_<T,n,n+1>::sc(this->getPtrP(),s.getPtr());\
    }

  template <typename T, int n>
  Hq_scale(n)

  template <typename T>
  Hq_scale(3)






  /*! void HqMatrix<T, n>::translate(const Vector<T,n> d)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   */
#define Hq_translate(n) inline\
    void HqMatrix<T, n>::translate(const Vector<T,n> d) {\
      HqMatrix<T,n> m(d);\
      (*this) = (*this)*m;\
    }

  template <typename T, int n>
  Hq_translate(n)

  template <typename T>
  Hq_translate(3)




  /*! void HqMatrix<T, n>::translateGlobal(const Vector<T,n> d)
   *  \brief Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   *
   *  Add a rotation to this matrix: (*this) = SqMatrix<T,n>(a,u,v)* (*this)
   */
#define Hq_translateGlobal(n) inline\
    void HqMatrix<T, n>::translateGlobal(const Vector<T,n> d) {\
      HqMatrix<T,n> m(d);\
      this->reverseMult( m );\
    }

  template <typename T, int n>
  Hq_translateGlobal(n)

  template <typename T>
  Hq_translateGlobal(3)




  /*! const Matrix<T,n,n>& HqMatrix<T, n>::translateGlobal(const Vector<T,n> d)
   *  \brief Get a clean rotation matrix: SqMatrix<T,n> sq = this->getRotationMatrix()
   *
   *  Get a clean rotation matrix: SqMatrix<T,n> sq = this->getRotationMatrix()
   */
#define Hq_getRotationMatrix(n) inline\
    const Matrix<T,n,n>& HqMatrix<T,n>::getRotationMatrix() const {\
      static Matrix<T,n,n> rot;\
      for( int i = 0; i < 3; ++i )\
        rot[i] = (*this)(i);\
      return rot;\
    }

  template <typename T, int n>
  Hq_getRotationMatrix(n)

  template <typename T>
  Hq_getRotationMatrix(3)




  /*! void HqMatrix<T, n>::invert2()
   *  \brief Invert
   *
   *  overloaded and use only for orthonormal (n-1 x n-1) sub-matrices.
   *  Inverting this matrix using transposing the nxn upper-left submatrix)
   *  and making the right column equal - multiplication of the right column
   *  with the orginal sub-matrix
   *  NB!  ONLY ROTATION and TRANSLATION !!!!!!
   */
#define Hq_invertOrthoNormal(n) inline\
    void HqMatrix<T, n>::invertOrthoNormal() {\
      T v[n];\
      GM_Static3_<T,n,n>::eq1(v, this->getPtr()+n);\
      GM_Static3_<T,n-1,n+1>::trn(this->getPtr()+n+1, this->getPtr()+1);\
      GM_Static3_<T,n,n>::cm_x( this->getPtr()+n, this->getPtr(),v);\
    }

  template <typename T, int n>
  Hq_invertOrthoNormal(n)

  template <typename T>
  Hq_invertOrthoNormal(3)




  /*! Matrix<T,n+1,n+1>& HqMatrix<T, n>::operator=(const Matrix<T,n+1,n+1>& v)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_op_eq(n) inline\
  Matrix<T,n+1,n+1>& HqMatrix<T, n>::operator=(const Matrix<T,n+1,n+1>& v) {\
    this->cpy(v);\
    return(*this);\
  }

  template <typename T, int n>
  Hq_op_eq(n)

  template <typename T>
  Hq_op_eq(3)





  /*! Point<T,n> HqMatrix<T, n>::operator*(const Point<T,n>& v) const
   *  \brief p =  T * q, where p and q are points
   *
   *  Returning a point: p =  T * q, where p and q are points i R^n
   *  and T is this matrix.
   */
#define Hq_op_mult_p(n) inline\
    Point<T,n> HqMatrix<T, n>::operator*(const Point<T,n>& p) const {\
      Point<T,n> r;\
      GM_Static_P_<T,n,n>::mv_xqP(r.getPtr(), this->getPtr(), p, this->getPtr()+n);\
      return r;\
    }

  template <typename T, int n>
  Hq_op_mult_p(n)

  template <typename T>
  Hq_op_mult_p(3)




  /*! Vector<T,n> HqMatrix<T, n>::operator*(const Vector<T,n>& v) const
   *  \brief u =  T * v, where u and v are vectors
   *
   *  Returning a vector: u =  T * v, where u and v are vectors i R^n
   *  and T is this matrix. NB!! u is not infected by translation.
   */
#define Hq_op_mult_v(n) inline\
    Vector<T,n> HqMatrix<T, n>::operator*(const Vector<T,n>& v) const {\
      Vector<T,n> r;\
      GM_Static_P_<T,n,n>::mv_xq(r.getPtr(), this->getPtr(), v);\
      return r;\
    }

  template <typename T, int n>
  Hq_op_mult_v(n)

  template <typename T>
  Hq_op_mult_v(3)




  /*! ScalarPoint<T,n> HqMatrix<T, n>::operator*(const ScalarPoint<T,n>& v)const
   *  \brief  Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_op_mult_sp(n) inline\
    ScalarPoint<T,n> HqMatrix<T, n>::operator*(const ScalarPoint<T,n>& p) const {\
      ScalarPoint<T,n> r;\
      GM_Static_P_<T,n,n>::mv_xqP(r.getPtr(), this->getPtr(), p.getPos(), this->getPtr()+n);\
      return r;\
    }

  template <typename T, int n>
  Hq_op_mult_sp(n)

  template <typename T>
  Hq_op_mult_sp(3)




  /*! Sphere<T,n> HqMatrix<T, n>::operator*(const Sphere<T,n>& v) const
   *  \brief  Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_op_mult_s(n) inline\
    Sphere<T,n> HqMatrix<T, n>::operator*(const Sphere<T,n>& s) const{\
      Sphere<T,n> r( s.isValid() );\
      if( s.isValid()) {\
        GM_Static_P_<T,n,n>::mv_xqP(r.getPtr(), this->getPtr(), s.getPos(), this->getPtr()+n);\
        Vector<T,n> v(T(0));\
        v[0]= s.getRadius();\
        v = (*this) * v;\
        r.resetRadius( v.getLength() );\
      }\
      return r;\
    }

  template <typename T, int n>
  Hq_op_mult_s(n)

  template <typename T>
  Hq_op_mult_s(3)




  /*! Arrow<T,n> HqMatrix<T, n>::operator*(const Arrow<T,n>& v) const
   *  \brief  Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_op_mult_a(n) inline\
    Arrow<T,n> HqMatrix<T, n>::operator*(const Arrow<T,n>& a) const{\
      Arrow<T,n> r;\
      GM_Static_P_<T,n,n>::mv_xqP(r.getPtr(),  this->getPtr(), a.getPos(), this->getPtr()+n);\
      GM_Static_P_<T,n,n>::mv_xq(r.getPtr()+n, this->getPtr(), a.getDir());\
      return r;\
    }

  template <typename T, int n>
  Hq_op_mult_a(n)

  template <typename T>
  Hq_op_mult_a(3)




  /*! Box<T,n> HqMatrix<T, n>::operator*(const Box<T,n>& v) const
   *  \brief  Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_op_mult_b(n) inline\
    Box<T,n>		HqMatrix<T, n>::operator*(const Box<T,n>& b)		const {\
      Box<T,n> r;\
      GM_Static_P_<T,n,n>::mv_xqP(r.getPtr(),   this->getPtr(), b.getPtr(),   this->getPtr()+n);\
      GM_Static_P_<T,n,n>::mv_xqP(r.getPtr()+n, this->getPtr(), b.getPtr()+n, this->getPtr()+n);\
      return r;\
    }

  template <typename T, int n>
  Hq_op_mult_b(n)

  template <typename T>
  Hq_op_mult_b(3)




  /*! Matrix<T,n+1,n+1>const&	HqMatrix<T, n>::operator*(const HqMatrix<T,n>& v)	const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_op_mult_hq(n) inline\
    Matrix<T,n+1,n+1>	HqMatrix<T, n>::operator*(const HqMatrix<T,n>& v)	const {\
      return (*static_cast<Matrix<T,n+1,n+1>const*>(this)) * (static_cast<const Matrix<T,n+1,n+1>&>(v));\
    }

  template <typename T, int n>
  Hq_op_mult_hq(n)

  template <typename T>
  Hq_op_mult_hq(3)




  /*! HqMatrix<T,n>::operator HqMatrix<G,n> () const
   *  \brief Pending Documentation
   *
   *  Casting operator: HqMatrix<T,n> a; HqMatrix<G,n> b = a;
   */
#define Hq_op_cast_gen(n) inline\
    HqMatrix<T,n>::operator HqMatrix<G,n> () const  {\
      HqMatrix<G,n> v;\
      GM_Static1_<G,T,(n+1)*(n+1)>::eq( v.getPtr(), this->getPtr() );\
      return v;\
    }

  template <typename T, int n>
  template <typename G>
  Hq_op_cast_gen(n)

  template <typename T>
  template <typename G>
  Hq_op_cast_gen(3)





  /*! HqMatrix<T,n>::operator HqMatrix<G,n> () const
   *  \brief Pending Documentation
   *
   *  Casting operator: HqMatrix<T,n> a; HqMatrix<G,n> b = a;
   */
#define Hq_cast_gen(n) inline\
    HqMatrix<G,n> HqMatrix<T,n>::toType() const  {\
      return this->operator HqMatrix<G,n>();\
    }

  template <typename T, int n>
  template <typename G>
  Hq_cast_gen(n)

  template <typename T>
  template <typename G>
  Hq_cast_gen(3)





  // **************************
  // Protected Member Functions


  /*! void HqMatrix<T, n>::hq_cpy(const Matrix<T,n,n>& v)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_cpy(n) inline\
    void HqMatrix<T, n>::hq_cpy(const Matrix<T,n,n>& v) {\
      memcpy( this->getPtr(), v.getPtr(), sizeof(Matrix<T,n,n>) );\
    }

  template <typename T, int n>
  Hq_cpy(n)

  template <typename T>
  Hq_cpy(3)




  /*! void HqMatrix<T, n>::hq_cpy_t(const Matrix<T,n,n>& v)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_cpy_t(n) inline\
    void HqMatrix<T, n>::hq_cpy_t(const Matrix<T,n,n>& v) {\
      GM_Static_P_<T,n,n>::eq_t( this->getPtrP(), v.getPtr() );\
    }

  template <typename T, int n>
  Hq_cpy_t(n)

  template <typename T>
  Hq_cpy_t(3)




  /*! void HqMatrix<T, n>::hq_cpy_r(const Point<T,n>& p,int i)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_cpy_r(n) inline\
    void HqMatrix<T, n>::hq_cpy_r(const Point<T,n>& p,int i) {\
      memcpy((*this)[i].getPtr(),p.getPtr(),sizeof(Point<T,n>));\
    }

  template <typename T, int n>
  Hq_cpy_r(n)

  template <typename T>
  Hq_cpy_r(3)




  /*! void HqMatrix<T, n>::hq_cpy_c(const Point<T,n>& p,int j)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
#define Hq_cpy_c(n) inline\
    void HqMatrix<T, n>::hq_cpy_c(const Point<T,n>& p,int j) {\
      GM_Static3_<T,n,n>::eq2(p.getPtr(),this->getPtr()+j);\
    }

  template <typename T, int n>
  Hq_cpy_c(n)

  template <typename T>
  Hq_cpy_c(3)




  /*! void HqMatrix<T, n>::makeOrtho(const Vector<T,n>& u, const Vector<T,n>& v, Matrix<T,n+1,n+1>& x)
   *  \brief To make an orthonormal set of basis-vectors using vector u and vector v as a start.
   *
   *  To make an orthonormal set of basis-vectors using vector u and vector v as a start.
   */
#define Hq_makeOrtho(n) inline\
    void HqMatrix<T, n>::makeOrtho(const Vector<T,n>& u, const Vector<T,n>& v, Matrix<T,n+1,n+1>& x) {\
      int i,j,k,ku,kv;\
      GM_Static_<T,n>::sc_r(x.getPtr(), u.getPtr(), (1/u.getLength()));\
      x[0][n]=T(0);\
      GM_Std_<T,n>::ortoNormal(x[1].getPtr(),v.getPtr(), x.getPtr());\
      x[1][n]=T(0);\
      ku = u.getMaxAbsIndex();\
      kv = x[1].getMaxAbsIndex();\
      for(k=0,i=2; i<n; i++,k++) {\
        if(k==ku) k++;\
        if(k==kv) {\
          k++;\
          if(k==ku) k++;\
        }\
        x.initRow(i, k);\
      }\
      for(i=2;i<n;i++) {\
        for(j=0; j<i; j++) {\
          T tmp = GM_Static_<T,n>::dpr(x[i].getPtr(),x[j].getPtr());\
          GM_Static_<T,n>::decomp(x[i].getPtr(),x[j].getPtr(),tmp);\
        }\
        x[i].normalize();\
      }\
      x.initRow(n,n);\
    }

  template <typename T, int n>
  Hq_makeOrtho(n)

  template <typename T>
  Hq_makeOrtho(3)



//****************************************************************
//*****     Functions for specialized HqMatrix<T,3>     **********
//****************************************************************



  /*! HqMatrix<T,3>::HqMatrix<T,3>(Angle a, const Vector<T,3>& rot_axis)
   *  \brief The clean general rotation constructor
   *
   *  The constructer gives a rotation matrix rotating in the plane
   *  defined by a normal vectors rot_axis, but located in origin.
   */
  template <typename T>
  inline
  HqMatrix<T,3>::HqMatrix(Angle a, const Vector<T,3>& rot_axis){

    Vector<float,3> lu = rot_axis.getLinIndVec();
    Vector<float,3> u  = lu ^ rot_axis;
    Vector<float,3> v  = rot_axis ^ u;
    this->makeOrtho(u, v, *this);
    Matrix<T,4,4> x(*this);
    this->rotateXY(a);
    this->basisChange(x);
  }




  /*! HqMatrix<T,3>::HqMatrix<T,3>(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p)
   *  \brief The general rotation constructor
   *
   *  The constructer gives a rotation matrix rotating in the plane
   *  defined by a normal vectors rot_axis and that is located in the point p.
   */
  template <typename T>
  inline
  HqMatrix<T,3>::HqMatrix(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p) {

    Vector<float,3> lu = rot_axis.getLinIndVec();
    Vector<float,3> u  = lu ^ rot_axis;
    Vector<float,3> v  = rot_axis ^ u;
    Vector<T,3> r;
    T sina = sin(a.getRad());
    T cosa = cos(a.getRad());
    this->makeOrtho(u, v, *this);
    SqMatrix<T,4> x(*this);
    GM_Static_<T,3>::rot_xy( this->getPtr(), this->getPtr()+(4), sina, cosa);
    this->basisChange(x);

    T x0 = GM_Static_<T,3>::dpr(p.getPtr(),x.getPtr());
    T x1 = GM_Static_<T,3>::dpr(p.getPtr(),x[1].getPtr());
    r[0] = (x0*cosa-x1*sina);
    r[1] = (x0*sina+x1*cosa);
    GM_Static_P_<T,1,3>::hq_2x(r.getPtr()+2,x[2].getPtr(),p);
    GM_Static_P_<T,3,3>::hq_3x(this->getPtr()+3,x.getPtr(),r,p.getPtr());
  }




  /*!
   *  \brief Creates a rotation matrix from a Quaternion
   *
   * Creates a rotation matrix from a quaternion.
   * Using homogeneous quaternion coordinates, which preserves orthogonality.
   */
  template <typename T>
  inline
  HqMatrix<T,3>::HqMatrix(const Quaternion<T>& q) {

    // Homogeneous expression
    (*this)[0][0] = std::pow(q(0),2) + std::pow(q(3),2) - std::pow(q(1),2) - std::pow(q(2),2);
    (*this)[0][1] =     2 * ( q(0) * q(1) - q(2) * q(3) );
    (*this)[0][2] =     2 * ( q(0) * q(2) + q(1) * q(3) );

    (*this)[1][0] =     2 * ( q(0) * q(1) + q(2) * q(3) );
    (*this)[1][1] = std::pow(q(1),2) + std::pow(q(3),2)- std::pow(q(2),2) - std::pow(q(0),2);
    (*this)[1][2] =     2 * ( q(1) * q(2) - q(0) * q(3) );

    (*this)[2][0] =     2 * ( q(0) * q(2) - q(1) * q(3) );
    (*this)[2][1] =     2 * ( q(1) * q(2) + q(0) * q(3) );
    (*this)[2][2] = std::pow(q(2),2) + std::pow(q(3),2) - std::pow(q(0),2) - std::pow(q(1),2);
  }




  /*!
   *  \brief Creates a rotation matrix from a UnitQuaternion
   *
   * Creates a rotation matrix from a unit-quaternion.
   * Using in-homogeneous quaternion coordinates.
   * The orthogonality is preserved through the unit-length
   * of the quaternion.
   */
  template <typename T>
  inline
  HqMatrix<T,3>::HqMatrix(const UnitQuaternion<T>& q) {

    // In-homogeneous expression
    (*this)[0][0] = 1 - 2 * ( std::pow(q(1),2) + std::pow(q(2),2));
    (*this)[0][1] =     2 * ( q(0) * q(1) - q(2) * q(3) );
    (*this)[0][2] =     2 * ( q(0) * q(2) + q(1) * q(3) );

    (*this)[1][0] =     2 * ( q(0) * q(1) + q(2) * q(3) );
    (*this)[1][1] = 1 - 2 * ( std::pow(q(0),2) + std::pow(q(2),2));
    (*this)[1][2] =     2 * ( q(1) * q(2) - q(0) * q(3) );

    (*this)[2][0] =     2 * ( q(0) * q(2) - q(1) * q(3) );
    (*this)[2][1] =     2 * ( q(1) * q(2) + q(0) * q(3) );
    (*this)[2][2] = 1 - 2 * ( std::pow(q(0),2) + std::pow(q(1),2));
  }




  /*! void HqMatrix<T,3>::rotate(Angle a, const Vector<T,3>& rot_axis)
   *  \brief Add a rotation to this matrix: (*this) = HqMatrix<T,3>(a,rot_axis) * (*this)
   *
   *  Add a rotation to this matrix: (*this) = HqMatrix<T,3>(a,rot_axis) * (*this)
   */
  template <typename T>
  inline
  void HqMatrix<T,3>::rotate(Angle a, const Vector<T,3>& rot_axis){
    HqMatrix<T,3> x(a,rot_axis);
    *this = *this * x;
  }




  /*! HqMatrix<T,3>::rotate(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p)
   *  \brief Add a rotation to this matrix: (*this) = HqMatrix<T,3>(a,rot_axis,p) * (*this)
   *
   *  Add a rotation to this matrix: (*this) = HqMatrix<T,3>(a,rot_axis,p) * (*this)
   */
  template <typename T>
  inline
  void HqMatrix<T,3>::rotate(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p){
    HqMatrix<T,3> x(a,rot_axis,p);
    *this = *this * x;
  }

  template <typename T>
  inline
  void HqMatrix<T,3>::rotate(const Quaternion<T>& q ) {
    HqMatrix<T,3> x(q);
    *this = *this * x;
  }

  template <typename T>
  inline
  void HqMatrix<T,3>::rotateGlobal(const Quaternion<T>& q ) {
    HqMatrix<T,3> x(q);
    this->reverseMult(x);
  }




  /*! void HqMatrix<T,3>::rotateGlobal(Angle a, const Vector<T,3>& rot_axis)
   *  \brief Add a rotation to this matrix: (*this) = HqMatrix<T,n>(a,rot_axis) * (*this)
   *
   *  Add a rotation to this matrix: (*this) = HqMatrix<T,3>(a,rot_axis) * (*this)
   */
  template <typename T>
  inline
  void HqMatrix<T,3>::rotateGlobal(Angle a, const Vector<T,3>& rot_axis){
    HqMatrix<T,3> x(a,rot_axis);
    this->reverseMult( x );
  }




  /*! HqMatrix<T,3>::rotateGlobal(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p)
   *  \brief Add a rotation to this matrix: (*this) = HqMatrix<T,3>(a,rot_axis,p) * (*this)
   *
   *  Add a rotation to this matrix: (*this) = HqMatrix<T,3>(a,rot_axis.p) * (*this)
   */
  template <typename T>
  inline
  void HqMatrix<T,3>::rotateGlobal(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p){
    HqMatrix<T,3> x(a,rot_axis,p);
    this->reverseMult( x );
  }




  template <typename T>
  inline
  void HqMatrix<T,3>::rotate(const UnitQuaternion<T>& q ) {
    HqMatrix<T,3> x(q);
    *this = *this * x;
  }




  template <typename T>
  inline
  void HqMatrix<T,3>::rotateGlobal(const UnitQuaternion<T>& q ) {
    HqMatrix<T,3> x(q);
    this->reverseMult(x);
  }



} // END namespace GMlib
