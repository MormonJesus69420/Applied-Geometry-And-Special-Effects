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

  /*! PHermiteSurface<T>::PHermiteSurface( const Array< PCurve<T>* >& c1, const Array<PCurve<T>* >& c2 )
   *
   *  Creates a generic Hermite surface from the curves provided in c1 and c2.
   *  It's expected two or four curves in each array. If two curves are given it will create a Coon's patch surface.
   *
   *  The curves in c1 describes the u-direction, while the curves in c2 describes the v-direction.
   *  The two first curves describes the boundaries, and the next two describes the derivatives along the respective boundary.
   *
   *  If c1[0] is f1, c1[1] is f2, c2[0] is f3 and c2[1] is f4, and the corners is denoted p00, p10, p01 and p11, then
   *  p00 is the corner at f1 and f3, p10 is the corner at f1 and f4, p01 is the corner at f2 and f3 and p11 is the corner at f3 and f4.
   *
   *  The direction of f1, f2, f3 and f4 are:
   *  f1: p00 to p10
   *  f2: p00 to p01
   *  f3: p01 to p11
   *  f4: p10 to p11
   *
   *  \param[in] c1 Curves describing the boundary in u-direction
   *  \param[in] c2 Curves describing the boundary in v-direction
   */
//*****************************************
// Constructors and destructor           **
//*****************************************

  template <typename T>
  PHermiteSurface<T>::PHermiteSurface( const Array< PCurve<T,3>* >& c1, const Array<PCurve<T,3>* >& c2 ) {

    init();

    _c1 = c1;
    _c2 = c2;

    for( int i = 0; i < _c1.getSize(); i++ )
      insertPatch( _c1[i] );

    for( int i = 0; i < _c2.getSize(); i++ )
      insertPatch( _c2[i] );

    _b.setDim( 2, 2 );
    _b[0][0] = (_c1[0]->evaluateParent( _c1[0]->getParStart(), 0 ))[0];
    _b[1][0] = (_c1[0]->evaluateParent( _c1[0]->getParEnd(), 0 ))[0];
    _b[0][1] = (_c2[0]->evaluateParent( _c2[0]->getParEnd(), 0 ))[0];
    _b[1][1] = (_c1[1]->evaluateParent( _c1[1]->getParEnd(), 0 ))[0];

  }

  /*! PHermiteSurface<T>::~PHermiteSurface()
   *
   *  Destructor.
   */
  template <typename T>
  PHermiteSurface<T>::~PHermiteSurface() {

    for( int i = 0; i < _c1.getSize(); i++ )
      SceneObject::remove( _c1[i] );

    for( int i = 0; i < _c2.getSize(); i++ )
      SceneObject::remove( _c2[i] );
  }





  //*****************************************
  //            Local functons             **
  //*****************************************

  template <typename T>
  inline
  void PHermiteSurface<T>::edit( SceneObject* /*obj*/ ) {

    _b[0][0] = (_c1[0]->evaluateParent( _c1[0]->getParStart(), 0 ))[0];
    _b[1][0] = (_c1[0]->evaluateParent( _c1[0]->getParEnd(), 0 ))[0];
    _b[0][1] = (_c2[0]->evaluateParent( _c2[0]->getParEnd(), 0 ))[0];
    _b[1][1] = (_c1[1]->evaluateParent( _c1[1]->getParEnd(), 0 ))[0];

    PSurf<T,3>::replot();
  }





  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PHermiteSurface<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PHermiteSurface<T>::isClosedV() const {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PHermiteSurface<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/) const {

    // set result set dim
    this->_p.setDim( d1+1, d2+1 );

    // vars
    Point<float,3> p(0.0f);

    // interpolate u in v direction
    for( int i = 0; i < _c1.getSize(); i++ )
      p += getH( _c2.getSize()/2, i, v ) * (_c1(i)->evaluateParent(u, 1))(0);

    // interpolate v in u direction
    for( int i = 0; i < _c2.getSize(); i++ )
      p += getH( _c1.getSize()/2, i, u ) * (_c2(i)->evaluateParent(v, 1))(0);

    // bi-linear interpolation
    p -=
        _b(0)(0) * (1.0f - u) * (1.0f - v) +
        _b(0)(1) * (1.0f - u) * v +
        _b(1)(0) * u * (1.0f - v) +
        _b(1)(1) * u * v;

    this->_p[0][0] = p;


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if( d1 > 0 ) {    // Su

        p = Point<float,3>(0.0f);

        // interpolate u in v direction
        for( int i = 0; i < _c1.getSize(); i++ )
          p += getH( _c2.getSize()/2, i, v ) * (_c1(i)->evaluateParent(u, 1))(1);

        // interpolate v in u direction
        for( int i = 0; i < _c2.getSize(); i++ )
          p += getHder( _c1.getSize()/2, i, u ) * (_c2(i)->evaluateParent(v, 1))(0);

        // bi-linear interpolation
        p -=
            _b(0)(0) * ( v - 1.0f ) +
            _b(0)(1) * ( -v ) +
            _b(1)(0) * (1.0f - v) +
            _b(1)(1) * v;

        this->_p[1][0] = p;
      }



      if( d2 > 0 ) {    // Sv

        p = Point<float,3>(0.0f);

        // interpolate u in v direction
        for( int i = 0; i < _c1.getSize(); i++ )
          p += getHder( _c2.getSize()/2, i, v ) * (_c1(i)->evaluateParent(u, 1))(0);

        // interpolate v in u direction
        for( int i = 0; i < _c2.getSize(); i++ )
          p += getH( _c1.getSize()/2, i, u ) * (_c2(i)->evaluateParent(v, 1))(1);

        // bi-linear interpolation
        p -=
            _b(0)(0) * (u - 1.0f) +
            _b(0)(1) * (1.0f - u) +
            _b(1)(0) * ( -u ) +
            _b(1)(1) * u;

        this->_p[0][1] = p;
      }



      if( d1 > 0 && d2 > 0 ) {

        p = Point<float,3>(0.0f);

        // interpolate u in v direction
        for( int i = 0; i < _c1.getSize(); i++ )
          p += getHder( _c2.getSize()/2, i, v ) * (_c1(i)->evaluateParent(u, 1))(1);

        // interpolate v in u direction
        for( int i = 0; i < _c2.getSize(); i++ )
          p += getHder( _c1.getSize()/2, i, u ) * (_c2(i)->evaluateParent(v, 1))(1);

        // bi-linear interpolation
        p -=
            _b(0)(0) * (1.0f - u) * (1.0f - v) +
            _b(0)(1) * (1.0f - u) * v +
            _b(1)(0) * u * (1.0f - v) +
            _b(1)(1) * u * v;

        this->_p[1][1] = p;
      }
    }
  }





  template <typename T>
  T PHermiteSurface<T>::getStartPU() const {
    return T(0);
  }



  template <typename T>
  T PHermiteSurface<T>::getEndPU() const {
    return T(1);
  }



  template <typename T>
  T PHermiteSurface<T>::getStartPV() const {
    return T(0);
  }



  template <typename T>
  T PHermiteSurface<T>::getEndPV() const {
    return T(1);
  }





  /*! T PHermiteSurface<T>::getH( int d, int k, T t )
   *
   *  Gives the hermite interpolation factor
   *
   *  \param[in]  d   Number of curves describing the given boundary
   *  \param[in]  k   Curve number k
   *  \param[in]  t   Parametric value [0-1]
   *  \return         The hermite interpolation factor [0-1].
   */
  template <typename T>
  inline
  T PHermiteSurface<T>::getH(  int d, int k, T t ) const {

    if(d==2)
    {
      switch (k)
      {
        case 1:  return (2*t-3)*t*t+1;
        case 2:  return (3-2*t)*t*t;
        case 3:  return (t*t-2*t+1)*t;
        case 4:  return (t-1)*t*t;
      }
    }

    // if(d==1)
    if(k==1)  return t;
    else      return 1.0-t;
  }

  /*! T PHermiteSurface<T>::getHder( int d, int k, T t )
   *
   *  Gives the hermite interpolation factor for derivatives
   *
   *  \param[in]  d   Number of curves describing the given boundary
   *  \param[in]  k   Curve number k
   *  \param[in]  t   Parametric value [0-1]
   *  \return     The hermite interpolation factor [0-1].
   */
  template <typename T>
  inline
  T PHermiteSurface<T>::getHder( int d, int k, T t ) const {

    if(d==2)
    {
        switch (k)
        {
        case 1:  return (t-1)*t*6;
        case 2:  return (1-t)*t*6;
        case 3:  return 3*t*t-4*t+1;
        case 4:  return (3*t-2)*t;
        }
    }

    // if(d==1)
    if(k==1)  return 1.0;
    else      return -1.0;
  }

  template <typename T>
  void PHermiteSurface<T>::insertPatch( PCurve<T,3>* patch ) {

    patch->replot( 10 );
    patch->setVisible( true );
    patch->setCollapsed( false );
    SceneObject::insert( patch );
  }

  /*! bool PHermiteSurface<T>::isValidCoonsPatch()
   *
   *  Validates the surfaces as a Coon's patch
   *  Returns whether the surface is classified as a Coon's patch.
   *
   *  \return Returns true if the surfaces is a Coon's patch
   */
  template <typename T>
  bool PHermiteSurface<T>::isValidCoonsPatch() {

    // Check number of curves
    if( _c1.getSize() < 2 || _c2.getSize() < 2 )
      return false;

    // Check that the corner points are joined together (b00, b10, b01 and b11)

    // _c1 -> c1/c2, _c2 -> f1/f2
    Point<T,3> c1_s, c1_e, c2_s, c2_e;
    Point<T,3> f1_s, f1_e, f2_s, f2_e;
    c1_s = _c1[0]->evaluateParent( _c1[0]->getParStart(), 0 )[0];
    c1_e = _c1[0]->evaluateParent( _c1[0]->getParEnd(), 0 )[0];
    c2_s = _c1[1]->evaluateParent( _c1[1]->getParStart(), 0 )[0];
    c2_e = _c1[1]->evaluateParent( _c1[1]->getParEnd(), 0 )[0];
    f1_s = _c2[0]->evaluateParent( _c2[0]->getParStart(), 0 )[0];
    f1_e = _c2[0]->evaluateParent( _c2[0]->getParEnd(), 0 )[0];
    f2_s = _c2[1]->evaluateParent( _c2[1]->getParStart(), 0 )[0];
    f2_e = _c2[1]->evaluateParent( _c2[1]->getParEnd(), 0 )[0];


    // Junction c1 x c2
    if( std::fabs( (c1_s - f1_s).getLength() ) > 1e-5 ) return false;

    // Junction c1 x c4
    if( std::fabs( (c1_e - f2_s).getLength() ) > 1e-5 ) return false;

    // Junction c2 x c3
    if( std::fabs( (f1_e - c2_s).getLength() ) > 1e-5 ) return false;

    // Junction c3 x c4
    if( std::fabs( (c2_e - f2_e).getLength() ) > 1e-5 ) return false;

    // return true if all criterias is met
    return true;
  }

  /*! bool PHermiteSurface<T>::isValidHermiteSurface()
   *
   *  Validates the surfaces as a Hermite surface
   *  Returns whether the surface is classified as a Hermite surface
   *
   *  \return Returns true if the surfaces is a hermite surface
   */
  template <typename T>
  bool PHermiteSurface<T>::isValidHermiteSurface() {

    // Needs to fullfill the requirements for a Coons patch
    if( !isValidCoonsPatch() )
      return false;

    // Check Size
    if( _c1.getSize() != 4 || _c2.getSize() != 4 )
      return false;

    // _c1 -> c1/c2/c3/c4, _c2 -> f1/f2/f3/f4
    // c3/c4 and f3/f4 represent derivatives
    Point<T,3> c1_s, c1_e, c2_s, c2_e, c3_s, c3_e, c4_s, c4_e;
    Point<T,3> f1_s, f1_e, f2_s, f2_e, f3_s, f3_e, f4_s, f4_e;

    c1_s = _c1[0]->evaluateParent( _c1[0]->getParStart(), 0 )[0];
    c1_e = _c1[0]->evaluateParent( _c1[0]->getParEnd(), 0 )[0];
    c2_s = _c1[1]->evaluateParent( _c1[1]->getParStart(), 0 )[0];
    c2_e = _c1[1]->evaluateParent( _c1[1]->getParEnd(), 0 )[0];
    c3_s = _c1[2]->evaluateParent( _c1[2]->getParStart(), 0 )[0];
    c3_e = _c1[2]->evaluateParent( _c1[2]->getParEnd(), 0 )[0];
    c4_s = _c1[3]->evaluateParent( _c1[3]->getParStart(), 0 )[0];
    c4_e = _c1[3]->evaluateParent( _c1[3]->getParEnd(), 0 )[0];
    f1_s = _c2[0]->evaluateParent( _c2[0]->getParStart(), 0 )[0];
    f1_e = _c2[0]->evaluateParent( _c2[0]->getParEnd(), 0 )[0];
    f2_s = _c2[1]->evaluateParent( _c2[1]->getParStart(), 0 )[0];
    f2_e = _c2[1]->evaluateParent( _c2[1]->getParEnd(), 0 )[0];
    f3_s = _c2[2]->evaluateParent( _c2[2]->getParStart(), 0 )[0];
    f3_e = _c2[2]->evaluateParent( _c2[2]->getParEnd(), 0 )[0];
    f4_s = _c2[3]->evaluateParent( _c2[3]->getParStart(), 0 )[0];
    f4_e = _c2[3]->evaluateParent( _c2[3]->getParEnd(), 0 )[0];


    // b00 -> c1_s f3'_s && f1_s c3'_s
    if( std::fabs( (c1_s - f3_s).getLength() ) > 1e-5 ) return false;
    if( std::fabs( (f1_s - c3_s).getLength() ) > 1e-5 ) return false;

    // b10 -> c1_e f4'_s && f2_s c3'_e
    if( std::fabs( (c1_e - f4_s).getLength() ) > 1e-5 ) return false;
    if( std::fabs( (f2_s - c3_e).getLength() ) > 1e-5 ) return false;

    // b01 -> f1_e c4'_s && c2_s f3'_e
    if( std::fabs( (f1_e - c4_s).getLength() ) > 1e-5 ) return false;
    if( std::fabs( (c2_s - f3_e).getLength() ) > 1e-5 ) return false;

    // b11 -> f2_e c3'_e && c2_e f4'_e
    if( std::fabs( (f2_e - c3_e).getLength() ) > 1e-5 ) return false;
    if( std::fabs( (c2_e - f4_e).getLength() ) > 1e-5 ) return false;

    // return true if all criterias is met
    return true;
  }

  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PHermiteSurface<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
