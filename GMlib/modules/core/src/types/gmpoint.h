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




#ifndef GM_CORE_TYPES_POINT_H
#define GM_CORE_TYPES_POINT_H


// **** GMlib ****
#include "gmangle.h"
#include "../static/gmstaticproc.h"
#include "../utils/gmstream.h"
#include "../containers/gmarray.h"


namespace GMlib {


  template <typename T, int n>
  class Point;

  template <typename T, int n>
  class Vector;

  template <typename T, int n>
  class UnitVector;

  template <typename T, int n>
  class Arrow;

  template <typename T, int n>
  class ScalarPoint;

  template <typename T, int n>
  class Sphere;

  template <typename T, int n>
  class Box;

  template <typename T, int n>
  class PlaneArrow;



// For following clas is for internal use only

// It is an help classes
  template <typename T, int n, int m>
  class M_I_;




  //******************************
  //**      The Point class     **
  //******************************


  /*! \class  Point - gmpoint.h <gmPoint>
   *  \brief  The Static Point Class
   *
   *  A template Point, the Point is static i.e. the dimentions
   *  can not be change. The template type must be clean, i.e. is not allocating
   *  memory and without any virtual functions
   *
   *  The point is only using n values, but it is acting as it is in homogenious
   *  coordinates. The (n+1).nt element, which is not there, is actually implicite 1
   */


  template <typename T, int n>
  class Point {
  public:
    Point();
    Point( T t );
    Point( const T *t );
    Point( const Point<T, n> &p );

    Angle                  getAngle(Point<T,n> p) const;
    Point<T, n>            getInverse() const;
    T                      getLength() const;
    int                    getMaxIndex() const;
    int                    getMaxAbsIndex() const;
    T*                     getPtr();
    const T*               getPtr() const;
    void                   setTestType( int t, const Point<T,n>& p, const Vector<T,n>& v = T(0) );

    const Point<T, n>&     operator =  ( const T t );
    const Point<T, n>&     operator =  ( const T *t );
    const Point<T, n>&     operator =  ( const Point<T, n> &p );
    const Point<T, n>&     operator += ( const Point<T, n> &p );
    const Point<T, n>&     operator -= ( const Point<T, n> &p );
    Vector<T, n>           operator -  () const;
    Vector<T, n>           operator +  ( const Point<T, n> &p ) const;
    Vector<T, n>           operator -  ( const Point<T, n> &p ) const;
    T                      operator *  ( const Point<T, n> &p ) const;
    T&                     operator [] ( int i);
    const T &              operator [] ( int i) const;
    const T &              operator () ( int i) const;

    // Scaling
    const Point<T, n>&     operator *= ( double d );
    Point<T, n>            operator *  ( double d ) const;
    const Point<T, n>&     operator %= ( const Point<T, n> &p );
    Point<T, n>            operator %  ( const Point<T, n> &p ) const;

    // Scaling: inverse
    const Point<T, n>&     operator /= ( double d );
    Point<T, n>            operator /  ( double d ) const;

    // Boolean on equality
    bool                   operator == ( const Point<T, n> &p ) const;
    bool                   operator != ( const Point<T, n> &p ) const; // caling operator == ()

    // Boolean on sorting
    bool                   operator <  ( const Point<T, n> &v ) const;
    bool                   operator >  ( const Point<T, n> &v ) const;
    bool                   operator <= ( const Point<T, n> &v ) const;
    bool                   operator >= ( const Point<T, n> &v ) const;

    // Casting
    template <typename G, int m>
    operator Point<G,m> () const;

    template <typename G, int m>
    operator Vector<G,m> () const;

    template <typename G, int m>
    operator UnitVector<G,m> () const;

    template <typename G,int m>
    Point<G,m>             to() const;

    template <typename G>
    Point<G,n>             toType() const;

    Point<T, n>&           toPoint();
    const Point<T, n>&     toPoint() const;

  protected:
    T                     _pt[n];

    void                  _cpy( const Point<T, n> &v );
    void                  _cpy( const T p[n] );
    void                  _cpy( const T &d );

  private:

    static Arrow<T, n>*   _arrow;    // Used for < and sorting see setTestType()
    static int            _sort_type;

  }; // END class Point






  template <typename T>
  class Point<T,2> {
  public:
      Point();
      Point( T t );
      Point( const T *t );
      Point( const Point<T,2> &p );

      Point( const Point<T,3> &p );
      Point( const T& x, const T& y);

      Angle        getAngle(Point<T,2> p) const;
      Point<T,2>   getInverse() const;
      T            getLength() const;
      int          getMaxIndex() const;
      int          getMaxAbsIndex() const;
      T*           getPtr();
      const T*     getPtr() const;
      void         setTestType( int t, const Point<T,2>& p, const Vector<T,2>& v = T(0) );

      Point<T,2>   getNormal();  // Return a vector 90 deg. to this.
      int          isInside(const Point<T,2>& p1,const Point<T,2>& p2,const Point<T,2>& p3) const;
      int          isInside(const Array<Point<T,2> >& a) const;
      int          isInsideCircle(const Point<T,2>& p1,const Point<T,2>& p2,const Point<T,2>& p3) const;

      T                      operator^(const Point<T,2>& v) const;  // wedge product.

      const Point<T,2>&      operator =  ( const T t );
      const Point<T,2>&      operator =  ( const T *t );
      const Point<T,2>&      operator =  ( const Point<T,2> &p );
      const Point<T,2>&      operator += ( const Point<T,2> &p );
      const Point<T,2>&      operator -= ( const Point<T,2> &p );
      Vector<T,2>            operator -  () const;
      Vector<T,2>            operator +  ( const Point<T,2> &p ) const;
      Vector<T,2>            operator -  ( const Point<T,2> &p ) const;
      T                      operator *  ( const Point<T,2> &p ) const;
      T&                     operator [] ( int i);
      const T &              operator [] ( int i) const;
      const T &              operator () ( int i) const;

      // Scaling
      const Point<T,2>&      operator *= ( double d );
      Point<T,2>             operator *  ( double d ) const;
      const Point<T,2>&      operator %= ( const Point<T,2> &p );
      Point<T,2>             operator %  ( const Point<T,2> &p ) const;

      // Scaling: inverse
      const Point<T,2>&      operator /= ( double d );
      Point<T,2>             operator /  ( double d ) const;

      // Boolean on equality
      bool                   operator == ( const Point<T,2> &p ) const;
      bool                   operator != ( const Point<T,2> &p ) const;

      // Boolean on sorting
      bool                   operator <  ( const Point<T,2> &v ) const;
      bool                   operator >  ( const Point<T,2> &v ) const;
      bool                   operator <= ( const Point<T,2> &v ) const;
      bool                   operator >= ( const Point<T,2> &v ) const;

      // Casting
      template <typename G, int m>
      operator Point<G,m> () const;

      template <typename G, int m>
      operator Vector<G,m> () const;

      template <typename G, int m>
      operator UnitVector<G,m> () const;

      template <typename G,int m>
      Point<G,m>             to() const;

      template <typename G>
      Point<G,2>             toType() const;

      Point<T,2>&            toPoint();
      const Point<T,2>&      toPoint() const;

    protected:
      T                     _pt[2];

      void                  _cpy( const Point<T,2> &v );
      void                  _cpy( const T p[2] );
      void                  _cpy( const T &d );

    private:

      static Arrow<T,2>*    _arrow;    // Used for < and sorting see setTestType()
      static int            _sort_type;

  };




  template <typename T>
  class Point<T,3>{
  public:
      Point();
      Point( T t );
      Point( const T *t );
      Point( const Point<T,3> &p );

      Point( const Point<T,2> &p );
      Point( const T& x, const T& y, const T& z);

      Angle                  getAngle(Point<T,3> p) const;
      Point<T,3>             getInverse() const;
      T                      getLength() const;
      int                    getMaxIndex() const;
      int                    getMaxAbsIndex() const;
      T*                     getPtr();
      const T*               getPtr() const;
      void                   setTestType( int t, const Point<T,3>& p, const Vector<T,3>& v = T(0) );

      Point<T,3> operator^(const Point<T,3>& v) const; // vector product.


      const Point<T,3>&      operator =  ( const T t );
      const Point<T,3>&      operator =  ( const T *t );
      const Point<T,3>&      operator =  ( const Point<T,3> &p );
      const Point<T,3>&      operator += ( const Point<T,3> &p );
      const Point<T,3>&      operator -= ( const Point<T,3> &p );
      Vector<T,3>            operator -  () const;
      Vector<T,3>            operator +  ( const Point<T,3> &p ) const;
      Vector<T,3>            operator -  ( const Point<T,3> &p ) const;
      T                      operator *  ( const Point<T,3> &p ) const;
      T&                     operator [] ( int i);
      const T &              operator [] ( int i) const;
      const T &              operator () ( int i) const;

      // Scaling
      const Point<T,3>&      operator *= ( double d );
      Point<T,3>             operator *  ( double d ) const;
      const Point<T,3>&      operator %= ( const Point<T,3> &p );
      Point<T,3>             operator %  ( const Point<T,3> &p ) const;

      // Scaling: inverse
      const Point<T,3>&      operator /= ( double d );
      Point<T,3>             operator /  ( double d ) const;

      // Boolean on equality
      bool                   operator == ( const Point<T,3> &p ) const;
      bool                   operator != ( const Point<T,3> &p ) const; // caling operator == ()

      // Boolean on sorting
      bool                   operator <  ( const Point<T,3> &v ) const;
      bool                   operator >  ( const Point<T,3> &v ) const;
      bool                   operator <= ( const Point<T,3> &v ) const;
      bool                   operator >= ( const Point<T,3> &v ) const;

      // Casting
      template <typename G, int m>
      operator Point<G,m> () const;

      template <typename G, int m>
      operator Vector<G,m> () const;

      template <typename G, int m>
      operator UnitVector<G,m> () const;

      template <typename G,int m>
      Point<G,m>             to() const;

      template <typename G>
      Point<G,3>             toType() const;

      Point<T,3>&            toPoint();
      const Point<T,3>&      toPoint() const;

    protected:
      T                     _pt[3];

      void                  _cpy( const Point<T,3> &v );
      void                  _cpy( const T p[2] );
      void                  _cpy( const T &d );

    private:

      static Arrow<T,3>*    _arrow;    // Used for < and sorting see setTestType()
      static int            _sort_type;

  };

  //****************************************************
  //***********       END class Point     **************
  //****************************************************



  /*! Point<T, n>& operator * ( double d, const Point<T, n> &p)
   *  \brief Multiply a double with a Point<T, n>
   *
   *  Multiply a double with a Point<T, n>.
   *  This function overloads the * operator of the double.
   *
   *  \param[in] d The double
   *  \param[in] p The Point<T,n>
   */
  template <typename T, int n>
  inline
  Point<T, n> operator * ( double d, const Point<T, n> &p ) {
    return p*d;
  }




  /*! bool Point<T,2>::convexHullFrom(Array<Point<T,2> >& a, const Vector<T,2>& v)
   *  \brief  Compute the convex hull of a point set
   *
   * This function compute the convex hull of a point
   * set stored in the array a.
   * The convex hull is also stored in the array a,
   * and it is a counter clockwise oriented polygon.
   * If the convex hull consist off all original points
   * from a then false is retuned else true is returned.
   */
  template <typename T>
  bool convexHullFrom(Array<Point<T,2> >& a, const Vector<T,2>& v) {

    bool removed = false;
    if (a.size()<4)  return removed;

    int i,j,k;
    Point<T,2> p;

    for(i=0; i<a.size(); i++) p += a[i];

    p.setTestType(3,p/a.size(),v);
    a.sort();

    for(i=0; i<a.size(); i++)  {
      if(i<a.size()-2)    { j=i+1; k=i+2; }
      else if(i<a.size()-1)  { j=i+1; k=0;   }
      else          { j=0;   k=1;   }
      p = a[j]-a[i];
      if((p^(a[k]-a[j])) < 0)  {
        a.removeIndex(j);
        removed = true;
        if(i==a.size()) i-=3;
        else if(i>0)  i-=2;
        else      i--;
      }
    }
    return removed;
  }




  #ifdef GM_STREAM
    /*! T_Stream &operator << ( T_Stream &out, const Point<T, n> &p )
     *  \brief Stream output operator
     *
     *  Stream output operator, taking a Point<T,n> as a second parameter.
     *
     *  \param out The output stream
     *  \param p The Point<T, n>
     *  \return The output stream
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream& operator << ( T_Stream &out, const Point<T, n> &p ) {
      for(int i = 0; i < n; i++)
        out << p(i) << GMseparator::element();
      return out;
    }


    /*! T_Stream& operator << ( T_Stream &out, const Point<T, n> *p )
     *  \brief Stream output operator
     *
     *  Stream output operator, taking a Point<T,n> pointer as a second parameter.
     *
     *  \param[in,out] out The output stream
     *  \param[in] p The Point<T, n> pointer
     *  \return The output stream
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream& operator << ( T_Stream &out, const Point<T, n> *p ) {
      for(int i=0;i<n;i++) out << (*p)(i) << GMseparator::element();
      return out;
    }


    /*! T_Stream& operator >> ( T_Stream &in, Point<T, n> &p )
     *  \brief Stream input operator
     *
     *  Stream input operator, taking a Point<T,n> as a second parameter.
     *
     *  \param[in,out] in The input stream
     *  \param[in] p The Point<T, n>
     *  \return The input stream
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream& operator >> ( T_Stream &in, Point<T, n> &p ) {
      Separator es(GMseparator::element());
      for(int i=0;i<n;i++) in >> p[i] >> es;
      return in;
    }

    /*! T_Stream& operator >> ( T_Stream &in, Point<T, n> *p )
     *  \brief Stream input operator
     *
     *  Stream input operator, taking a Point<T,n> pointer as a second parameter.
     *
     *  \param[in,out] in The input stream
     *  \param[in] p The Point<T, n> pointer
     *  \return The input stream
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream& operator >> ( T_Stream &in, Point<T, n> *p ) {
      Separator es(GMseparator::element());
      for(int i=0;i<n;i++) in >> (*p)[i] >> es;
      return in;
    }
  #endif



    //*******************************
    //**      The Vector class     **
    //*******************************


  /*! \class Vector gmpoint.h <gmPoint>
   *  \brief The Static Vector Class
   *
   *  A template Vector, the Vector is static i.e. the dimentions
   *  can not be change. The template type must be clean, i.e. is not allocating
   *  memory and without any virtual functions.
   *
   *  The vector is only using n values, but it is acting as it is in homogenious
   *  coordinates. The (n+1).nt element, which is not there, is actually implicite 0.
   */
  template <typename T, int n>
  class Vector : public Point<T,n> {
  public:
    Vector():Point<T,n>(){}
    Vector( T t ):Point<T,n>(t){}
    Vector( const T *t ):Point<T,n>(t){}
    Vector( const Point<T,n> &p ):Point<T,n>(p){}

    Point<T,n>           getNormalized() const;
    Vector<T,n>          getLinIndVec() const;
    const Point<T,n>&    normalize();
    void                 setLength( T length );

  };


  template <typename T>
  class Vector<T,2> : public Point<T,2> {
  public:
    Vector<T,2>():Point<T,2>(){}
    Vector<T,2>( T t ):Point<T,2>(t){}
    Vector<T,2>( const T *t ):Point<T,2>(t){}
    Vector<T,2>( const Point<T,2> &p ):Point<T,2>(p){}

    Vector<T,2>( const Point<T,3> &p );
    Vector<T,2>( const Point<T,4> &p );
    Vector<T,2>( const T& x, const T& y);

    Point<T,2>           getNormalized() const;
    Vector<T,2>          getLinIndVec() const;
    const Point<T,2>&    normalize();
    void                 setLength( T length );

    Point<T,2>           getNormal() const;  // Return a vector 90 deg. to this.

    T                    operator^(const Point<T,2>& v) const;// wedge product.

  };


  template <typename T>
  class Vector<T,3> : public Point<T,3> {
  public:
    Vector<T,3>():Point<T,3>(){}
    Vector<T,3>( T t ):Point<T,3>(t){}
    Vector<T,3>( const T *t ):Point<T,3>(t){}
    Vector<T,3>( const Point<T,3> &p ):Point<T,3>(p){}

    Vector<T,3>( const Point<T,2> &p );
    Vector<T,3>( const Point<T,4> &p );
    Vector<T,3>( const T& x, const T& y, const T& z);

    Point<T,3>           getNormalized() const;
    Vector<T,3>          getLinIndVec() const;
    const Point<T,3>&    normalize();
    void                 setLength( T length );

    Point<T,3>           operator^(const Point<T,3>& v) const;// vector product.

  };



  template <typename T>
  class Vector<T,4> : public Point<T,4> {
  public:
    Vector<T,4>():Point<T,4>(){}
    Vector<T,4>( T t ):Point<T,4>(t){}
    Vector<T,4>( const T *t ):Point<T,4>(t){}
    Vector<T,4>( const Point<T,4> &p ):Point<T,4>(p){}

    Vector<T,4>( const Point<T,2> &p );
    Vector<T,4>( const Point<T,3> &p );
    Vector<T,4>( const T& x0, const T& x1, const T& x2, const T& x3 );

    Point<T,4>           getNormalized() const;
    Vector<T,4>          getLinIndVec() const;
    const Point<T,4>&    normalize();
    void                 setLength( T length );

  };

  // END class Vector



  // ************
  // Type Casting

  /*! Vector<T, n>&   toVector( Point<T,n> &p )
   *  \brief Casts a Point<T,n> to a Vector<T,n>
   *
   *  Casts a Point<T,n> to a Vector<T,n>
   *
   *  \param[in] p The Point to be casted to Vector
   *  \return The casted vector
   */
  template <typename T, int n>
  inline const Vector<T, n>&   toVector( const Point<T,n> &p ) {
    return static_cast<const Vector<T,n>&>(p);
  }

  /*! Point<T, n>&   toPoint( Vector<T,n> &v )
   *  \brief Casts a Vector<T,n> to a Point<T,n>
   *
   *  Casts a Vector<T,n> to a Point<T,n>
   *
   *  \param[in] v The Vector to be casted to Point
   *  \return The casted point
   */
  template <typename T, int n>
  const Point<T, n>&   toPoint( const Vector<T,n> &v ) {
    return *static_cast<const Point<T,n>*>(&v);
  }


    //***********************************
    //**      The UnitVector class     **
    //***********************************

  /*! \class UnitVector gmpoint.h <gmPoint>
   *  \brief The Static UnitVector class
   *
   *  A template UnitVector, the UnitVector is static i.e. the dimentions
   *  can not be change. The template type must be clean, i.e. is not allocating
   *  memory and without any virtual functions.
   *
   *  The UnitVector has, as the name indicate, always length 1. Be aware the you do not
   *  initiate it by a zero vector or by zero constant. In that case it will produce an overflow.
   *
   *  The unit vector is only using n values, but it is acting as it is in homogenious
   *  coordinates. The (n+1).nt element (there is not there) is actually implicite 0
   */
  template <typename T, int n>
  class UnitVector : public Vector<T,n> {
  public:
    UnitVector( T t = 1 );
    UnitVector( const T t[n]);
    UnitVector( const Point<T, n>& p);
    UnitVector( const UnitVector<T,n>& uv);

    const Vector<T, n>&   operator =  ( const T t );
    const Vector<T, n>&   operator =  ( const T t[n] );
    const Vector<T, n>&   operator =  ( const Point<T, n> &p );
//    const Vector<T, n>&   operator =  ( const Vector<T, n> &p );
    const Vector<T, n>&   operator =  ( const UnitVector<T, n>& uv );
    const T&              operator [] ( int i );
    const Vector<T, n>&   operator += ( const Vector<T, n> &p );
    const Vector<T, n>&   operator -= ( const Vector<T, n> &p );
    const Vector<T, n>&   operator %= ( const Vector<T, n> &p );
    const Vector<T, n>&   operator *= ( const double d );
    const Vector<T, n>&   operator /= ( double d );

  }; // END class UnitVector


  template <typename T>
  class UnitVector<T,2> : public Vector<T,2> {
  public:
    UnitVector<T,2>( T t = 1 );
    UnitVector<T,2>( const T t[2]);
    UnitVector<T,2>( const Point<T,2>& p);
    UnitVector<T,2>( const UnitVector<T,2>& uv);
    UnitVector<T,2>( const T& x, const T& y);

    const Vector<T,2>&    operator =  ( const T t );
    const Vector<T,2>&    operator =  ( const T t[2] );
    const Vector<T,2>&    operator =  ( const Point<T,2> &p );
//    const Vector<T,2>&    operator =  ( const Vector<T,2> &p );
    const Vector<T,2>&    operator =  ( const UnitVector<T,2>& uv );
    const T&              operator [] ( int i );
    const Vector<T,2>&    operator += ( const Vector<T,2> &p );
    const Vector<T,2>&    operator -= ( const Vector<T,2> &p );
    const Vector<T,2>&    operator %= ( const Vector<T,2> &p );
    const Vector<T,2>&    operator *= ( const double d );
    const Vector<T,2>&    operator /= ( double d );

  }; // END class UnitVector



  template <typename T>
  class UnitVector<T,3> : public Vector<T,3> {
  public:
    UnitVector<T,3>( T t = 1 );
    UnitVector<T,3>( const T t[3]);
    UnitVector<T,3>( const Point<T,3>& p);
    UnitVector<T,3>( const UnitVector<T,3>& uv);
    UnitVector<T,3>( const T& x, const T& y, const T& z);

    const Vector<T,3>&    operator =  ( const T t );
    const Vector<T,3>&    operator =  ( const T t[3] );
    const Vector<T,3>&    operator =  ( const Point<T,3> &p );
//    const Vector<T,3>&    operator =  ( const Vector<T,3> &p );
    const Vector<T,3>&    operator =  ( const UnitVector<T,3>& uv );
    const T&              operator [] ( int i );
    const Vector<T,3>&    operator += ( const Vector<T,3> &p );
    const Vector<T,3>&    operator -= ( const Vector<T,3> &p );
    const Vector<T,3>&    operator %= ( const Vector<T,3> &p );
    const Vector<T,3>&    operator *= ( const double d );
    const Vector<T,3>&    operator /= ( double d );

  }; // END class UnitVector

  #ifdef GM_STREAM
    /*! T_Stream& operator >> ( T_Stream& in, UnitVector<T, n>& v )
     *  \brief Brief description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream& operator >> ( T_Stream& in, UnitVector<T, n>& v ) {
      Point<T, n> p;
      in >> p;
      v = p;
      return in;
    }

    /*! T_Stream& operator >> (T_Stream& in, UnitVector<T,n>* v)
     *  \brief Brief description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream& operator >> (T_Stream& in, UnitVector<T,n>* v) {
      Point<T,n> p;
      in >> p;
      (*v) = p;
      return in;
    }
  #endif




    //***********************************
    //**      The Quaternion class     **
    //***********************************


  /*! \class Quaternion gmpoint.h <gmPoint>
   *  \brief The Static Quaternion class
   */
  template <typename T>
  class Quaternion : public Vector<T,4> {
  public:
    Quaternion() : Vector<T,4>() {}
    Quaternion( const T *t ):Vector<T,4>(t){}
    Quaternion( const T& q0, const T& q1, const T& q2, const T& q3 ) : Vector<T,4>(q0,q1,q2,q3) {}
    Quaternion( const Quaternion& q ):Vector<T,4>(q){}
    Quaternion( const Vector<T,4> &p ):Vector<T,4>(p){}

  }; // END class Quaternion



    //***************************************
    //**      The UnitQuaternion class     **
    //***************************************


  /*! \class UnitQuaternion gmpoint.h <gmPoint>
   *  \brief The Static UnitQuaternion class
   */
  template <typename T>
  class UnitQuaternion : public Quaternion<T> {
  public:
    UnitQuaternion();
    UnitQuaternion( const T t[4] );
    UnitQuaternion( const T& q0, const T& q1, const T& q2, const T& q3 );
    UnitQuaternion( const UnitQuaternion& uq );
    UnitQuaternion( const Vector<T,4> &p );

    const Vector<T,4>&    operator =  ( const T t );
    const Vector<T,4>&    operator =  ( const T t[3] );
    const Vector<T,4>&    operator =  ( const Vector<T,3> &p );
    const Vector<T,4>&    operator =  ( const UnitQuaternion<T>& uv );
    const T&              operator [] ( int i );
    const Vector<T,4>&    operator += ( const Vector<T,4> &p );
    const Vector<T,4>&    operator -= ( const Vector<T,4> &p );
    const Vector<T,4>&    operator %= ( const Vector<T,4> &p );
    const Vector<T,4>&    operator *= ( const double d );
    const Vector<T,4>&    operator /= ( double d );
  };




    //*******************************
    //**      The Arrow class     **
    //*******************************


  /*! \class Arrow gmpoint.h <gmPoint>
   *  \brief The Static Arrow class
   *
   *  A template Arrow, the Arrow is static i.e. the dimentions
   *  can not be change. The template type must be clean, i.e. is not allocating
   *  memory and without any virtual functions.
   *
   *  The Arrow is a Point and a conected Vector.
   */
  template <typename T, int n>
  class Arrow : public Point<T,n> {
  public:
    Arrow();
    Arrow(const Point<T,n> &p);
    Arrow(const Point<T,n> &p, const Vector<T,n> &v );
    Arrow(const Arrow<T,n> &a);

    const Vector<T,n>&   getDir() const;
    const Point<T,n>&    getPos() const;
    void                 setDir( const Vector<T,n> &v );
    void                 setPos( const Point<T,n> &p );

    const Arrow<T,n>&    operator =  ( const Arrow<T,n> &a );
    const Arrow<T,n>&    operator += ( const Point<T,n> &p );
    const Arrow<T,n>&    operator -= ( const Point<T,n> &p );
    Arrow<T,n>           operator +  ( const Point<T,n> &p ) const;
    Arrow<T,n>           operator -  ( const Point<T,n> &p ) const;
    const Arrow<T,n>&    operator += ( const Vector<T,n> &v );
    const Arrow<T,n>&    operator -= ( const Vector<T,n> &v );
    Arrow<T,n>           operator +  ( const Vector<T,n> &v ) const;
    Arrow<T,n>           operator -  ( const Vector<T,n> &v ) const;
    Arrow<T,n>           operator -  () const;

    template <typename G, int m>
    operator Arrow<G, m> () const;

    Arrow<float,n>       toFloat() const;
    Arrow<double,n>      toDouble() const;


  protected:
    Vector<T,n>          _dir;

  }; // END class Arrow



  // *****************************
  // IOSTREAM overloaded operators

  #ifdef GM_STREAM

  /*! T_Stream &operator<<(T_Stream &out, const Arrow<T,n> &a)
   *  \brief Brief description
   *
   *  Detailed Description
   */
  template <typename T_Stream,typename T, int n>
  T_Stream &operator << ( T_Stream &out, const Arrow<T,n> &a ) {

    out << a.getPos() << GMseparator::element() << a.getDir();
    return out;
  }


  /*! T_Stream& operator<<(T_Stream &out, const Arrow<T,n> *a)
   *  \brief Brief Description
   *
   *  Detailed Description
   */
  template <typename T_Stream,typename T, int n>
  T_Stream& operator << ( T_Stream &out, const Arrow<T,n> *a ) {

    out << a->getPos() << GMseparator::element() << a->getDir();
    return out;
  }


  /*! T_Stream& operator>>(T_Stream &in, Arrow<T,n> &a)
   *  \brief Brief Description
   *
   *  Detailed Description
   */
  template <typename T_Stream,typename T, int n>
  T_Stream& operator >> ( T_Stream &in, Arrow<T,n> &a ) {

    Separator es(GMseparator::element());
    Point<T,n>  pt;
    Vector<T,n> dir;

    in >> pt >> es >> dir;
    a.setPos(pt);
    a.setDir(dir);

    return in;
  }


  /*! T_Stream& operator>>(T_Stream &in, Arrow<T,n> *a)
   *  \brief Brief Description
   *
   *  Detailed Description
   */
  template <typename T_Stream,typename T, int n>
  T_Stream& operator >> ( T_Stream &in, Arrow<T,n> *a ) {

    Separator es(GMseparator::element());
    Point<T,n>  pt;
    Vector<T,n> dir;

    in >> pt >> es >> dir;
    a->setPos(pt);
    a->setDir(dir);

    return in;
  }

  #endif



  //************************************
  //**      The ScalarPoint class     **
  //************************************

  /*! \class ScalarPoint gmpoint.h <gmPoint>
   *  \brief The Static ScalarPoint class
   *  A template ScalarPoint, the ScalarPoint is static i.e. the dimentions
   *  can not be change. The template type must be clean, i.e. is not allocating
   *  memory and without any virtual functions.
   *
   *  The ScalarPoint is a Point and a conected scalar value.
   */
  template <typename T, int n>
  class ScalarPoint {
  public:
    ScalarPoint();
    ScalarPoint( const Point<T, n>& p, T v = T(0) );
    ScalarPoint( const ScalarPoint<T, n>& s );


    const  Point<T, n>&   getPos() const;
    T*                    getPtr();
    const T*              getPtr() const;
    T                     getValue() const;
    void                  reset( const Point<T, n>& p , T v = T(0) );
    void                  reset();
    void                  resetValue( T t );
    void                  resetPos( const Point<T, n>& p );


    ScalarPoint<T,n>&           operator =  ( const ScalarPoint<T,n>& ) = default;
    const ScalarPoint<T, n>&    operator += ( const Point<T, n>& p );
    ScalarPoint<T, n>           operator +  ( const Point<T, n>& p ) const;
    const ScalarPoint<T, n>&    operator += ( T p );
    ScalarPoint<T, n>           operator +  ( T p ) const;
    const ScalarPoint<T, n>&    operator += ( const ScalarPoint<T, n>& p );
    ScalarPoint<T, n>           operator +  ( const ScalarPoint<T, n>& p ) const;
    const ScalarPoint<T, n>&    operator *= ( double d );
    ScalarPoint<T, n>           operator *  ( double d ) const;

    const ScalarPoint<T, n>&    operator /= ( double d );
    ScalarPoint<T, n>           operator /  ( double d ) const;

    const ScalarPoint<T, n>&    operator %= ( const Point<T, n>& p );
    ScalarPoint<T, n>           operator %  ( const Point<T, n>& p ) const;
/*
    friend
    ScalarPoint<T, n>           operator *  ( double d, ScalarPoint<T, n> p ) { p*=d; return p; }
*/

    // Casting
    template <typename G, int m>
    operator ScalarPoint<G, m> () const;

    template <typename G,int m>
    ScalarPoint<G,m>            to() const;

    template <typename G>
    ScalarPoint<G,n>            toType() const;



  protected:
    Point<T, n>           _pos;
    T                     _value;

  }; // END class ScalarPoint




  #ifdef GM_STREAM

    /*! T_Stream &operator<<(T_Stream& os, const ScalarPoint<T,n>& s)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream &operator<<(T_Stream& os, const ScalarPoint<T,n>& s) {
      os << s.getPos() << GMseparator::element() << s.getValue();
      return os;
    }

    /*! T_Stream &operator<<(T_Stream& os, const ScalarPoint<T,n>* s)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream &operator<<(T_Stream& os, const ScalarPoint<T,n>* s) {
      os << s->getPos() << GMseparator::element() << s->getValue();
      return os;
    }

    /*! T_Stream &operator>>(T_Stream& is, ScalarPoint<T,n>& s)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream &operator >> ( T_Stream& is, ScalarPoint<T, n>& s ) {
      Separator es(GMseparator::element());
      Point<T,n> p1;
      T v;
      is >> p1 >> es >> v;
      s.reset(p1,v);
      return is;
    }

    /*! T_Stream &operator>>(T_Stream& is, ScalarPoint<T,n>* s)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream &operator>>(T_Stream& is, ScalarPoint<T,n>* s) {
      Separator es(GMseparator::element());
      Point<T,n> p1;
      T v;
      is >> p1 >> es >> v;
      s->reset(p1,v);
      return is;
    }

  #endif



    //*******************************
    //**      The Sphere class     **
    //*******************************


  /*! \class Sphere gmpoint.h <gmPoint>
   *  \brief The Static Sphere class
   *  A template Sphere, the Sphere is static i.e. the dimentions
   *  can not be change. The template type must be clean, i.e. is not allocating
   *  memory and without any virtual functions.
   *
   *  The Sphere is a centre Point and a conected scalar value, radius.
   *  It also inclue a boolean telling if the Sphere is valid or not.
   */
  template <typename T, int n>
  class Sphere : public ScalarPoint<T, n> {
  public:
    Sphere( bool sphere = false );
    Sphere( const Point<T, n>& p, T v = T(0) );
    Sphere( const Point<T, n>& p1, const Point<T, n>& p2 );
    Sphere( const ScalarPoint<T, n>& s );
    Sphere( const Sphere<T, n>& s );

    T                     getRadius() const;
    bool                  isValid() const;
    bool                  isIntersecting(const Sphere<T,n>& p) const;
    void                  resetPos( const Point<T, n>& p );
    void                  resetRadius( T t );
    void                  reset();

    Sphere<T,n>&          operator = ( const Sphere<T,n>& other ) = default;
    const Sphere<T, n>&   operator += ( const Point<T, n>& p );
    Sphere<T, n>          operator +  ( const Point<T, n>& p ) const;
    const Sphere<T, n>&   operator += ( const Sphere<T, n>& p );
    Sphere<T, n>          operator +  ( const Sphere<T, n>& p ) const;

    // Casting
    template <typename G, int m>
    operator Sphere<G, m> () const;

  private:
    bool _valid;

  }; // END class Sphere










  #ifdef GM_STREAM

    /*! T_Stream &operator<<(T_Stream& os, const Sphere<T,n>& s)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream &operator<<(T_Stream& os, const Sphere<T,n>& s) {
      if(s.isValid())
        os << static_cast<ScalarPoint<T,n> >(s);
      else
        os << ScalarPoint<T,n>(Point<T,n>(),T(-1));
      return os;
    }

    /*! T_Stream &operator<<(T_Stream& os, const Sphere<T,n>* s)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream &operator<<(T_Stream& os, const Sphere<T,n>* s) {
      if(s->isValid())
        os << reinterpret_cast< ScalarPoint<T,n> >( (*s) );
      else
        os << ScalarPoint<T,n>(Point<T,n>(),T(-1));

      return os;
    }

    /*! T_Stream &operator>>(T_Stream& is, Sphere<T,n>& s)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream &operator>>(T_Stream& is, Sphere<T,n>& s) {
      ScalarPoint<T,n> ss;
      is >> ss;
      if(ss.getValue()<0)
        s.reset();
      else
        s = Sphere<T,n>(ss);
      return is;
    }

    /*! T_Stream &operator>>(T_Stream& is, Sphere<T,n>* s)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template <typename T_Stream, typename T, int n>
    inline
    T_Stream &operator>>(T_Stream& is, Sphere<T,n>* s) {
      ScalarPoint<T,n> ss;
      is >> ss;
      if(ss.getValue()<0)
        (*s).reset();
      else
        (*s) = Sphere<T,n>(ss);
      return is;
    }

  #endif



    //****************************
    //**      The Box class     **
    //****************************


  /*! \class Box gmpoint.h <gmPoint>
   *  \brief The Static Box class
   *  A template Box, the Box is static i.e. the dimentions
   *  can not be change. The template type must be clean, i.e. is not allocating
   *  memory and without any virtual functions.
   *
   *  The Box is described by a minimum Point and a maximum Point (the diaginal).
   */
  template <typename T, int n>
  class Box {
  public:
    Box();
    Box( const Point<T, n>& p );
    Box( const Box<T, n>& b );
    Box( const Point<T, n>& p1, const Point<T, n>& p2 );
    Box( const Point<T, n>& p1, const Point<T, n>& p2, const Point<T,n>& p3 );

    Point<T, n>       getPointMin() const;
    Point<T, n>       getPointMax() const;
    Point<T, n>       getPointCenter() const;
    Vector<T, n>      getPointDelta() const;
    T*                getPtr() const;

    T&                getValueAt( int i, int j );
    T                 getValueMin( int i ) const;
    T                 getValueMax( int i ) const;
    T                 getValueCenter( int i ) const;
    T                 getValueDelta( int i ) const;
    void              insert( const Point<T, n>& );
    void              insert( const Box<T, n>& );
    bool              isIntersecting( const Box<T,n>& b ) const;
    bool              isSurrounding( const Point<T,n>& p ) const;
    bool              isSurrounding( const Box<T,n>& b )  const;
    void              reset();
    void              reset( const Point<T, n>& p );

    const Box<T,n>&   operator += ( const Point<T, n>& p );
    Box<T,n>          operator +  ( const Point<T, n>& p );
    const Box<T,n>&   operator += ( const Box<T, n>& b );
    Box<T,n>          operator +  ( const Box<T, n>& b );



  private:
    Point<T, n>     _min;
    Point<T, n>     _max;

  }; // END class Box





  #ifdef GM_STREAM

    /*! T_Stream &operator<<(T_Stream& os, const Box<T,n>& b)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template<typename T_Stream, typename T, int n>
    T_Stream &operator<<(T_Stream& os, const Box<T,n>& b) {
      os << b.getPointMin() << GMseparator::element() << b.getPointMax();
      return os;
    }

    /*! T_Stream &operator<<(T_Stream& os, const Box<T,n>* b)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template<typename T_Stream, typename T, int n>
    T_Stream &operator<<(T_Stream& os, const Box<T,n>* b) {
      os << b->getPointMin() << GMseparator::element() << b->getPointMax();
      return os;
    }

    /*! T_Stream &operator>>(T_Stream& is, Box<T,n>& b)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template<typename T_Stream, typename T, int n>
    T_Stream &operator>>(T_Stream& is, Box<T,n>& b) {
      Separator es(GMseparator::element());
      Point<T,n> p1,p2;
      is >> p1 >> es >> p2;
      b.reset(p1);
      b.insert(p2);
      return is;
    }

    /*! T_Stream &operator>>(T_Stream& is, Box<T,n>* b)
     *  \brief Brief Description
     *
     *  Detailed Description
     */
    template<typename T_Stream, typename T, int n>
    T_Stream &operator>>(T_Stream& is, Box<T,n>* b) {
      Separator es(GMseparator::element());
      Point<T,n> p1,p2;
      is >> p1 >> es >> p2;
      b->reset(p1);
      b->insert(p2);
      return is;
    }

  #endif



    //***********************************
    //**      The PlaneArrow class     **
    //***********************************


  /*! \class  PlaneArrow gmpoint.h <gmPoint>
   *
   *  \brief  PlaneArrow class
   *
   *  Detailed Description of class
   */

  template <typename T, int n>
  class PlaneArrow : public Arrow<T,n> {
  public:
    PlaneArrow();
    PlaneArrow(const Point<T,n>& p);
    PlaneArrow(const Point<T,n>& p ,const Vector<T,n>& v);
    PlaneArrow(const Arrow<T,n>& a);

    void                setNormal(const Vector<T,n>& v);

    const Vector<T,n>&  getNormal() const;
    Point<T,n>          getClosestPoint(const Point<T,n>& p) const;
    Vector<T,n>         getDistanceVector(const Point<T,n>& p) const;
    T                   getDistanceTo(const Point<T,n>& p) const;

  }; // END class PlaneArrow




  //*********************************************************
  //********  The init of I-matrix and SubSpace  ************
  //************   NOT FOR EXTERNAL USE !!!!!!   ************
  //*********************************************************
  /*! \class M_I_ gmpoint.h <gmPoint>
   *  \brief The init of I-matrix and SubSpace
   *
   *  NOT FOR EXTERNAL USE !!!!!!
   */
  template <typename T, int n, int m>
  class M_I_ {
  public:
    M_I_();
    const T* getPtr() const;

  private:
    T  _p[n*m];
  }; // END class M_I_

} // END namespace GMlib




// Include implementations
#include "gmpoint.c"



#endif // GM_CORE_TYPES_POINT_H
