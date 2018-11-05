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





#include "gmperbssurf.h"


namespace GMlib {

  /*! PERBSSurf<T>::PERBSSurf()
   *
   *  Default constructor.
   *  Dummy.
   */
  template <typename T>
  inline
  PERBSSurf<T>::PERBSSurf() {

    this->_type_id = GM_SO_TYPE_SURFACE_ERBS;

    init();
  }

  /*! PERBSSurf<T>::PERBSSurf( const DMatrix< DMatrix< Vector<T,3> > >& c, const DVector<T>& u, const DVector<T>& v, bool closed_u, bool closed_v)
   *
   *  Creates a Parametric Expo-Rational Surface interpolating the data points provided in c.
   *  Interpolates according to the knot vectors u and v.
   *
   *  \param[in]  c         Data set. Each data point consists of { position, 1st derivative, 2nd derivative, ... }.
   *  \param[in]  u         Knot vector for parametric direction u.
   *  \param[in]  v         Knot vector for parametric direction v.
   *  \param[in]  closed_u  Whether data set is closed in parametric direction u.
   *  \param[in]  closed_v  Whether data set is closed in parametric direction v.
   */
  template <typename T>
  inline
  PERBSSurf<T>::PERBSSurf( const DMatrix< DMatrix< Vector<T,3> > >& c, const DVector<T>& u, const DVector<T>& v, bool closed_u, bool closed_v ) : _u(u), _v(v) {

    this->_type_id = GM_SO_TYPE_SURFACE_ERBS;

    init();

    _closed_u = closed_u;
    _closed_v = closed_v;

    // Set dimension of data set
    int udim, vdim;

    if( _closed_u ) udim = c.getDim1() + 1;
    else            udim = c.getDim1();

    if( _closed_v ) vdim = c.getDim2() + 1;
    else            vdim = c.getDim2();

    _c.setDim( udim, vdim );

    // knot vectors
    padKnotVector( _u, _closed_u );
    padKnotVector( _v, _closed_v );

    // Generate local patches (data set)
    for( int i = 0; i < c.getDim1(); i++ ) {
      for( int j = 0; j < c.getDim2(); j++ ) {

        _c[i][j] = new PBezierSurf<T>(
            c(i)(j),
            _u[i], _u[i+1], _u[i+2],
            _v[j], _v[j+1], _v[j+2]
            );
        insertPatch( _c[i][j] );
      }
    }

    if( _closed_u )
      for( int j = 0; j < _c.getDim2(); j++ )
        _c[_c.getDim1()-1][j] = _c[0][j];

    if( _closed_v )
      for( int i = 0; i < _c.getDim1(); i++ )
        _c[i][_c.getDim2()-1] = _c[i][0];
  }

  /*! PERBSSurf<T>::PERBSSurf( const DMatrix< DMatrix< Vector<T,3> > >& c, T s_u, T e_u, T s_v, T e_v, bool closed_u, bool closed_v )
   *
   *  Creates a Parametric Expo-Rational Surface interpolating the data points provided in c.
   *  Interpolates according to the knot vectors u and v.
   *
   *  \param[in]  c         Data set. Each data point consists of { position, 1st derivative, 2nd derivative, ... }.
   *  \param[in]  u_s       Knot vector interval start (parametric direction u).
   *  \param[in]  u_e       Knot vector interval end (parametric direction u).
   *  \param[in]  v_s       Knot vector interval start (parametric direction v).
   *  \param[in]  v_e       Knot vector interval end (parametric direction v).
   *  \param[in]  closed_u  Whether data set is closed in parametric direction u.
   *  \param[in]  closed_v  Whether data set is closed in parametric direction v.
   */
  template <typename T>
  inline
  PERBSSurf<T>::PERBSSurf( const DMatrix< DMatrix< Vector<T,3> > >& c, T u_s, T u_e, T v_s, T v_e, bool closed_u , bool closed_v ) {

    this->_type_id = GM_SO_TYPE_SURFACE_ERBS;

    init();

    _closed_u = closed_u;
    _closed_v = closed_v;

    int udim, vdim;

    if( _closed_u ) udim = c.getDim1() + 1;
    else            udim = c.getDim1();

    if( _closed_v ) vdim = c.getDim2() + 1;
    else            vdim = c.getDim2();

    _c.setDim( udim, vdim );

    generateKnotVector(
        _u,
        u_s,
        (u_e - u_s) / (_c.getDim1()-1),
        _c.getDim1()+2,
        closed_u
        );

    generateKnotVector(
        _v,
        v_s,
        (v_e - v_s) / (_c.getDim2()-1),
        _c.getDim2()+2,
        closed_v
        );

    for( int i = 0; i < c.getDim1(); i++ ) {
      for( int j = 0; j < c.getDim2(); j++ ) {

        _c[i][j] = new PBezierSurf<T>(
            c(i)(j),
            _u[i], _u[i+1], _u[i+2],
            _v[j], _v[j+1], _v[j+2]
            );
        insertPatch( _c[i][j] );
      }
    }

    if( _closed_u )
      for( int j = 0; j < _c.getDim2(); j++ )
        _c[_c.getDim1()-1][j] = _c[0][j];

    if( _closed_v )
      for( int i = 0; i < _c.getDim1(); i++ )
        _c[i][_c.getDim2()-1] = _c[i][0];
  }

  /*! PERBSSurf<T>::PERBSSurf( const DMatrix< PBezierSurf<T>* >& c, DVector<T> u, DVector<T> v, bool closed_u, bool closed_v )
   *
   *  Creates a Parametric Expo-Rational Surface interpolating the local patches provided in c.
   *  Interpolates according to the knot vectors u and v.
   *
   *  \param[in] c        Local patches. Each BezierSurf is a local patch for an interpolation point.
   *  \param[in] u        Knot vector for parametric direction u.
   *  \param[in] v        Knot vector for parametric direction v.
   *  \param[in] closed_u Whether data set is closed in parametric direction u.
   *  \param[in] closed_v Whether data set is closed in parametric direction v.
   */
  template <typename T>
  inline
  PERBSSurf<T>::PERBSSurf( const DMatrix< PBezierSurf<T>* >& c, DVector<T> u, DVector<T> v, bool closed_u, bool closed_v ) : _u(u), _v(v) {

    this->_type_id = GM_SO_TYPE_SURFACE_ERBS;

    init();

    _closed_u = closed_u;
    _closed_v = closed_v;

    // knot vectors
    padKnotVector( _u, _closed_u );
    padKnotVector( _v, _closed_v );


    // Add local patches
    _c.setDim( c.getDim1(), c.getDim2() );
    for( int i = 0; i < c.getDim1(); i++ ) {
      for( int j = 0; j < c.getDim2(); j++ ) {
        _c[i][j] = c(i)(j);
        insertPatch( _c[i][j] );
      }
    }
  }

  /*! PERBSSurf<T>::PERBSSurf( PSurf<T>* g, int no_locals_u, int no_locals_v, int d1, int d2 )
   *
   *  Creates a Parametric Expo-Rational Surface approximating the surface g.
   *  Quality of the interpolation is controlled by the number of local patches and number of derivatives.
   *
   *  \param[in]  g             The surface to approximate.
   *  \param[in]  no_locals_u   Number of local patches to create in the u-direction.
   *  \param[in]  no_locals_v   Number of local patches to create in the v-direction.
   *  \param[in]  d1            Number of derivatives to use when creating local patches in u.
   *  \param[in]  d2            Number of derivatives to use when creating local patches in v.
   */
  template <typename T>
  inline
  PERBSSurf<T>::PERBSSurf( PSurf<T,3>* g, int no_locals_u, int no_locals_v, int d1, int d2 ) {

    this->_type_id = GM_SO_TYPE_SURFACE_ERBS;

    init();

    _closed_u = g->isClosedU();
    _closed_v = g->isClosedV();

    if( _closed_u ) no_locals_u++;
    if( _closed_v ) no_locals_v++;

    _c.setDim( no_locals_u, no_locals_v );
    generateKnotVector( g );

    // Create the inner surface patches
    int i, j;
    for( i = 0; i < no_locals_u-1; i++ ) {    // locals should be -1, and then later handle the edges.
      for( j = 0; j < no_locals_v-1; j++ ) {

        _c[i][j] = new PBezierSurf<T>(
          g->evaluateParent( _u[i+1], _v[j+1], d1, d2 ),
          _u[i], _u[i+1], _u[i+2],
          _v[j], _v[j+1], _v[j+2]
        );
        insertPatch( _c[i][j] );
      }

      if( _closed_v )
        _c[i][j] = _c[i][0];
      else {
        _c[i][j] = new PBezierSurf<T>(
          g->evaluateParent( _u[i+1], _v[j+1], d1, d2 ),
          _u[i], _u[i+1], _u[i+2],
          _v[j], _v[j+1], _v[j+2]
        );
        insertPatch( _c[i][j] );
      }
    }

    if( _closed_u )
      for( j = 0; j < no_locals_v; j++ )
        _c[i][j] = _c[0][j];
    else{
      for( j = 0; j < no_locals_v; j++ ) {
        _c[i][j] = new PBezierSurf<T>(
          g->evaluateParent( _u[i+1], _v[j+1], d1, d2 ),
          _u[i], _u[i+1], _u[i+2],
          _v[j], _v[j+1], _v[j+2]
        );
        insertPatch( _c[i][j] );
      }
    }

    for( int i = 0; i < _c.getDim1(); i++ ) {
      for( int j = 0; j < _c.getDim2(); j++ ) {

        _c[i][j]->setLighted( true );
        _c[i][j]->setMaterial( GMmaterial::ruby() );
      }
    }
  }

  /*! PERBSSurf<T>::PERBSSurf( PSurf<T>* g, int no_locals_u, int no_locals_v, int d1, int d2, T u_s, T u_e, T v_s, T v_e )
   *
   *  Creates a Parametric Expo-Rational Surface approximating a sub-surface of the surface g.
   *  Quality of the interpolation is controlled by the number of local patches and number of derivatives.
   *
   *  \param[in]  g             The surface to approximate.
   *  \param[in]  no_locals_u   Number of local patches to create in the u-direction.
   *  \param[in]  no_locals_v   Number of local patches to create in the v-direction.
   *  \param[in]  d1            Number of derivatives to use when creating local patches in u.
   *  \param[in]  d2            Number of derivatives to use when creating local patches in v.
   *  \param[in]  u_s           The start parameter for the u-direction.
   *  \param[in]  u_e           The end parameter for the u-direction.
   *  \param[in]  v_s           The start parameter for the v-direction.
   *  \param[in]  v_e           The end parameter for the v-direction.
   */
  template <typename T>
  inline
  PERBSSurf<T>::PERBSSurf( PSurf<T,3>* g, int no_locals_u, int no_locals_v, int d1, int d2, T u_s, T u_e, T v_s, T v_e ) {

    this->_type_id = GM_SO_TYPE_SURFACE_ERBS;

    init();

    if( u_s < 0 ) u_s = 0;
    if( u_s > 1 ) u_s = 1;
    if( u_e < 0 ) u_e = 0;
    if( u_e > 1 ) u_e = 1;
    if( v_s < 0 ) v_s = 0;
    if( v_s > 1 ) v_s = 1;
    if( v_e < 0 ) v_e = 0;
    if( v_e > 1 ) v_e = 1;


    if( u_e - u_s >= 1 )
      _closed_u = g->isClosedU();
    else
      _closed_u = false;

    if( v_e - v_s >= 1 )
      _closed_v = g->isClosedV();
    else
      _closed_v = false;

    if( _closed_u ) no_locals_u++;
    if( _closed_v ) no_locals_v++;

    _c.setDim( no_locals_u, no_locals_v );
    generateKnotVector( g, u_s, u_e, v_s, v_e );

    // Create the inner surface patches
    int i, j;
    for( i = 0; i < no_locals_u-1; i++ ) {    // locals should be -1, and then later handle the edges.
      for( j = 0; j < no_locals_v-1; j++ ) {

        _c[i][j] = new PBezierSurf<T>(
          g->evaluateParent( _u[i+1], _v[j+1], d1, d2 ),
          _u[i], _u[i+1], _u[i+2],
          _v[j], _v[j+1], _v[j+2]
        );
        insertPatch( _c[i][j] );
      }

      if( _closed_v )
        _c[i][j] = _c[i][0];
      else {
        _c[i][j] = new PBezierSurf<T>(
          g->evaluateParent( _u[i+1], _v[j+1], d1, d2 ),
          _u[i], _u[i+1], _u[i+2],
          _v[j], _v[j+1], _v[j+2]
        );
        insertPatch( _c[i][j] );
      }
    }

    if( _closed_u )
      for( j = 0; j < no_locals_v; j++ )
        _c[i][j] = _c[0][j];
    else{
      for( j = 0; j < no_locals_v; j++ ) {
        _c[i][j] = new PBezierSurf<T>(
          g->evaluateParent( _u[i+1], _v[j+1], d1, d2 ),
          _u[i], _u[i+1], _u[i+2],
          _v[j], _v[j+1], _v[j+2]
        );
        insertPatch( _c[i][j] );
      }
    }

    for( int i = 0; i < _c.getDim1(); i++ ) {
      for( int j = 0; j < _c.getDim2(); j++ ) {

        _c[i][j]->setLighted( true );
        _c[i][j]->setMaterial( GMmaterial::ruby() );
      }
    }
  }

  /*! PERBSSurf<T>::PERBSSurf( const PERBSSurf<T>& copy )
   *
   *  Copy constructor.
   *
   *  \param[in]  copy  Parametric Expo-Rational surface to copy.
   */
  template <typename T>
  inline
  PERBSSurf<T>::PERBSSurf( const PERBSSurf<T>& copy ) : PSurf<T,3>( copy ) {

    init();

    _closed_u = copy._closed_u;
    _closed_v = copy._closed_v;

    _ru = copy._ru;
    _rv = copy._rv;
    _Bc = copy._Bc;

    _c0 = copy._c0;
    _c1 = copy._c1;

    _uk = copy._uk;
    _vk = copy._vk;

    _u  = copy._u;
    _v  = copy._v;

    // sync local patches
    const DMatrix< PSurf<T,3>* >  &c = copy._c;
    _c.setDim( c.getDim1(), c.getDim2() );
    Array< unsigned int > cl;
    Array< std::pair<int,int> > cli;

    for( int i = 0; i < c.getDim1(); ++i ) {
      for( int j = 0; j < c.getDim2(); ++j ) {
        std::pair<int,int> idx(i,j);
        cli += idx;
        cl += c(i)(j)->getName();
      }
    }

    // find and save the reference to the local patch
    for( int i = 0; i < this->getChildren().getSize(); ++i ) {

      SceneObject *child = this->getChildren()[i];
      for( int j = cl.getSize()-1; j >= 0; --j ) {

        if( cl[j] == child->_copy_of->getName() ) {

          std::pair<int,int> idx = cli[j];

          // Static cast to PSurf<T>* as the local patch has to be of this type.
          _c[idx.first][idx.second] = static_cast<PSurf<T,3>*>(child);

          cl.removeIndex(j);
          cli.removeIndex(j);
        }
      }
    }
  }

  /*! PERBSSurf<T>::~PERBSSurf()
   *
   *  Destructor
   */
  template <typename T>
  inline
  PERBSSurf<T>::~PERBSSurf() {

    for( int i = 0; i < _c.getDim1(); i++ )
      for( int j = 0; j < _c.getDim2(); j++ )
        SceneObject::remove( _c[i][j] );

    delete _evaluator;
  }

  template <typename T>
  inline
  void PERBSSurf<T>::edit( SceneObject* obj ) {

    int i, j;
    for( i = 0; i < _c.getDim1()-1; i++ )
      for( j = 0; j < _c.getDim2()-1; j++ )
        if( _c[i][j] == obj )
          goto edit_loop_break;

    edit_loop_break:

    PBezierSurf<T> *bezier = dynamic_cast<PBezierSurf<T>*>(_c[i][j]);
    if( bezier )
      bezier->updateCoeffs( _c[i][j]->getPos() - _c[i][j]->evaluateParent( 0.5, 0.5, 0, 0 )[0][0] );

    replot();
  }


//  template <typename T>
//  inline
//  void PERBSSurf<T>::multEval(const DMatrix<T>& bu, const DMatrix<T>& bv, const DVector<int>& ii, const DVector<int>& ij, int du, int dv) {

//      DMatrix<Vector<T,3>> c(_ku, dv+1);

//      // We do these two operations manually here!
//      //    bv.transpose();
//      //    this->_p = bu * (c^bv);

//      //    c= _c^bvT
//      for(int i=0; i< _ku; i++)
//          for(int j=0; j<=dv; j++) {
//              c[i][j] = _c[ii(i)][ij(0)]*bv(j)(0);
//              for(int k=1; k<_kv; k++)
//                  c[i][j] += _c[ii(i)][ij(k)]*bv(j)(k);
//          }
//      //    _p = bu * c
//      for(int i=0; i<=dv; i++)
//          for(int j=0; j<=du; j++) {
//              this->_p[i][j] = bu(i)(0)*c[0][j];
//              for(int k=1; k<_ku; k++)
//                  this->_p[i][j] += bu(i)(k)*c[k][j];
//          }

//      // Get result of inner loop for second patch in v
//      DMatrix< Vector<T,3> > s1 = getC( u, v, uk, vk+1, d1, d2 );

//      // Evaluate ERBS-basis in v direction
//      DVector<T> B;
//      getB( B, _v, vk, v, d2 );

//      // Compute "Pascals triangle"-numbers and correct patch matrix
//      DVector<T> a( B.getDim() );
//      s0 -= s1;
//      s0.transpose(); s1.transpose();
//      for( int i = 0; i < B.getDim(); i++ ) {

//        a[i] = 1;
//        for( int j = i-1; j > 0; j-- )
//          a[j] += a[j-1];                           // Compute "Pascals triangle"-numbers

//        for( int j = 0; j <= i; j++ )
//          s1[i] += (a[j]*B[j]) * s0[i-j];       // "column += scalar x column"
//      }
//      s1.transpose();

//      this->_p = s1;
//  }



  template <typename T>
  inline
  void PERBSSurf<T>::eval( T u, T v, int d1, int d2, bool lu, bool lv ) const {


      if(this->_resample) {
          int i = this->_ind[0];
          int j = this->_ind[1];


          // Find Knot Indices u_k and v_k
          int uk = _ru(i).ind;
          int vk = _rv(j).ind;

          // Get result of inner loop for first patch in v
          DMatrix< Vector<T,3> > s0 = getC( u, v, uk, vk, d1, d2 );

          // If placed on a knot, return only first patch result
          if( std::abs(v - _v(vk)) < 1e-5 ) {
              this->_p = s0;
              //      std::cout << "_p[0][0]: " << this->_p[0][0] << "  (knot)" << std::endl;
              return;
          }
          else {    // Blend Patches

              // Get result of inner loop for second patch in v
              DMatrix< Vector<T,3> > s1 = getC( u, v, uk, vk+1, d1, d2 );

              // Evaluate ERBS-basis in v direction
              const DVector<T>& B = _rv(j).m;

              // Compute "Pascals triangle"-numbers and correct patch matrix
              DVector<T> a( B.getDim() );
              s0 -= s1;
              s0.transpose(); s1.transpose();
              for( int i = 0; i <= d2; i++ ) {

                  a[i] = 1;
                  for( int j = i-1; j > 0; j-- )
                      a[j] += a(j-1);                           // Compute "Pascals triangle"-numbers

                  for( int j = 0; j <= i; j++ )
                      s1[i] += (a(j)*B(j)) * s0(i-j);       // "column += scalar x column"
              }
              s1.transpose();

              this->_p = s1;
              //      std::cout << "_p[0][0]: " << this->_p[0][0] << std::endl;
          }
      }



//    // Find Knot Indices u_k and v_k
//    int uk, vk;
//    for( uk = 1; uk < _u.getDim()-2; ++uk ) if( u < _u[uk+1] ) break;
//    for( vk = 1; vk < _v.getDim()-2; ++vk ) if( v < _v[vk+1] ) break;

//    // If right-evaluation, find first knot
//    if(!lu) while( std::abs( _u[uk] - _u[uk-1] ) < 1e-5 ) --uk;
//    if(!lv) while( std::abs( _v[vk] - _v[vk-1] ) < 1e-5 ) --vk;

//    // Get result of inner loop for first patch in v
//    DMatrix< Vector<T,3> > s0 = getC( u, v, uk, vk, d1, d2 );

//    // If placed on a knot, return only first patch result
//    if( std::abs(v - _v[vk]) < 1e-5 ) {
//      this->_p = s0;
////      std::cout << "_p[0][0]: " << this->_p[0][0] << "  (knot)" << std::endl;
//      return;
//    }
//    else {    // Blend Patches

//      // Get result of inner loop for second patch in v
//      DMatrix< Vector<T,3> > s1 = getC( u, v, uk, vk+1, d1, d2 );

//      // Evaluate ERBS-basis in v direction
//      DVector<T> B;
//      getB( B, _v, vk, v, d2 );

//      // Compute "Pascals triangle"-numbers and correct patch matrix
//      DVector<T> a( B.getDim() );
//      s0 -= s1;
//      s0.transpose(); s1.transpose();
//      for( int i = 0; i < B.getDim(); i++ ) {

//        a[i] = 1;
//        for( int j = i-1; j > 0; j-- )
//          a[j] += a[j-1];                           // Compute "Pascals triangle"-numbers

//        for( int j = 0; j <= i; j++ )
//          s1[i] += (a[j]*B[j]) * s0[i-j];       // "column += scalar x column"
//      }
//      s1.transpose();

//      this->_p = s1;
////      std::cout << "_p[0][0]: " << this->_p[0][0] << std::endl;
//    }

  }



  //*****************************************
  // Overrided virtual functons from PSurf **
  //*****************************************

  template <typename T>
  void  PERBSSurf<T>::preSample( int dir, int m ) {
      if( dir==1 )
          internalPreSample( _ru, _u, m, _u[1], _u[_u.getDim()-2] );
      if( dir==2 )
          internalPreSample( _rv, _v, m, _v[1], _v[_v.getDim()-2] );
  }



  //*****************************
  // Localfunctons             **
  //*****************************

  template <typename T>
  inline
  void PERBSSurf<T>::internalPreSample( DVector< PreVec > & p, const DVector<T>& t, int m, T start, T end ) {

    // compute dt (step in parameter)
    const T dt = ( end - start ) / T(m-1);

    // Set the dimension of the Bernstein-Hermite Polynomial DVector
    p.setDim(m);

    // Compute the Bernstein-Hermite Polynomiale, for the B-spline Surface
    for( int j = 0, i=1; j < m; j++ ) {
        T s = start+dt*j;

        for(; i < t.getDim()-2; ++i ) if( s < t(i+1) ) break;
        if( i== t.getDim()-2) while( std::abs( t(i) - t(i-1) ) < 1e-5 ) --i;

        p[j].ind = i;
        getB( p[j].m, t, i, start+dt*j, 2 );
    }
  }


  template <typename T>
  inline
  void PERBSSurf<T>::evalPre( T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) {

    // Find the u/v index for the preevaluated data.
    int iu, iv;
    iv = 0;
    iu = 0;
    findIndex( u, v, iu, iv );

    // Find Knot Indices u_k and v_k
    int uk, vk;
    uk = _uk[iu][iv];
    vk = _vk[iu][iv];


    // Get result of inner loop for first patch in v
    DMatrix< Vector<T,3> > s0 = getCPre( u, v, uk, vk, d1, d2, iu, iv );

    // If placed on a knot return only first patch result
    if( std::abs(v - _v[vk]) < 1e-5 ) {
      this->_p = s0;
      return;
    }
    // Blend Patches
    else {

      // Get result of inner loop for second patch in v
      DMatrix< Vector<T,3> > s1 = getCPre( u, v, uk, vk+1, d1, d2, iu, iv );

      // Evaluate ERBS-basis in v direction
      const DVector<T> &B = _rv[iu][iv];

      // Compute "Pascals triangle"-numbers and correct patch matrix
      DVector<T> a( B.getDim() );
      s0 -= s1;
      s0.transpose(); s1.transpose();
      for( int i = 0; i < B.getDim(); i++ ) {

        a[i] = 1;
        for( int j = i-1; j > 0; j-- )
          a[j] += a[j-1];                           // Compute "Pascals triangle"-numbers

        for( int j = 0; j <= i; j++ )
          s1[i] += (a[j]*B(j)) * s0[i-j];       // "column += scalar x column"
      }
      s1.transpose();

      this->_p = s1;
    }
  }

  /*! void PERBSSurf<T>::findIndex( T u, T v, int& iu, int& iv )
   *
   *  Finds a knot vector index pair asociated with a parametric value pair.
   *
   *  \param[in]  u   Parametric value for u-direction.
   *  \param[in]  v   Parametric value for v-direction.
   *  \param[out] iu  Knot vector index for v.
   *  \param[out] iv  Knot vector index for u.
   */
  template <typename T>
  inline
  void PERBSSurf<T>::findIndex( T u, T v, int& iu, int& iv ) {

    iu = (this->_no_sam_u-1)*(u-this->getParStartU())/(this->getParDeltaU())+0.1;
    iv = (this->_no_sam_v-1)*(v-this->getParStartV())/(this->getParDeltaV())+0.1;
  }

  /*! void PERBSSurf<T>::generateKnotVector( DVector<T>& kv, const T s, const T d, int kvd, bool closed )
   *
   *  Generates a linear knot vector.
   *
   *  \param[out] kv      Generated knot vector.
   *  \param[in]  s       Knot vector start value.
   *  \param[in]  d       Knot vector delta value.
   *  \param[in]  kvd     Knot vector dimension.
   *  \param[in]  closed  Whether the data set associated with the knot vector is closed.
   */
  template <typename T>
  inline
  void PERBSSurf<T>::generateKnotVector( DVector<T>& kv, const T s, const T d, int kvd, bool closed ) {

    // Set Knot Vector dimension
    kv.setDim( kvd );

    // Compute all interior knot values
    for( int i = 0; i < kvd - 2; i++ )
      kv[i+1] = s + i * d;

    // Handle the edges wheather it is closed or not
    if( closed ) {
      kv[0] = kv[1] - ( kv[kvd-2] - kv[kvd-3]);
      kv[kvd-1] = kv[kvd-2] + ( kv[2] - kv[1] );
    }
    else {
      kv[0] = kv[1];
      kv[kvd-1] = kv[kvd-2];
    }
  }

  /*! void PERBSSurf<T>::generateKnotVector( PSurf<T>* g )
   *
   *  Generates a linear knot vector pair for the given surface.
   *
   *  \param[in]  g The cure for which the knot vectors is generated.
   */
  template <typename T>
  inline
  void PERBSSurf<T>::generateKnotVector( PSurf<T,3>* g ) {

    // Knot Vector in U direction
    generateKnotVector(
      _u,
      g->getParStartU(),
      g->getParDeltaU() / ( _c.getDim1()-1 ),
      _c.getDim1() + 2,
      isClosedU()
    );

    // Knot Vector in V direction
    generateKnotVector(
      _v,
      g->getParStartV(),
      g->getParDeltaV() / ( _c.getDim2()-1 ),
      _c.getDim2() + 2,
      isClosedV()
    );
  }

  /*! void PERBSSurf<T>::generateKnotVector( PSurf<T>* g, T u_s, T u_e, T v_s, T v_e )
   *
   *  Generates a linear knot vector pair for the sub-surface of the given surface.
   *
   *  \param[in]  g   The cure for which the knot vectors is generated.
   *  \param[in]  u_s The start parameter for the u-direction.
   *  \param[in]  u_e The end parameter for the u-direction.
   *  \param[in]  v_s The start parameter for the v-direction.
   *  \param[in]  v_e The end parameter for the v-direction.
   */
  template <typename T>
  inline
  void PERBSSurf<T>::generateKnotVector( PSurf<T,3>* g, T u_s, T u_e, T v_s, T v_e ) {

    T start_u = g->getParStartU() + g->getParStartU() * u_s;
    T delta_u = g->getParDeltaU() * u_e - u_s;

    T start_v = g->getParStartV() + g->getParStartV() * v_s;
    T delta_v = g->getParDeltaV() * v_e - v_s;


    // Knot Vector in U direction
    generateKnotVector(
      _u,
      start_u,
      delta_u / ( _c.getDim1()-1 ),
      _c.getDim1() + 2,
      isClosedU()
    );

    // Knot Vector in V direction
    generateKnotVector(
      _v,
      start_v,
      delta_v / ( _c.getDim2()-1 ),
      _c.getDim2() + 2,
      isClosedV()
    );
  }

  template <typename T>
  inline
  void PERBSSurf<T>::getB( DVector<T>& B, const DVector<T>& kv, int tk, T t, int d ) {

    B.setDim(d+1);

    _evaluator->set( kv(tk), kv(tk+1) - kv(tk) );
    B[0] = 1 - _evaluator->operator()(t);
    B[1] = - _evaluator->getDer1();
    B[2] = - _evaluator->getDer2();
  }

  template <typename T>
  inline
  DMatrix< Vector<T,3> > PERBSSurf<T>::getC( T u, T v, int uk, int vk, T du, T dv ) const {

      if(this->_resample) {
          // Init Indexes and get local u/v values
          const int cu = uk-1;
          const int cv = vk-1;

          // Evaluate First local patch
          DMatrix< Vector<T,3> > c0 = _c(cu)(cv)->evaluateParent( mapToLocal(u,v,uk,vk), Point<T,2>( du, dv ) );

          // If on a interpolation point return only first patch evaluation
          if( std::abs(u - _u(uk)) < 1e-5 )
              return c0;

          // Select next local patch in u direction

          // Evaluate Second local patch
          DMatrix< Vector<T,3> > c1 = _c(cu+1)(cv)->evaluateParent( mapToLocal(u,v,uk+1,vk), Point<T,2>( du, dv) );

          DVector<T> a(du+1);

          // Evaluate ERBS-basis in u direction
          const DVector<T>& B = _ru(uk).m;

          // Compute "Pascals triangle"-numbers and correct patch matrix
          c0 -= c1;
          for( int i = 0; i < B.getDim(); i++ ) {

              a[i] = 1;
              for( int j = i-1; j > 0; j-- )
                  a[j] += a[j-1];

              for( int j = 0; j <= i; j++ )
                  c1[i] += (a(j) * B(j)) * c0(i-j);
          }
          return c1 ;
      }
      else
      return DMatrix< Vector<T,3> >();
  }

  template <typename T>
  Point<T,2> PERBSSurf<T>::mapToLocal( T u, T v, int uk, int vk ) const {

    PSurf<T,3> *c = _c(uk-1)(vk-1);

    T csu = c->getParStartU();
    T cdu = c->getParDeltaU();
    T csv = c->getParStartV();
    T cdv = c->getParDeltaV();


    // U
    if( std::abs(_u(uk) - _u(uk+1)) < 1e-5 && uk != _u.getDim()-2 && _c(uk-1)(vk-1) == _c(uk)(vk-1) ) {

      cdu /= T(2);
    }
    else if( std::abs(_u(uk-1) - _u(uk)) < 1e-5 && uk != 1 && uk != _u.getDim()-1 && _c(uk-2)(vk-1) == _c(uk-1)(vk-1)) {

      csu += cdu/2;
      cdu /= T(2);
    }

    // V
    if( std::abs(_v(vk) - _v(vk+1)) < 1e-5 && vk != _v.getDim()-2 && _c(uk-1)(vk-1) == _c(uk-1)(vk) ) {

      cdv /= T(2);
    }
    else if( std::abs(_v(vk-1) - _v(vk)) < 1e-5 && vk != 1 && vk != _v.getDim()-1 && _c(uk-1)(vk-2) == _c(uk-1)(vk-1) ) {

      csv += cdv/2;
      cdv /= T(2);
    }

    return Point<T,2>( csu + (u - _u(uk-1)) / (_u(uk+1) - _u(uk-1)) * cdu,
                       csv + (v - _v(vk-1)) / (_v(vk+1) - _v(vk-1)) * cdv );
  }

  template <typename T>
  inline
  DMatrix< Vector<T,3> > PERBSSurf<T>::getCPre( T u, T v, int uk, int vk, T du, T dv, int iu, int iv ) {

    // Init Indexes and get local u/v values
    const int cu = uk-1;
    const int cv = vk-1;

    // Evaluate First local patch
    DMatrix< Vector<T,3> > c0 = _c[cu][cv]->evaluateParent( mapToLocal(u,v,uk,vk), Point<T,2>(du, dv) );

    // If on a interpolation point return only first patch evaluation
    if( std::abs(u - _u[uk]) < 1e-5 )
      return c0;

    // Select next local patch in u direction
    uk++;

    // Init Indexes and get local u/v values
    const int cu2 = uk-1;
    const int cv2 = vk-1;

    // Evaluate Second local patch
    DMatrix< Vector<T,3> > c1 = _c[cu2][cv2]->evaluateParent( mapToLocal(u,v,uk,vk), Point<T,2>(du, dv) );

    DVector<T> a(du+1);

      // Evaluate ERBS-basis in u direction
    const DVector<T> &B = _ru[iu][iv];

      // Compute "Pascals triangle"-numbers and correct patch matrix
    c0 -= c1;
    for( int i = 0; i < B.getDim(); i++ ) {

      a[i] = 1;
      for( int j = i-1; j > 0; j-- )
        a[j] += a[j-1];

      for( int j = 0; j <= i; j++ )
        c1[i] += (a[j] * B(j)) * c0[i-j];
    }

    return c1 ;
  }

  template <typename T>
  T PERBSSurf<T>::getEndPU() const {
    return _u(_u.getDim()-2);
  }

  template <typename T>
  T PERBSSurf<T>::getEndPV() const {
    return _v(_v.getDim()-2);
  }

  /*! DVector<T>& PERBSSurf<T>::getKnotsU()
   *
   *  Returns a reference to the u-direction knot vectors.
   *
   *  \return Knot vector references.
   */
  template <typename T>
  inline
  DVector<T>& PERBSSurf<T>::getKnotsU() {

    return _u;
  }

  /*! DVector<T>& PERBSSurf<T>::getKnotsV()
   *
   *  Returns a reference to the v-direction knot vectors.
   *
   *  \return Knot vector references.
   */
  template <typename T>
  inline
  DVector<T>& PERBSSurf<T>::getKnotsV() {

    return _v;
  }

  /*! DMatrix< PSurf<T>* >& PERBSSurf<T>::getLocalPatches()
   *
   *  Returns a reference to the local pathces.
   *
   *  \return A reference to the local patches.
   */
  template <typename T>
  inline
  DMatrix< PSurf<T,3>* >& PERBSSurf<T>::getLocalPatches() {

    return _c;
  }

  /*! int PERBSSurf<T>::getNoLocalPatchesU() const
   *
   *  Returns number of local patches in u-direction.
   *
   *  \return Number of local pathces.
   */
  template <typename T>
  inline
  int PERBSSurf<T>::getNoLocalPatchesU() const {

    return _c.getDim1();
  }


  /*! int PERBSSurf<T>::getNoLocalPatchesV() const
   *
   *  Returns number of local patches in v-direction.
   *
   *  \return Number of local pathces.
   */
  template <typename T>
  inline
  int PERBSSurf<T>::getNoLocalPatchesV() const {

    return _c.getDim2();
  }

  template <typename T>
  T PERBSSurf<T>::getStartPU() const {
    return _u(1);
  }

  template <typename T>
  T PERBSSurf<T>::getStartPV() const {
    return _v(1);
  }

  /*! void PERBSSurf<T>::hideLocalPatches()
   *
   *  Sets the local patches not visible.
   */
  template <typename T>
  inline
  void PERBSSurf<T>::hideLocalPatches() {

    for( int i = 0; i < _c.getDim1(); i++ )
      for( int j = 0; j < _c.getDim2(); j++ )
        _c[i][j]->setVisible( false, -1 );
  }

  template <typename T>
  inline
  void PERBSSurf<T>::init() {

    _no_sam_u                       = 20;
    _no_sam_v                       = 20;
    _no_der_u                       = 1;
    _no_der_v                       = 1;

//    _evaluator = ERBSEvaluator<long double>::getInstance();
    _evaluator = new ERBSEvaluator<long double>;
    _resamp_mode = GM_RESAMPLE_PREEVAL;
    _pre_eval = true;
  }

  /*! void PERBSSurf<T>::insertPatch( PSurf<T>* patch )
   *
   *  Inserts a patch.
   *  A helper function for the ERBSSurf class.
   *
   *  \param[in]  patch   Patch to insert.
   */
  template <typename T>
  inline
  void PERBSSurf<T>::insertPatch( PSurf<T,3>* patch ) {

    patch->setVisible( false );
    patch->setCollapsed( true );
    this->insert( patch );
  }

  template <typename T>
  inline
  bool PERBSSurf<T>::isClosedU() const {

    return _closed_u;
  }

  template <typename T>
  inline
  bool PERBSSurf<T>::isClosedV() const {

    return _closed_v;
  }

  /*! bool PERBSSurf<T>::isLocalPatchesVisible() const
   *
   *  Returns whether the local pathces is visible.
   *
   *  \return Whether the local patches is visible.
   */
  template <typename T>
  inline
  bool PERBSSurf<T>::isLocalPatchesVisible() const {

    for( int i = 0; i < _c.getDim1(); i++ )
      for( int j = 0; j < _c.getDim2(); j++ )
        if( _c(i)(j)->isVisible() )
          return true;

    return false;
  }

  /*! void PERBSSurf<T>::padKnotVector( DVector<T>& kv, bool closed )
   *
   *  Prepends and appends an element to the knot vector.
   *  Value of the padded elements depends on the whether the data set is
   *  closed in the knot vector direction.
   *
   *  \param kv Knot vector which is to be padded.
   *  \param closed Whether or not the data set is closed in the knot vectors direction.
   */
  template <typename T>
  inline
  void PERBSSurf<T>::padKnotVector( DVector<T>& kv, bool closed ) {

    if( closed ) {

      T d_start, d_end;
      d_start = kv[1] - kv[0];
      d_end = kv[kv.getDim()-1] - kv[kv.getDim()-2];
      kv.prepend( kv[0] - d_end );
      kv.append( kv[kv.getDim()-1] + d_start );
    }
    else {

      kv.prepend( kv[0] );
      kv.append( kv[kv.getDim()-1] );
    }
  }


//  template <typename T>
//  inline
//  void PERBSSurf<T>::preSample( int m1, int m2, int d1, int d2, T s_u, T s_v, T e_u, T e_v ) {

//    // break out of the preSample function if no preevalution is to be used
//    switch( _resamp_mode ) {
//    case GM_RESAMPLE_PREEVAL: break;
//    case GM_RESAMPLE_INLINE:
//    default:
//      return;
//    }

//    // break out and return if preevaluation isn't necessary.
//    if( !_pre_eval && m1 == _uk.getDim1() && m2 == _uk.getDim2() )
//      return;

//    int uk, vk;
//    uk = vk = 1;

//    // compute du and dv (step in parametric u and v direction)
//    const T du = ( e_u - s_u ) / T(m1-1);
//    const T dv = ( e_v - s_v ) / T(m2-1);

//    // Set dimension for B and index value tables.
//    _Bu.setDim( m1, m2 );
//    _Bv.setDim( m1, m2 );
//    _uk.setDim( m1, m2 );
//    _vk.setDim( m1, m2 );

//    // Outer loop: u dir
//    for( int i = 0; i < m1; i++ ) {

//      // Find u
//      const T u = getStartPU() + T(i) * du;

//      // Inner loop: v dir
//      for( int j = 0; j < m2; j++ ) {

//        // Find v
//        const T v = getStartPV() + T(j) * dv;

//        // Find indices for uk and vk
//        for( uk = 1; uk < _u.getDim()-2; uk++ ) if( u < _u[uk+1] ) break;
//        for( vk = 1; vk < _v.getDim()-2; vk++ ) if( v < _v[vk+1] ) break;

//        // Save indices
//        _uk[i][j] = uk;
//        _vk[i][j] = vk;

//        // Evaluate ERBS basis in u direction
//        if( !(std::abs(u - _u[uk]) < 1e-5) )
//          getB( _Bu[i][j], _u, uk, u, d1 );

//        // Evaluate ERBS basis in v direction
//        if( !(std::abs(v - _v[vk]) < 1e-5) )
//          getB( _Bv[i][j], _v, vk, v, d2 );
//      }
//    }

//    _pre_eval = false;
//  }


  template <typename T>
  inline
  void PERBSSurf<T>::setResampleMode( GM_RESAMPLE_MODE mode ) {

    _resamp_mode = mode;
  }

  /*! void PERBSSurf<T>::showLocalPatches()
   *
   *  Makes the local patches visible.
   */
  template <typename T>
  inline
  void PERBSSurf<T>::showLocalPatches() {

    for( int i = 0; i < _c.getDim1(); i++ ) {
      for( int j = 0; j < _c.getDim2(); j++ ) {

        if( !_c[i][j]->getDefaultVisualizer() ) {
           _c[i][j]->enableDefaultVisualizer(true);
           _c[i][j]->replot( 10, 10, 1, 1 );
        }

        _c[i][j]->setVisible( true, -1 );
      }
    }
  }

  template <typename T>
  void PERBSSurf<T>::toggleLocalPatches() {

    for( int i = 0; i < _c.getDim1(); i++ )
      for( int j = 0; j < _c.getDim2(); j++ )
        _c[i][i]->toggleVisible();
  }

  template <typename T>
  void PERBSSurf<T>::insertVisualizer(Visualizer* visualizer) {


    PSurfVisualizer<T,3> *visu = dynamic_cast<PSurfVisualizer<T,3>*>( visualizer );
    if( !visu ) {

      PSurf<T,3>::insertVisualizer( visualizer );
      return;
    }

    if( _pv.exist( visu ) )
      return;

    _pv += visu;
  }

  template <typename T>
  void PERBSSurf<T>::removeVisualizer(Visualizer* visualizer) {

    PSurfVisualizer<T,3> *visu = dynamic_cast<PSurfVisualizer<T,3>*>( visualizer );
    if( !visu ) {

      PSurf<T,3>::removeVisualizer( visualizer );
      return;
    }

    if( !_pv.exist(visu) )
      return;

    _pv.remove(visu);
  }

  template <typename T>
  void PERBSSurf<T>::replot(int m1, int m2, int d1, int d2) {



    // Correct sample domain
    if( m1 < 2 )
      m1 = _no_sam_u;
    else
      _no_sam_u = m1;

    if( m2 < 2 )
      m2 = _no_sam_v;
    else
      _no_sam_v = m2;

    // Correct derivatives
    if( d1 < 1 )
      d1 = _no_der_u;
    else
      _no_der_u = d1;

    if( d2 < 1 )
      d2 = _no_der_v;
    else
      _no_der_v = d2;



    // pre-sampel / pre evaluate data for a given parametric surface, if wanted/needed
//    preSample(
//      m1, m2, d1, d2,
//      getStartPU(),
//      getStartPV(),
//      getEndPU(),
//      getEndPV()
//    );


    // Predict number of "visualizer"-segments u/v
    // u
    DVector< Vector<float,2> > u_ps(0);
    float u_pos = _u[1];
    for( int i = 2; i < _u.getDim()-2; ++i ) {
      if( std::abs( _u[i] - _u[i-1] ) <  1e-5 ) {

        u_ps.append( Vector<float,2>( u_pos, _u[i-1]) );
        u_pos = _u[i-1];
      }
    }
    u_ps.append( Vector<float,2>(u_pos,_u[_u.getDim()-2]) );

    // v
    DVector< Vector<float,2> > v_ps(0);
    float v_pos = _v[1];
    for( int i = 2; i < _v.getDim()-2; ++i ) {
      if( std::abs( _v[i] - _v[i-1] ) <  1e-5 ) {

        v_ps.append( Vector<float,2>( v_pos, _v[i-1]) );
        v_pos = _v[i-1];
      }
    }
    v_ps.append( Vector<float,2>(v_pos,_v[_v.getDim()-2]) );


    // Clean up "visualizers"
    for( int i = 0; i < _pvi.getDim1(); ++i )
    for( int j = 0; j < _pvi.getDim2(); ++j )
      for( int k = 0; k < _pvi[i][j].visus.getSize(); ++k )
        PSurf<T,3>::removeVisualizer( _pvi[i][j].visus[k] );

    if( _pvi.getDim1() != u_ps.getDim() || _pvi.getDim2() != v_ps.getDim() )
      _pvi.resetDim( u_ps.getDim(), v_ps.getDim() );


    // Insert new visualizers and replot
    Sphere<T,3>  s;
    DMatrix< DMatrix< Vector<T,3> > > p;
    DMatrix< Vector<T,3> > normals;
    for( int i = 0; i < _pvi.getDim1(); ++i ) {
      for( int j = 0; j < _pvi.getDim2(); ++j ) {

        // Update sub-visualizer set
        _pvi[i][j].updateVisualizerSet(_pv);
        _pvi[i][j].seg_u = u_ps[i];
        _pvi[i][j].seg_v = v_ps[j];


        // Get visualizer and domain of (i,j)-th sub-visualizer set
        const Array< PSurfVisualizer<T,3>* >  &sub_visus  = _pvi[i][j].visus;
        const Vector<float,2>                 &seg_u      = _pvi[i][j].seg_u;
        const Vector<float,2>                 &seg_v      = _pvi[i][j].seg_v;

        // Insert visualizers
        for( int k = 0; k < sub_visus.getSize(); ++k )
          PSurf<T,3>::insertVisualizer( sub_visus(k) );

        // Resample (i,j)-th segment
        this->resample( p, m1, m2, d1, d2,
                  seg_u(0), seg_v(0),
                  seg_u(1), seg_v(1) );

        // Resample normals for (i,j)-th segment
        this->resampleNormals( p, normals );

        // Replot visualizers of (i,j)-th segment
        for( int k = 0; k < sub_visus.getSize(); ++k )
          sub_visus(k)->replot( p, normals, m1, m2, d1, d2,
                                (_pvi.getDim1() == 1) && isClosedU(),
                                (_pvi.getDim2() == 1) && isClosedV() );

        // Surrounding sphere
        if( i == 0 && j == 0 )
          s.resetPos( p(0)(0)(0)(0) );
        else
          s += p(0)(0)(0)(0);

        s += Point<T,3>( p( p.getDim1()-1 )( p.getDim2()-1 )(0)(0) );
        s += Point<T,3>( p( p.getDim1()/2 )( p.getDim2()/2 )(0)(0) );
        s += Point<T,3>( p( p.getDim1()-1 )( 0             )(0)(0) );
        s += Point<T,3>( p( 0             )( p.getDim2()-1 )(0)(0) );
        s += Point<T,3>( p( p.getDim1()-1 )( p.getDim2()/2 )(0)(0) );
        s += Point<T,3>( p( p.getDim1()/2 )( p.getDim2()-1 )(0)(0) );
        s += Point<T,3>( p( 0             )( p.getDim2()/2 )(0)(0) );
        s += Point<T,3>( p( p.getDim1()/2 )( 0             )(0)(0) );
      }
    }


    // Set surrounding sphere
    Parametrics<T,2,3>::setSurroundingSphere( s.template toType<float>() );





//    // Correct sample domain
//    if( m1 < 2 )
//      m1 = _no_sam_u;
//    else
//      _no_sam_u = m1;

//    if( m2 < 2 )
//      m2 = _no_sam_v;
//    else
//      _no_sam_v = m2;

//    // Correct derivatives
//    if( d1 < 1 )
//      d1 = _no_der_u;
//    else
//      _no_der_u = d1;

//    if( d2 < 1 )
//      d2 = _no_der_v;
//    else
//      _no_der_v = d2;


//    // pre-sampel / pre evaluate data for a given parametric surface, if wanted/needed
//    preSample(
//      m1, m2, d1, d2,
//      getStartPU(),
//      getStartPV(),
//      getEndPU(),
//      getEndPV()
//    );


//    // Sample Positions and related Derivatives
//    DMatrix< DMatrix< Vector<T,n> > > p;
//    resample(
//      p, m1, m2, d1, d2,
//      getStartPU(),
//      getStartPV(),
//      getEndPU(),
//      getEndPV()
//    );

//    // Sample Normals
//    DMatrix< Vector<T,n> > normals;
//    resampleNormals( p, normals );

//    // Set The Surrounding Sphere
//    setSurroundingSphere( p );

//    // Replot Visaulizers
//    for( int i = 0; i < this->_psurf_visualizers.getSize(); i++ )
    //      this->_psurf_visualizers[i]->replot( p, normals, m1, m2, d1, d2, isClosedU(), isClosedV() );
  }

  template <typename T>
  void PERBSSurf<T>::splitKnot(int uk, int vk)  {

    if( uk > _c.getDim1() || uk < 0 || vk > _c.getDim2() || vk < 0 )
      return;


    // if conected double knot



    //// expand knot vectors
    _u.insert( uk+1, _u(uk+1) );
    _v.insert( vk+1, _v(vk+1) );

    //// expand local patches
//    std::cout << " _c: " << _c << std::endl;



    DMatrix< PSurf<T,3>* > c( _c.getDim1()+1, _c.getDim2()+1, static_cast<PSurf<T,3>*>(0x0) );
//    std::cout << " c after initializtion: " << c << std::endl;

    // lower quadrant
    for( int i = 0; i < uk; ++i )
      for( int j = 0; j < vk; ++j )
        c[i][j] = _c[i][j];

    // u
    for( int i = uk+1; i < c.getDim1(); ++i )
      for( int j = 0; j < vk; ++j )
        c[i][j] = _c[i-1][j];

    // v
    for( int i = 0; i < uk; ++i )
      for( int j = vk+1; j < c.getDim2(); ++j )
        c[i][j] = _c[i][j-1];

    for( int i = uk+1; i < c.getDim1(); ++i )
      for( int j = vk+1; j < c.getDim2(); ++j )
        c[i][j] = _c[i-1][j-1];

//    std::cout << " c after locals move: " << c << std::endl;

    // u lower
    PSurf<T,3> *op, *np;
    for( int j = 0; j < vk; ++j ) {
//      op = c[uk+1][j];
//      np = static_cast<PSurf<T,3>*>( op->makeCopy() );
//      c[uk][j] = np;
      c[uk][j] = c[uk+1][j];
//      insertPatch( np );
    }

    // u upper
    for( int j = vk+2; j < c.getDim2(); ++j ) {
//      op = c[uk+1][j];
//      np = static_cast<PSurf<T,3>*>( op->makeCopy() );
//      c[uk][j] = np;
      c[uk][j] = c[uk+1][j];
//      insertPatch( np );
    }

    // v lower
    for( int i = 0; i < uk; ++i ) {
//      op = c[i][vk+1];
//      np = static_cast<PSurf<T,3>*>( op->makeCopy() );
//      c[i][vk] = np;
      c[i][vk] = c[i][vk+1];
//      insertPatch( np );
    }

    // v upper
    for( int i = uk+2; i < c.getDim1(); ++i ) {
//      op = c[i][vk+1];
//      np = static_cast<PSurf<T,3>*>( op->makeCopy() );
//      c[i][vk] = np;
      c[i][vk] = c[i][vk+1];
//      insertPatch( np );
    }

    // Center block
    op = c[uk+1][vk+1];

    np = static_cast<PSurf<T,3>*>( op->makeCopy() );
    c[uk][vk] = np;
    insertPatch( np );
    np->setDomainU( T(0), T(0.5) );
    np->setDomainV( T(0), T(0.5) );

    np = static_cast<PSurf<T,3>*>( op->makeCopy() );
    c[uk+1][vk] = np;
    insertPatch( np );
    np->setDomainU( T(0.5), T(1) );
    np->setDomainV( T(0), T(0.5) );

    np = static_cast<PSurf<T,3>*>( op->makeCopy() );
    c[uk][vk+1] = np;
    insertPatch( np );
    np->setDomainU( T(0), T(0.5) );
    np->setDomainV( T(0.5), T(1) );


    op->setDomainU( T(0.5), T(1) );
    op->setDomainV( T(0.5), T(1) );


    /*!
     *  Scale domain of splited patches
     *
     *  \todo Does this scaling handle multiple knot splits to the "same" knot ?
     */


//    std::cout << " c after locals insert: " << c << std::endl;


    _c = c;
  }

  template <typename T>
  void PERBSSurf<T>::splitKnotAlong(int uk, int vk)  {

    if( uk > _c.getDim1() || uk < 0 || vk > _c.getDim2() || vk < 0 )
      return;

    //// expand knot vectors
    _u.insert( uk+1, _u(uk+1) );
    _v.insert( vk+1, _v(vk+1) );

    //// expand local patches
//    std::cout << " _c: " << _c << std::endl;



    DMatrix< PSurf<T,3>* > c( _c.getDim1()+1, _c.getDim2()+1, static_cast<PSurf<T,3>*>(0x0) );
//    std::cout << " c after initializtion: " << c << std::endl;

    // lower quadrant
    for( int i = 0; i < uk; ++i )
      for( int j = 0; j < vk; ++j )
        c[i][j] = _c[i][j];

    // u
    for( int i = uk+1; i < c.getDim1(); ++i )
      for( int j = 0; j < vk; ++j )
        c[i][j] = _c[i-1][j];

    // v
    for( int i = 0; i < uk; ++i )
      for( int j = vk+1; j < c.getDim2(); ++j )
        c[i][j] = _c[i][j-1];

    for( int i = uk+1; i < c.getDim1(); ++i )
      for( int j = vk+1; j < c.getDim2(); ++j )
        c[i][j] = _c[i-1][j-1];

//    std::cout << " c after locals move: " << c << std::endl;

    // u lower
    PSurf<T,3> *op, *np;
    for( int j = 0; j < vk; ++j ) {
      op = c[uk+1][j];
      np = static_cast<PSurf<T,3>*>( op->makeCopy() );
      c[uk][j] = np;
      insertPatch( np );

      np->setDomainU( T(0), T(0.5) );
      op->setDomainU( T(0.5), T(1) );
    }

    // u upper
    for( int j = vk+2; j < c.getDim2(); ++j ) {
      op = c[uk+1][j];
      np = static_cast<PSurf<T,3>*>( op->makeCopy() );
      c[uk][j] = np;
      insertPatch( np );

      np->setDomainU( T(0), T(0.5) );
      op->setDomainU( T(0.5), T(1) );
    }

    // v lower
    for( int i = 0; i < uk; ++i ) {
      op = c[i][vk+1];
      np = static_cast<PSurf<T,3>*>( op->makeCopy() );
      c[i][vk] = np;
      insertPatch( np );

      np->setDomainV( T(0), T(0.5) );
      op->setDomainV( T(0.5), T(1) );
    }

    // v upper
    for( int i = uk+2; i < c.getDim1(); ++i ) {
      op = c[i][vk+1];
      np = static_cast<PSurf<T,3>*>( op->makeCopy() );
      c[i][vk] = np;
      insertPatch( np );

      np->setDomainV( T(0), T(0.5) );
      op->setDomainV( T(0.5), T(1) );
    }

    // Center block
    op = c[uk+1][vk+1];

    np = static_cast<PSurf<T,3>*>( op->makeCopy() );
    c[uk][vk] = np;
    insertPatch( np );
    np->setDomainU( T(0), T(0.5) );
    np->setDomainV( T(0), T(0.5) );

    np = static_cast<PSurf<T,3>*>( op->makeCopy() );
    c[uk+1][vk] = np;
    insertPatch( np );
    np->setDomainU( T(0.5), T(1) );
    np->setDomainV( T(0), T(0.5) );

    np = static_cast<PSurf<T,3>*>( op->makeCopy() );
    c[uk][vk+1] = np;
    insertPatch( np );
    np->setDomainU( T(0), T(0.5) );
    np->setDomainV( T(0.5), T(1) );


    op->setDomainU( T(0.5), T(1) );
    op->setDomainV( T(0.5), T(1) );


    /*!
     *  Scale domain of splited patches
     *
     *  \todo Does this scaling handle multiple knot splits to the "same" knot ?
     */


//    std::cout << " c after locals insert: " << c << std::endl;


    _c = c;
  }



} // END namespace GMlib


