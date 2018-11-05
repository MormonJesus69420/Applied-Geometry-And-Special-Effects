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




#ifndef GM_CORE_CONTAINERS_DVECTORN_H
#define GM_CORE_CONTAINERS_DVECTORN_H


// gmlib
#include "../types/gmpoint.h"
#include "gmdvector.h"
#include "gmdmatrix.h"


namespace GMlib {

  template <typename T, int n, class K = float>
  class DVectorN {
  public:
    DVectorN( int i = 0 );
    DVectorN( const DVectorN<T,n,K>& v );
    DVectorN( const T p[] );
    ~DVectorN();

    static unsigned long int            makeIdx1D( DVector<unsigned long int> k, int res );
    static DVector<unsigned long int>   makeIdx2D( unsigned long int k, int res );
    static DVector<unsigned long int>   makeIdxND( unsigned long int k, int res, int dim );



    Vector<int,n>&                      getDimRev() const;
//    DVector<T>*               flat();
    Vector<int,n>&                      getDim() const;

    DVectorN<T,n,K>                     getTransposed() const;
    T                                   getValue(int iFlat) const;
    void                                setDim( const Vector<int,n>& i );
    void                                setValue(int i, T val);


    DVectorN<T,n,K>&                    operator =  ( const DVectorN<T,n,K>& v );
    DVectorN<T,n,K>&                    operator =  ( T p[] );

    DVectorN<T,n,K>&                    operator += ( const DVectorN<T,n,K>& ); //(+) On all elements.
    DVectorN<T,n,K>&                    operator -= ( const DVectorN<T,n,K>& ); //(-) On all elements.
    DVectorN<T,n,K>&                    operator %= ( const DVectorN<T,n,K>& );
    DVectorN<T,n,K>&                    operator *= ( double d );
    DVectorN<T,n,K>&                    operator /= ( double d );
    DVectorN<T,n-1>&                    operator *  ( const DVector<K>& v );		// ND-matrix by vector multiplication.
    DVectorN<T,n,K>&                    operator *  ( const DMatrix<K>& m );		// ND-matrix by 2D-matrix multiplication
    DVectorN<T,n-1>&                    operator ^  ( const DVector<K>& v );		// ND-matrix by vector inner product.

    DVectorN<T,n,K>                     operator +  ( const DVectorN<T,n,K>& a )	const;
    DVectorN<T,n,K>                     operator -  ( const DVectorN<T,n,K>& a )	const;
    DVectorN<T,n,K>                     operator %  ( const DVectorN<T,n,K>& v )	const;
    DVectorN<T,n,K>                     operator *  ( double d ) const;
    DVectorN<T,n,K>                     operator /  ( double d ) const;



    DVectorN<T,n-1>&                    operator [] ( int i ); // return given element.
    const DVectorN<T,n-1>&              operator () ( int i ) const; // return const of given element.

    T                                   operator *  ( const DVectorN<T,n,K>& v );

  private:
    DVectorN<T,n-1>                    *_pt;
    int                                 _n;


    void                                _copy( const T p[] );
    void                                _copy( const DVectorN<T,n,K>& v );

    void                                _indexOK( int i ) const;



  public:
//    // ********** PRINT ***********
//    template<class T_Stream>
//    friend T_Stream& operator << ( T_Stream& out, const DVectorN<T,n,K>& v ) {

//      for( int i = 0; i < v.getDim()[0]; i++ )
//        out << v(i) << object_separator;
//      return out;
//    }

//    template<class T_Stream>
//    friend T_Stream& operator >> ( T_Stream& in, DVectorN<T,n,K>& v ) {

//      static Separator os(object_separator);
//      for( int i = 0; i < v.getDim()[0]; i++ )
//        in >> v[i] >> os;
//      return in;
//    }

  }; // END class DVectorN






  /* See documentation for main class. This is a terminator */
  template <typename T, class K>
  class DVectorN<T,1,K> : public DVector<T> {
  public:
    DVectorN( int i = 0 );
    DVectorN( const DVector<T>& v);
    DVectorN( const T p[]);

    Vector<int,1>     getDimRev() const;
//    DVector<T>*       flat();
    Vector<int,1>&    getDim() const;
    T                 getValue(int i);
    void              setDim(const Vector<int,1>& i);
    void              setValue(int i, T val);


    DVectorN<T,1>&    operator * ( const DMatrix<K>& m );

  private:
    void              _copy( const DVectorN<T,1>& v );

  }; // END class DVectorN<T,1,K> (DVectorN terminator class)



} // END namespace GMlib


// Include DVectorN implementations
#include "gmdvectorn.c"




#endif // GM_CORE_CONTAINERS_DVECTORN_H
