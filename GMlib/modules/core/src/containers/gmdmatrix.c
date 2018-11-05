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





// STL includes
#include <cmath>

// Platform
//#include <omp.h>


#ifdef GM_INTEL
#include <mkl_lapack.h>	// NBNBNB!!!   Remember also to link with mkl_c.lib // on mkl7.0 it is mkl_lapack32.h
#endif


namespace GMlib {


  template<typename T>
  inline
  DMatrix<T>::DMatrix(int i, int j) {
    _p = (i>4 ? new DVector<T>[i]:_init); _n=i;
    for (int k=0; k<_n; k++) _p[k].setDim(j);
    _private = true;
  }


  template<typename T>
  inline
  DMatrix<T>::DMatrix(int i, int j, T val) {
    _p = (i>4 ? new DVector<T>[i]:_init); _n=i;
    for(int k=0; k<_n; k++) _p[k].increaseDim(j, val);
    _private = true;
  }


  template<typename T>
  inline
  DMatrix<T>::DMatrix(int i, int j, const T p[]) {

    _p = (i>4 ? new DVector<T>[i]:_init);
    _n = i;
    for (int k=0; k<_n; k++) _p[k].setDim(j);
    _cpy(p);
    _private = true;
  }


  template<typename T>
  inline
  DMatrix<T>::DMatrix(const DMatrix<T>& v) {
    _n=0; _p=_init;
    _cpy(v);
    _private = true;
  }


  template<typename T>
  inline
  DMatrix<T>::~DMatrix() {
    if(_p != _init && _private) delete [] _p;
  }



//  template <typename T>
//  inline
//  T DMatrix<T>::getDeterminant() const {}


  template <typename T>
  inline
  int DMatrix<T>::getDim1() const	{
    return _n;
  }


  template <typename T>
  inline
  int DMatrix<T>::getDim2() const	{
    if (_p) return _p[0].getDim();
    return 0;
  }


  template <typename T>
  inline
  void  DMatrix<T>::increaseDim(int i, int j, T val, bool h_end, bool v_end) {

    if(i>0)
    {
      int k, m  = _n + i;
      DVector<T>* tmp = (m>4 ? new DVector<T>[m] : _init);
      if(v_end)
      {
        if(m>4) for (k=0; k<_n; k++)
        {
          tmp[k] = _p[k];
          tmp[k].increaseDim(j,val,h_end);
        }
        for (k=_n; k<m; k++)
        {
          tmp[k].setDim(j);
          tmp[k].clear(val);
        }
      }
      else
      {
        for (k=m-1; k>=i; k--)
        {
          tmp[k] = _p[k-i];
          tmp[k].increaseDim(j,val,h_end);
        }
        for (k=0; k<i; k++)
        {
          tmp[k].setDim(j);
          tmp[k].clear(val);
        }
      }
      if(_p != _init) delete [] _p;
      _p = tmp;
      _n = m;
    }
  }


  /*! \brief Pending more documentation
   *
   *  Implementation of inverting using either lapack
   *  or local non optimal implementation.
   */
  template <typename T>
  DMatrix<T>& DMatrix<T>::invert() {

  #ifdef GM_INTEL       // This requires MKL from Intel (mkl_c.lib),
                        // or BLAS and Lapack binaries in a compatible format
    if(getDim1()==getDim2() && getDim1()>1)
    {
      int nk=getDim1();
      Array<float> aa;
      aa.setSize(nk*nk);
      for(int i=0; i<nk; i++)   // daft cast
        for(int j=0; j<nk; j++) aa[i+nk*j] = (float) (*this)[i][j];

      Array<float> work; work.setSize(nk*32);	// temporary work array
      int wsize=nk*32;                          // work array size - should be optimized
      Array<int> ipiv; ipiv.setSize(nk);        // pivot table (result), size max(1,mmm,nnn)
      int info=0;                               // error message, i=info>0 means that a[i][i]=0 ): singular,
      int mmm=nk;                               // dimentsion
      int nnn=nk;                               // dimentsion  i.e. a is mxn matrix
      int lda=nk;                               // leading dimentsion
      // float *a                               // input array

      // LAPACK When calling LAPACK routines from C-language programs, make sure that you follow Fortran rules:
      // Pass variables by 'address' as opposed to pass by 'value'. Be sure to store your data Fortran-style,
      // i.e. data stored column-major rather than row-major order
      // Project neeeds mkl_c.lib and mkl_lapack32.h

      sgetrf(&mmm, &nnn, aa.ptr(), &lda, ipiv.ptr(), &info);               // using Lapack LU-fact a is overwritten by LU
      sgetri(&mmm, aa.ptr(), &lda, ipiv.ptr(), work.ptr(), &wsize, &info); // a should now contain the inverse

      // if(info==0)
      for(int i=0; i<nk; i++)
        for(int j=0; j<nk; j++) (*this)[i][j]=(T) aa[i+j*nk];
    }
  #else           // gauss-jordan implementation from Numerical recipes
                  // Ordinary LU-decomp used.
    DMatrix<T> a=(*this);
    Array<int> indx(a.getDim2());

    const double TINY=1.0e-20;
    int i,imax,j,k;
    double big,dum,sum,temp;

    int n=a.getDim1();  //nrows
    Array<double> vv(n);
    double d=1.0;
#ifdef _OPENMP
  #pragma omp parallel for
#endif
    for (i=0;i<n;i++)
    {
      big=0.0;
      for (j=0;j<n;j++) if ((temp=std::abs(a[i][j])) > big) big=temp;
//      if (big == 0.0) return (*this);   //nrerror("Singular matrix in routine ludcmp");
      vv[i]=1.0/big;
    }
#ifdef _OPENMP
  #pragma omp parallel for
#endif
    for (j=0;j<n;j++)
    {
#ifdef _OPENMP
  #pragma omp parallel for
#endif
      for (i=0;i<j;i++)
      {
        sum=a[i][j];
        for (k=0;k<i;k++) sum -= a[i][k]*a[k][j];
        a[i][j]=sum;
      }
      big=0.0;
#ifdef _OPENMP
  #pragma omp parallel for
#endif
      for (i=j;i<n;i++)
      {
        sum=a[i][j];
        for (k=0;k<j;k++) sum -= a[i][k]*a[k][j];
        a[i][j]=sum;
        if ((dum=vv[i]*std::abs(sum)) >= big) { big=dum; imax=i; }
      }
      if (j != imax)
      {
#ifdef _OPENMP
  #pragma omp parallel for
#endif
        for (k=0;k<n;k++)
        {
          dum=a[imax][k];
          a[imax][k]=a[j][k];
          a[j][k]=dum;
        }
        d = -d;
        vv[imax]=vv[j];
      }
      indx[j]=imax;
      if (a[j][j] == 0.0) a[j][j]=TINY;
      if (j != n-1)
      {
        dum=1.0/(a[j][j]);
        for (i=j+1;i<n;i++) a[i][j] *= dum;
      }
    }                           // LU-decomp. finished, stored in a[][]

    DVector<T> b(a.getDim1());  // LU-back subst. begins
#ifdef _OPENMP
  #pragma omp parallel for
#endif
    for(int cols=0; cols<a.getDim2(); cols++)
    {
      int i2,ii=0,ip,j2;
      for(i2=0; i2<getDim1(); i2++) if(i2==cols) b[i2]=T(1.0); else b[i2]=T(0.0);
      T sum2;
      int n2=a.getDim1();
#ifdef _OPENMP
  #pragma omp parallel for
#endif
      for (i2=0;i2<n2;i2++)
      {
        ip=indx[i2];
        sum2=b[ip];
        b[ip]=b[i2];
        if (ii != 0)
          for (j2=ii-1;j2<i2;j2++) sum2 -= a[i2][j2]*b[j2];
        else if (sum2 != 0.0) ii=i2+1;
        b[i2]=sum2;
      }
#ifdef _OPENMP
  #pragma omp parallel for
#endif
      for (i2=n2-1;i2>=0;i2--)
      {
        sum2=b[i2];
        for (j2=i2+1;j2<n2;j2++) sum2 -= a[i2][j2]*b[j2];
        b[i2]=sum2/a[i2][i2];
      }
      // LU-back subst. finished,
#ifdef _OPENMP
  #pragma omp parallel for
#endif
      for(i2=0; i2<getDim1(); i2++)    // inverse stored in this, a and b is disappearing?
        (*this)[i2][cols]=b[i2];
    }
  #endif
    return (*this);
  }


  template <typename T>
  inline
  void  DMatrix<T>::resetDim(int i, int j) {
    int m, k = std::min<int>(i,_n);
    DVector<T>* tmp = (i>4 ? new DVector<T>[i]:_init);
    for(m=0; m<k; m++)
    {
      tmp[m] = _p[m];
      tmp[m].resetDim(j);
    }
    for(; m<i; m++)
    {
      tmp[m].setDim(j);
      tmp[m].clear(T());
    }
    if(_p != _init) delete [] _p;
    _n = i;
    _p = tmp;
  }


  template <typename T>
  inline
  void  DMatrix<T>::setDim(int i, int j) {
    if(i>4 && i>_n)
    {
      if(_p != _init) delete [] _p;
      _p = new DVector<T>[i];
    }
    _n = i;
    for (int k=0; k<_n; k++) _p[k].setDim(j);
  }


  template <typename T>
  inline
  void  DMatrix<T>::setIdentity() {
    for(int i=0; i<getDim1(); i++)
      for(int j=0; j<getDim2(); j++)
        if(i==j)	(*this)[i][j]=T(1.0);
        else		(*this)[i][j]=T(0.0);
  }


  template <typename T>
  inline
  DVector<T> DMatrix<T>::toDVector() const {
    DVector<T> r(getDim1()*getDim2());
    for(int k=0,i=0; i<getDim1(); i++)
      for(int j=0; j<getDim2(); j++)
        r[k++]=(*this)(i)(j);
    return r;
  }


  template <typename T>
  inline
  DMatrix<T>&	DMatrix<T>::transpose() {
    int i,j;
    if(getDim1()!=getDim2())
    {
      DVector<T>*    ptn= new DVector<T>[getDim2()];
      for (int s=0;s<getDim2();s++) ptn[s].setDim(getDim1());
      for(i=0; i<getDim1(); i++)
        for(j=0;j<getDim2();j++)
          ptn[j][i]=_p[i][j];
      _n=getDim2();
      delete [] _p;
      _p = ptn;
    }else
    {
      for(i=0; i<getDim1(); i++)
        for(j=0;j<i;j++)
          std::swap(_p[j][i],_p[i][j]);
    }
    return (*this);
  }


  template <typename T>
  inline
  DMatrix<T>& DMatrix<T>::operator=(const DMatrix<T>& v)	{
    _cpy(v);
    return(*this);
  }


  template <typename T>
  inline
  DMatrix<T>& DMatrix<T>::operator=(T p[]) {

    _cpy(p);
    return(*this);
  }


  /*! \brief Return given element. */
  template <typename T>
  inline
  DVector<T>&  DMatrix<T>::operator[](int i) {
  #ifdef DEBUG
    if (i<0 || i>=_n) std::cerr << "Error index m " << i << " is outside(0," << _n << ")\n";
  #endif
    return _p[i];
  }


  /*! \brief Return copy of given element. */
  template <typename T>
  inline
  const DVector<T>&  DMatrix<T>::operator()(int i) const {
  #ifdef DEBUG
    if (i<0 || i>=_n) std::cerr << "Error index m " << i << " is outside(0," << _n << ")\n";
  #endif
    return _p[i];
  }


  template <typename T>
  inline
  bool DMatrix<T>::operator<(const DMatrix<T>& v) const {
    return getDeterminant() < v.getDeterminant();
  }

  /*! \brief Element-wise += */
  template<typename T>
  inline
  DMatrix<T>& DMatrix<T>::operator+=(const DMatrix<T>& v) {
  #ifdef DEBUG
    if (_n != v.getDim())
      std::cerr << "Matrix dimension error, dim=" << _n << " ,dim=" << v.getDim() << std::endl;
  #endif
    for (int i=0; i <_n; i++) _p[i] += v._p[i];
    return *this;
  }


  /*! \brief Element-wise -= */
  template<typename T>
  inline
  DMatrix<T>& DMatrix<T>::operator-=(const DMatrix<T>& v) {
  #ifdef DEBUG
    if (_n != v.getDim())
      std::cerr << "Matrix dimension error, dim=" << _n << " ,dim=" << v.getDim() << std::endl;
  #endif
    for (int i=0; i <_n; i++) _p[i] -= v._p[i];
    return *this;
  }


  /*! \brief Element-wise + */
  template<typename T>
  inline
  DMatrix<T> DMatrix<T>::operator+(const DMatrix<T>& a) const {
    DMatrix<T> na=(*this);
    return na += a;
  }


  /*! \brief Element-wise - */
  template<typename T>
  inline
  DMatrix<T> DMatrix<T>::operator-(const DMatrix<T>& a) const {
    DMatrix<T> na=(*this);
    return na -= a;
  }


  template <typename T>
  inline
  DMatrix<T>& DMatrix<T>::operator*=(double d) {
    for(int i=0; i<_n; i++) _p[i] *= d;
    return *this;
  }


  template <typename T>
  inline
  DMatrix<T>& DMatrix<T>::operator/=(double d) {
    for(int i=0; i<_n; i++) _p[i] /= d;
    return *this;
  }


  template <typename T>
  inline
  DMatrix<T> DMatrix<T>::operator*(double d) const {
    DMatrix<T> ret = *this;
    return ret *= d;
  }


  template <typename T>
  inline
  DMatrix<T> DMatrix<T>::operator/(double d) const {
    DMatrix<T> ret = *this;
    return ret /= d;
  }


  template <typename T>
  inline
  void  DMatrix<T>::_cpy(const DMatrix<T>& v) {
    if(v._n>4 && v._n>_n)
    {
      if (_p != _init) delete [] _p;
      _p = new DVector<T>[v._n];
    }
    _n = v._n;
    for(int i=0; i<_n; i++) _p[i] = v._p[i];
  }


  template <typename T>
  inline
  void  DMatrix<T>::_cpy(const T p[]) {
    for(int k=0,i=0; i<_n; i++)
      for(int j=0;j<_p[i].getDim();j++)	_p[i][j] = p[k++];
  }

} // END namespace GMlib
