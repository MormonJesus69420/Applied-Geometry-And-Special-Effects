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




#ifndef GM_CORE_TYPES_SUBSPACE_H
#define GM_CORE_TYPES_SUBSPACE_H



// GMlib includes
#include "gmpoint.h"
#include "gmmatrix.h"


namespace GMlib {


template <class T, int n, int m>
class SubSpace;

template <class T, int n>
class Line;

template <class T, int n>
class Plane;

template <class T, int n>
class Cube;


  /*! \class  SubSpace gmsubspace.h <gmSubSpace>
   *  \brief  The Static SubSpace class
   *
   *  The Static SubSpace class.
   *  A template (n x m) SubSpace, the SubSpace is static i.e. the dimentions
	 *  can not be change. The template type must be clean, i.e. is not allocating
	 *  memory and without any virtual functions
   *
	 *  The template n denote the dimention of the space the SubSpace is embedded into.
	 *  The temlate m denote the SubSpace dimention, i.e. m=1 is a line , m=2 is a plane etc.
   */
  template <typename T, int n, int m>
  class SubSpace : public Point<T,n> {
  public:
    SubSpace( const Point<T,n>& p );// = Point<T,n>(T(0)) );
    SubSpace( const Vector<Vector<T,n>,m>& v );
    SubSpace( const Arrow<T,n>& a );
    SubSpace( const Point<T,n>& p, const Vector<T,n>& v );
    SubSpace( const Point<T,n>& p, const Vector<Vector<T,n>,m>& v );
    SubSpace( const SubSpace<T,n,m>& s );

    Point<T,n>            getClosestPoint( const Point<T,n>& p ) const;
    Vector<T,n>&          getDir( int i = 0 );
    Vector<T,n> const&    getDirC( int i = 0 ) const;
    T                     getDistanceAlong( const Point<T,n>& p, int i = 0 ) const;
    T                     getDistanceTo( const Point<T,n>& p ) const;
    Vector<T,n>           getDistanceVector( const Point<T,n>& p ) const;
    Matrix<T,m,n>&        getMat();
    Matrix<T,m,n> const&  getMatC() const;
    Point<T,n>&	          getPos();
    Point<T,n> const&     getPosC() const;


  protected:
    Matrix<T,m,n>         _matrix;

  }; // END class SubSpace




  template <typename T, int n>
  class SubSpace<T,n,0> : public Point<T,n> {
  public:
    SubSpace( const Point<T,n>& p );// = Point<T,n>(T(0)) );
    SubSpace( const SubSpace<T,n,0>& s );

    Point<T,n>          getClosestPoint( const Point<T,n>& p ) const;
    Vector<T,n>         getDir( int i = 0 );
    Vector<T,n> const   getDirC( int i = 0 ) const;
    T                   getDistanceAlong(const Point<T,n>& p, int i = 0);
    T                   getDistanceTo(const Point<T,n>& p) const;
    Vector<T,n>         getDistanceVector(const Point<T,n>& p) const;
    Point<T,n>&         getPos();
    Point<T,n> const&   getPosC() const;

  }; // END class SubSpace<t,n,0> "Terminator"









  /*! \class  Line gmsubspace.h <gmSubSpace>
   *  \brief  The Static Line class
   *
   *  The Static Line class.
   *  A template Line, the Line is static i.e. the dimentions of
   *  the embedding space can not be change. The template type must be clean,
	 *  i.e. is not allocating memory and without any virtual functions.
   */
  template <class T, int n>
  class Line : public SubSpace<T,n,1> {
  public:
    // All implemented constructor calls the default constructor of SubSpace
    Line();                                           // Call the SubSpace<T,n,1>()
    Line( const Point<T,n>& p );                      // Call the SubSpace<T,n,1>(p)
    Line( const Point<T,n>& p, const Vector<T,n>& v); // Call the SubSpace<T,n,1>(p,v)
    Line( const Arrow<T,n>& a );                      // Call the SubSpace<T,n,1>(a)
    Line( const SubSpace<T,n,1>& s);                  // Call the SubSpace<T,n,1>(a)

  }; // END class Line





  /*! \class  Plane gmsubspace.h <gmSubSpace>
   *  \brief  The Static Plane class
   *
   *  The Static Plane class.
   *  A template Plane, the Plane is static i.e. the dimentions of
	 *  the embedding space can not be change. The template type must be clean,
   *  i.e. is not allocating memory and without any virtual functions.
   */

  template <class T, int n>
  class Plane : public SubSpace<T,n,2> {
  public:
    // All implemented constructor calls the default constructor of SubSpace
    Plane();                                            // Call the SubSpace<T,n,2>()
    Plane( const Point<T,n>& p );                       // Call the SubSpace<T,n,2>(p)
    Plane( const Point<T,n>& p, const Vector<T,n>& v ); // Call the SubSpace<T,n,2>(p,v)
    Plane( const Arrow<T,n>& a );                       // Call the SubSpace<T,n,2>(a)
    Plane( const SubSpace<T,n,2>& s );                  // Call the SubSpace<T,n,2>(a)

  };




  template <class T>
  class Plane<T,3> : public SubSpace<T,3,2> {
  public:
    // All implemented constructor calls the default constructor of SubSpace
    Plane(): SubSpace<T,3,2>(){}
    Plane( const Point<T,3>& p ): SubSpace<T,3,2>(p){}
    Plane( const Point<T,3>& p, const Vector<T,3>& v ): SubSpace<T,3,2>(p,v){}
    Plane( const Arrow<T,3>& a ): SubSpace<T,3,2>(a){}
    Plane( const SubSpace<T,3,2>& s ): SubSpace<T,3,2>(s){}

    void                setNormal(const Vector<T,3>& v);

    const Vector<T,3>&	getNormal() const;
    Point<T,3>          getClosestPoint(const Point<T,3>& p) const;
    Vector<T,3>         getDistanceVector(const Point<T,3>& p) const;
    T                   getDistanceTo(const Point<T,3>& p) const;
  }; // END class Plane





  /*! \class  Cube gmsubspace.h <gmSubSpace>
   *  \brief  The Static Cube class
   *
   *  The Static Cube class.
   *  A template Cube, the Cube is static i.e. the dimentions of
   *  the embedding space can not be change. The template type must be clean,
	 *  i.e. is not allocating memory and without any virtual functions.
   */
  template <class T, int n>
  class Cube : public SubSpace<T,n,3> {
  public:
    // All implemented constructor calls the default constructor of SubSpace
    Cube();                                             // Call the SubSpace<T,n,3>()
    Cube( const Point<T,n>& p );                        // Call the SubSpace<T,n,3>(p)
    Cube( const Point<T,n>& p, const Vector<T,n>& v );  // Call the SubSpace<T,n,3>(p,v)
    Cube( const Arrow<T,n>& a );                        // Call the SubSpace<T,n,3>(a)
    Cube( const SubSpace<T,n,3>& s );                   // Call the SubSpace<T,n,3>(a)

  }; // END class Cube








  // ****************************************************
  // SubSpace operators for SubSpace multiplication
  // ****************************************************

  template <class T, int n, int m, int k>
  inline
  SubSpace<T,n,k> operator * ( const Matrix<T,n,m>& mat, const SubSpace<T,m,k>& s ) {

    SubSpace<T,n,k> r( multTrans( mat, s.getMatC() ) );
    r.getPos() = s.getPosC();
    return r;
  }


  template <class T, int n, int k>
  inline
  SubSpace<T,n,k> operator * (const HqMatrix<T,n>& mat, const SubSpace<T,n,k>& s ) {

    SubSpace<T,n,k> r( multTrans( mat, s.getMatC() ) );
    r.getPos() = mat * s.getPosC();
    return r;
  }


  #ifdef GM_STREAM

  // ************************************************
  // Stream operators for std-stream, Qt-stream, etc.
  // ************************************************

    /*!
     *  \brief
     */
    template <class T_Stream, class T, int n, int m>
    inline
    T_Stream &operator<<( T_Stream& out, const SubSpace<T,n,m>& s ) {

      out << Point<T,n>(s) << GMseparator::group() << s.getMat();
      return out;
    }


    /*!
     *  \brief
     */
    template <class T_Stream, class T, int n, int m>
    inline
    T_Stream& operator<<( T_Stream& out, const SubSpace<T,n,m>* s ) {

      out << s->getPos() << GMseparator::group() << s->getMat();
      return out;
    }


    /*!
     *  \brief
     */
    template <class T_Stream, class T, int n, int m>
    inline
    T_Stream& operator>>( T_Stream& in, SubSpace<T,n,m>& s ) {

      Separator     es;
      Point<T,n>    p;
      Matrix<T,n,m> mat;

      in >> p >> es >> mat;

      s.getPosRef() = p;
      s.getMatRef() = mat;
      return in;
    }


    /*!
     *  \brief
     */
    template <class T_Stream, class T, int n, int m>
    inline
    T_Stream& operator>>( T_Stream& in, SubSpace<T,n,m>* s ) {
      Separator     es;
      Point<T,n>    p;
      Matrix<T,n,m> mat;

      in >> p >> es >> mat;

      s->getPosRef() = p;
      s->getMatRef() = mat;
      return in;
    }

  #endif

} // END namespace GMlib


// Include implementations
#include "gmsubspace.c"


#endif // GM_CORE_TYPES_SUBSPACE_H
