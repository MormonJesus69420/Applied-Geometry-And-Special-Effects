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



/*! \file gmstaticproc.h
 *
 *  interface for the Static meta programming
 *  to roll out the code usen in:
 *  Point, Vector, UnitVector, Arrow, Scalarfield, Sphere,
 *  Matrix SqMatrix, HqMatrix classes.
 */


#ifndef GM_CORE_STATIC_STATICPROC_H
#define GM_CORE_STATIC_STATICPROC_H




// stl
#include <utility>


/////////////////////////////////////////
// For Point and Vectors ( 1 dimentional)

namespace GMlib {

  /*! \class  GM_Static_ gmstaticproc.h <gmStaticProc>
   *  \brief  For Point and Vectors ( 1 dimentional)
   *
   *  For Point and Vectors ( 1 dimentional)
   */
  template <typename T, int n>
  class GM_Static_ {
  public:
    static T	  dpr(const T *a, const T *b);                    // a*b innerproduct
    static T	  dprm(const T *a, const T *b);                   // -a*b innerproduct

    static void   eq(T *a, const T& b);                           // a =  b  set equal
    static void   eqm(T *a, const T* b);	                      // a = -b

    static void   sc(T *a, double b);                             // a *= b  scale
    static void   sc_r(T *a, const T *b, double c);               // a = b*c scale

    static void   peq(T *a, const T *b);	                      // a += b
    static void   meq(T *a, const T *b);	                      // a -= b
    static void   xeq(T *a, const T* b);                          // a %= b

    static void   eq_p(T *a, const T* b, const T*c);              // a = b + c
    static void   eq_m(T *a, const T* b, const T*c);              // a = b - c
    static void   eq_x(T *a, const T* b, const T*c);              // a = b % c

    static void   rot_xy(T *a, T *b, const T& sa, const T& ca);

    static void   decomp(T *a, const T *b, const T& c);           // a -= c*b
    static void   normalizeHq(T *a, T *b, const T& c);

    static int    maxIndex(T *a);
    static void   maxIdx(T *a, int& i);

  }; // END class GM_Static_

  /* See documentation for main class. This is a terminator */
  template <typename T>
  class GM_Static_<T, 1> {
  public:
    static T	  dpr(const T *a, const T *b);                    // a*b innerproduct
    static T	  dprm(const T *a, const T *b);			          // a*b innerproduct

    static void   eq(T *a, const T& b);                           // a =  b  set equal
    static void   eqm(T *a, const T* b);                          // a = -b

    static void   sc(T *a, double b);                             // a *= b  scale a with b
    static void   sc_r(T *a, const T *b, double c);               // a = b*c scale b with c and put into a

    static void   peq(T *a, const T *b);                          // a += b
    static void   meq(T *a, const T *b);                          // a -= b
    static void   xeq(T *a, const T* b);                          // a %= b

    static void   eq_p(T *a, const T* b, const T*c);              // a = b + c
    static void   eq_m(T *a, const T* b, const T*c);              // a = b - c
    static void   eq_x(T *a, const T* b, const T*c);              // a = b % c

    static void   rot_xy(T *a, T *b, const T& sa, const T& ca);

    static void   decomp(T *a, const T *b, const T& c);           // a -= c*b

    static void   maxIdx(T *a, int& i);

  }; // END class GM_Static_<T, 1> "Terminator"





  /*! \class  GM_Static_u_ gmstaticproc.h <gmStaticProc>
   *  \brief  For Point and Vectors with strides ( 1 dimentional)
   *
   *  For Point and Vectors with strides( 1 dimentional)
   */
  template <typename T, int n, int u>
  class GM_Static_u_ {
  public:
    static void   eq(T *a, const T& b);                           // a =  b  set equal
    static void   sc(T *a, double b);                             // a *= b  scale
  }; // END class GM_Static_u_<T, n, u>


  template <typename T, int u>    /*This is a terminator */
  class GM_Static_u_<T, 1, u> {
  public:
    static void   eq(T *a, const T& b);                           // a =  b  set equal
    static void   sc(T *a, double b);                             // a *= b  scale a with b
  }; // END class GM_Static_u_<T, 1, u> "Terminator"




  /*! \class  GM_Static_uv_ gmstaticproc.h <gmStaticProc>
   *  \brief  For Point and Vectors with strides ( 1 dimentional)
   *
   *  For Point and Vectors with strides( 1 dimentional)
   */
  template <typename T, int n, int u, int v>
  class GM_Static_uv_ {
  public:
    static T	  dpr(const T *a, const T *b);                    // a*b innerproduct
    static T	  dprm(const T *a, const T *b);                   // -a*b innerproduct

    static void   eq(T *a, const T* b);                           // a =  b  set equal
    static void   eqm(T *a, const T* b);	                      // a = -b
    static void   sc_r(T *a, const T *b, double c);               // a = b*c scale

    static void   peq(T *a, const T *b);	                      // a += b
    static void   meq(T *a, const T *b);	                      // a -= b
    static void   xeq(T *a, const T* b);                          // a %= b

    static void   rot_xy(T *a, T *b, const T& sa, const T& ca);

    static void   decomp(T *a, const T *b, const T& c);           // a -= c*b
  }; // END class GM_Static_uv_<T, n, u, v>


  template <typename T, int u, int v>    /*This is a terminator */
  class GM_Static_uv_<T, 1, u, v> {
  public:
    static T	  dpr(const T *a, const T *b);                    // a*b innerproduct
    static T	  dprm(const T *a, const T *b);			          // a*b innerproduct

    static void   eq(T *a, const T* b);                           // a =  b  set equal
    static void   eqm(T *a, const T* b);                          // a = -b
    static void   sc_r(T *a, const T *b, double c);               // a = b*c scale b with c and put into a

    static void   peq(T *a, const T *b);                          // a += b
    static void   meq(T *a, const T *b);                          // a -= b
    static void   xeq(T *a, const T* b);                          // a %= b

    static void   rot_xy(T *a, T *b, const T& sa, const T& ca);

    static void   decomp(T *a, const T *b, const T& c);           // a -= c*b
  }; // END class GM_Static_uv_<T, 1, u, v> "Terminator"




  /*! \class  GM_Static_uvw_ gmstaticproc.h <gmStaticProc>
   *  \brief  For Point and Vectors with strides ( 1 dimentional)
   *
   *  For Point and Vectors with strides( 1 dimentional)
   */
  template <typename T, int n, int u, int v, int w>
  class GM_Static_uvw_ {
  public:
    static void   eq_p(T *a, const T* b, const T*c);              // a = b + c
    static void   eq_m(T *a, const T* b, const T*c);              // a = b - c
    static void   eq_x(T *a, const T* b, const T*c);              // a = b % c
  }; // END class GM_Static_uv_<T, n, u, v, w>


  template <typename T, int u, int v, int w>    /*This is a terminator */
  class GM_Static_uvw_<T, 1, u, v, w> {
  public:
    static void   eq_p(T *a, const T* b, const T*c);              // a = b + c
    static void   eq_m(T *a, const T* b, const T*c);              // a = b - c
    static void   eq_x(T *a, const T* b, const T*c);              // a = b % c
  }; // END class GM_Static_uv_<T, 1, u, v, w> "Terminator"





  /*! \class  GM_Static1_ gmstaticproc.h <gmStaticProc>
   *  \brief  Pending Documentation
   *
   *  For Point and Vectors ( 1 dimentional)
   */
  template <typename T, typename G, int n>
  class GM_Static1_ {
  public:
    static void eq(T *a, const G* b);   // a =  b  set equal

  }; // END class GM_Static1_


  /* See Documentation for main class, this is the terminator */
  template <typename T, typename G>
  class GM_Static1_<T,G,1> {
  public:
    static void eq(T *a, const G* b);    // a =  b  set equal

  }; // END class GM_Static1_<T, G, 1> "Terminator"

} // END namespace GMlib



// Include gmPoint for future use
#include "../types/gmpoint.h"

namespace GMlib {

  ///////////////////////////////////////////////////////////
  // For Vectors in Matrices ( column-vectors, using strides)

  /*! \class GM_Static2_ gmstaticproc.h <gmStaticProc>
   *  \brief Pending Documentation
   *
   *  For Vectors in Matrices ( column-vectors, using strides)
   */
  template <typename T, int n, int m>
  class GM_Static2_ {
  public:
    static T      dpr(const T *a, const T *b);      // a*b innerproduct (row x column) (n x m)
    static T      dpc(const T *a, const T *b);      // a*b innerproduct (column x column) (n x m)

    static void   eq1(T *a, const T *b);            // a = b  a-vector in n x m  matrix, b-column in m x n matrix
    static void   eq2(const T *a, T *b);            // b = a  a-vector in n x m  matrix, b-column in m x n matrix
    static void   eq3(T *a, const T *b);            // a = b  a-column in n x m  matrix, b-column in n x m matrix

    static void   sc(T *a, double b);               // a *= b scale column vector
    static void   decomp(T *a, const T* b);
    static void   decomp1(T *a, const T* b, T& c);

  }; // END class GM_Static2_


  /* See documentation for the main class, this is a terminator */
  template <typename T, int m>
  class GM_Static2_<T, 1, m> {
  public:
    static T	dpr(const T *a, const T *b);          // a*b innerproduct (row x column) (n x m)
    static T	dpc(const T *a, const T *b);          // a*b innerproduct (column x column) (n x m)

    static void eq1(T *a, const T *b);              // a = b  a-vector in n x m  matrix, b-column in transposed matrix
    static void eq2(const T *a, T *b);              // b = a  a-vector in n x m  matrix, b-column in transposed matrix
    static void eq3(T *a, const T *b);              // a = b  a-column in n x m  matrix, b-column in n x m matrix

    static void sc(T *a, double b);                 // a *= b scale column vector

    static void decomp1(T *a, const T* b, T& c);
  }; // END class GM_Static2_<T, 1, m> "Terminator"











  /////////////////////////////////////////////////////////////////////
  // For Diagonals in Matrices or homogenius matrix (using strides m+1)


  /*! \class GM_Static3_ gmstaticproc.h <gmStaticProc>
   *  \brief Pending Documentation
   *
   *  For Diagonals in Matrices or homogenius matrix (using strides m+1)
   */
  template <typename T, int n, int m>
  class GM_Static3_ {
  public:
    static T	    dpr(const T *a, const T *b);              // a*b innerproduct (row x column) (n x m)

    static void   eq1(T *a, const T *b);              // a = b  a-vector, b-diagonal in matrix
    static void   eq2(const T *a, T *b);              // b = a  a-vector, b-diagonal in matrix
    static void   eq3(const T *a, T *b);              // b = -a  a-vector, b-diagonal in matrix

    static void   sc(T *a, double b);	          // a *= b scale column vector

    static void   swp(T* a, T* b);	            // swap colomn a and line b
    static void   trn(T* a, T* b);              // swap colomn a and line b

    static void   cm_x(T *a, const T* b, const T* c);	      // col = -mat x vec

    static void   decomp(T *a, const T* b);
    static void   decomp1(T *a, const T* b, T& c);

  }; // END class GM_Static3_


  /* See documentation for main class, this is a terminatro */
  template <typename T, int m>
  class GM_Static3_<T, 1, m> {
  public:
    static T	    dpr(const T *a, const T *b);	            // a*b innerproduct (row x column) (n x m)

    static void   eq1(T *a, const T *b);              // a = b  a-vector, b-diagonal in matrix
    static void   eq2(const T *a, T *b);              // b = a  a-vector, b-diagonal in matrix
    static void   eq3(const T *a, T *b);              // b = a  a-vector, b-diagonal in matrix

    static void   sc(T *a, double b);           // a *= b scale column vector

    static void   swp(T* a, T* b);              // swap colomn a and line b
    static void   trn(T* a, T* b);              // swap colomn a and line b

    static void   cm_x(T *a, const T* b, const T* c);	      // col = -mat x vec

    static void   decomp1(const T *a, const T* b, T& c);
  }; // END class GM_Static3_<T, 1, m> "Terminator"













  /*! \class GM_Std_ gmstaticproc.h <gmStaticProc>
   *  \brief Pending Documentation
   *
   *  For Diagonals in Matrices or homogenius matrix (using strides m+1)
   */
  template <typename T, int n>
  class GM_Std_ {
  public:
    static void b_min_d_c(T *a, const T* b, const T* c, const T& d);    // a = b - d*c,    a,b,c is n-dim vector, and d is scalar
    static void orto(T *a, const T* b, const T* c);                     // a = b - <b,c> c , where |c| = 1, and all are n-dim vectors
    static void ortoNormal(T *a, const T* b, const T* c);               // The same as abow, but normalizing the result 'a' after
  }; // END class GM_Std_


  /* See documentation for Main class, this is the terminator */
  template <typename T>
  class GM_Std_<T,1> {
  public:
    static void b_min_d_c(T *a, const T* b, const T* c, const T& d);
  }; // END class GM_Std_<T, 1> "Terminator"

} // END namespace GMlib














///////////////////////////////////////////////////////////////////////////////
///
/// IMPLEMENTATION
///
///////////////////////////////////////////////////////////////////////////////


namespace GMlib {



  ///////////////////////////////
  // template <typename T, int n>
  // class GM_Static_<T, n>::


  /*! \brief  Innerproduct of &lt;a,b&gt;
   *
   * \tparam T Affine point/vector type
   * \tparam n Dimension
   *
   *  \param[in] a point/vector
   *  \param[in] b point/vector
   *  \result Innerproduct result
   */
  template <typename T, int n>
  inline
  T	GM_Static_<T, n>::dpr(const T *a, const T *b) {
    return (*a) * (*b) + GM_Static_<T,n-1>::dpr(a+1,b+1);
  }


  /*! \brief  -a*b innerproduct */
  template <typename T, int n>
  inline
  T	GM_Static_<T, n>::dprm(const T *a, const T *b) {
    return -(*a) * (*b) + GM_Static_<T,n-1>::dprm(a+1,b+1);
  }

  /*! \brief  a =  b  set equal */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::eq(T *a, const T& b) {
    (*a) = b;
    GM_Static_<T,n-1>::eq(a+1,b);
  }

  /*! \brief  a = -b */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::eqm(T *a, const T* b) {
    (*a) = -(*b);
    GM_Static_<T,n-1>::eqm(a+1,b+1);
  }

  /*! \brief  a *= b  scale */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::sc(T *a, double b) {
    (*a) *= b;
    GM_Static_<T,n-1>::sc(a+1,b);
  }

  /*! \brief  a = b*c scale */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::sc_r(T *a, const T *b, double c)	{
    (*a) = (*b)*c;
    GM_Static_<T,n-1>::sc_r(a+1,b+1,c);
  }

  /*! \brief  a += b */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::peq(T *a, const T *b) {
    (*a) += (*b);
    GM_Static_<T,n-1>::peq(a+1,b+1);
  }

  /*! \brief  a -= b */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::meq(T *a, const T *b) {
    (*a) -= (*b);
    GM_Static_<T,n-1>::meq(a+1,b+1);
  }

  /*! \brief  a %= b */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::xeq(T *a, const T* b) {
    (*a) *= (*b);
    GM_Static_<T,n-1>::xeq(a+1,b+1);
  }

  /*! \brief  a = b + c */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::eq_p(T *a, const T* b, const T*c) {
    (*a)=(*b)+(*c);
    GM_Static_<T,n-1>::eq_p(a+1,b+1,c+1);
  }

  /*! \brief  a = b - c */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::eq_m(T *a, const T* b, const T*c) {
    (*a)=(*b)-(*c);
    GM_Static_<T,n-1>::eq_m(a+1,b+1,c+1);
  }

  /*! \brief  a = b % c */
  template <typename T, int n>
  inline
  void GM_Static_<T, n>::eq_x(T *a, const T* b, const T*c) {
    (*a)=(*b)*(*c);
    GM_Static_<T,n-1>::eq_x(a+1,b+1,c+1);
  }

  template <typename T, int n>
  inline
  void GM_Static_<T, n>::rot_xy(T *a, T *b, const T& sa, const T& ca) {
    T tmp=(*b)*sa+(*a)*ca;
    (*b)=(*b)*ca-(*a)*sa;
    (*a)=tmp;
    GM_Static_<T,n-1>::rot_xy(a+1,b+1,sa,ca);
  }

  template <typename T, int n>
  inline
  void GM_Static_<T, n>::decomp(T *a, const T *b, const T& c)	{
    (*a) -= c * (*b);
    GM_Static_<T,n-1>::decomp(a+1,b+1,c);
  }

  template <typename T, int n>
  inline
  void GM_Static_<T, n>::normalizeHq(T *a, T *b, const T& c) {
    T d = GM_Static_<T,n>::dpr(b,b);
    d = 1/d; GM_Static_<T,n>::sc_r(a,b,d);
    (*(a+n)) = c;
  }

  template <typename T, int n>
  inline
  int  GM_Static_<T, n>::maxIndex(T *a) {
    int i=n;
    GM_Static_<T,n-1>::maxIdx(a+1,i);
    return n-i;
  }

  template <typename T, int n>
  inline
  void GM_Static_<T, n>::maxIdx(T *a, int& i) {
    if( std::abs(*a) > std::abs(*(a+n-i))) i = n;
    GM_Static_<T,n-1>::maxIdx(a+1,i);
  }









  ///////////////////////////////
  // template <typename T>
  // class GM_Static_<T, 1>::





  /*! \brief a*b innerproduct */
  template <typename T>
  inline
  T GM_Static_<T, 1>::dpr(const T *a, const T *b) {
    return (*a) * (*b);
  }


  /*! \brief  a*b innerproduct */
  template <typename T>
  inline
  T GM_Static_<T, 1>::dprm(const T *a, const T *b) {
    return -(*a) * (*b);
  }


  /*! \brief  a =  b  set equal */
  template <typename T>
  inline
  void GM_Static_<T, 1>::eq(T *a, const T& b) {
    (*a) = b;
  }


  /*! \brief  a = -b */
  template <typename T>
  inline
  void GM_Static_<T, 1>::eqm(T *a, const T* b) {
    (*a) = -(*b);
  }


  /*! \brief  a *= b;  scale a with b */
  template <typename T>
  inline
  void GM_Static_<T, 1>::sc(T *a, double b) {
    (*a) *= b;
  }


  /*! \brief  a = b*c; scale b with c and put into a */
  template <typename T>
  inline
  void GM_Static_<T, 1>::sc_r(T *a, const T *b, double c)	{
    (*a) = (*b)*c;
  }


  /*! \brief  a += b */
  template <typename T>
  inline
  void GM_Static_<T, 1>::peq(T *a, const T *b) {
    (*a) += (*b);
  }


  /*! \brief  a -= b */
  template <typename T>
  inline
  void GM_Static_<T, 1>::meq(T *a, const T *b) {
    (*a) -= (*b);
  }


  /*! \brief  a %= b */
  template <typename T>
  inline
  void GM_Static_<T, 1>::xeq(T *a, const T* b) {
    (*a) *= (*b);
  }


  /*! \brief  a = b + c */
  template <typename T>
  inline
  void GM_Static_<T, 1>::eq_p(T *a, const T* b, const T*c) {
    (*a)=(*b)+(*c);
  }


  /*! \brief  a = b - c */
  template <typename T>
  inline
  void GM_Static_<T, 1>::eq_m(T *a, const T* b, const T*c) {
    (*a)=(*b)-(*c);
  }


  /*! \brief  a = b % c */
  template <typename T>
  inline
  void GM_Static_<T, 1>::eq_x(T *a, const T* b, const T*c) {
    (*a)=(*b)*(*c);
  }


  template <typename T>
  inline
  void GM_Static_<T, 1>::rot_xy(T *a, T *b, const T& sa, const T& ca) {
    T tmp=(*b)*sa+(*a)*ca;
    (*b)=(*b)*ca-(*a)*sa;
    (*a)=tmp;
  }


  /*! \brief  a -= c*b */
  template <typename T>
  inline
  void GM_Static_<T, 1>::decomp(T *a, const T *b, const T& c)	{
    (*a) -= c * (*b);
  }


  template <typename T>
  inline
  void GM_Static_<T, 1>::maxIdx(T *a, int& i) {
    if(std::fabs(*a) > std::fabs(*(a+1-i))) i = 1;
  }

  ///////////////////////////////////////////
  // template <typename T, typename G, int n>
  // class GM_Static1_<T, G, n>::

  /*! \brief a =  b  set equal */
  template <typename T, typename G, int n>
  inline
  void GM_Static1_<T, G, n>::eq(T *a, const G* b) {
    (*a) = static_cast<T>(*b);
    GM_Static1_<T,G,n-1>::eq(a+1,b+1);
  }





  ////////////////////////////////////
  // template <typename T, typename G>
  // class GM_Static1_<T, G, 1>::

  /*! \brief a =  b  set equal b */
  template<typename T, typename G>
  void GM_Static1_<T, G, 1>::eq(T *a, const G* b) {
    (*a) = static_cast<T>(*b);
  }

  ///////////////////////////////////////
  // template <typename T, int n, int m>
  // class GM_Static2_<T, n, m>::

  /*! \brief a*b innerproduct (row x column) (n x m) */
  template <typename T, int n, int m>
  inline
  T	GM_Static2_<T, n, m>::dpr(const T *a, const T *b)	{
    return (*a)*(*b)+ GM_Static2_<T,n-1,m>::dpr(a+1,b+m);
  }


  /*! \brief a*b innerproduct (column x column) (n x m) */
  template <typename T, int n, int m>
  inline
  T	GM_Static2_<T, n, m>::dpc(const T *a, const T *b)	{
    return (*a)*(*b)+ GM_Static2_<T,n-1,m>::dpc(a+m,b+m);
  }

  /*! \brief a = b  a-vector in n x m  matrix, b-column in m x n matrix */
  template <typename T, int n, int m>
  inline
  void GM_Static2_<T, n, m>::eq1(T *a, const T *b) {
    (*a) = (*b);
    GM_Static2_<T,n-1,m>::eq1(a+1,b+m);
  }

  /*! \brief b = a  a-vector in n x m  matrix, b-column in m x n matrix */
  template <typename T, int n, int m>
  inline
  void GM_Static2_<T, n, m>::eq2(const T *a, T *b) {
    (*b) = (*a);
    GM_Static2_<T,n-1,m>::eq2(a+1,b+m);
  }

  /*! \brief a = b  a-column in n x m  matrix, b-column in n x m matrix */
  template <typename T, int n, int m>
  inline
  void GM_Static2_<T, n, m>::eq3(T *a, const T *b) {
    (*a) = (*b);
    GM_Static2_<T,n-1,m>::eq3(a+m,b+m);
  }

  /*! \brief a *= b scale column vector */
  template <typename T, int n, int m>
  inline
  void GM_Static2_<T, n, m>::sc(T *a, double b)	{
    (*a) *= b;	GM_Static2_<T,n-1,m>::sc(a+m,b);
  }

  template <typename T, int n, int m>
  inline
  void GM_Static2_<T, n, m>::decomp(T *a, const T* b) {
    T c = GM_Static_<T,m>::dpr(a,b);
    GM_Static_<T,m>::decomp(a,b,c);
    GM_Static2_<T,n-1,m>::decomp1(a,b+m,c);
  }

  template <typename T, int n, int m>
  inline
  void GM_Static2_<T, n, m>::decomp1(T *a, const T* b, T& c) {
    c = GM_Static_<T,m>::dpr(a,b);
    GM_Static_<T,m>::decomp(a,b,c);
    GM_Static2_<T,n-1,m>::decomp1(a,b+m,c);
  }




  ///////////////////////////////////////
  // template <typename T, int m>
  // class GM_Static2_<T, 1, m>::

  /*! \brief a*b innerproduct (row x column) (n x m) */
  template <typename T, int m>
  inline
  T	GM_Static2_<T, 1, m>::dpr(const T *a, const T *b)	{
    return (*a) * (*b);
  }


  /*! \brief a*b innerproduct (column x column) (n x m) */
  template <typename T, int m>
  inline
  T	GM_Static2_<T, 1, m>::dpc(const T *a, const T *b)	{
    return (*a) * (*b);
  }


  /*! \brief a = b  a-vector in n x m  matrix, b-column in transposed matrix */
  template <typename T, int m>
  inline
  void GM_Static2_<T, 1, m>::eq1(T *a, const T *b) {
    (*a) = (*b);
  }


  /*! \brief b = a  a-vector in n x m  matrix, b-column in transposed matrix */
  template <typename T, int m>
  inline
  void GM_Static2_<T, 1, m>::eq2(const T *a, T *b) {
    (*b) = (*a);
  }


  /*! \brief b = a  a-column in n x m  matrix, b-column in n x m matrix */
  template <typename T, int m>
  inline
  void GM_Static2_<T, 1, m>::eq3(T *a, const T *b)	{
    (*a) = (*b);
  }


  /*! \brief a *= b scale column vector */
  template <typename T, int m>
  inline
  void GM_Static2_<T, 1, m>::sc(T *a, double b)	{
    (*a) *= b;
  }


  template <typename T, int m>
  inline
  void GM_Static2_<T, 1, m>::decomp1(T *a, const T* b, T& c) {
    c = GM_Static_<T,m>::dpr(a,b);
    GM_Static_<T,m>::decomp(a,b,c);
  }

  ///////////////////////////////////////
  // template <typename T, int n, int m>
  // class GM_Static3_<T, n, m>::

  /*! \brief a*b innerproduct (row x column) (n x m) */
  template <typename T, int n, int m>
  inline
  T	GM_Static3_<T, n, m>::dpr(const T *a, const T *b)	{
    return (*a)*(*b)+ GM_Static3_<T,n-1,m>::dpr(a+1,b+m+1);
  }

  /*! \brief a = b  a-vector, b-diagonal in matrix */
  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::eq1(T *a, const T *b) {
    (*a) = (*b);
    GM_Static3_<T,n-1,m>::eq1(a+1,b+m+1);
  }

  /*! \brief b = a  a-vector, b-diagonal in matrix */
  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::eq2(const T *a, T *b) {
    (*b) = (*a);
    GM_Static3_<T,n-1,m>::eq2(a+1,b+m+1);
  }

  /*! \brief b = -a  a-vector, b-diagonal in matrix */
  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::eq3(const T *a, T *b) {
    (*b) = -(*a);
    GM_Static3_<T,n-1,m>::eq3(a+1,b+m+1);
  }

  /*! \brief a *= b scale column vector */
  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::sc(T *a, double b)	{
    (*a) *= b;
    GM_Static2_<T,n-1,m>::sc(a+m+1,b);
  }

  /*! \brief swap colomn a and line b */
  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::swp(T* a, T* b) {
    std::swap(*a,*b);
    GM_Static3_<T,n-1,m>::swp(a+m,b+1);
  }

  /*! \brief swap colomn a and line b */
  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::trn(T* a, T* b) {
    GM_Static3_<T,n,m>::swp(a,b);
    GM_Static3_<T,n-1,m>::trn(a+m+1,b+m+1);
  }

  /*! \brief col = -mat x vec */
  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::cm_x(T *a, const T* b, const T* c)	{
    (*a) = -GM_Static_<T,m>::dpr(b,c);
    GM_Static3_<T,n-1,m>::cm_x(a+m+1,b+m+1,c);
  }

  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::decomp(T *a, const T* b) {
    T c = GM_Static_<T,m>::dpr(a,b);
    GM_Static_<T,m>::decomp(a,b,c);
    GM_Static2_<T,n-1,m>::decomp1(a,b+m+1,c);
  }

  template <typename T, int n, int m>
  inline
  void GM_Static3_<T, n, m>::decomp1(T *a, const T* b, T& c) {
    c = GM_Static_<T,m>::dpr(a,b);
    GM_Static_<T,m>::decomp(a,b,c);
    GM_Static2_<T,n-1,m>::decomp1(a,b+m+1,c);
  }





  ///////////////////////////////////////
  // template <typename T, int m>
  // class GM_Static3_<T, 1, m>::


  /*! \brief a*b innerproduct (row x column) (n x m) */
  template <typename T, int m>
  inline
  T	GM_Static3_<T, 1, m>::dpr(const T *a, const T *b)	{
    return (*a) * (*b);
  }


  /*! \brief a = b  a-vector, b-diagonal in matrix */
  template <typename T, int m>
  inline
  void GM_Static3_<T, 1, m>::eq1(T *a, const T *b) {
    (*a) = (*b);
  }


  /*! \brief b = a  a-vector, b-diagonal in matrix */
  template <typename T, int m>
  inline
  void GM_Static3_<T, 1, m>::eq2(const T *a, T *b) {
    (*b) = (*a);
  }


  /*! \brief b = a  a-vector, b-diagonal in matrix */
  template <typename T, int m>
  inline
  void GM_Static3_<T, 1, m>::eq3(const T *a, T *b) {
    (*b) = -(*a);
  }


  /*! \brief a *= b scale column vector */
  template <typename T, int m>
  inline
  void GM_Static3_<T, 1, m>::sc(T *a, double b)	{
    (*a) *= b;
  }


  /*! \brief swap colomn a and line b */
  template <typename T, int m>
  inline
  void GM_Static3_<T, 1, m>::swp(T* a, T* b) {
    std::swap(*a,*b);
  }


  /*! \brief swap colomn a and line b */
  template <typename T, int m>
  inline
  void GM_Static3_<T, 1, m>::trn(T* a, T* b) {
    std::swap(*a,*b);
  }


  /*! \brief col = -mat x vec */
  template <typename T, int m>
  inline
  void GM_Static3_<T, 1, m>::cm_x(T *a, const T* b, const T* c)	{
    (*a) = -GM_Static_<T,m>::dpr(b,c);
  }


  template <typename T, int m>
  inline
  void GM_Static3_<T, 1, m>::decomp1(const T *a, const T* b, T& c) {
    c = GM_Static_<T,m>::dpr(a,b);
    GM_Static_<T,m>::decomp(a,b,c);
  }

  ///////////////////////////////
  // template <typename T, int n>
  // class GM_Std_<T, n>::


  /*! \brief a = b - d*c,    a,b,c is n-dim vector, and d is scalar */
  template <typename T, int n>
  inline
  void GM_Std_<T, n>::b_min_d_c( T *a, const T* b, const T* c, const T& d) {
    (*a) = (*b) - d*(*c);
    GM_Std_<T,n-1>::b_min_d_c(a+1,b+1,c+1,d);
  }

  /*! \brief a = b - &lt;b,c&gt; c , where |c| = 1, and all are n-dim vectors */
  template <typename T, int n>
  inline
  void GM_Std_<T, n>::orto(T *a, const T* b, const T* c) {
    T d = GM_Static_<T,n>::dpr(b,c);
    GM_Std_<T,n>::b_min_d_c(a,b,c,d);
  }

  /*! \brief The same as abow, but normalizing the result 'a' after \see void GM_Std_<T, n>::orto(T *a, T* b, T* c) */
  template <typename T, int n>
  inline
  void GM_Std_<T, n>::ortoNormal(T *a, const T* b, const T* c) {
    GM_Std_<T,n>::orto(a,b,c);
    T d = GM_Static_<T,n>::dpr(a,a);
    GM_Static_<T,n>::sc(a,sqrt(1/d));
  }






  ////////////////////////
  // template <typename T>
  // class GM_Std_<T, 1>::

  template <typename T>
  inline
  void GM_Std_<T, 1>::b_min_d_c(T *a, const T* b, const T* c, const T& d) {
    (*a) = (*b) - d*(*c);
  }




//**************************************************************************************//



  ///////////////////////////////
  // template <typename T, int n, int u>
  // class GM_Static_uv_ {

  /*! \brief  Innerproduct of &lt;a,b&gt;
   *
   * \tparam T Affine point/vector type
   * \tparam n Dimension
   * \tparam u Stride for 1st vector
   * \tparam v Stride for 2nd vector
   *
   *  \param[in] a point/vector
   *  \param[in] b point/vector
   *  \result
   */

  /*! \brief  a = b  set equal */
  template <typename T, int n, int u>
  inline
  void GM_Static_u_<T,n,u>::eq(T *a, const T& b) {
    (*a) = b;
    GM_Static_u_<T,n-1,u>::eq(a+u,b);
  }


  /*! \brief  a *= b  scale */
  template <typename T, int n, int u>
  inline
  void GM_Static_u_<T,n,u>::sc(T *a, double b) {
    (*a) *= b;
    GM_Static_u_<T,n-1,u>::sc(a+u,b);
  }




  /*! \brief  a =  b  set equal */
  template <typename T, int u>
  inline
  void GM_Static_u_<T,1,u>::eq(T *a, const T& b) {
    (*a) = b;
  }



  /*! \brief  a *= b;  scale a with b */
  template <typename T, int u>
  inline
  void GM_Static_u_<T,1,u>::sc(T *a, double b) {
    (*a) *= b;
  }





  ///////////////////////////////
  // template <typename T, int n, int u, int v>
  // class GM_Static_uv_ {

  /*! \brief  Innerproduct of &lt;a,b&gt;
   *
   * \tparam T Affine point/vector type
   * \tparam n Dimension
   * \tparam u Stride for 1st vector
   * \tparam v Stride for 2nd vector
   *
   *  \param[in] a point/vector
   *  \param[in] b point/vector
   *  \result Innerproduct or ...
   */
  template <typename T, int n, int u, int v>
  inline
  T	GM_Static_uv_<T,n,u,v>::dpr(const T *a, const T *b) {
    return (*a) * (*b) + GM_Static_uv_<T,n-1,u,v>::dpr(a+u,b+v);
  }


  /*! \brief  -a*b innerproduct */
  template <typename T, int n, int u, int v>
  inline
  T	GM_Static_uv_<T,n,u,v>::dprm(const T *a, const T *b) {
    return -(*a) * (*b) + GM_Static_uv_<T,n-1,u,v>::dprm(a+u,b+v);
  }


  /*! \brief  a = -b */
  template <typename T, int n, int u, int v>
  inline
  void GM_Static_uv_<T,n,u,v>::eq(T *a, const T* b) {
    (*a) = (*b);
    GM_Static_uv_<T,n-1,u,v>::eq(a+u,b+v);
  }


  /*! \brief  a = -b */
  template <typename T, int n, int u, int v>
  inline
  void GM_Static_uv_<T,n,u,v>::eqm(T *a, const T* b) {
    (*a) = -(*b);
    GM_Static_uv_<T,n-1,u,v>::eqm(a+u,b+v);
  }


  /*! \brief  a = b*c scale */
  template <typename T, int n, int u, int v>
  inline
  void GM_Static_uv_<T,n,u,v>::sc_r(T *a, const T *b, double c)	{
    (*a) = (*b)*c;
    GM_Static_uv_<T,n-1,u,v>::sc_r(a+u,b+v,c);
  }

  /*! \brief  a += b */
  template <typename T, int n, int u, int v>
  inline
  void GM_Static_uv_<T,n,u,v>::peq(T *a, const T *b) {
    (*a) += (*b);
    GM_Static_uv_<T,n-1,u,v>::peq(a+u,b+v);
  }

  /*! \brief  a -= b */
  template <typename T, int n, int u, int v>
  inline
  void GM_Static_uv_<T,n,u,v>::meq(T *a, const T *b) {
    (*a) -= (*b);
    GM_Static_uv_<T,n-1,u,v>::meq(a+u,b+v);
  }

  /*! \brief  a %= b */
  template <typename T, int n, int u, int v>
  inline
  void GM_Static_uv_<T,n,u,v>::xeq(T *a, const T* b) {
    (*a) *= (*b);
    GM_Static_uv_<T,n-1,u,v>::xeq(a+u,b+v);
  }

  template <typename T, int n, int u, int v>
  inline
  void GM_Static_uv_<T,n,u,v>::rot_xy(T *a, T *b, const T& sa, const T& ca) {
    T tmp=(*b)*sa+(*a)*ca;
    (*b)=(*b)*ca-(*a)*sa;
    (*a)=tmp;
    GM_Static_uv_<T,n-1,u,v>::rot_xy(a+u,b+v,sa,ca);
  }

  template <typename T, int n, int u, int v>
  inline
  void GM_Static_uv_<T,n,u,v>::decomp(T *a, const T *b, const T& c)	{
    (*a) -= c * (*b);
    GM_Static_uv_<T,n-1,u,v>::decomp(a+u,b+v,c);
  }




  ///////////////////////////////
  // template <typename T, n, int u, int v, int w>
  // class GM_Static_uvw_ {



  /*! \brief  a = b + c */
  template <typename T, int n, int u, int v, int w>
  inline
  void GM_Static_uvw_<T,n,u,v,w>::eq_p(T *a, const T* b, const T*c) {
    (*a)=(*b)+(*c);
    GM_Static_uvw_<T,n-1,u,v,w>::eq_p(a+u,b+v,c+w);
  }

  /*! \brief  a = b - c */
  template <typename T, int n, int u, int v, int w>
  inline
  void GM_Static_uvw_<T,n,u,v,w>::eq_m(T *a, const T* b, const T*c) {
    (*a)=(*b)-(*c);
    GM_Static_uvw_<T,n-1,u,v,w>::eq_m(a+u,b+v,c+w);
  }

  /*! \brief  a = b % c */
  template <typename T, int n, int u, int v, int w>
  inline
  void GM_Static_uvw_<T,n,u,v,w>::eq_x(T *a, const T* b, const T*c) {
    (*a)=(*b)*(*c);
    GM_Static_uvw_<T,n-1,u,v,w>::eq_x(a+u,b+v,c+w);
  }







  ///////////////////////////////
  // template <typename T, 1, int u, int v>
  // class GM_Static_uv_ {



  /*! \brief a*b innerproduct */
  template <typename T, int u, int v>
  inline
  T GM_Static_uv_<T,1,u,v>::dpr(const T *a, const T *b) {
    return (*a) * (*b);
  }


  /*! \brief  a*b innerproduct */
  template <typename T, int u, int v>
  inline
  T GM_Static_uv_<T,1,u,v>::dprm(const T *a, const T *b) {
    return -(*a) * (*b);
  }



  /*! \brief  a = -b */
  template <typename T, int u, int v>
  inline
  void GM_Static_uv_<T,1,u,v>::eq(T *a, const T* b) {
    (*a) = (*b);
  }


  /*! \brief  a = -b */
  template <typename T, int u, int v>
  inline
  void GM_Static_uv_<T,1,u,v>::eqm(T *a, const T* b) {
    (*a) = -(*b);
  }


  /*! \brief  a = b*c; scale b with c and put into a */
  template <typename T, int u, int v>
  inline
  void GM_Static_uv_<T,1,u,v>::sc_r(T *a, const T *b, double c)	{
    (*a) = (*b)*c;
  }


  /*! \brief  a += b */
  template <typename T, int u, int v>
  inline
  void GM_Static_uv_<T,1,u,v>::peq(T *a, const T *b) {
    (*a) += (*b);
  }


  /*! \brief  a -= b */
  template <typename T, int u, int v>
  inline
  void GM_Static_uv_<T,1,u,v>::meq(T *a, const T *b) {
    (*a) -= (*b);
  }


  /*! \brief  a %= b */
  template <typename T, int u, int v>
  inline
  void GM_Static_uv_<T,1,u,v>::xeq(T *a, const T* b) {
    (*a) *= (*b);
  }


  template <typename T, int u, int v>
  inline
  void GM_Static_uv_<T,1,u,v>::rot_xy(T *a, T *b, const T& sa, const T& ca) {
    T tmp=(*b)*sa+(*a)*ca;
    (*b)=(*b)*ca-(*a)*sa;
    (*a)=tmp;
  }


  /*! \brief  a -= c*b */
  template <typename T, int u, int v>
  inline
  void GM_Static_uv_<T,1,u,v>::decomp(T *a, const T *b, const T& c)	{
    (*a) -= c * (*b);
  }





  ///////////////////////////////
  // template <typename T, 1, int u, int v, int w>
  // class GM_Static_uvw_ {




  /*! \brief  a = b + c */
  template <typename T, int u, int v, int w>
  inline
  void GM_Static_uvw_<T,1,u,v,w>::eq_p(T *a, const T* b, const T*c) {
    (*a)=(*b)+(*c);
  }


  /*! \brief  a = b - c */
  template <typename T, int u, int v, int w>
  inline
  void GM_Static_uvw_<T,1,u,v,w>::eq_m(T *a, const T* b, const T*c) {
    (*a)=(*b)-(*c);
  }


  /*! \brief  a = b % c */
  template <typename T, int u, int v, int w>
  inline
  void GM_Static_uvw_<T,1,u,v,w>::eq_x(T *a, const T* b, const T*c) {
    (*a)=(*b)*(*c);
  }





} //END namespace GMlib





#endif   // GM_CORE_STATIC_STATICPROC_H

