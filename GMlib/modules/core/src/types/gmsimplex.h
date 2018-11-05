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




#ifndef GM_CORE_TYPES_SIMPLEX_H
#define GM_CORE_TYPES_SIMPLEX_H

#include "gmpoint.h"
#include "gmsubspace.h"

namespace GMlib {

  template <typename T, int n, int m>
  class Simplex;

  template <typename T, int n>
  class LineSegment;

  template <typename T, int n>
  class Triangle;

  template <typename T, int n>
  class Tetrahedron;

  /*! \class  Simplex gmsimplex.h <gmSimplex.h>
   *
   *  \brief  interface for the Simplex, LineSegment, Triangle, Tetrahedron classes.
   *
   *  A template (n x m) Simplex, the simplex is static i.e. the dimentions
	 *  can not be change. The template type must be clean, i.e. is not allocating
	 *  memory and without any virtual functions
   *
	 *  The template n denote the dimention of the space the simplex is embedded into.
	 *  The temlate m denote the simplex dimention, i.e. m=2 is a line segment, m=3 is a triangle etc.
   */
  template <typename T, int n, int m>
  class Simplex : public Vector<Point<T,n>,m> {
  public:

    // Constructor
    Simplex();
    Simplex( const Point<T,n>& p);
    Simplex( const Point<T,n>& p1, const Point<T,n>& p2);
    Simplex( const Arrow<T,n>& a);
    Simplex( const Point<T,n>& p1, const Point<T,n>& p2, const Point<T,n>& p3);
    Simplex( const SubSpace<T,n,m-1>& s);

    // Copy constructor
    Simplex( const Vector< Point<T ,n>, m>& copy );


    Point<T,n>	                  closestPoint(const Point<T,n>& p) const;
    Vector<T,n>                   distanceVector(const Point<T,n>& p) const;
    T                             distanceTo(const Point<T,n>& p) const;
    T                             distanceAlong(const Point<T,n>& p, int i=0) const;
    Vector<Simplex<T,n,m-1>,m+2>  sub();

  }; // END class Simplex










  template <typename T, int n, int m, int k>
  inline
  Simplex<T,n,k> operator*(const Matrix<T,n,m>& ma, const Simplex<T,m,k>& s){
    SubSpace<T,n,k> r(multTrans(ma, s.matC()));
    r.pos() = s.posC();
    return r;
  }

  template <typename T, int n, int k>
  inline
  Simplex<T,n,k> operator*(const HqMatrix<T,n>& ma, const Simplex<T,n,k>& s){
    SubSpace<T,n,k> r(multTrans(ma, s.matC()));
    r.pos() = ma*s.posC();
    return r;
  }


  /*! \class LineSegment gmsimplex.h <gmSimplex.h>
   *
   *  \brief The Static LineSegment class
   *
   * A template LineSegment, the LineSegment is static i.e. the dimentions of
   * the embedding space can not be change. The template type must be clean,
   * i.e. is not allocating memory and without any virtual functions
   */
  template <typename T, int n>
  class LineSegment : public Simplex<T,n,2>
  {
  public:
    LineSegment();
    LineSegment( const Point<T,n>& p );
    LineSegment( const Point<T,n>& p1 ,const Point<T,n>& p2 );
    LineSegment( const Arrow<T,n>& a );
    LineSegment( const SubSpace<T,n,1>& s );
    LineSegment( const Vector<Point<T,n>,1>& s );

  }; // END class LineSegment









  /*!	\class Triangle gmsimplex.h <gmSimplex.h>
  *
  *  \brief The Static LineSegment class
	*
	* A template Triangle, the Triangle is static i.e. the dimentions of
  * the embedding space can not be change. The template type must be clean,
	* i.e. is not allocating memory and without any virtual functions
	*/
  template <typename T, int n>
  class Triangle : public Simplex<T,n,3>
  {
  public:
    Triangle();
    Triangle( const Point<T,n>& p );
    Triangle( const Point<T,n>& p ,const Vector<T,n>& v );
    Triangle( const Arrow<T,n>& a );
    Triangle( const SubSpace<T,n,2>& s );
    Triangle( const Vector<Point<T,n>,3>& s );

  }; // END class Triangle











  /*!	\class Tetrahedron gmsimplex.h <gmSimplex.h>
  *
  *  \brief The Static Tetrahedron class
	*
  * A template Tetrahedron, the Tetrahedron is static i.e. the dimentions of
	* the embedding space can not be change. The template type must be clean,
	* i.e. is not allocating memory and without any virtual functions
	*/
  template <class T, int n>
  class Tetrahedron : public Simplex<T,n,4> {
  public:
    Tetrahedron();
    Tetrahedron( const Point<T,n>& p );
    Tetrahedron( const Point<T,n>& p ,const Vector<T,n>& v );
    Tetrahedron( const Arrow<T,n>& a );
    Tetrahedron( const SubSpace<T,n,3>& s );
    Tetrahedron( const Vector<Point<T,n>,4>& s );

  }; // END class Tetrahedron

} // END namespace GMlib



// Include implementations
#include "gmsimplex.c"






#endif   // GM_CORE_TYPES_SIMPLEX_H
