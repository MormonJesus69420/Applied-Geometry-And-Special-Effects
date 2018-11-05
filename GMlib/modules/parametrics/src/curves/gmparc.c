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


//*****************************************
// Constructors and destructor           **
//*****************************************

/*! PArc<T>::PArc( T speed, T curvature )
 *  Default constructor,
 *  to make an arc curve determined by speed and curvature
 *  The curve start in origin and follows the x-axis
 *  It lies in the xy-plane
 *  \param[in] speed      The speed of the circle at start |c'(0)|
 *  \param[in] curvature  The curvature = 1/radius of the circle |c''(0)|
 *  \param[in] start      (default 0) The start of the arc
 *  \param[in] end        (default 2Pi) The end of the arc
 */
  template <typename T>
  inline
  PArc<T>::PArc( T speed, T curvature, T start, T end ) : PCurve<T,3>(20, 0, 3) {
    _d = speed;
    _k = curvature;

    _start = start;
    _end   = end;
    _x     = 0.5;
  }




  /*! PArc<T>::PArc( DVector< Vector<T,3> >& p, T s, T t, T e )
   *  Default constructor,
   *  to make an arc curve determined by speed and curvature
   *  The curve is related to origin=c(t) - see parameter t below,
   *  and follows the x-axis and lies in the xy-plane
   *
   *  \param[in] p  Vector of vectors: p[0] = c(t), p[2] = c'(t), p[2] = a c''(t) + b c'(t) (for any a,b)
   *  \param[in] s  The start parameter of the arc
   *  \param[in] t  where c(t) = origin in the local coordinate system
   *  \param[in] e  The end parameter of the arc
   */
  template <typename T>
  inline
  PArc<T>::PArc( DVector< Vector<T,3> >& p, T s, T t, T e ) : PCurve<T,3>(20, 0, 3) {
    Vector<T,3> x, y, z;

    _d     = p[1].getLength();
    _start = (s-t);
    _end   = (e-t);
    _x     = (t-s)/(e-s);

    x      = p[1]/_d;
    y      = ( p[2] - ( x * p[2] ) * x ) / (_d * _d);
    _k     = y.getLength();

    if( _k > 1e-5 ) {
      y /= _k;
      z = y^x;
    }
    else {
      y = x.getLinIndVec();
      y.normalize();
      z = y^x;
    }

    // Do basis change
    this->_side = y.template toType<float>();
    this->_up   = z.template toType<float>();
    this->_dir  = x.template toType<float>();
    this->_pos  = p[0];
    this->basisChange( this->_side, this->_up, this->_dir, this->_pos );
  }



  /*! PArc<T>::PArc( const Vector<T,3>& p1, const Vector<T,3>& p2, const Vector<T,3>& p3 )
   *  Default constructor,
   *  to make an arc curve determined by three points
   *
   *  \param[in] p1      The first point, start of the arc
   *  \param[in] p2      The secont point, on the arc
   *  \param[in] p3      The thired point, end of the arc
   *  \param[in] n       (default 2, the midt point)
   *                       Local origin in point 1, 2 or 3,
   *                       it is also c(0) = pn, n = 1,2,3
   */
    template <typename T>
    inline
    PArc<T>::PArc( const Vector<T,3>& p1, const Vector<T,3>& p2, const Vector<T,3>& p3, int n ) : PCurve<T,3>(20, 0, 2) {

        Vector<T,3> x, y, z;
        Vector<T,3> p = n==1 ? p1 : (n==2 ? p2 : p3);
        Vector<T,3> q1 = (p1+p2)/2.0;
        Vector<T,3> q2 = (p3+p2)/2.0;
        Vector<T,3> v1 = p2-p1;
        Vector<T,3> v2 = p3-p2;

        T v11 = v1*v1;
        T v12 = v1*v2;
        T v22 = v2*v2;
        T a   = v12/v11;
        T d   = v22-a*v12;
        T d1, d2;

        if(d<1.0e-5){ // special case, a stright line
            _k = 0;
            _d = 1;//2*q.getLength();
            d1 = sqrt(v11);
            d2 = sqrt(v22);
            x = UnitVector<T,3>(v1);
            y = v1.getLinIndVec();
            z = UnitVector<T,3>(y^x);
            y = x^z;
         }
        else {
            T xx = ((q2-q1)*v2)/d;
            Vector<T,3>  p0 = q1+xx*(v2-a*v1);
            Vector<T,3>  a1 = p0-p1;
            Vector<T,3>  a2 = p0-p2;
            Vector<T,3>  a3 = p0-p3;
            T r  = a1.getLength();
            _k   = 1/r;
            _d   = 1;
            d1 = r*acos((a1*a2)/(r*r));
            d2 = r*acos((a2*a3)/(r*r));
            y = _k*(p0 - p);
            z = UnitVector<T,3>(n==1 ? y^v1 : y^v2);
            x = z^y;
        }
        if(n==1){
            _start = 0;
            _end   = d1+d2;
        } else if(n==2) {
            _start = -d1;
            _end   = d2;
        } else if(n==3) {
            _start = -d1-d2;
            _end   = 0;
        }
        _x = d1/(d1+d2);

        // Do basis change
        this->_side = y.template toType<float>();
        this->_up   = z.template toType<float>();
        this->_dir  = x.template toType<float>();
        this->_pos  = p;
        this->basisChange( this->_side, this->_up, this->_dir, this->_pos );
    }




  /*! PArc<T>::PArc(const PArc<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve (arc)
   *
   *  \param[in] copy The curve to copy
   */
  template <typename T>
  inline
  PArc<T>::PArc( const PArc<T>& copy ) : PCurve<T,3>(copy) {
    _d = copy._d;
    _k = copy._k;

    _start = copy._start;
    _end   = copy._end;
  }



  /*! PArc<T>::~PArc()
   *  The destructor
   *  clean up and destroy all private data
   */
  template <typename T>
  PArc<T>::~PArc() {}




  //**************************************
  //        Public local functons       **
  //**************************************

  /*! void PArc<T>::setCurvature( T curvature )
   *  Will change the curvature / radius of the arc
   *
   *  \param[in] curvature   The new curvature / radius of the arc
   */
  template <typename T>
  inline
  void PArc<T>::setCurvature( T curvature ) {
      _k = curvature;
  }



  /*! T PArc<T>::getCurvature() const
   *  Give you the curvature of the arc (curve)
   *
   *  \return  The curvature of the circle
   */
  template <typename T>
  inline
  T PArc<T>::getCurvature() const {
    return _k;
  }



  /*! void PArc<T>::setSpeed( T speed )
   *  Will change the speed of the arc
   *
   *  \param[in] speed   The new speed of the arc
   */
  template <typename T>
  inline
  void PArc<T>::setSpeed( T speed ) {
      _d = speed;
  }



  /*! T PArc<T>::getSpeed() const
   *  Give you the speed of the arc (curve)
   *
   *  \return  The speed of the arc
   */
  template <typename T>
  inline
  T PArc<T>::getSpeed() const {
    return _d;
  }



  /*! void PArc<T>::setStart( T start )
   *  Will set the start parameter of the arc
   *
   *  \param[in] speed   The new start parameter of the arc
   */
  template <typename T>
  inline
  void PArc<T>::setStart( T start ) {
      _start = start;
  }



  /*!  void PArc<T>::setEnd( T end )
   *  Will set the end parameter of the arc
   *
   *  \param[in] speed   The new end parameter of the arc
   */
  template <typename T>
  inline
  void PArc<T>::setEnd( T end ) {
      _end = end;
  }



  /*! void PArc<T>::setFirstPart( T x )
   *  Will set the devidion of the internal domain in two,
   *  it is: x(end-start) and (1-x)(end-start)
   *
   *  \param[in] x   The first part is x(end-start)
   */
  template <typename T>
  inline
  void PArc<T>::setFirstPart( T x ) {
      _x = x;
  }



  /*! void PArc<T>::setFirstDomain( T d )
   *  Will change the parametrisation so that the first part is d
   *
   *  \param[in] d   The first part of the domain
   */
  template <typename T>
  inline
  void PArc<T>::setFirstDomain( T d ) {
      T s = d/(_x*(_end-_start));
      _start *= s;
      _end   *= s;
      _d     /= s;
  }



  /*! T PArc<T>::getLastDomain() const
   *  Give you the last domain
   *
   *  \return  The last domain
   */
  template <typename T>
  inline
  T PArc<T>::getLastDomain() const {
    return (1-_x)*_d;
  }




  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************

  /*! bool PCircle<T>::isClosed() const
   *  To tell if this curve (arc) is a circle or nor.
   */
  template <typename T>
  bool PArc<T>::isClosed() const {
    return GMutils::compValueF(_k*_d*(_end-_start), T(M_2PI));
  }




  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  /*! void PArc<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  2 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (dummy) because left and right are always equal
   */
  template <typename T>
  void PArc<T>::eval( T t, int d, bool /*l*/ ) const {

      this->_p.setDim( d + 1 );

      if( _k < 1e-5 ) { // special case, a stright line
          this->_p[0][0] = _d * t;
          this->_p[0][1] = T(0);
          this->_p[0][2] = T(0);
          if( this->_dm == GM_DERIVATION_EXPLICIT ) {
              if( d > 0 ) {
                  this->_p[1][0] = _d;
                  this->_p[1][1] = T(0);
                  this->_p[1][2] = T(0);
                  if( d > 1 ) {
                      this->_p[2][0] = T(0);
                      this->_p[2][1] = T(0);
                      this->_p[2][2] = T(0);
                      if( d > 2 ) {
                          this->_p[3][0] = T(0);
                          this->_p[3][1] = T(0);
                          this->_p[3][2] = T(0);
                      }
                  }
              }
          }
      }
      else { //  a circle - the curvature _k is bigger than the tolerance
          T kdt = _k * _d * t;
          T skdt = sin(kdt)/_k;
          T ckdt = cos(kdt)/_k;
          this->_p[0][0] = skdt;
          this->_p[0][1] = 1/_k - ckdt;
          this->_p[0][2] = T(0);
          if( this->_dm == GM_DERIVATION_EXPLICIT ) {
              T g = _k * _d;
              if( d > 0 ) {
                  this->_p[1][0] = g * ckdt;
                  this->_p[1][1] = g * skdt;
                  this->_p[1][2] = T(0);
                  if( d > 1 ) {
                      g *= g;
                      this->_p[2][0] = -g * skdt;
                      this->_p[2][1] =  g * ckdt;
                      this->_p[2][2] =  T(0);
                      if( d > 2 ) {
                          g *= g;
                          this->_p[3][0] = -g * ckdt;
                          this->_p[3][1] = -g * skdt;
                          this->_p[3][2] =  T(0);
                      }
                  }
              }
          }
      }
  }



  /*! T PArc<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at start of the internal domain
   */
  template <typename T>
  T PArc<T>::getStartP() const {
    return _start;
  }



  /*! T PArc<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *
   *  \return The parametervalue at end of the internal domain
   */
  template <typename T>
  T PArc<T>::getEndP() const {
    return _end;
  }


} // END namespace GMlib



