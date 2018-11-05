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




#ifndef GM_PARAMETRICS_PARAMETRICS_H
#define GM_PARAMETRICS_PARAMETRICS_H


#include "evaluators/gmevaluatorstatic.h"


// gmlib
#include <core/containers/gmarray.h>
#include <scene/gmsceneobject.h>
#include <scene/utils/gmmaterial.h>

// stl
#include <string>




namespace GMlib {


  enum GM_PARAMETRICS {
    GM_POINT = 0,
    GM_CURVE = 1,
    GM_SURFACE = 2,
    GM_VOLUME = 3,
    GM_FLOW = 4
  };

  enum GM_RESAMPLE_MODE {
    GM_RESAMPLE_INLINE,
    GM_RESAMPLE_PREEVAL
  };

  enum GM_DERIVATION_METHOD {
    GM_DERIVATION_EXPLICIT,
    GM_DERIVATION_DD
  };


  /*!
   *
   *  \tparam T Template type parameter
   *  \tparam m Manifold dimension \see GM_PARAMETRICS
   *  \tparam n Manifold embedding dimension
   */
  template <typename T, int m, int n>
  class Parametrics : public SceneObject {
  public:
    Parametrics();
    Parametrics( const Parametrics<T,m,n>& copy );
   ~Parametrics();

    void                        setDerivationMethod( GM_DERIVATION_METHOD method );

  protected:
    GM_DERIVATION_METHOD        _dm;

    HqMatrix<T,m>               _A;   //! Domain transition

  private:
    void                        _initSoType();

  }; // END class Parametrics





  /******************************************************************
   ******************************************************************
   ******************************************************************
   ****** The following code is about preevaluation and partitioning
   ******       It should be in a separate file "b-spline_util.h"
   ******************************************************************
   ******************************************************************
   ******************************************************************/




  /***********************************************************************
   ****** The partition vector.
   ****** Based on continuity, the domain is partitioned.
   ****** Here a knot vector is partitioned in intervals where each
   ****** partition is expressed by a pair of indexes in the knot vector.
   ***********************************************************************/

    template <typename T>
    class VisPart : public std::vector<int> {
        T tol;  // Can be static - and depending on the type T
        bool eq(T a, T b) { return a-b < tol; }

    public:
        VisPart(const DVector<T>& t, int k, int dis) : tol(1e-5) {
            // t   - is the knot vector
            // k   - is the order of the spline
            // dis = 0 - split where we have a discontinue function
            // dis = 1 - split where we have a discontinue 1. derivative
            // dis = 2 - split where we have a discontinue 2. derivative ...
            // dis must not be bigger than d=k-1 !!!
//            tol   = 1e-5;
            int d = k-1;
            if(test1(t, d))
                comp(t, d, d);
            else if(test2(t, d)) {
                comp(t, d, dis);
                corr(t,2*d,t.getDim()-2*d-1);
            }
            else
                comp(t, d, dis);
        }


    private:
        void comp(const DVector<T>& t, int d, int dis) {
            int n = t.getDim()-d-1;
            int i = d;
            while(eq(t(i+1), t(i))) ++i;
            this->push_back(i);
            for(++i; i <= n; i++) {
                int j=1;
                while (eq(t(i+j), t(i))) ++j;
                if(i+j-1 >= n) {
                    this->push_back(i);
                    break;
                }
                if(j > d-dis) {
                    this->push_back(i);
                    this->push_back(i+=j-1);
                }
            }
        }


        void corr(const DVector<T>& t, int a, int b) {
            unsigned int i=1;
            for(; i < this->size(); i++)
                if((*this)[i] > b) break;
            if((*this)[i-1] < a) {
                for(int j = b; j>=a; j--)
                    if(t(j-1) < t(j)) break;
                this->insert(this->begin()+i,2,b);
            }
        }


        bool test1( const DVector<T>& t, int d ) {
            if(eq(t(d),t(0)))   return false;
            else                return t.getDim() < (4*d+1);
        }


        bool test2( const DVector<T>& t, int d ) {
            if(eq(t(d),t(0)))   return false;
            else                return t.getDim() >= (4*d+1);
        }
    }; // end class VisPart



  /********************************************************************
   ****** The samling vector
   ****** Determines how sampling is distributed over the partitioning
   *******************************************************************/

    template <typename T>
    class SampNr : public std::vector<int> {
    public:
        SampNr(const DVector<T>& t, const VisPart<T>& vp, int m) {
            // t   - is the knot vector
            // vp  - is the partition vector
            // m   - is the total number of samples (at least)
            T tot = t(vp.back())- t(vp.front());
            int n = vp.size()/2;
            this->resize(n);

            for(int i=0; i < n; i++) {
                T tmp = T(m+n-1)*(t(vp[2*i+1]) - t(vp[2*i]))/tot + T(0.5);
                (*this)[i] = int(tmp);
            }
        }
    }; // end class SampNr





  /********************************************************************
   ****** The index vector for B-splines
   ****** makes the indices for the controll points in evaluation
   *******************************************************************/

    class IndexBsp : public std::vector<int> {
    public:
        IndexBsp( int i, int k, int n );
        IndexBsp(){}
        IndexBsp( const IndexBsp& ind ) : std::vector<int>(ind)  {}

        void init( int i, int k, int n );

    private:
        void _init( int i, int k, int n );
    };



    inline
    IndexBsp::IndexBsp( int i, int k, int n ) : std::vector<int>(k) {
        _init( i, k, n );
    }

    inline
    void IndexBsp::init( int i, int k, int n ) {
        this->resize(k);
        _init( i, k, n );
    }

    inline
    void IndexBsp::_init( int i, int k, int n ) {
        // i    - the index of the knot on left side of the value: t[i] <= t < t[i+1]
        // k    - the order of the spline
        // n    - the number of control points
        i -= (k-1);
        if( i+k > n) { // closed, and go circular
            int j, s = n-i;
            for (j=0; j < s; j++)
                (*this)[j] = i++;
            while(j < k)
                for (i=0 ; i < n; i++) {
                    (*this)[j++] = i;
                    if(j==k) break;
                }
        }
        else    // ordinary, only a sequence.
            for(int j=0; j<k; j++)
                (*this)[j]= i+j;
    }


/*************************************************************************
 ****** B*c^T, where PreMat.m = B and PreMat.ind = index to pick part of c
 *************************************************************************/

    struct VisuPar {
        int m;
        int is, ie;
    };


    template <typename T>
    struct Partition : public std::vector<DVector<Vector<T,3>>> {      //!< Vertices for plotting
      std::vector<T>           sample_val;  //!< The parameter values for each sample points
      Sphere<T,3>              sur_sphere;  //!< Surrounding sphere of this set
      Vector<T,2>              s_e;         //!< Start and end parameter values for this sampling
      std::vector<Visualizer*> vis;         //!< Visualizers for plotting
    };


    template <typename T>
    struct PreMat : public DMatrix< T > {  // B-spline hermite matrix at a given t
        IndexBsp     ind;   // The index of the coeffisients to compute
    };


    template <typename T>
    struct PreBasis : public std::vector<PreMat<T>> {
 //       PreBasis() : std::vector<PreMat<T>>(1) {}
    };



/********************************************************************
 ****** Prepare making partitions of geometry based on continuity
 *******************************************************************/
    template <typename T>
    inline
    void  preparePartition( DVector<VisuPar>& vp, const DVector<T>& t, int k, int dis, int m ) {

        VisPart<T> pu( t, k, dis );
        SampNr<T>  su( t, pu, m );
        vp.setDim(su.size());
        for(int i=0; i<vp.getDim(); i++) {
            vp[i].m  = su[i];
            vp[i].is = pu[2*i];
            vp[i].ie = pu[2*i+1];
        }
    }


  /*!  void  makePartition( std::vector<Partition2<T>>& partition, const DVector<T>& t, int k, int n, int dis, int m, bool close = false )
   *  To make a partitioning of a spline.
   *
   *  \param[out] partition  The result of the computation (return value)
   *  \param[in]  t          The knot vector
   *  \param[in]  k          The order of the spline (degree + 1)
   *  \param[in]  n          the number of control points
   *  \param[in]  dis        the partition criteria (continuity C^dis)
   *  \param[in]  m          the number of sample points
   *  \param[in]  close      whether the spline is open or closed (cyclic)
   */
//    template <typename T>
//    inline
//    void  makePartition( PreBasis& pre_bas, std::vector<T> sam,
//                         const DVector<T>& t, int k, int n, int dis, int m, bool close = false ) {

//        VisPart<T> pu( t, k, dis );
//        if(close && pu.size()==2) {
//            pu.resize(4);
//            pu[3] = pu[1];
//            pu[1] = pu[2] = n;
//        }
//        SampNr<T>  su( t, pu, m );
//        pre_bas.resize(su.size());
//        for(uint i=0; i<pre_bas.size(); i++) {
//            pre_bas[i].resize(su[i]);
//            pre_bas[i].is = pu[2*i];
//            pre_bas[i].ie = pu[2*i+1];
//            pre_bas[i].samp.resize(su[i]);
//            T dt = (t[pre_bas[i].ie] - t[pre_bas[i].is])/(su[i]-1);
//            for(int j=0; j<su[i]-1; j++) {
//                pre_bas[i].samp[j] = t[pre_bas[i].is] + j*dt;
//                int ii = EvaluatorStatic<T>::evaluateBSp( pre_bas[i][j].m, pre_bas[i].samp[j], t, k-1, false);
//                pre_bas[i][j].ind.init( ii, k, n );
//            }
//            pre_bas[i].samp[su[i]-1] = t[pre_bas[i].ie];
//            int ii = EvaluatorStatic<T>::evaluateBSp( partition[i][su[i]-1].m, pre_bas[i].samp[su[i]-1], t, k-1, true);
//            pre_bas[i][su[i]-1].ind.init( ii, k, n );
//        }
//    }


    // Sampling for pre-evaluation, independent of direction
    //*******************************************************
    template <typename T>
    void preSamples( DVector< PreMat<T> >& p, const DVector<T>& t, int m, int d, int n, T start, T end ) {

        // compute dt (step in parameter)
        const T dt = ( end - start ) / T(m-1);

        // Set the dimension of the Bernstein-Hermite Polynomial DVector
        p.setDim(m);

        // Compute the Bernstein-Hermite Polynomiale, for the B-spline Surface
        for( int j = 0; j < m-1; j++ ) {
            int i = EvaluatorStatic<T>::evaluateBSp( p[j].m, start+j*dt, t, d, false ) - d;
            p[j].ind.init( i, d+1, n);
        }
        int i = EvaluatorStatic<T>::evaluateBSp( p[m-1].m, end, t, d, true ) - d;
        p[m-1].ind.init( i, d+1, n);
    }


    template <typename T>
    inline
    void computeUniformParamVal( std::vector<T>& sample, int m, T s, T e ) {
        sample.resize(m--);
        T dt = (e-s)/m;
        sample[0] = s;
        for (int i=1; i<m; i++)
            sample[i] = s + i*dt;
        sample[m] = e;
    }




} // END namespace GMlib




// Include Paramterics class function implementations
#include "gmparametrics.c"

#endif // GM_PARAMETRICS_PARAMETRICS_H
