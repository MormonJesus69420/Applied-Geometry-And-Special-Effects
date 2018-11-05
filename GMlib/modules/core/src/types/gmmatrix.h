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




//**********************************************************
// GM_Matrix.h: interface for the Static matrices classes.
//**********************************************************

#ifndef GM_CORE_TYPES_MATRIX_H
#define GM_CORE_TYPES_MATRIX_H

#include "../static/gmstaticproc2.h"

namespace GMlib {

  //********************************************************************
  //*** The three matrix classes in the linear and homogenius system ***
  //********************************************************************

  template <typename T, int n, int m>
  class Matrix;                            // The general (n x m) matrix

  template <typename T, int n>
  class SqMatrix;                          // The squere  (n x n) matrix

  template <typename T, int n>
  class HqMatrix;                          // The homogenius squere (n+1 x n+1) matrix (n->dim)






  /*! \class  Matrix gmmatrix.h <gmMatrix.h>
   *
   *  \brief  The Static Matrix class
   *
   *  A template (n x m) matrix (n rows and m columns), the matrix is static i.e. the dimentions
   *  can not be change. The template type must be clean, i.e. is not allocating
   *  memory and without any virtual functions
   */
  template <typename T, int n, int m>
  class Matrix : public Vector<Vector<T,m>,n>  {
  public:

    Matrix();
    Matrix( bool i );
    Matrix( const Point<T,m>& r, const Point<T,n>& c );
    Matrix( const Vector<T,m>& u );
    Matrix( const Vector<T,m>& u, const Vector<T,m>& v );
    Matrix( const Vector<Vector<T,m>,n>& v );
    Matrix( const Vector<Vector<T,n>,m>& v, bool trans );
    Matrix( const T* v, bool trans );

    Point<T,n> const&         getCol(int i) const;
    Matrix<T,n,n>             getMultTrans();
    T*                        getPtr();
    const T*                  getPtr() const;
    Point<T,m>*               getPtrP();
    const Point<T,m>*         getPtrP() const;
    Point<T,m> const&         getRow(int j) const;
    Matrix<T,m,m>             getTransMult();
    Matrix<T,m,n>             getTransposed() const;

    void                      initCol(int i, int j);
    void                      initRow(int i, int j);

    void                      reset();
    void                      setCol(const Point<T,n>& c, int i);
    void                      setRow(const Point<T,m>& r, int j);
    const Matrix<T,n,m>&      setTransposed(const Vector<Vector<T,m>,n>& mat);
    void                      scale(const Point<T,n>& s);
    void                      swap(int i,int j);
    void                      swap(int i,int j,int a,int b);

    const Matrix<T,n,m>&      operator=(const Vector<Vector<T,m>,n>& mat);

    // ***** Additions and substractions. ******
    const Matrix<T,n,m>&      operator+=(const Matrix<T,n,m>& mat);
    const Matrix<T,n,m>&      operator-=(const Matrix<T,n,m>& mat);
    Matrix<T,n,m>             operator+(const Matrix<T,n,m>& mat) const;
    Matrix<T,n,m>             operator-(const Matrix<T,n,m>& mat) const;
    Matrix<T,n,m>             operator-() const;

    // ******* Multiplication vector ******
    const Point<T,n>&        operator*(const Point<T,m>& v) const;


    //***** Scaling. *****
    const Matrix<T,n,m>&      operator*=(const double d);
    Matrix<T,n,m>             operator*(const double d) const;

    //***** Invers scaling. *****
    const Matrix<T,n,m>&      operator/=(double d);
    Matrix<T,n,m>             operator/(double d) const;

    // Casting
    template <typename G>
    operator Matrix<G,n,m> () const;

    template <typename G>
    Matrix<G,n,m>             toType() const;

    // Dummy for Array - should maby use determinant
    bool                      operator < ( const Matrix<T,n,m>& ) const;

  protected:

    static              M_I_<T,n,m>    _I;
    static              Vector<T,n>    _c;

    void                cpy();
    void                cpy(const T* v);
    void                cpy(const Vector<Vector<T,m>,n>& v);
    void                cpy_t(const T* v);
    void                cpy_t(const Vector<Vector<T,n>,m>& v);
    void                cpy_r(const Point<T,m>& p,int i);
    void                cpy_c(const Point<T,n>& p,int j);

    const Vector<T,n>&  getStatCol( int j ) const;
    const Vector<T,m>&  getStatRow( int i ) const;

    void                setOrtho( const Vector<T,m>& u );
    void                setOrtho( const Vector<T,m>& u, const Vector<T,m>& v );

  }; // END class Matrix







  //*************************************
  //***** Turned scaling d * point  *****
  //*************************************

  template <typename T, int n, int m>
  inline
  const Matrix<T,n,m>& operator*(double d, const Matrix<T,n,m>& p) {

    return p*d;
  }

  //*************************************
  //***  Matrix multiplication a * b  ***
  //*************************************

  /*
  // The following operator* is suposed to replace the 5 operator* belowe,
  // but this can only be done when opertator typeof is imlemented !!
  template <typename T, typename G, int n, int m, int k>
  inline
  Matrix<typeof(T()*G()),n,k>& operator*(const Matrix<T,n,m>& a, const Matrix<G,m,k>& b)
  {
    static Matrix<typeof(T()*G()),n,k> r;
    GM_Static_P2_<T,n,m,k>::mm_x(r.getPtrP(),a.getPtrP(),b.getPtr);
    return r;
  }
  */

  template <typename T, int n, int m, int k>
  inline
  Matrix<T,n,k> operator*(const Matrix<T,n,m>& a, const Matrix<T,m,k>& b) {

    Matrix<T,n,k> r;
    GM_Static_P2_<T,n,m,k>::mm_x( r.getPtrP(),a.getPtrP(),b.getPtr());
    return r;
  }




  template <typename T, int n, int m, int k>
  inline
  Matrix<T,n,k> multTrans(const Matrix<T,n,m>& a, const Matrix<T,k,m>& b) {

    Matrix<T,n,k> r;
    GM_Static_P2_<T,n,m,k>::mm_xT(r.getPtrP(),a.getPtrP(),b.getPtr());
    return r;
  }




  //***************************************************************************************
  // The 3 next operators are made as a lack of "typeof" functionality in C++ at the moment
  //***************************************************************************************
  template <typename T, int n, int m, int k, int r>
  inline
  Matrix<Point<T,r>,n,k> operator*(const Matrix<Point<T,r>,n,m>& a, const Matrix<T,m,k>& b) {

    Matrix<Point<T,r>,n,k> mat;
    GM_Static_P2_<T,n,m,k>::mm_x(mat.getPtrP(),a.getPtrP(),b.getPtr());
    return mat;
  }




  template <typename T, int n, int m, int k, int r>
  inline
  Matrix<Point<T,r>,n,k> operator*(const Matrix<T,n,m>& a, const Matrix<Point<T,r>,m,k>& b) {

    Matrix<Point<T,r>,n,k> mat;
    GM_Static_P2_<T,n,m,k>::mm_x(mat.getPtrP(),a.getPtrP(),b.getPtr());
    return mat;
  }




  template <typename T, int n, int m, int k, int r>
  inline
  Matrix<T,n,k> operator*(const Matrix<Point<T,r>,n,m>& a, const Matrix<Point<T,r>,m,k>& b) {

    Matrix<T,n,k> mat;
    GM_Static_P2_<T,n,m,k>::mm_x(mat.getPtrP(),a.getPtrP(),b.getPtr());
    return mat;
  }




  template <typename T, int n, int m>
  inline
  Point<T,m> operator*(const Point<T,n>& p, const Matrix<T,n,m>& b) {

    Point<T,m> r;
    GM_Static_P2_<T,n,m,n>::vm_x(r.getPtr(),const_cast<Point<T,n>*>(&p), b.getPtr());
    return r;
  }




#ifdef GM_STREAM
  //**************************************************************
  //***** Stream operators for std-stream or QT-stream, etc. *****
  //**************************************************************

  template <typename T_Stream, class T, int n, int m>
  inline
  T_Stream &operator<<(T_Stream& out, const Matrix<T,n,m>& v) {

    for(int i=0;i<n;i++) out << v(i) << GMseparator::group();
    return out;
  }




  template <typename T_Stream, class T, int n, int m>
  inline
  T_Stream& operator<<(T_Stream& out, const Matrix<T,n,m>* v) {

    for(int i=0;i<n;i++) out << (*v)(i) << GMseparator::group();
    return out;
  }




  template <typename T_Stream, class T, int n, int m>
  inline
  T_Stream& operator>>(T_Stream& in, Matrix<T,n,m>& v) {

    Separator gs(GMseparator::group());
    for(int i=0;i<n;i++) in >> v[i] >> gs;
    return in;
  }




  template <typename T_Stream, class T, int n, int m>
  inline
  T_Stream& operator>>(T_Stream& in, Matrix<T,n,m>* v) {

    Separator gs(GMseparator::group());
    for(int i=0;i<n;i++) in >> (*v)[i] >> gs;
    return in;
  }
#endif







/*! class SqMatrix: public Matrix<T,n,n>
 *  \brief The Static Square Matrix class
 *
 *  A template (n x n) matrix, the matrix is static i.e. the dimentions
 *  can not be change. The template type must be clean, i.e. is not allocating
 *  memory and without any virtual functions.
 *  This Square Matrix inherit from the static Matrix class
 */
template <typename T, int n>
class SqMatrix: public Matrix<T,n,n> {
public:
  SqMatrix();
  SqMatrix( bool i);
  SqMatrix( const Point<T,n>& r, const Point<T,n>& c);
  SqMatrix( const Vector<Vector<T,n>,n>& m );
  SqMatrix(const Vector<Vector<T,n>,n>& m, bool trans);
  SqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v);
  SqMatrix(const T* m, bool trans);
  SqMatrix(Angle a, int x = 0, int y = 1);


  void                    basisChange(const Matrix<T,n,n>& x);
  void                    basisChangeInv(const Matrix<T,n,n>& x);

  //T                       getDeterminant(); // NOT IMPLEMENTED
  Point<T,n> const&       getDiagonal(int i) const;
  //Matrix<T,n,n> const&    getInverse() const ;    // Not changing this: a = b-1// NOT IMPLEMENTED
  //Matrix<T,n,n> const&    getInverse2() const ;// NOT IMPLEMENTED

  void                    invert();
  void                    invertOrthoNormal();    // Use only for orthonormal matrices (rotation matrices)

  Matrix<T,n,n> const&    reverseMult(const Matrix<T,n,n>& m);            // Changing this ( is a kind of *= operator): *this = m * *this

  void                    rotateXY(Angle a, int x=0, int y=1);
  void                    rotate(Angle a, const Vector<T,n>& u, const Vector<T,n>& v);

  void                    setDiagonal(const Point<T,n>& r);

  // improve transpose by using swap!
  Matrix<T,n,n> const&    transpose();//Matrix<T,n,n> v(*this,true); *this = v; return(*this);}
  Matrix<T,n,n> const&    transposeMult(const Matrix<T,n,n>& m) const ;    // Not changing this: a = this->transpose * m

  // Casting
  template <typename G>
  operator SqMatrix<G,n> () const ;

protected:

  void                    cpy_d(const Point<T,n>& p);
  const Point<T,n>&       getStatDiagonal(int i)const;
  void                    makeOrtho(const Vector<T,n>& u, const Vector<T,n>& v, Matrix<T,n,n>& x);

}; // END class SqMatrix







/*! class HqMatrix: public SqMatrix<T,n+1>
 *  \brief The Static Homogenius Matrix class
 *
 *  A template (n+1 x n+1) homogenious matrix (i.e. the matrix are working
 *  on homogenious coordinates), the matrix is static i.e. the dimentions
 *  can not be change. The template type must be clean, i.e. is not allocating
 *  memory and without any virtual functions.
 *  This Homogenious Matrix inherit from the static Square Matrix class
 */
template <typename T, int n>
class HqMatrix: public SqMatrix<T,n+1> {
public:
  HqMatrix();
  HqMatrix(bool i);
  HqMatrix(const Matrix<T,n+1,n+1>& v);
  HqMatrix(const Matrix<T,n+1,n+1>& v, bool trans);
  HqMatrix(const Matrix<T,n,n>& v, bool trans=false);
  HqMatrix(const Vector<T,n>& d);
  HqMatrix(Angle a,  int x, int y);
  HqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v);
  HqMatrix(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p);

  //HqMatrix<T,n> const&    getInverse2() const; // NOT IMPLEMENTED

  void                    invertOrthoNormal();        // overloaded and use only for orthonormal (n-1 x n-1) sub-matrices (rotation matrices)

  void                    rotateXY(Angle a, int x=0, int y=1);

  void                    rotate(Angle a, const Vector<T,n>& u, const Vector<T,n>& v);
  void                    rotate(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p);

  void                    rotateGlobal(Angle a, const Vector<T,n>& u, const Vector<T,n>& v);
  void                    rotateGlobal(Angle a, const Vector<T,n>& u, const Vector<T,n>& v, const Point<T,n>& p);

  void                    scale(const Point<T,n>& s);

  Matrix<T,n+1,n+1>&      setTranspose(const Matrix<T,n+1,n+1>& v);

  void                    translate(const Vector<T,n> d);
  void                    translateGlobal(const Vector<T,n> d);

  const Matrix<T,n,n>&    getRotationMatrix() const;


  Matrix<T,n+1,n+1>&     operator=(const Matrix<T,n+1,n+1>& v);

  // Multiplication vector
  Point<T,n>             operator*(const Point<T,n>& v)      const;
  Vector<T,n>            operator*(const Vector<T,n>& v)      const;
  ScalarPoint<T,n>       operator*(const ScalarPoint<T,n>& v)const;
  Sphere<T,n>            operator*(const Sphere<T,n>& v)      const;
  Arrow<T,n>             operator*(const Arrow<T,n>& v)      const;
  Box<T,n>               operator*(const Box<T,n>& v)          const;
  Matrix<T,n+1,n+1>      operator*(const HqMatrix<T,n>& v)  const;

  // Casting
  template <typename G>
  operator HqMatrix<G,n> () const;

  template <typename G>
  HqMatrix<G,n>          toType() const;

protected:
  void                   hq_cpy(const Matrix<T,n,n>& v);
  void                   hq_cpy_t(const Matrix<T,n,n>& v);
  void                   hq_cpy_r(const Point<T,n>& p,int i);
  void                   hq_cpy_c(const Point<T,n>& p,int j);

  void                   makeOrtho(const Vector<T,n>& u, const Vector<T,n>& v, Matrix<T,n+1,n+1>& x);
};





template <typename T>
class HqMatrix<T,3> : public SqMatrix<T,4>{
public:
  HqMatrix();
  HqMatrix(bool i);
  HqMatrix(const Matrix<T,4,4>& v);
  HqMatrix(const Matrix<T,4,4>& v, bool trans);
  HqMatrix(const Matrix<T,3,3>& v, bool trans=false);
  HqMatrix(const Vector<T,3>& d);
  HqMatrix(Angle a,  int x, int y);
  HqMatrix(Angle a, const Vector<T,3>& u, const Vector<T,3>& v);
  HqMatrix(Angle a, const Vector<T,3>& u, const Vector<T,3>& v, const Point<T,3>& p);

  HqMatrix(Angle a, const Vector<T,3>& rot_axis);
  HqMatrix(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p);

  HqMatrix(const Quaternion<T>& q );
  HqMatrix(const UnitQuaternion<T>& q );


  //HqMatrix<T,n> const&    getInverse2() const; // NOT IMPLEMENTED

  void          invertOrthoNormal(); // overloaded - use only for orthonormal (n-1 x n-1) sub-matrices (rotation)

  void          rotateXY(Angle a, int x=0, int y=1);

  void          rotate(Angle a, const Vector<T,3>& u, const Vector<T,3>& v);
  void          rotate(Angle a, const Vector<T,3>& u, const Vector<T,3>& v, const Point<T,3>& p);
  void          rotate(Angle a, const Vector<T,3>& rot_axis);
  void          rotate(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p);
  void          rotate(const Quaternion<T>& q);
  void          rotate(const UnitQuaternion<T>& q);

  void          rotateGlobal(Angle a, const Vector<T,3>& u, const Vector<T,3>& v);
  void          rotateGlobal(Angle a, const Vector<T,3>& u, const Vector<T,3>& v, const Point<T,3>& p);
  void          rotateGlobal(Angle a, const Vector<T,3>& rot_axis);
  void          rotateGlobal(Angle a, const Vector<T,3>& rot_axis, const Point<T,3>& p);
  void          rotateGlobal(const Quaternion<T>& q);
  void          rotateGlobal(const UnitQuaternion<T>& q);

  void                   scale(const Point<T,3>& s);

  Matrix<T,4,4>&         setTranspose(const Matrix<T,4,4>& v);
  void                   translate(const Vector<T,3> d);
  void                   translateGlobal(const Vector<T,3> d);

  const Matrix<T,3,3>&   getRotationMatrix() const;


  Matrix<T,4,4>&         operator=(const Matrix<T,4,4>& v);

  // Multiplication vector
  Point<T,3>             operator*(const Point<T,3>& v)      const;
  Vector<T,3>            operator*(const Vector<T,3>& v)      const;
  ScalarPoint<T,3>       operator*(const ScalarPoint<T,3>& v)const;
  Sphere<T,3>            operator*(const Sphere<T,3>& v)      const;
  Arrow<T,3>             operator*(const Arrow<T,3>& v)      const;
  Box<T,3>               operator*(const Box<T,3>& v)          const;
  Matrix<T,4,4>          operator*(const HqMatrix<T,3>& v)  const;

  // Casting
  template <typename G>
  operator HqMatrix<G,3> () const;

  template <typename G>
  HqMatrix<G,3>          toType() const;


protected:
  void                   hq_cpy(const Matrix<T,3,3>& v);
  void                   hq_cpy_t(const Matrix<T,3,3>& v);
  void                   hq_cpy_r(const Point<T,3>& p,int i);
  void                   hq_cpy_c(const Point<T,3>& p,int j);

  void                   makeOrtho(const Vector<T,3>& u, const Vector<T,3>& v, Matrix<T,4,4>& x);

};  // END class HqMatrix







// ****************************************
// Matrix multiplication (special versions)

template <typename T, int n, int m>
inline
Matrix<T,n,m> operator*(const HqMatrix<T,n>& a, const Matrix<T,n,m>& b) {

  Matrix<T,n,m> r;
  GM_Static_P2_<T,n,n,m>::mm_xH(r.getPtrP(),a.getPtrP(),b.getPtr());
  return r;
}

template <typename T, int n, int m>
inline
Matrix<T,m,n> multTrans(const HqMatrix<T,n>& a, const Matrix<T,m,n>& b) {

  Matrix<T,m,n> r;
  GM_Static_P2_<T,n,n,m>::mm_xHT(r.getPtr(),a.getPtrP(),b.getPtr());
  return r;
}

} // END namespace GMlib

// Include implementations
#include "gmmatrix.c"




#endif // GM_CORE_TYPES_MATRIX_H
