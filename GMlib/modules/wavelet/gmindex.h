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

#ifndef __GM_WAVELET_INDEX_H__
#define __GM_WAVELET_INDEX_H__

namespace GMlib {
namespace Wavelet {

/*! \namespace Index
    \brief Indexing algorithms.
 */
/*! \namespace Index::Cartesian
    \brief Cartesian index mapping algorithms.
 */
/*! \namespace Index::Cantor
    \brief Cantor index mapping algorithms.
 */
namespace Index {

  static
  int log2( unsigned int val ) {

    int ret = -1;
    while (val != 0) {
      val >>= 1;
      ret++;
    }
    return ret;
  }


  /*! \enum Index::INDEX_ERRORS
   *  A enum describing different type of indexing errors
   */
  enum INDEX_ERRORS {

    INDEX_NO_ERROR            = 0x0000, /*!< No errors. */
    INDEX_DIM_RES_MISSMATCH   = 0x0001, /*!< Missmatch in dimension and/or resolution between sets. */
    INDEX_DIM_OUT_OF_BOUNDS   = 0x0002  /*!< Index respective to a dimension is out of bounds. */
  };


  /*! \struct IndexND index.h
   *  \brief n-dimensional index.
   *
   *  Describes a n-dimensional index.
   *
   *  \tparam T Indexing typename, e.g.: short, int, unsigned int.
   *  \tparam n Index dimension.
   */
  template <typename T, int n>
  struct IndexND {

    T s[n];

    bool equals(const IndexND &idx) const;

  }; // END struct IndexND

  /*! \var IndexND::s[n]
   *
   *  An array of length n where each position holds the index for the respective,
   *  0th to (n-1)th, dimension.
   */

  /*! IndexND::equals( const IndexND& idx ) const
   *
   *  Checks wheter the the input index differs from this index.
   *
   *  \param[in] idx Another index.
   *  \return bool Whether the input index differs from this index.
   */
  template <typename T, int n>
  bool IndexND<T,n>::equals( const IndexND& idx ) const {

    bool eq = true;
    for( int i = 0; i < n; i++ )
      if( s[i] != idx.s[i] )
        eq = false;

    return eq;
  }


  /*! \struct IndexMVND index.h
   *  \brief Multivariate n-dimensional index.
   *
   *  Describes a multivariate n-dimensional index.
   *
   *  \tparam T Indexing typename, e.g.: short, int, unsigned int.
   *  \tparam n Index dimension.
   */
  template <typename T, int n>
  struct IndexMVND : IndexND<T,n> {

    T m;

    bool equals( const IndexMVND& idx) const;

  }; // END struct IndexMVND

  /*! \var IndexMVND::m
   *
   *  Holds the variate number index.
   */

  /*! IndexND::equals( const IndexND& idx ) const
   *
   *  Checks wheter the the input index differs from this index.
   *  This function checks also the variate index.
   *
   *  \param[in] idx Another index.
   *  \return bool Whether the input index differs from this index.
   */
  template <typename T, int n>
  bool IndexMVND<T,n>::equals( const IndexMVND& idx) const {

    return IndexND<T,n>::equals( idx ) && m == idx.m;
  }


  /*! T getSumOfIndices( const T* indices )
   *
   *  Computes the sum of all the index factors.
   *
   *  \tparam T Indexing typename, e.g.: short, int, unsigned int.
   *
   *  \param[in] indices
   *  \return T Returns the sum of all the index factors.
   */
  template <typename T, int n>
  T getSumOfIndices( const T* indices ) {

    T sum = 0;

    for( int i = 0; i < n; i++ )
      sum += indices[i];

    return sum;
  }


  namespace Cartesian {


    /*! T to1D( const T* indices, const T* dims )
      \brief Maps a n-dimensinal index to 1D.

      Maps a n-dimensional index in one dimension.

      \tparam T Indexing typename, e.g.: short, int, unsigned int.
      \tparam n Index dimension.

      \param[in] indices Pointer to an array of n T-values holding the index in each dimension.
      \param[in] dims Pointer to an array of n T-values holding the resolution of each dimension.
      \return T The one-dimensional index mapping to the given n-dimensional index.
    */
    template <typename T, int n>
    T to1D( const T* indices, const T* dims ) {

      T m = 1; // dim multiplicator

      T index = indices[n-1];
      for( int i = n-1; i > 0; i-- ) {

        m *= dims[i];
        index += indices[i-1] * m;
      }

      return index;
    }

    /*! void toND( T* indices, const T* dims, const T& idx_1d )
      \brief Maps a one-dimensional index to ND.

      Maps a one-dimensional index in n dimensions.

      \tparam T Indexing typename, e.g.: short, int, unsigned int.
      \tparam n Index dimension.

      \param[out] indices Pointer to an array of n T-values which will hold the index in nD.
      \param[in] dims Pointer to an array of n T-values holding the resolution of each dimension in the result index.
      \param[in] idx_1d The one-dimensional index.
    */
    template <typename T, int n>
    void toNDfrom1D( T* indices, const T* dims, const T& idx_1d ) {


      T rest = idx_1d;
      for( int i = 0; i < n; i++ ) {

        // Product of dims
        T pod = 1;
        for( int j = n-1; j > i; j-- )
          pod *= dims[j];

        // Compute indice of i
        indices[i] = rest / pod;

        // Update 'rest'
        rest -= indices[i] * pod;
      }
    }

    /*! T sqTo1D( const T* index )
      \brief Maps a n-dimensional index to 1D.

      Maps a n-dimensional index to one dimension.
      The resolution is equal for each dimension.

      \tparam T Indexing typename, e.g.: short, int, unsigned int.
      \tparam n Index dimension.
      \tparam res_fact Resolution factor. Described as 2^(res_fact - 1).

      \param[in] index Pointer to an array of n T-values describing the n-dimensional index.
      \return T The one-dimensional index.
    */
    template <typename T, int n, int res_fact>
    T sqTo1D( const T* index ) {

      const IndexND<T,n> &idx = *(const IndexND<T,n>*)index;

      T idx_1d = 0;
      for( int i = 0; i < n; i++ )
        idx_1d += idx.s[i] * std::pow( 2, res_fact * ( n - i - 1 ) );

      return idx_1d;
    }

    /*! void sqToND( const T& index_1d, T* index )
      \brief Maps a univariat index to multivariate.

      Maps a univariat index to multivariate.

      \tparam T Indexing typename, e.g.: short, int, unsigned int.
      \tparam n Index dimension.
      \tparam res_fact Resolution factor. Described as 2^(res_fact - 1).

      \param[in] index_1d The One dimensional index.
      \param[out] index Pointer to the n-dimensional index. Array of n T-values. Must be initialized.
    */
    template <typename T, int n, int res_fact>
    void sqToND( const T& index_1d, T* index ) {

      IndexND<T,n> &idx = *(IndexND<T,n>*)index;

      int e = index_1d; // Elements
      for( int i = n - 1; i >= 0; i-- ) {

        T lvl_size = std::pow( 2, res_fact * i);
        idx.s[n - i - 1] = e / lvl_size;
        e -= idx.s[n - i - 1] * lvl_size;
      }
    }

    /*! void sqMapping( const T& index_1d, T* index )
      \brief Maps a m-dimensional index to nD.

      Maps the m-variat index into a n-variat index.

      The total number of elements in both the m- and n-dimensional space must match,
      ( m * res_fact_m == n * res_fact_n ).

      Usage example:
      \dontinclude index_cartesian_sqmapping.cpp
      \skip Get mapping from
      \until Index::Cartesian::sqMapping

      \tparam T Indexing typename, e.g.: short, int, unsigned int.
      \tparam m Index dimension.
      \tparam res_fact_m Resolution factor. Described as 2^(res_fact_m - 1).
      \tparam n Index dimension.
      \tparam res_fact_n Resolution factor. Described as 2^(res_fact_n - 1).

      \param[in] index_m Pointer to the m-dimensional index. Array of m T-values.
      \param[out] index_n Pointer to the n-dimensional index. Array of n T-values. Must be initialized.
      \param[out] err Holds an error status. \see Index::INDEX_ERRORS.
    */
    template <typename T, int m, int res_fact_m, int n, int res_fact_n>
    void sqMapping( const T* index_m, T* index_n, int& err ) {

      err = INDEX_NO_ERROR;

      if( (m * res_fact_m) != (n * res_fact_n) )
        err |= INDEX_DIM_RES_MISSMATCH;

      const IndexND<T,m> &idx_m = *(const IndexND<T,m>*)index_m;

      const int res_m = std::pow( 2, res_fact_m );
      for( int i = 0; i < m; i++ )
        if( idx_m.s[i] >= res_m )
          err |= INDEX_DIM_OUT_OF_BOUNDS;

      if( err != INDEX_NO_ERROR )
        return;

      sqToND<T,n,res_fact_n>( sqTo1D<T,m,res_fact_m>( index_m ), index_n );
    }

    /*! void sqWaweletTo1D( T& gamma, T& kappa, const T& i, const T& j, const T *k )
      \brief


      \tparam T Indexing typename, e.g.: short, int, unsigned int.
      \tparam n Index dimension.

      \param[out] gamma Level block offset index
      \param[out] kappa Current block - internal index
      \param[in] j
      \param[in] i
      \param[in] k
    */
    template <typename T, int n>
    void sqWaweletTo1D( T& gamma, T& kappa, const T& j, const T& i, const T *k ) {

      const IndexND<T,n> &idx = *(const IndexND<T,n>*)k;

      const int log2_i = log2(i);

      gamma = j * n + log2_i;

      kappa = std::pow( 2, j * n ) * ( i - std::pow( 2, log2_i ) );
      for( int l = 1; l <= n; l++ ) {

        kappa += idx.s[l-1] * std::pow( 2, j * ( n - l ) );
      }
    }

    /*! void sqWaweletToND( T& i, T& j, T* k, const T& gamma, const T& kappa )
      \brief

      \tparam T Indexing typename, e.g.: short, int, unsigned int.
      \tparam n Index dimension.

      \param[out] i
      \param[out] j
      \param[out] k
      \param[in] gamma
      \param[in] kappa
    */
    template <typename T, int n>
    void sqWaweletToND( T& j, T& i, T* k, const T& gamma, const T& kappa ) {

      /////////////////////////////
      // Formula comments
      //
      // 'n' in the formulas is here
      // denoted by 'nu',
      // which again is assigned
      // 'kappa_0'.

      // Transfer vars
      IndexND<T,n> &idx = *(IndexND<T,n>*)k;

      // Compute help vars
      const int lbs = std::pow( 2, ( gamma / n ) * n ); // Level-block size
    //  std::cout << "lbs: " << lbs << std::endl;
      const int kappa_0 = kappa - ( kappa / lbs ) * lbs;
    //  std::cout << "kappa_0: " << kappa_0 << std::endl;

      // Compute j
      j = gamma / n;

      // Compute i
      i = std::pow( 2, gamma % n ) + kappa / lbs;


      // Compute the k_l's
      int nu = kappa_0;

      const int mu = gamma / n;
      for( int itr = n - 1; itr >= 0; itr-- ) {

    //    std::cout << "nu = kappa_" << (n - itr - 1) << ": " << nu << std::endl;
        T fact = std::pow( 2, mu * itr );
        idx.s[n - itr - 1] = nu / fact;
        nu -= idx.s[n - itr - 1] * fact;
      }
    }

  } // END namespace (Index)::Cartesian


  namespace Cantor {


    /*!
     *
     *  This is the univariat implementation.
     *
     *  \param T Mapping table from 1D to ND. Must be allocated: n \times \prod_{i=1}^{n}( N[i] ).
     *  \param n Dimension of resulting index
     *  \param indices The resulting indice paring must be allocated with n elements.
     *  \param dims The dimensions of the requested indice pairing.
     */
    template <typename T, int n>
    void generateMappingTableFrom1DToND( T* mapping_table, const T* dims ) {

      // Compute
      // N - the product of the resolution of all dimensions
      // L - the sum of the resolution of all dimensions, minus the dimension
      T N = 1;
      T L = -n;
      for( int i = 0; i < n; i++ ) {
        L += dims[i];
        N *= dims[i];
      }


      // List of lists of n-dim vectors
      std::vector< std::vector< IndexND<T,n> > > S( L+T(1), std::vector< IndexND<T,n> >() );

      // Indice of (k1, ..., kn)
      IndexND<T,n> k_indices;

      for( int i = 0; i < N; i++ ) {

        // Compute indices, the index of (k1, ..., kn)
        Cartesian::toNDfrom1D<T,n>( (T*)&k_indices, dims, i );

        // Sum of the indices
        T sum = getSumOfIndices<T,n>( (T*)&k_indices );

        // Append indice to the appropriate list
        S[sum].push_back( k_indices );
      }

      // Order the lists S into one one-dimensional list :)
      IndexND<T,n>* mt = (IndexND<T,n>*)mapping_table;
      // Iterate through the lists S
      for( typename::std::vector< std::vector< IndexND<T,n> > >::iterator itr_i = S.begin(); itr_i != S.end(); itr_i++ )
        // Iterate through the given list S_i
        for( typename::std::vector< IndexND<T,n> >::iterator itr_j = (*itr_i).begin(); itr_j != (*itr_i).end(); itr_j++ )
          *mt++ = *itr_j;
    }


    /*!
     *
     *  This is the multivariat implementation.
     *
     *  \param T Mapping table from 1D to ND. Must be allocated: m \times n \times \prod_{i=1}^{n}( N[i] ).
     *  \param n Dimension of resulting index
     *  \param indices The resulting indice paring must be allocated with n elements.
     *  \param dims The dimensions of the requested indice pairing.
     */
    template <typename T, int m, int n>
    void generateMappingTableFrom1DToMVND( T* mapping_table, const T* dims ) {

      // Compute
      // N - the product of the resolution of all dimensions
      // L - the sum of the resolution of all dimensions, minus the dimension
      T N = 1;
      T L = -n;
      for( int i = 0; i < n; i++ ) {
        L += dims[i];
        N *= dims[i];
      }


      // List of lists of n-dim vectors
      std::vector< std::vector< IndexMVND<T,n> > > S( L+T(1), std::vector< IndexMVND<T,n> >() );

      // Indice of (k1, ..., kn)
      IndexMVND<T,n> k_indices;

      for( int i = 0; i < N; i++ ) {

        // Compute indices, the index of (k1, ..., kn)
        Cartesian::toNDfrom1D<T,n>( (T*)&k_indices, dims, i );

        // Sum of the indices
        T sum = getSumOfIndices<T,n>( (T*)&k_indices );

        // Append indice to the appropriate list
        for( int j = 0; j < m; j++ ) {

          k_indices.m = j;
          S[sum].push_back( k_indices );
        }
      }

      // Order the lists S into one one-dimensional list :)
      IndexMVND<T,n> *mt = (IndexMVND<T,n>*)mapping_table;
      // Iterate through the lists S
      for( typename::std::vector< std::vector< IndexMVND<T,n> > >::iterator itr_i = S.begin(); itr_i != S.end(); itr_i++ )
        // Iterate through the given list S_i
        for( typename::std::vector< IndexMVND<T,n> >::iterator itr_j = (*itr_i).begin(); itr_j != (*itr_i).end(); itr_j++ )
          *mt++ = *itr_j;
    }


    template <typename T, int n, int N>
    T to1DfromMappingTable( const T* mapping_table, const T* indices ) {

      const IndexND<T,n>    &k_idx = *(const IndexND<T,n>*)indices;
      const IndexND<T,n>    *mt = (const IndexND<T,n>*)mapping_table;

      T i = T(0);
      for( i = 0; i < N; i++ )
        if( mt[i].equals( k_idx ) )
          break;

      return i;
    }


    template <typename T, int n, int N>
    T toMV1DfromMappingTable( const T* mapping_table, const T* indices ) {

      const IndexMVND<T,n>    &k_idx = *(const IndexMVND<T,n>*)indices;
      const IndexMVND<T,n>    *mt = (const IndexMVND<T,n>*)mapping_table;

      T i = T(0);
      for( i = 0; i < N; i++ )
        if( mt[i].equals( k_idx ) )
          break;

      return i;
    }


    template <typename T, int n>
    void toMVNDfromMappingTable( T* indices, const T* mapping_table, const T& index ) {

      IndexND<T,n>          &k_idx = *(IndexND<T,n>*)indices;
      const IndexND<T,n>    *mt = (const IndexND<T,n>*)mapping_table;

      k_idx = mt[index];
    }


    template <typename T, int n>
    void toNDfromMappingTable( T* indices, const T* mapping_table, const T& index ) {

      IndexMVND<T,n>          &k_idx = *(IndexMVND<T,n>*)indices;
      const IndexMVND<T,n>    *mt = (const IndexMVND<T,n>*)mapping_table;

      k_idx = mt[index];
    }

  } // END namespace (Index)::Cantor

} // END namespace Index

} // END namespace Wavelet
} // END namespace GMlib


#endif // __GM_WAVELET_INDEX_H__
