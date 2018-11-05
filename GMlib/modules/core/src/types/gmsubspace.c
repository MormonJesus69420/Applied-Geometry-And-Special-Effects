/**********************************************************************************
**
** Copyright (C) 1994 - 2017 University of Tromsø - The Arctic University of Norway
** Contact: GMlib Online Portal at https://source.uit.no/gmlib/gmlib/wikis/home
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



  template <typename T, int n, int m>
  inline
  SubSpace<T,n,m>::SubSpace( const Point<T,n>& p ) : Point<T,n>(p), _matrix(true) {
  }


  template <typename T, int n, int m>
  inline
  SubSpace<T,n,m>::SubSpace( const Vector<Vector<T,n>,m>& v ) : Point<T,n>(T(0)), _matrix(v) {
  }


  template <typename T, int n, int m>
  inline
  SubSpace<T,n,m>::SubSpace( const Arrow<T,n>& a ) : Point<T,n>(a.getPos()), _matrix(a.getDir())	 {
  }


  template <typename T, int n, int m>
  inline
  SubSpace<T,n,m>::SubSpace( const Point<T,n>& p, const Vector<T,n>& v ) : Point<T,n>(p), _matrix(v)	{
  }


  template <typename T, int n, int m>
  inline
  SubSpace<T,n,m>::SubSpace( const Point<T,n>& p, const Vector<Vector<T,n>,m>& v ) : Point<T,n>(p), _matrix(v) {
  }


  template <typename T, int n, int m>
  inline
  SubSpace<T,n,m>::SubSpace( const SubSpace<T,n,m>& s ) : Point<T,n>(s), _matrix(s._matrix) {
  }


  template <typename T, int n, int m>
  inline
  Point<T,n> SubSpace<T,n,m>::getClosestPoint( const Point<T,n>& p ) const {

    Point<T,n>  cp  = getPosC();
    Vector<T,n> d   = p - cp;

    for(int i = 0; i < m; i++) {
      cp += ( (d * _matrix[i]) / (_matrix[i] * _matrix[i]) ) * _matrix[i];
    }
    return cp;
  }


  template <typename T, int n, int m>
  inline
  Vector<T,n>& SubSpace<T,n,m>::getDir( int i ){

    return _matrix[i];
  }


  template <typename T, int n, int m>
  inline
  Vector<T,n> const& SubSpace<T,n,m>::getDirC( int i ) const {

    return _matrix(i);
  }


  template <typename T, int n, int m>
  inline
  T SubSpace<T,n,m>::getDistanceAlong( const Point<T,n>& p, int i ) const {

    return ( p - getPos() ) * getDir(i) / (getDir(i).getLength() );
  }


  template <typename T, int n, int m>
  inline
  T SubSpace<T,n,m>::getDistanceTo( const Point<T,n>& p ) const {

    return getDistanceVector(p).getLngth();
  }


  template <typename T, int n, int m>
  inline
  Vector<T,n> SubSpace<T,n,m>::getDistanceVector( const Point<T,n>& p ) const {

    return reinterpret_cast<Vector<T,n>&>(p - getClosestPoint(p));
  }


  template <typename T, int n, int m>
  inline
  Matrix<T,m,n>& SubSpace<T,n,m>::getMat() {

    return _matrix;
  }


  template <typename T, int n, int m>
  inline
  Matrix<T,m,n> const& SubSpace<T,n,m>::getMatC() const {

    return _matrix;
  }


  template <typename T, int n, int m>
  inline
  Point<T,n>& SubSpace<T,n,m>::getPos() {

    return reinterpret_cast<Point<T,n>&>(*this);
  }


  template <typename T, int n, int m>
  inline
  Point<T,n> const& SubSpace<T,n,m>::getPosC() const {

    return reinterpret_cast<Point<T,n>const&>(*this);
  }




  // *****************************************
  // The SubSpace <T,n,0> class inplementation

  template <class T, int n>
  inline
  SubSpace<T,n,0>::SubSpace( const Point<T,n>& p ): Point<T,n>(p) {
  }


  template <class T, int n>
  inline
  SubSpace<T,n,0>::SubSpace( const SubSpace<T,n,0>& s ) : Point<T,n>(s) {
  }


  template <class T, int n>
  inline
  Point<T,n> SubSpace<T,n,0>::getClosestPoint( const Point<T,n>& ) const {

    return (*this);
  }


  template <class T, int n>
  inline
  Point<T,n>& SubSpace<T,n,0>::getPos(){

    return reinterpret_cast<Point<T,n>const&>(*this);
  }


  template <class T, int n>
  inline
  Point<T,n> const& SubSpace<T,n,0>::getPosC() const {

    return reinterpret_cast<Point<T,n>&>(*this);
  }


  template <class T, int n>
  inline
  Vector<T,n> SubSpace<T,n,0>::getDir(int) {

    return Vector<T,n>(0.0);
  }


  template <class T, int n>
  inline
  Vector<T,n> const SubSpace<T,n,0>:: getDirC(int) const {

    return Vector<T,n>(0.0);
  }


  template <class T, int n>
  inline
  T SubSpace<T,n,0>::getDistanceAlong(const Point<T,n>&, int) {

    return T(0);
  }


  template <class T, int n>
  inline
  T SubSpace<T,n,0>::getDistanceTo(const Point<T,n>& p) const {

    return getDistanceVector(p).getLength();
  }


  template <class T, int n>
  inline
  Vector<T,n> SubSpace<T,n,0>::getDistanceVector(const Point<T,n>& p) const {

    return reinterpret_cast<Vector<T,n>&>( p - getClosestPoint(p));
  }

  /*!
   *  \brief
   */
  template <class T, int n>
  inline
  Line<T,n>::Line() : SubSpace<T,n,1> () {}


  /*!
   *  \brief
   */
  template <class T, int n>
  inline
  Line<T,n>::Line( const Point<T,n>& p ) : SubSpace<T,n,1>(p) {}


  /*!
   *  \brief
   */
  template <class T, int n>
  inline
  Line<T,n>::Line( const Point<T,n>& p, const Vector<T,n>& v) : SubSpace<T,n,1>(p,v) {}


  /*!
   *  \brief
   */
  template <class T, int n>
  inline
  Line<T,n>::Line( const Arrow<T,n>& a ) : SubSpace<T,n,1>(a) {}


  /*!
   *  \brief
   */
  template <class T, int n>
  inline
  Line<T,n>::Line( const SubSpace<T,n,1>& s) :  SubSpace<T,n,1>(s) {}

  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Plane<T,n>::Plane() : SubSpace<T,n,2>()	{
  }


  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Plane<T,n>::Plane( const Point<T,n>& p ) : SubSpace<T,n,2>(p)	{
  }


  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Plane<T,n>::Plane( const Point<T,n>& p, const Vector<T,n>& v ) : SubSpace<T,n,2>(p,v) {
  }


  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Plane<T,n>::Plane( const Arrow<T,n>& a ) : SubSpace<T,n,2>(a) {
  }


  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Plane<T,n>::Plane( const SubSpace<T,n,2>& s ) : SubSpace<T,n,2>(s) {
  }

  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Cube<T,n>::Cube() : SubSpace<T,n,3>() {
  }


  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Cube<T,n>::Cube( const Point<T,n>& p ) : SubSpace<T,n,3>(p) {
  }


  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Cube<T,n>::Cube( const Point<T,n>& p, const Vector<T,n>& v ) : SubSpace<T,n,3>(p,v) {
  }


  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Cube<T,n>::Cube( const Arrow<T,n>& a ) :  SubSpace<T,n,3>(a) {
  }


  /*!
   *  \brief  Default constructor
   */
  template <class T, int n>
  inline
  Cube<T,n>::Cube( const SubSpace<T,n,3>& s ) : SubSpace<T,n,3>(s) {
  }

} // END namespace GMlib
