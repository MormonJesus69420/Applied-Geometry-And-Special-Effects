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



  ///////////////////////////////
  // template <typename T, int n, int m>
  // class GM_Static_P_<T,n,m>


  /*! \brief vec = mat x vec */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::mv_x(T *a, const Point<T,m>* b, const Point<T,m>& c) {

    (*a)=(*b)*c; GM_Static_P_<T,n-1,m>::mv_x(a+1,b+1,c);
  }


  /*! \brief vec = HqMat x vec */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::mv_xq(T *a, const T* b, const Point<T,m>& c) {

    (*a) = GM_Static_<T,m>::dpr(b,c.getPtr()); GM_Static_P_<T,n-1,m>::mv_xq(a+1,b+m+1,c);
  }


  /*! \brief vec = HqMat x vec + h(homogen col) */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::mv_xqP(T *a, const T* b, const Point<T,m>& c, const T* d) {

    (*a) = GM_Static_<T,m>::dpr(b,c.getPtr())+(*d); GM_Static_P_<T,n-1,m>::mv_xqP(a+1,b+m+1,c,d+m+1);
  }


  /*! \brief a = b^T	matrix transposing */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::eq_t(Point<T,m> *a, const T *b) {

    GM_Static_P2_<T,n,m,n>::eq_t(a,b);
  }


  /*! \brief a = b x c tensor (outer) product */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::vv_ox(Point<T,m> *a, const Point<T,m> *b, const T *c) {

    GM_Static_<T,m>::sc_r(a->getPtr(),b->getPtr(),double(*c)); GM_Static_P_<T,n-1,m>::vv_ox(a+1,b,c+1);
  }


  /*! \brief vec = col x mat */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::cm_x(T *a, const T* b, const T* c) {

    (*a) = GM_Static2_<T,m,m>::dpc(b,c); GM_Static_P_<T,n-1,m>::cm_x(a+1,b,c+1);
  }


  /*! \brief a = b(col)*c	matrix multiplication, but b is transposed */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::mc_x(T *a, const T *b, const T *c) {

    GM_Static_P_<T,m,m>::cm_x(a,b,c); GM_Static_P_<T,n-1,m>::mc_x(a+m,b+1,c);
  }


  /*! \brief scaling of matrix (row by row) */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::sc(Point<T,m>* a, const T *b) {

    GM_Static_<T,m>::sc(a->getPtr(),static_cast<double>(*b)); return GM_Static_P_<T,n-1,m>::sc(a+1,b+1);
  }

  /*! \brief vec = HqMat x vec */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::hq_2x(T *a, const T* b, const Point<T,m>& c) {

    (*a) = GM_Static_<T,m>::dpr(b,c.getPtr()); GM_Static_P_<T,n-1,m>::hq_2x(a+1,b+m+1,c);
  }


  /*! \brief col = vec - vec x HqMat */
  template <typename T, int n, int m>
  inline
  void GM_Static_P_<T,n,m>::hq_3x(T *a, const T* b, const Vector<T,m>& r, const T* p) {

    (*a) = (*p) - GM_Static3_<T,m,m>::dpr(r.getPtr(),b); GM_Static_P_<T,n-1,m>::hq_3x(a+m+1,b+1,r,p+1);
  }




  ///////////////////////////////
  //  template <class T,int m>
  //  class GM_Static_P_<T,1,m>


  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::mv_x(T *a, const Point<T,m>* b, const Point<T,m>& c) {

     (*a)=(*b)*c;
  }


  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::mv_xq(T *a, const T* b, const Point<T,m>& c) {

    (*a) = GM_Static_<T,m>::dpr(b,c.getPtr());
  }


  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::mv_xqP(T *a, const T* b, const Point<T,m>& c, const T* d) {

    (*a) = GM_Static_<T,m>::dpr(b,c.getPtr())+(*d);
  }


  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::vv_ox(Point<T,m> *a, const Point<T,m> *b, T *c) {

    GM_Static_<T,m>::sc_r(a->getPtr(),b->getPtr(),double(*c));
  }

  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::cm_x(T *a, const T* b, const T* c) {

    (*a) = GM_Static2_<T,m,m>::dpc(b,c);
  }

  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::mc_x(T *a, const T *b, const T *c) {

    GM_Static_P_<T,m,m>::cm_x(a,b,c);
  }

  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::sc(Point<T,m>* a, const T *b) {

    GM_Static_<T,m>::sc(a->getPtr(),static_cast<double>(*b));
  }

  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::hq_2x(T *a, const T* b, const Point<T,m>& c) {

    (*a) = GM_Static_<T,m>::dpr(b,c.getPtr());
  }

  template <typename T,int m>
  inline
  void GM_Static_P_<T,1,m>::hq_3x(T *a, const T* b, const Vector<T,m>& r, const T* p) {

    (*a) = (*p) - GM_Static3_<T,m,m>::dpr(r.getPtr(),b);
  }


  ///////////////////////////////////////////
  //template <typename T, int n, int m, int k>
  //class GM_Static_P2_<T,n,m,k>


  /*! \brief vec = vec x mat */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::vm_x(T *a, const Point<T,m>* b, const T* c) {
    (*a) = GM_Static2_<T,m,k>::dpr(b->getPtr(),c); GM_Static_P2_<T,n-1,m,k>::vm_x(a+1,b,c+1);
  }


  /*! \brief a = b*c	matrix multiplication */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::mm_x(Point<T,k> *a, const Point<T,m> *b, const T *c)	{

    GM_Static_P2_<T,k,m,k>::vm_x(a->getPtr(),b,c); GM_Static_P2_<T,n-1,m,k>::mm_x(a+1,b+1,c);
  }


  /*! \brief vec =  mat x vec (a = c x b) */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::vm_xT(T *a, const Point<T,m>* b, const T* c)	{
    (*a) = GM_Static_<T,m>::dpr(b->getPtr(),c); GM_Static_P2_<T,n-1,m,k>::vm_xT(a+1,b,c+m);
  }


  /*! \brief a = b*c	matrix multiplication (c transposed) */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::mm_xT(Point<T,k> *a, const Point<T,m> *b, const T *c) {
    GM_Static_P2_<T,k,m,k>::vm_xT(a->getPtr(),b,c); GM_Static_P2_<T,n-1,m,k>::mm_xT(a+1,b+1,c);
  }


  /*! \brief a(vec) = b(col) x mat */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::vm_xTT(T *a, const T* b, const T* c)	{
    (*a) = GM_Static2_<T,m,k>::dpc(b,c); GM_Static_P2_<T,n-1,m,k>::vm_xTT(a+1,b,c+1);
  }


  /*! \brief a = b*c	matrix multiplication (b transposed) */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::mm_xTT(Point<T,k> *a, const T *b, const T *c)	{
    GM_Static_P2_<T,k,m,k>::vm_xTT(a->getPtr(),b,c); GM_Static_P2_<T,n-1,m,k>::mm_xTT(a+1,b+1,c);
  }


  /*! \brief vec(a) = vec(c) x mat(b) (b homogen) */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::vm_xH(T *a, const Point<T,m+1>* b, const T* c)	{
    (*a) = GM_Static2_<T,m,k>::dpr(b->getPtr(),c); GM_Static_P2_<T,n-1,m,k>::vm_xH(a+1,b,c+1);
  }


  /*! \brief a = Hb * c	matrix multiplication (b homogen) */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::mm_xH(Point<T,k> *a, const Point<T,m+1> *b, const T *c) {
    GM_Static_P2_<T,k,m,k>::vm_xH(a->getPtr(),b,c); GM_Static_P2_<T,n-1,m,k>::mm_xH(a+1,b+1,c);
  }


  /*! \brief vec =  mat x vec (a = c x b) */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::vm_xHT(T *a, const Point<T,m+1>* b, const T* c) {
    (*a) = GM_Static_<T,m>::dpr(b->getPtr(),c); GM_Static_P2_<T,n-1,m,k>::vm_xHT(a+m,b,c+m);
  }


  /*! \brief a = b*c	matrix multiplication (c transposed) */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::mm_xHT(T *a, const Point<T,m+1> *b, const T *c) {
    GM_Static_P2_<T,k,m,k>::vm_xHT(a,b,c); GM_Static_P2_<T,n-1,m,k>::mm_xHT(a+1,b+1,c);
  }


  /*! \brief a = b^T	matrix transposeing (a = n x m) */
  template <typename T, int n, int m, int k>
  inline
  void GM_Static_P2_<T,n,m,k>::eq_t(Point<T,m> *a, const T *b) {
    GM_Static2_<T,m,n>::eq1(a->getPtr(),b); GM_Static_P2_<T,n,m,k-1>::eq_t(a+1,b+1);
  }






  ///////////////////////////////
  //  template <class T, int m, int k>
  //  class GM_Static_P2_<T,1,m,k>


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::vm_x(T *a, const Point<T,m>* b, const T* c) {
    (*a) = GM_Static2_<T,m,k>::dpr(b->getPtr(),c);
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::mm_x(Point<T,k> *a, const Point<T,m> *b, const T *c)	{

    GM_Static_P2_<T,k,m,k>::vm_x( a->getPtr(), b, c );
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::vm_xT(T *a, const Point<T,m>* b, const T* c)	{

    (*a) = GM_Static_<T,m>::dpr(b->getPtr(),c);
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::mm_xT(Point<T,k> *a, const Point<T,m> *b, const T *c) {
    GM_Static_P2_<T,k,m,k>::vm_xT(a->getPtr(),b,c);
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::vm_xTT(T *a, const T* b, const T* c)	{
    (*a) = GM_Static2_<T,m,k>::dpc(b,c);
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::mm_xTT(Point<T,k> *a, const T *b, const T *c) {
    GM_Static_P2_<T,k,m,k>::vm_xTT(a->getPtr(),b,c);
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::vm_xH(T *a, const Point<T,m+1>* b, const T* c) {
    (*a) = GM_Static2_<T,m,k>::dpr(b->getPtr(),c);
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::mm_xH(Point<T,k> *a, const Point<T,m+1> *b, const T *c) {
    GM_Static_P2_<T,k,m,k>::vm_xH(a->getPtr(),b,c);
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::vm_xHT(T *a, const Point<T,m+1>* b, const T* c) {
    (*a) = GM_Static_<T,m>::dpr(b->getPtr(),c);
  }


  template <typename T, int m, int k>
  inline
  void GM_Static_P2_<T,1,m,k>::mm_xHT(T *a, const Point<T,m+1> *b, const T *c) {
    GM_Static_P2_<T,k,m,k>::vm_xHT(a,b,c);
  }



  ///////////////////////////////
  //template <class T, int n, int m>
  //class GM_Static_P2_<T,n,m,1>


  /*! \brief a = b^T	matrix transposeing */
  template <typename T, int n, int m>
  void GM_Static_P2_<T,n,m,1>::eq_t(Point<T,m> *a, const T *b) {
    GM_Static2_<T,m,n>::eq1(a->getPtr(),b);
  }


} // END namespace GMlib
