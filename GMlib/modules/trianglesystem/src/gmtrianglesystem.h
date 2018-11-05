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






#ifndef GM_TRIANGLESYSTEM_TRIANGLESYSTEM_H
#define GM_TRIANGLESYSTEM_TRIANGLESYSTEM_H



// gmlib
#include <core/containers/gmarray.h>
#include <core/containers/gmarrayt.h>
#include <core/containers/gmarraylx.h>
#include <core/containers/gmdmatrix.h>
#include <scene/gmsceneobject.h>


namespace GMlib {



  template <typename T>
  class TriangleFacetsVisualizer;

  template <typename T>
  class TriangleFacetsDefaultVisualizer;




  template <typename T>
  class TriangleFacets;

  template <typename T>
  class TriangleSystem;

  template <typename T>
  class TSVertex;

  template <typename T>
  class TSEdge;

  template <typename T>
  class TSTriangle;

  template <typename T>
  class TSTile;

  template <typename T>
  class TSLine;

  template <typename T>
  class TSVEdge;


  /** \class  TriangleFacets gmtrianglesystem.h <gmTriangleSystem>
   *  \brief  The storage class of the Triangle system
   *
   *  This is the main class. Here you can make, adjust
   *  and handle a triangle based surface. The class is also
   *  the storage class for the vertices, edges and triangles
   */
  template <typename T>
  class  TriangleFacets : public ArrayLX< TSVertex<T> >, public SceneObject {
    GM_SCENEOBJECT(TriangleFacets)
  public:

    TriangleFacets( int d = 0 );
    TriangleFacets( const ArrayLX<TSVertex<T> >& v);
    ~TriangleFacets();



    Point<T,3>                        eval(T x, T y, int deg=1) const;
    Point<T,3>                        eval(const Point<T,2>& p, int deg=1) const;


    T                                 evalZ(const Point<T,2>&, int deg=1); //const
    T                                 evalZ(T x, T y, int deg=1);

    void                              clear(int d=-1);
    void                              computeNormals();

    void                              createVoronoi();
    Box<T,3>                          getBoundBox() const;

    TSEdge<T>*                        getEdge(int i) const;
    int                               getNoVertices() const;
    int                               getNoEdges() const;
    int                               getNoTriangles() const;

    TSTriangle<T>*                    getTriangle(int i) const;
    TSVertex<T>*                      getVertex(int i) const;

    const Array<TSVEdge<T> >&         getVoronoiEdges() const;
    const Array<Point<T,2> >&         getVoronoiPoints() const;

    void                              insertLine( TSLine<T>& );
    bool                              insertVertex( TSVertex<T>&, bool c = false );
    bool                              removeVertex( TSVertex<T>& v );
    bool                              removeVertexNew( TSVertex<T>& v);

    void                              renderVoronoi();
    void                              replot() const;

    bool                              setConstEdge(TSVertex<T> v1, TSVertex<T> v2);

    void                              triangulateDelaunay();


    void                              enableDefaultVisualizer( bool enable = true );
    void                              insertVisualizer( Visualizer *visualizer );
    void                              removeVisualizer( Visualizer *visualizer );
    void                              toggleDefaultVisualizer();

   protected:
    int	                              _dlist_name;
    GLuint                            _vbo;
    GLuint                            _ibo;

    mutable Array< TriangleFacetsVisualizer<T>*>  _tf_visualizers;
    mutable TriangleFacetsDefaultVisualizer<T>*   _default_visualizer;

  private:
    ArrayLX< TSEdge<T>* >             _edges;
    ArrayLX< TSTriangle<T>* >         _triangles;
    Array< TSTile<T> *>               _tmptiles;
    Array<TSVEdge<T> >                _voredges;
    Array<Point<T,2> >                _vorpnts;

    int                               _d;

    DMatrix<ArrayT<TSTriangle<T>*> >  _tri_order;
    ArrayT<T>                         _u;
    ArrayT<T>                         _v;
    Box<T,3>                          _box;

    TSVertex<T>                       __v;  // dummy because of MS-VC++ compiler
    TSEdge<T>                         __e;  // dummy because of MS-VC++ compiler
    TSTriangle<T>                     __t;  // dummy because of MS-VC++ compiler

    bool                              _fillPolygon(Array<TSEdge<T>*>&);
    bool                              _removeLastVertex();
    void                              _set(int i);
    int                               _surroundingTriangle(TSTriangle<T>*&, const TSVertex<T>&);// const;


  friend class TriangleSystem<T>;
  private:
    void                              _adjustTriangle( TSTriangle<T>*, bool wider = false );
    ArrayLX<TSEdge<T>* >&             _getEdges();
    TSVertex<T>*                      _find( const Point<T,3>& ) const;
    TSEdge<T>*                        _find( const Point<T,3>&, const Point<T,3>& ) const;
    void                              _insertTriangle( TSTriangle<T>* );
    void                              _removeTriangle( TSTriangle<T>* );
    ArrayLX<TSTriangle<T>* >&         _triangle();



  #if defined GM_STREAM


//  public:
//    int  getStreamMode() const;
//    void print( string prompt = " TriangleFacets<T>", std::ostream& out = std::cout ) const;
//    void setStreamMode( int i = 1 );
//
//    friend std::ostream& operator<<(std::ostream& out, const  TriangleFacets<T>& v) {
//      return v._prOut(out);
//    }
//    friend std::ostream& operator<<(std::ostream& out, const  TriangleFacets<T>* v) {
//      return v->_prOut(out);
//    }
//    friend std::istream& operator>>(std::istream& in,  TriangleFacets<T>& v) {
//      return std::istream >> v;
//    }
//    friend std::istream& operator>>(std::istream& in,  TriangleFacets<T>* v) {
//      return v->_prIn(in);
//    }
//
//
//  private:
//    int _stream_mode;
//
//    std::ostream& _prOut(std::ostream& out) const;
//    std::istream& _prIn(std::istream& in);

  #endif
  };



  /** \class TriangleSystem gmtrianglesystem.h <gmTriangleSystem>
   *  \brief The TriangleSystem base class
   *
   *  The base class for vertices, edges and triangles
   */
  template <typename T>
  class TriangleSystem {
  public:
    void                        set( TriangleFacets<T>& ts );

  protected:
    void                        adjust( TSTriangle<T> *t, bool wider = false );
    TSVertex<T>*                find( const Point<T,3>& p ) const;
    TSEdge<T>*                  find( const Point<T,3>& p1, const Point<T,3>& p2 );

//    int                         getStreamMode();

    void                        insert( TSEdge<T> *e );
    void                        insert( TSTriangle<T> *t );
    void                        remove( TSEdge<T> *e );
    void                        remove( TSTriangle<T> *t );


  private:
    static TriangleFacets<T>    *_tv;
  };

  /** \class VEdge
   *  \brief The Voronoi Edge class
   *
   *  Simple class containing the two voronoi points
  */
  template <typename T>
  class TSVEdge {
  public:
    TSVEdge();
    TSVEdge(const Point<T,2> &p, const Point<T,2> &q);


    bool operator==(const TSVEdge<T> &e) const;
    bool operator<(const TSVEdge<T> &e) const;
    Point<T,2> const &operator() (int i) const;
  private:
    Point<T,2> _pnts[2];
  };


  /** \class  TSVertex
   *  \brief  The TSVertex class
   *
   *  The vertex class storing 3D position and a normal
   */
  template <typename T>
  class TSVertex : public Arrow<T,3>, public TriangleSystem<T> {
  public:
    TSVertex();
    TSVertex( const Point<T,2>& v );
    TSVertex( const Point<T,3>& p );
    TSVertex( const Point<T,3>& p, const Vector<T,3>& n );
    TSVertex( const T& x, const T& y, const T& z = T() );
    ~TSVertex();


    bool                    boundary() const;
    Arrow<T,3>              getArrow();
    ArrayT<TSEdge<T>*>&	    getEdges();
    Vector<T,3>             getNormal() const;
    Array<TSEdge<T>*>       getOuterEdges() const;
    Point<T,2>              getParameter() const;
    Point<T,3>              getPosition() const;
    T                       getRadius();
    T                       getRadiusMax();
    T                       getRadiusMin();
    Array<TSTriangle<T>*>   getTriangles() const;

    bool                    isConst()	const;
    int                     isInside( TSTriangle<T>* ) const;

    void                    setConst( bool c = true );
    void                    setRadius( T r );
    void                    setRadiusMax( T r );
    void                    setRadiusMin( T r );
    void                    setZ( T z );

    TSVertex<T>&            operator=(const TSVertex<T>& t);
    bool                    operator==(const TSVertex<T>& t)	const;
    bool                    operator<(const TSVertex<T> &t) const;



  private:

    T                       _radius;
    T                       _maxradius;
    T                       _minradius;
    ArrayT<TSEdge<T>*>      _edges;

    bool                    _const;


    void                    _set( const Point<T,3>& p, const Vector<T,3>& n );


  friend class TSEdge<T>;
  friend class TriangleFacets<T>;
  private:
    void                    _computeNormal();
    void                    _deleteEdges();
    bool                    _insertEdge(TSEdge<T>* e);
    bool                    _removeEdge(TSEdge<T>* e);
    void                    _set( const TSVertex<T>& v );


//  #if defined GM_STREAM
//
//
//  public:
//    friend std::ostream&       operator<<(std::ostream& out, const TSVertex<T>& v) { return v._prOut(out); }
//    friend std::ostream&       operator<<(std::ostream& out, const TSVertex<T>* v) { return v->_prOut(out); }
//    friend std::istream&       operator>>(std::istream& in, TSVertex<T>& v)	{ return v._prIn(in); }
//    friend std::istream&       operator>>(std::istream& in, TSVertex<T>* v)	{ return v->_prIn(in); }
//
//    void                  print(char prompt[]="TSVertex<T>", std::ostream & out = std::cout) const
//    { out << prompt << ": " << (*this) << "\n"; }
//
//  private:
//    std::ostream&              _prOut(std::ostream& out)const;
//    std::istream&              _prIn(std::istream& in);
//
//  #endif

  };



  /** \class TSEdge gmtrianglesystem.h <gmTriangleSystem>
   *  \brief The TSEdge class
   *
   *  The edge class defined by 2 vertices, can be fixed
   */
  template <typename T>
  class TSEdge : public TriangleSystem<T> {
  public:
    TSEdge();
    TSEdge(TSVertex<T>& s, TSVertex<T>& e);
    TSEdge(const TSEdge<T>& e);
    ~TSEdge();

    bool                    boundary() const;
    TSVertex<T>*            getCommonVertex(const TSEdge<T>&) const;
    TSVertex<T>*            getFirstVertex() const;
    Point<T,3>              getCenterPos();
    Point<T,2>              getCenterPos2D();
    T                       getLength();
    T                       getLength2D();
    Array<TSTriangle<T>*>   getTriangle();
    Vector<T,3>             getVector();
    Vector<T,2>             getVector2D();
    TSVertex<T>*            getLastVertex() const;
    TSVertex<T>*            getOtherVertex(const TSVertex<T>&) const;
    void                    setConst(bool c = true);

    bool                    operator <  (const TSEdge<T>& v) const;
    bool                    operator >  (const TSEdge<T>& v) const;
    bool                    operator <= (const TSEdge<T>& v) const;
    bool                    operator >= (const TSEdge<T>& v) const;


  private:
    TSVertex<T>             *_vertex[2];
    TSTriangle<T>           *_triangle[2];
    bool                    _const;

    bool                    _swap();
    void                    _upv();


  friend class TSTriangle<T>;
  friend class TriangleFacets<T>;
  private:

    TSEdge<T>*              _getNext();
    bool                    _hasPoints(const Point<T,3>& , const Point<T,3>&) const;
    bool                    _isFirst(TSVertex<T>* v);
    bool                    _isLast(TSVertex<T>* v);

    void                    _okDelaunay();

    TSTriangle<T>*          _getOther(TSTriangle<T>*);
    bool                    _removeTriangle(TSTriangle<T>*);
    void                    _reverse();
    void                    _setTriangle(TSTriangle<T>* t1,TSTriangle<T>* t2);
    bool                    _split( TSVertex<T>& p );
    bool                    _swapTriangle(TSTriangle<T>* ot, TSTriangle<T>* nt);
    bool                    _swapVertex(TSVertex<T>& is_v,TSVertex<T>& new_v);



//  #if defined GM_STREAM
//  private:
//    std::ostream& _prOut(std::ostream& out)const;
//    std::istream& _prIn(std::istream& in);
//  public:
//
//    friend std::ostream& operator<<(std::ostream& out, const TSEdge<T>& v);
//    friend std::ostream& operator<<(std::ostream& out, const TSEdge<T>* v);
//    friend std::istream& operator>>(std::istream& in, TSEdge<T>& v);
//    friend std::istream& operator>>(std::istream& in, TSEdge<T>* v);
//
//    void print(char prompt[]="TSEdge<T>", std::ostream & out = std::cout) const;
//  #endif
  };



  /** \class  TSTriangle gmtrianglesystem.h <gmTriangleSystem>
   *  \brief  The TriangleSystem Trangle class
   *
   *  The triangle class defined by 3 edges
   */
  template <typename T>
  class TSTriangle: public TriangleSystem<T> {
  public:
    TSTriangle();
    TSTriangle( TSEdge<T>* e1, TSEdge<T>* e2, TSEdge<T>* e3 );
    TSTriangle( const TSTriangle<T>& t );
    ~TSTriangle();


    T                       getAngleLargest();
    T                       getAngleSmallest();
    T                       getArea();
    T                       getArea2D();

    Point<T,3>              getCenterPos();
    Point<T,2>              getCenterPos2D();
    T                       getCircum();
    T                       getCircum2D();

    Array<TSEdge<T>*>       getEdges() const;
    Vector<T,3>             getNormal() const;
    Array<TSVertex<T>*>     getVertices() const;
    int	                    isAround(const TSVertex<T>& v) const;



  private:
    TSEdge<T>              *_edge[3];
    Box<unsigned char,2>    _box;



  friend class TSEdge<T>;
  friend class TriangleFacets<T>;
    Point<T,2>              _vorpnt;
  private:

    T                       _evalZ( const Point<T,2>& p, int deg = 1 ) const;
    Box<unsigned char,2>&   _getBox();
    void                    _render();//  const;
    bool                    _reverse( TSEdge<T>* edge );
    void                    _setEdges( TSEdge<T>* e1, TSEdge<T>* e2, TSEdge<T>* e3 );
    bool                    _split( TSVertex<T>& p );
    void                    _updateBox( ArrayT<T>& u, ArrayT<T>& v, int d );



//  #if defined GM_STREAM
//  public:
//    friend std::ostream& operator << ( std::ostream& out, const TSTriangle<T>& v );
//    friend std::ostream& operator << ( std::ostream& out, const TSTriangle<T>* v );
//    friend std::istream& operator >> ( std::istream& in, TSTriangle<T>& v );
//    friend std::istream& operator >> ( std::istream& in, TSTriangle<T>* v );
//
//    void print( char prompt[] = "TSTriangle<T>", std::ostream & out = std::cout ) const;
//
//  private:
//    std::ostream& _prOut( std::ostream& out )const;
//    std::istream& _prIn( std::istream& in );
//  #endif
  };



  /** \class  TSTile gmtrianglesystem.h <gmTriangleSystem>
   *  \brief  The TSTile class
   *
   *  The tile class defined by vertex, and dual of its triangles
   */
  template <typename T>
  class TSTile : public SceneObject {
    GM_SCENEOBJECT(TSTile)
  public:
    TSTile();
    TSTile( TSVertex<T>* v, Box<T,3> domain );

    T                     getBigR();
    T                     getSmallR();
    void                  render();

  protected:
    void                  localDisplay();
    void                  localSelect();

  private:
    Array< Point<T,2> >   _vorpts;
    TSVertex<T>           *_vertex;
    T                     _inscribed;
    T                     _circumscribed;

    Point<T,2>            _voronoi( const Point<T,2>& v1, const Point<T,2>& v2, const Point<T,2>& v3 );
  };



  /** \class  TSLine gmtrianglesystem.h <gmTriangleSystem>
   *  \brief  The TSLine class
   *
   *  TSLine
   */
  template <typename T>
  class TSLine : public Array< TSVertex<T> > {
  public:
    TSLine( int d = 0);
    TSLine( const Array<TSVertex<T> >& v );

    TSVertex<T>     interpolate( int i, double t ) const;
  };


} // end namespace




// Include implementations
#include "gmtrianglesystem.c"



#endif // GM_TRIANGLESYSTEM_TRIANGLESYSTEM_H


