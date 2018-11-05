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



/*! \file gmstaticproc2.h
 *
 * gmstaticproc2.h: interface for the Static meta programming
 * to roll out the code usen in:
 * Matrix SqMatrix, HqMatrix classes.
 */

#ifndef GM_CORE_STATIC_STATICPROC2_H
#define GM_CORE_STATIC_STATICPROC2_H

#include "../types/gmpoint.h"

namespace GMlib {

  /*! \class  GM_Static_P_ gmstaticproc2.h <gmStaticProc2.h>
   *  \brief  For Matrices ( 2 dimentional)
   */
  template <typename T, int n, int m>
  class GM_Static_P_ {
  public:
    static void mv_x(T *a, const Point<T,m>* b, const Point<T,m>& c);		// vec = mat x vec
    static void mv_xq(T *a, const T* b, const Point<T,m>& c);				// vec = HqMat x vec
    static void mv_xqP(T *a, const T* b, const Point<T,m>& c, const T* d);		// vec = HqMat x vec + h(homogen col)

    static void eq_t(Point<T,m> *a, const T *b);							// a = b^T	matrix transposeing
    static void sc(Point<T,m>* a, const T *b);							// scaling of matrix (row by row)
    static void vv_ox(Point<T,m> *a, const Point<T,m> *b, const T *c);	// a = b x c tensor (outer) product

    static void cm_x(T *a, const T* b, const T* c);								// vec = col x mat
    static void mc_x(T *a, const T *b, const T *c);								// a = b(col)*c	matrix multiplication, but b is transposed

    static void hq_2x(T *a, const T* b, const Point<T,m>& c);			// vec = HqMat x vec
    static void hq_3x(T *a, const T* b, const Vector<T,m>& r, const T* p);	// col = vec - vec x HqMat

    //static Point<T,m>& orto(const Point<T,m>& a, Point<T,m> *b)	// To orthogonalization of vectors
    //{ return GM_Static_<T,m>::dpr(a.ptr(),b->ptr())*(*b) + GM_Static_P_<T,n-1,m>::orto(a,b+1); }
  }; // END class GM_Static_P_


  /*! \class  GM_Static_P_ gmstaticproc2.h <gmStaticProc2.h>
   *  \brief  For Matrices ( 2 dimentional)
   */
  template <typename T,int m>
  class GM_Static_P_<T,1,m> {
  public:
    static void mv_x(T *a, const Point<T,m>* b, const Point<T,m>& c);
    static void mv_xq(T *a, const T* b, const Point<T,m>& c);
    static void mv_xqP(T *a, const T* b, const Point<T,m>& c, const T* d);

    static void vv_ox(Point<T,m> *a, const Point<T,m> *b, T *c);

    static void cm_x(T *a, const T* b, const T* c);
    static void mc_x(T *a, const T *b, const T *c);

    static void sc(Point<T,m>* a, const T *b);

    static void hq_2x(T *a, const T* b, const Point<T,m>& c);
    static void hq_3x(T *a, const T* b, const Vector<T,m>& r, const T* p);

    //static Point<T,m>& orto(const Point<T,m>& a, Point<T,m> *b) { return GM_Static_<T,m>::dpr(a.ptr(),b->ptr())*(*b); }
  }; // END class GM_Static_P_<T, 1, m> "Terminator"











  template <typename T, int n, int m>
  inline
  void v_eq_m_x_v(T* /*a*/, Point<T,m>* /*b*/, const Point<T,m>& /*c*/) {}


  /*! \class  GM_Static_P2_ gmstaticproc2.h <gmStaticProc2.h>
   */
  template <typename T, int n, int m, int k>
  class GM_Static_P2_ {
  public:
    static void vm_x(T *a, const Point<T,m>* b, const T* c);					// vec = vec x mat
    static void vm_xHT(T *a, const Point<T,m+1>* b, const T* c);				// vec =  mat x vec (a = c x b)
    static void vm_xH(T *a, const Point<T,m+1>* b, const T* c);					// vec(a) = vec(c) x mat(b) (b homogen)
    static void vm_xT(T *a, const Point<T,m>* b, const T* c);					// vec =  mat x vec (a = c x b)
    static void vm_xTT(T *a, const T* b, const T* c);							// a(vec) = b(col) x mat

    static void mm_xT(Point<T,k> *a, const Point<T,m> *b, const T *c);			// a = b*c	matrix multiplication (c transposed)
    static void mm_xTT(Point<T,k> *a, const T *b, const T *c);					// a = b*c	matrix multiplication (b transposed)
    static void mm_xH(Point<T,k> *a, const Point<T,m+1> *b, const T *c);		// a = b*c	matrix multiplication (b homogen)
    static void mm_x(Point<T,k> *a, const Point<T,m> *b, const T *c);			// a = b*c	matrix multiplication
    static void mm_xHT(T *a, const Point<T,m+1> *b, const T *c);				// a = b*c	matrix multiplication (c transposed)

    static void eq_t(Point<T,m> *a, const T *b);							// a = b^T	matrix transposeing (a = n x m)

  }; // END class GM_Static_P2_


  /*! \class  GM_Static_P2_ gmstaticproc2.h <gmStaticProc2.h>
   */
  template <class T, int m, int k>
  class GM_Static_P2_<T,1,m,k> {
  public:
    static void vm_x(T *a, const Point<T,m>* b, const T* c);
    static void vm_xT(T *a, const Point<T,m>* b, const T* c);
    static void vm_xTT(T *a, const T* b, const T* c);
    static void vm_xH(T *a, const Point<T,m+1>* b, const T* c);
    static void vm_xHT(T *a, const Point<T,m+1>* b, const T* c);

    static void mm_x(Point<T,k> *a, const Point<T,m> *b, const T *c);
    static void mm_xT(Point<T,k> *a, const Point<T,m> *b, const T *c);
    static void mm_xTT(Point<T,k> *a, const T *b, const T *c);
    static void mm_xH(Point<T,k> *a, const Point<T,m+1> *b, const T *c);
    static void mm_xHT(T *a, const Point<T,m+1> *b, const T *c);

  }; // END class GM_Static_P2_<T, 1, m, k> "Terminator"


  /*! \class  GM_Static_P2_ gmstaticproc2.h <gmStaticProc2.h>
   *  \brief  a = b^T	matrix transposeing
   */
  template <class T, int n, int m>
  class GM_Static_P2_<T,n,m,1> {
  public:
    static void eq_t(Point<T,m> *a, const T *b);

  }; // END class GM_Static_P2_<T, n, m, 1> "Terminator"

} // END namespace GMlib



// Include implementations
#include "gmstaticproc2.c"




#endif   // GM_CORE_STATIC_STATICPROC2_H

