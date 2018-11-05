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





#ifndef GM_CORE_CONTAINERS_DMATRIX_H
#define GM_CORE_CONTAINERS_DMATRIX_H


// gmlib
#include "../utils/gmstream.h"
#include "gmdvector.h"

namespace GMlib{


  template <typename T>
  class DMatrix {
   public:

    DMatrix(int i=0, int j=0);
    DMatrix(int i, int j, T val);
    DMatrix(int i, int j, const T p[]);
    DMatrix(const DMatrix<T>& v);
   ~DMatrix();

    T                   getDeterminant() const;
    int                 getDim1() const;
    int                 getDim2() const;
    void                increaseDim(int i, int j, T val=T(0), bool h_end=true, bool v_end=true);
    DMatrix<T>&         invert();
    void                resetDim(int i, int j);
    void                setDim(int i, int j);
    void                setIdentity();
    DVector<T>          toDVector() const;
    DMatrix<T>&         transpose();

    DMatrix<T>&         operator = (const DMatrix<T>& v);
    DMatrix<T>&         operator = (T p[]);

    // The += -=  + - operators NB!!!! assume the dimensions to be equal
    // NB!! this operators are not doubling the dimensions!!
    //-----------------------------------------------------------------
    DMatrix<T>&         operator += (const DMatrix<T>&);
    DMatrix<T>&         operator -= (const DMatrix<T>&);
    DMatrix<T>	        operator +  ( const DMatrix<T>&) const;
    DMatrix<T>	        operator -  ( const DMatrix<T>&) const;

    // The scalingoperator, there is also a non-member scaling operator with oposit order!!
    //-------------------------------------------------------------------------------------
    DMatrix<T>&	        operator *= (double d);
    DMatrix<T>	        operator *  (double d) const;
    DMatrix<T>&	        operator /= (double d);
    DMatrix<T>	        operator /  (double d) const;

    DVector<T>&         operator [] (int i);
    const DVector<T>&   operator () (int i) const;
    bool                operator <  (const DMatrix<T>& m) const;


  private:
    bool                _private;
    int                 _n;
    DVector<T>*         _p;
    DVector<T>          _init[4];

    void                _cpy(const DMatrix<T>& v);
    void                _cpy(const T p[]);


  }; // END DMatrix class



  //***************************************************
  //******   Scaling operators (opposit order)   ******
  //***************************************************


  template <typename T>
  inline
  DMatrix<T> operator*(const double d, DMatrix<T> p) {
    return p*=d;
  }


  #ifdef GM_STREAM
    //********************************************************
    //******  Template iostream operators for DMatrix  ******
    //********************************************************


    template <typename T_Stream, typename T>
    T_Stream& operator<<(T_Stream& out, const DMatrix<T>& v)   {
      out << v.getDim1() << GMseparator::element() << v.getDim2() << GMseparator::group();
      for (int i=0; i<v.getDim1(); i++)
      {
        for(int j=0;j<v.getDim2(); j++) out << v(i)(j) << GMseparator::element();
        out << GMseparator::group();
      }
      return out;
    }


    template< typename T_Stream, typename T>
    T_Stream& operator>>(T_Stream& in, DMatrix<T>& v) {
      static Separator gs(GMseparator::group());
      static Separator es(GMseparator::element());
      int a,b;
      in >> a >> es >> b >> gs;
      v.setDim(a,b);
      for (int i=0; i<v.getDim1(); i++)
      {
        for(int j=0;j<v.getDim2(); j++) in >> v[i][j] >> es;
        in >> gs;
      }
      return in;
    }
  #endif

  //***********************************************************
  // The multiplication operators between DMatrix and DVectors
  // and multiplication operators between DMatrix and DMatrix
  //***********************************************************

  template <typename T, typename G>
  inline
  const DVector<T>&  operator*(const DMatrix<G>& m, const DVector<T>& b) {
    static DVector<T> r;

    if(m.getDim2() != b.getDim()) return r;
    r.setDim(m.getDim1());
    int i,k;
    for(i=0;i<m.getDim1();i++)
    {
      r[i] = m(i)(0)*b(0);
      for(k=1;k<b.getDim();k++)
        r[i] += m(i)(k)*b(k);
    }
    return r;
  }


  template <typename T, typename G>
  inline
  const DMatrix<T>&  operator*(const DMatrix<G>& m, const DMatrix<T>& b) {
    static DMatrix<T> r;

    if(m.getDim2() != b.getDim1()) return r;
    r.setDim(m.getDim1(),b.getDim2());
    int i,j,k;
    for(i=0;i<m.getDim1();i++)
      for(j=0;j<b.getDim2();j++)
      {
        r[i][j] = m(i)(0)*b(0)(j);
        for(k=1;k<m.getDim2();k++)
          r[i][j] += m(i)(k)*b(k)(j);
      }
    return r;
  }


  template <typename T, typename G>
  inline
  const DVector<G>&  operator^(const DMatrix<G>& m, const DVector<T>& b) {
    static DVector<G> r;

    if(m.getDim2() != b.getDim()) return r;
    r.setDim(m.getDim1());
    int i,k;
    for(i=0;i<m.getDim1();i++)
    {
      r[i] = m(i)(0)*b(0);
      for(k=1;k<b.getDim();k++)
        r[i] += m(i)(k)*b(k);
    }
    return r;
  }


  template <typename T, typename G>
  inline
  const DMatrix<G>&  operator^(const DMatrix<G>& m, const DMatrix<T>& b) {
    static DMatrix<G> r;

    if(m.getDim2() != b.getDim1()) return r;
    r.setDim(m.getDim1(),b.getDim2());
    int i,j,k;
    for(i=0;i<m.getDim1();i++)
      for(j=0;j<b.getDim2();j++)
      {
        r[i][j] = m(i)(0)*b(0)(j);
        for(k=1;k<m.getDim2();k++)
          r[i][j] += m(i)(k)*b(k)(j);
      }
    return r;
  }


} // END namespace GMlib

// Include DMatrix class function implementations
#include "gmdmatrix.c"
//#include <core/blas/gmblasdmatrixincludes.h>

#endif  // GM_CORE_CONTAINERS_DMATRIX_H
