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



#include "visualizers/gmtrianglefacetsdefaultvisualizer.h"

// stl
#include <cmath>
#include <iostream>


namespace GMlib {

  template <class T>
  TriangleFacets<T>* TriangleSystem<T>::_tv = NULL;

  template <typename T>
  inline
  TriangleFacets<T>::TriangleFacets( int d )
    : ArrayLX<TSVertex<T> >( d > 0 ? d+3 : 0 ), _edges(), _triangles()
  {

    //setStreamMode();
    _dlist_name=0;
    glGenBuffers( 1, &_vbo );
    glGenBuffers( 1, &_ibo );

    _default_visualizer = 0x0;
  }


  template <typename T>
  inline
  TriangleFacets<T>::TriangleFacets( const ArrayLX<TSVertex<T> >& v)
    : ArrayLX<TSVertex<T> >(v.size()+3), _edges(),_triangles()
  {
    this->setSize(v.size());
    for(int i=0; i<v.size(); i++) (*this)[i] = v(i);

    _dlist_name = 0;
    glGenBuffers( 1, &_vbo );
    glGenBuffers( 1, &_ibo );

    _default_visualizer = 0x0;
  }


  template <typename T>
  TriangleFacets<T>::~TriangleFacets() {

    int k=0,r=0,s=0,p=0;
    int i,j;
    for(i=0; i < _tri_order.getDim1(); i++)
    {
      for(j=0; j< _tri_order.getDim2(); j++)
      {
        r += _tri_order[i][j].getMaxSize();
        s += _tri_order[i][j].getSize();
        k += _tri_order[i][j].getMaxSize() - _tri_order[i][j].getSize();
        if(_tri_order[i][j].getMaxSize() > 20) p++;
      }
    }

    int ptbt=8*_tri_order.getDim1()*_tri_order.getDim2()+8 + 8*_tri_order.getDim1();

    std::cout << "antall element strre enn 20: " << p << " Av totalt: "
       << _tri_order.getDim1()*_tri_order.getDim2() << std::endl;

    std::cout << "Total byte pr element i matrise  : "
       << (4*r)/double(_tri_order.getDim1()*_tri_order.getDim2()) << std::endl;

    std::cout << "Total kbyte bruk i matrise  : " << (4*r + ptbt)/1024 << std::endl;
    std::cout << "Minimum kbyte bruk i matrise: " << (4*s + ptbt)/1024 << std::endl;
    std::cout << "For mye kbyte bruk i matrise: " << (4*k)/1024  << std::endl;

    std::cout << "Total byte pr pkt i matrise  : " << (4*r + ptbt)/double(this->getSize()) << std::endl;
    std::cout << "Minimum byte pr pkt i matrise: " << (4*s + ptbt)/double(this->getSize()) << std::endl;
    std::cout << "For mye byte pr pkt i matrise: " << (4*k)/double(this->getSize()) << std::endl;

    for(k=i=0; i< this->getSize(); i++) k += 4+8+4*(*this)[i].getEdges().getMaxSize();
    for(i=0; i< _edges.getSize(); i++) k += 4*5;
    for(i=0; i< _triangles.getSize(); i++) k += 14;


    std::cout << "Totalt brukt memory :" << (4*r+ptbt+k)/1024 << " kbyte\n";


    for(r=k=i=0; i< this->getSize(); i++)
    {
      k += (*this)[i].getEdges().getSize();
      r += (*this)[i].getEdges().getMaxSize();
    }
    std::cout << "Antall edger i snitt i Verticene: " << double(k)/i << std::endl;
    std::cout << "Max edger i snitt i Verticene   : " << double(r)/i << std::endl;

    clear();

    glDeleteBuffers( 1, &_vbo );
    glDeleteBuffers( 1, &_ibo );

    enableDefaultVisualizer( false );
    if( _default_visualizer )
      delete _default_visualizer;
  }


  template <typename T>
  void TriangleFacets<T>::_adjustTriangle( TSTriangle<T>* t, bool wider ) {

    int i,j;
    Box<unsigned char,2> b	= t->_getBox();
    t->_updateBox( _u, _v, _d );
    Box<unsigned char,2> a	= t->_getBox();

    for( i = b.getValueAt(0,0); i <= b.getValueAt(1,0); i++ )
      for( j = b.getValueAt(0,1); j <= b.getValueAt(1,1); j++ )
        if( i < a.getValueAt(0,0) || i > a.getValueAt(1,0) )
          _tri_order[i][j].remove(t);
        else if( j < a.getValueAt(0,1) || j > a.getValueAt(1,1) )
          _tri_order[i][j].remove(t);

    if(wider) {
      for(i=a.getValueAt(0,0); i<=a.getValueAt(1,0); i++)
        for(j=a.getValueAt(0,1); j<=a.getValueAt(1,1); j++)
          if( i < b.getValueAt(0,0) || i > b.getValueAt(1,0) )
            _tri_order[i][j] += t;
          else if( j < b.getValueAt(0,1) || j > b.getValueAt(1,1) )
            _tri_order[i][j] += t;
    }
  }


  template <typename T>
  inline
  ArrayLX<TSEdge<T>* >& TriangleFacets<T>::_getEdges() {

    return _edges;
  }


  template <typename T>
  bool TriangleFacets<T>::_fillPolygon( Array<TSEdge<T>*>& e ) {

    int i, j;


    for(i=0; i< e.getSize(); i++)
    {
      TSTriangle<T>* t = e[i]->other(NULL);
      t->_reverse(e[i]);
      Array<TSEdge<T>*> edg = t->getEdges();
      if(e[i]->isFirst(e[i]->getCommonVertex(*edg[1])))
          e[i]->reverse();
    }

    for(i=0; i< e.getSize()-1; i++)
      for(j=i+1; j< e.getSize(); j++)
        if(e[i]->isLast(e[j]->getFirstVertex())) e.swap(i+1,j);


    TSEdge<T> *a = e[0];
    TSEdge<T> *b = e[1];
    int k0,k1,k2;

    while(e.getSize() > 3)
    {
      // Angle ang;
      Array<Angle> ang;
      Array<int> index;

      for(i=0 ;i<e.getSize(); i++)
      {
        a = i==0? e[e.getSize()-1] : e[i-1];
        b = e[i];
        ang += (b->getVector2D()).getAngle(-(a->getVector2D()));
      }

      ang.sort();
      index=ang.getSortedIndex();

      for(i=index.getSize()-1; i>=0; i--)
      {
        k1 = index[i];
        k0 = k1==0 ? e.getSize()-1 : k1-1;
        k2 = k1==e.getSize()-1 ? 0 : k1+1;
        for (j=0;  j< e.getSize(); j++)
        {
          if(j==k0 || j==k1 || j==k2) continue;

          if(e[j]->getFirstVertex()->getParameter().isInside(e[k0]->getFirstVertex()->getParameter(),
                                 e[k1]->getFirstVertex()->getParameter(),
                                 e[k2]->getFirstVertex()->getParameter()))
                                 break;
        }
        if(j==e.getSize()) break;
      }

      if(i<0) ;//error
      else
      {
        a = e[k0];
        b = e[k1];

        TSEdge<T> *ne = new TSEdge<T>(*(a->getFirstVertex()),*(b->getLastVertex()));
        TSTriangle<T> *nt = new TSTriangle<T>(ne,b,a);
        a->swapTriangle(NULL,nt);
        b->swapTriangle(NULL,nt);
        ne->setTriangle(nt,NULL);
        _edges += ne;
        insertTriangle(nt);
        e[k0] = ne;
        e.remove(b);
      }
    }

    TSTriangle<T> *nt = new TSTriangle<T>(e[2],e[1],e[0]);
    e[0]->swapTriangle(NULL,nt);
    e[1]->swapTriangle(NULL,nt);
    e[2]->swapTriangle(NULL,nt);
    insertTriangle(nt);

    return 1;
  }


  template <typename T>
  TSVertex<T>*  TriangleFacets<T>::_find( const Point<T,3>& p ) const {

    int i;
    for( i = 0; i < this->getSize(); i++ )
      if( this->getElement(i).getPosition() == p) break;

    if (i < this->getSize()) return &( this->getElement(i));
    else			return NULL;
  }


  template <typename T>
  TSEdge<T>*   TriangleFacets<T>::_find( const Point<T,3>& p1, const Point<T,3>& p2 ) const {

    int i;
    for(i=0; i<_edges.getSize(); i++)
      if( _edges(i)->hasPoints( p1, p2 ) ) break;

    if (i < _edges.getSize()) return _edges(i);
    else			return NULL;
  }


  template <typename T>
  void TriangleFacets<T>::_insertTriangle( TSTriangle<T>* t ) {

    _triangles += t;

    t->_updateBox( _u, _v, _d );

    Box<unsigned char,2> b	= t->_getBox();

    for( int i = b.getValueAt(0,0); i <= b.getValueAt(1,0); i++ )
      for( int j = b.getValueAt(0,1); j <= b.getValueAt(1,1); j++ )
        _tri_order[i][j] += t;
  }


  template <typename T>
  inline
  bool TriangleFacets<T>::_removeLastVertex() {

    return removeVertex((*this)[this->getSize()-1]);
  }


  template <typename T>
  void TriangleFacets<T>::_removeTriangle( TSTriangle<T>* t ) {

    _triangles.remove(t);

    Box<unsigned char,2> b	= t->_getBox();

    for( int i = b.getValueAt(0,0); i <= b.getValueAt(1,0); i++ )
      for( int j = b.getValueAt(0,1); j <= b.getValueAt(1,1); j++ )
        _tri_order[i][j].remove(t);
  }


  /** void TriangleFacets<T>::_set( int i )
   *  \brief SET -COMPUTE THE TRIANGLE SYSTEM FOR THE VERTEX
   *
   *  Go through all vertices and if a vertex is on
   *  an edge split the edge and the two neighbour
   *  triangles, or if the vertex is inside one triangle
   *  split the triangle into three treangles
   */
  template <typename T>
  void TriangleFacets<T>::_set( int i ) {

    TSTriangle<T>* t;
    int k = _surroundingTriangle(t, (*this)[i]);

    if (k < 0)
      t->getEdges()[-(k+1)]->_split((*this)[i]);		// Split an edge
    else if (k>0)
      t->_split((*this)[i]);						// Split a triangle
    else
    {
        // Error vertex outside system or?
    }
  }


  template <typename T>
  int  TriangleFacets<T>::_surroundingTriangle( TSTriangle<T>*& t, const TSVertex<T>& v ) {

    int k,s,n = 1 << _d;
    Point<T,2>	nb = v.getParameter();

    int i=0,j=0;
    int it;
    for(it=1; it <= _d; it++)
    {
      s = n >> it;
      k = i + s;
      if ( _u(k) < nb[0]) i = k;
      k = j + s;
      if ( _v(k) < nb[1]) j = k;
    }

    k=0;
    for (it=_tri_order[i][j].getSize()-1; it>=0; it--)
      if( ( k = v.isInside( _tri_order[i][j](it) ) ) ) break;

    t = (it >= 0 ? _tri_order[i][j](it) : NULL);

    return k;
  }


  template <typename T>
  inline
  ArrayLX<TSTriangle<T>* >&	TriangleFacets<T>::_triangle()	{

    return _triangle;
  }


  template <typename T>
  inline
  Point<T,3> TriangleFacets<T>::eval( T x, T y, int deg ) const {

    return Point<T,3>( x, y, evalZ( x, y, deg ) );
  }


  template <typename T>
  inline
  Point<T,3> TriangleFacets<T>::eval( const Point<T,2>& p, int deg ) const {

    return eval( p(0), p(1), deg );
  }


  template <typename T>
  T TriangleFacets<T>::evalZ( const Point<T,2>& p, int deg ) {

    T z = 0;
    TSTriangle<T>* t;
    int idx = _surroundingTriangle( t, TSVertex<T>(p) );

    if( idx )
      z=t->evalZ( p, deg );

    return z;
  }


  template <typename T>
  inline
  T TriangleFacets<T>::evalZ( T x, T y, int deg ) {

    return evalZ( Point<T,2>( x, y ), deg );
  }


  template <typename T>
  void TriangleFacets<T>::clear( int d ) {

    __e.set(*this);

    while( _triangles.getSize() > 0 )
      delete _triangles[0];

    while( _edges.getSize() > 0 )
      delete _edges[0];

    _triangles.clear();
    _edges.clear();

  _vorpnts.clear();
  _voredges.clear();

    ArrayLX<TSVertex<T>>::clear();

    if (d >= 0)
      _d = d;

  if (_dlist_name) {
    glDeleteLists(_dlist_name,1);
    _dlist_name = 0;
  }
  }


  template <typename T>
  void TriangleFacets<T>::computeNormals() {

    for( int i = 0; i < this->getSize(); i++ )
      (*this)[i]._computeNormal();
  }


  template <typename T>
  inline
  void TriangleFacets<T>::createVoronoi() {

    for (int i=0; i<_triangles.size(); i++) {
      Point<T,2> c;

      Array<TSVertex<T> *> v = _triangles(i)->getVertices();

      Point<T,2> p1 = Point<T,2>(v(0)->getPosition());
      Point<T,2> p2 = Point<T,2>(v(1)->getPosition());
      Point<T,2> p3 = Point<T,2>(v(2)->getPosition());

      T b1 = p1*p1;
      T b2 = p2*p2;
      T b3 = p3*p3;

      Point<T,2> b(b2-b1,b3-b2);
      Point<T,2> a1 = p2 - p1;
      Point<T,2> a2 = p3 - p2;

          c = (0.5/(a1^a2))*
            Point<T,2>(Point<T,2>(a2[1],-a1[1])*b,Point<T,2>(-a2[0],a1[0])*b);
      _triangles(i)->_vorpnt = c;
    }

    int k = 0;
    for (int i=0; i<_triangles.size(); i++) {
      for (int j=0; j<_triangles(i)->getEdges().size(); j++) {
        Array<TSTriangle<T> *> tt = _triangles(i)->getEdges()(j)->getTriangle();

        if (tt.size() > 1) {
          if (tt[0] == _triangles[i]) k=1; else k=0;
          //int m = _triangles.getIndex(tt[k]);
         // std::cout << "triangle found: " << m << "--" << i << std::endl;
          //if (m!=-1)

          //_voredges.insert(TSVEdge<T>(_vorpnts(i),_vorpnts(m)));
          _voredges.insert(TSVEdge<T>(_triangles(i)->_vorpnt,tt[k]->_vorpnt));
        } else {
          // insert a voronoi edge splitting boundary edge
          /*
          Point<T,2> p = _triangles(i)->getEdges()(j)->getCenterPos2D();
          T length = max(T(1),T(2)*(p-_vorpnts(i)).getLength());
        //  int res = _triangles(i)->isAround(new TSVertex<T>(_vorpnts(i)));
          if ((_vorpnts(i)-p).getLength() > T(0)) {
            p = (p-_vorpnts(i))*length+_vorpnts(i);
         // else p = length*(_vorpnts(i)-_triangles(i)->getCenterPos2D())+_vorpnts(i);
            _voredges.insert(TSVEdge<T>(_vorpnts(i),p));
          }
          */
        }
      }
    }

    // Ikke fungerende versjon
    //std::cout << "tiles: " << _tmptiles.size() << std::endl;
    //_tmptiles.setSize(this->size());
    //Box<T,3> domain=getBoundBox();
    //for (int i=0; i<size(); i++)
     // //_triangles[i]->getCenterPos();
     // _tmptiles[i] = new TSTile<float> (&(*this)[i],domain);
  }


  template <typename T>
  inline
  Box<T,3> TriangleFacets<T>::getBoundBox() const	{

    return _box;
  }


  template <typename T>
  inline
  TSEdge<T>* TriangleFacets<T>::getEdge( int i )	const	{

    return _edges(i);
  }


  template <typename T>
  inline
  int TriangleFacets<T>::getNoVertices()	const {

    return this->getSize();
  }


  template <typename T>
  inline
  int TriangleFacets<T>::getNoEdges() const {

    return _edges.getSize();
  }


  template <typename T>
  inline
  int TriangleFacets<T>::getNoTriangles() const {

    return _triangles.getSize();
  }


  template <typename T>
  inline
  TSTriangle<T>* TriangleFacets<T>::getTriangle( int i )	const {

    return _triangles(i);
  }


  template <typename T>
  inline
  TSVertex<T>* TriangleFacets<T>::getVertex( int i )	const	{

    return &this->getElement(i);
  }


  template <typename T>
  const Array<TSVEdge<T> >& TriangleFacets<T>::getVoronoiEdges() const {

    return _voredges;
  }


  template <typename T>
  const Array<Point<T,2> >& TriangleFacets<T>::getVoronoiPoints() const {

    return _vorpnts;
  }



  template <typename T>
  void TriangleFacets<T>::insertLine( TSLine<T>& pwl ) {

    __e.set(*this);

    bool next;
    int i,j,k,m;
    T s,t,tt;

    for(i=0; i<pwl.getSize()-1; i++)
    {
      insertVertex(pwl[i]);
      j = index(pwl[i]);
      Point<T,2> p  = (*this)[j].getParameter();		// current point
      Point<T,2> np = pwl[i+1].getParameter();		// next new point

      ArrayT<TSEdge<T>*> edges = (*this)[j].getEdges();

      Array<Point<T,2> > pt;						// Find all neighbour points
      for(k=0; k<edges.getSize(); k++)
        pt += edges[k]->otherVertex((*this)[j])->getParameter();
      p.setTestType(3,p,Point<T,2>(0,1));
      pt.sort();									// All points sorted in a circle
      tt = 0;

      do
      {
        UnitVector<T,2>  v;
        Vector<T,2>      v1 = np - p;

        for(k=0; k<pt.getSize(); k++)				// If the next point is equal one of
          if(np == pt[k]) break;				// the neighbour points continue to
        if(k<pt.getSize()) break;					// the top and insert the new point.

        for(k=0; k<pt.getSize(); k++)				// If the next point is on one of
        {										// the neighbour edges continue to
          m = (k == (pt.getSize()-1))? 0:k+1;	// the top and insert the new point.
          v  = pt[m] - p[k];
          if(std::fabs(v^(np-pt[k])) < POS_TOLERANCE ) break;
        }
        if(k<pt.getSize()) break;

        next = true;

        for(k=0; k<pt.getSize(); k++)
        {
          m = (k == (pt.getSize()-1))? 0:k+1;
          t = 2;

          Vector<T,2> v2 = pt[k] - pt[m];
          Vector<T,2> v3 = pt[k] - p;

          T det = v1^v2;
          if (std::fabs(det) < 1e-15) continue;

          det = 1/det;

          t = -det*(v2.getNormal()*v3);
          s = det*(v1.getNormal()*v3);

          if (t>0 && s>=0 && s<=1)
          {
            v = v2;
            break;
          }
        }
        if (t<1)
          if((v^(np-pt[m])) > POS_TOLERANCE)	// outside all triangles
          {
            tt = tt +(1-tt)*t;
            TSVertex<T> vt = pwl.interpolate(i,tt);///FEILLLL
            vt.setConst();
            insertVertex(vt);
            j = this->getIndex(vt);
            p  = (*this)[j].getParameter();

            edges = (*this)[j].getEdges();
            pt.clear();						// Find all neighbour points
            for(k=0; k<edges.getSize(); k++)
              pt += edges[k]->otherVertex((*this)[j])->getParameter();
            p.setTestType(3,p,Point<T,2>(0,1));
            pt.sort();						// All points sorted in a circle

            next = false;
          }

      }while(!next);
    }
    insertVertex(pwl[pwl.getSize()-1]);
  }



  template <typename T>
  bool TriangleFacets<T>::insertVertex( TSVertex<T>& v, bool c ) {

    __e.set( *this );

    bool inserted = true;

    int i = this->getIndex( v );

    if (i<0) {

      insertAlways(v);
      i = this->getSize()-1;
    }
    else
      inserted = false;

    if (!inserted)
    {
      (*this)[i]._set(v);			// Vertex is already a vertex
      return inserted;
    }


    if (c)
      (*this)[i].setConst();

    _set(i);

    return inserted;
  }



  template <typename T>
  bool TriangleFacets<T>::removeVertex( TSVertex<T>& v ) {

    __e.set(*this);

    int id = this->getIndex(v);
    if( id < 0 ) return false;

    v._deleteEdges();

    ArrayT<TSEdge<T>*> edges = (*this)[this->getSize()-1].getEdges();
    for ( int i=0; i < edges.getSize(); i++ )
      edges[i]->_swapVertex((*this)[this->getSize() - 1],(*this)[id]);

    return this->removeIndex(id);
  }


  template <typename T>
  bool TriangleFacets<T>::removeVertexNew( TSVertex<T>& v ) {

    Array<TSEdge<T>*>   o_edges = v.getOuterEdges();

    removeVertex(v);

    return fillPolygon( o_edges );
  }


 // #ifdef __gmOPENGL_H__

  template <typename T>
  inline
  void TriangleFacets<T>::renderVoronoi() {

  // /*
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glColor3f(1.0f,0.0f,0.0f);
    for (int j=0; j<_vorpnts.size(); j++) {
      Point<T,2> c = _vorpnts(j);
      glVertex2f(c[0],c[1]);
    }
    glEnd();
    //glDisable(GL_POINT_SMOOTH);
    //*/

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i=0; i<_voredges.size(); i++) { // i+=2) {
      glColor3f(1.0f,0.0f,0.0f);
      glVertex2f(_voredges(i)(0)(0), _voredges(i)(0)(1));
      glVertex2f(_voredges(i)(1)(0), _voredges(i)(1)(1));
    }
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
  /*  for (int k=0; k<_vec.size(); k++) {
      glVertex3f(0.0f,0.0f,0.0f);
      glVertex3f(_vec(k)(0),_vec(k)(1),0.0f);

    }*/
    glEnd();

    /*
    for (int h=0; h<_triangles.size(); h++) {
      Point<float,2> p;
      p[0] = _triangles(h)->getVertices()(0)->getPosition()(0);
      p[1] = _triangles(h)->getVertices()(0)->getPosition()(1);
      float r = (_vorpnts(h) - p).getLength();
      double a; int n = 36;
      glBegin(GL_LINE_LOOP);
        glColor3f(0.0f,0.0f,1.0f);
        for (int s=0;s<n;s++) {
          a = ((double)s*2.0*3.14)/((double)n);
          float x = r * cos(a) + _vorpnts(h)(0);
          float y = r * sin(a) + _vorpnts(h)(1);
          glVertex2f(x,y);
        }
      glEnd();
    }
    */




  //  glEnable(GL_LIGHTING);
    // rendring av ikke fungerende versjon
  /*	for (int i=0;i<_tmptiles.size();i++)
      _tmptiles[i]->render();*/
  }

  template <typename T>
  void TriangleFacets<T>::replot() const {
    Sphere<float,3> s( getVertex(0)->getPos() );
    for( int j = 1; j < this->getSize(); j++ )
      s+= getVertex(j)->getPos();
    setSurroundingSphere(s);

    // Replot Visaulizers
    for( int i = 0; i < this->_tf_visualizers.getSize(); i++ )
      _tf_visualizers(i)->replot(this);
  }

 // #endif


  template <typename T>
  bool TriangleFacets<T>::setConstEdge( TSVertex<T> v1, TSVertex<T> v2 ) {

    int i;

    for( i=0; i < getNoEdges(); i++ ) {
      if(
        (*(getEdge(i)->_vertex[0]) == v1 && (*getEdge(i)->_vertex[1]) == v2) ||
        (*(getEdge(i)->_vertex[0]) == v2 && (*getEdge(i)->_vertex[1]) == v1)
      ) {

        getEdge(i)->setConst();
        return true;
      }
    }

    return false;
  }


  template <typename T>
  void TriangleFacets<T>::triangulateDelaunay() {

    __e.set( *this );

    int i,j;

    if( this->getSize() < 3 ) return;

    ArrayLX<TSVertex<T> >& vertex = *this;

    // Set the size of edge and triangle array to the upper bound.
    // Remember that there is only 3 outer edges.

    _edges.setMaxSize(3* this->getSize()+3);
    _triangles.setMaxSize(2* this->getSize()+1);

    // Make surrounding box and tree new vertices

    _box.reset(vertex[0].getPosition());

    for (i=1; i<vertex.getSize(); i++)
      _box += vertex[i].getPosition();

    double dx	  = _box.getValueDelta(0);
    double dy	  = _box.getValueDelta(1);
    double delta  = dx>dy?dx:dy;

    TSVertex<T> p1(_box.getValueMin(0) - delta,   _box.getValueMin(1) - delta);
    TSVertex<T> p2(_box.getValueMax(0) + 3*delta, _box.getValueMin(1) - delta);
    TSVertex<T> p3(_box.getValueMin(0) - delta,   _box.getValueMax(1) + 3*delta);

    vertex += p1;
    vertex += p2;
    vertex += p3;

    // Make tree Edges.

    _edges += new TSEdge<T>(vertex[vertex.getSize()-3], vertex[vertex.getSize()-2]);
    _edges += new TSEdge<T>(vertex[vertex.getSize()-2], vertex[vertex.getSize()-1]);
    _edges += new TSEdge<T>(vertex[vertex.getSize()-1], vertex[vertex.getSize()-3]);

    // Make a triangle.

    _triangles += new TSTriangle<T>(_edges[0],_edges[1],_edges[2]);

    _edges[0]->_setTriangle(_triangles[0],NULL);
    _edges[1]->_setTriangle(_triangles[0],NULL);
    _edges[2]->_setTriangle(_triangles[0],NULL);

    //**********************************************************************
    // Here we constuct the dervided structure for speeding up the algoritm
    //**********************************************************************

    if(this->getSize() < 200)         _d = 2;
    else if(this->getSize() < 800)    _d = 3;
    else if(this->getSize() < 3200)   _d = 4;
    else if(this->getSize() < 12800)  _d = 5;
    else if(this->getSize() < 51200)  _d = 6;
    else if(this->getSize() < 204800) _d = 7;
    else                              _d = 8;

    int n = 1 << _d;

    _tri_order.setDim(n,n);
    _u.setMaxSize(n+1);
    _v.setMaxSize(n+1);

    for(i=0; i<= n; i++)
    {
      _u += _box.getValueMin(0) + i*_box.getValueDelta(0)/n;
      _v += _box.getValueMin(1) + i*_box.getValueDelta(1)/n;
    }

    for(i=0; i< n; i++)
      for(j=0; j< n; j++)
      {
        _tri_order[i][j].setMaxSize(20);//,10);
        _tri_order[i][j] += _triangles[0];
      }

    //*****************************************************
    // End dervided structure for speeding up the algoritm
    //*****************************************************


    // Go through all vertices and if a vertex is on an edge split the edge
    // and the two neighbour triangles, or if the vertex is inside one triangle
    // split the triangle into tree treangles.

    for ( i = 0; i < vertex.getSize()-3; i++ ) _set(i);

    // remove constructed outer triangles

    _removeLastVertex();
    _removeLastVertex();
    _removeLastVertex();

    // Insert Edges and Triangles to make a convecs boundary

    int n_bound=0;
    for(i=0; i< _edges.getSize(); i++)
      if(_edges[i]->boundary())
      {
        n_bound++;
        TSTriangle<T>* t = _edges[i]->_getOther(NULL);
        if (t) {
          t->_reverse(_edges[i]);
          Array<TSEdge<T>*> edges = t->getEdges();
          if( _edges[i]->_isFirst( _edges[i]->getCommonVertex(*edges[1]) ) )
            _edges[i]->_reverse();
        }
      }

    for(i=0; i< _edges.getSize(); i++)
      if( _edges[i]->boundary() )
        break;

    TSEdge<T> *a = _edges[i];
    TSEdge<T> *b = a->_getNext();


    for(i=0; i<n_bound;i++)
    {
      Vector<T,2>		v = a->getVector2D();
      UnitVector<T,2>	u = v + b->getVector2D();
      if((u^v) > POS_TOLERANCE) {

        TSEdge<T> *ne = new TSEdge<T>(*(a->getFirstVertex()),*(b->getLastVertex()));
        TSTriangle<T> *nt = new TSTriangle<T>(ne,b,a);
        a->_swapTriangle(NULL,nt);
        b->_swapTriangle(NULL,nt);
        ne->_setTriangle(nt,NULL);
        _edges += ne;
        _insertTriangle(nt);
        a = ne;
        b = a->_getNext();
        i--;
      }
      else
      {
        a = b;
        b = a->_getNext();
      }
    }
  }







  //#if defined GM_STREAM
  //
  //
  //
  //template <typename T>
  //inline
  //int TriangleFacets<T>::getStreamMode() const {
  //
  //  return _stream_mode;
  //}
  //
  //
  //template <typename T>
  //inline
  //void TriangleFacets<T>::setStreamMode( int i = 1 ) {
  //
  //  _stream_mode = i;
  //}
  //
  //
  //template <typename T>
  //inline
  //void TriangleFacets<T>::print( string prompt = " TriangleFacets<T>", std::ostream& out = cout ) const {
  //
  //  out << prompt << ": " << (*this) << "\n";
  //}
  //
  //
  //template <typename T>
  //std::ostream& TriangleFacets<T>::_prOut(std::ostream& out) const {
  //
  //	const ArrayLX< TSVertex<T> >& vertex = *this;
  //	if( _stream_mode == 1 || _stream_mode == 4 || _stream_mode == 5 || _stream_mode == 7 )
  //		out << vertex << endl << endl;
  //	if( _stream_mode == 2 || _stream_mode == 4 || _stream_mode == 6 || _stream_mode == 7 )
  //		out << _edge << endl;
  //	if( _stream_mode == 3 || _stream_mode == 5 || _stream_mode == 6 || _stream_mode == 7 )
  //		out << _triangle << endl;
  //	return out;
  //}
  //
  //
  //template <typename T>
  //std::istream& TriangleFacets<T>::_prIn(std::istream& in) {
  //
  //	__e.set(*this);
  //
  //	ArrayLX<TSVertex<T> >& vertex = *this;
  //
  //	if( _stream_mode == 1 || _stream_mode == 4 || _stream_mode == 7 ) {
  //
  //		int n , mark = in.tellg();
  //		in >> n;
  //		in.seekg(mark);
  //		vertex.setMax(n+3);
  //		in >> vertex;
  //	}
  //	if( _stream_mode == 4 || _stream_mode == 7 )
  //		in >> _edge;
  //	if( _stream_mode == 7 )
  //		in >> _triangle;
  //	if( _stream_mode == 2 || _stream_mode == 3 || _stream_mode == 5 || _stream_mode == 6 )
  //	{
  //		// Error message
  //	}
  //	return in;
  //}
  //
  //
  //
  //#endif

  template <typename T>
  void TriangleFacets<T>::enableDefaultVisualizer( bool enable ) {

    if( !enable )
      removeVisualizer( _default_visualizer );
    else {

      if(!_default_visualizer) _default_visualizer = new TriangleFacetsDefaultVisualizer<T>();
      insertVisualizer( _default_visualizer );
    }
  }

  template <typename T>
  void TriangleFacets<T>::insertVisualizer( Visualizer *visualizer ) {

    SceneObject::insertVisualizer( visualizer );

    TriangleFacetsVisualizer<T> *visu = dynamic_cast<TriangleFacetsVisualizer<T>*>( visualizer );
    if( !visu )
      return;

    if( _tf_visualizers.exist( visu ) )
      return;

    _tf_visualizers += visu;
  }


  template <typename T>
  void TriangleFacets<T>::removeVisualizer( Visualizer *visualizer ) {

    TriangleFacetsVisualizer<T> *visu = dynamic_cast<TriangleFacetsVisualizer<T>*>( visualizer );
    if( visu )
      _tf_visualizers.remove( visu );

    SceneObject::removeVisualizer( visu );
  }

  template <typename T>
  void TriangleFacets<T>::toggleDefaultVisualizer() {

    if( !_tf_visualizers.exist( _default_visualizer ) )
      enableDefaultVisualizer( true );
    else
      enableDefaultVisualizer( false );
  }

  template <typename T>
  inline
  void TriangleSystem<T>::adjust( TSTriangle<T> *t, bool wider )	{

    _tv->_adjustTriangle(t,wider);
  }


  template <typename T>
  inline
  TSVertex<T>* TriangleSystem<T>::find( const Point<T,3>& p) const {

    return _tv->_find(p);
  }


  template <typename T>
  inline
  TSEdge<T>* TriangleSystem<T>::find( const Point<T,3>& p1,const Point<T,3>& p2){

    return _tv->_find(p1,p2);
  }


  //template <typename T>
  //inline
  //int TriangleSystem<T>::getStreamMode() {
  //
  //  return _tv->getStreamMode();
  //}


  template <typename T>
  inline
  void TriangleSystem<T>::insert( TSEdge<T> *e ) {

    (_tv->_getEdges()) += e;
  }


  template <typename T>
  inline
  void TriangleSystem<T>::insert( TSTriangle<T> *t) {

    _tv->_insertTriangle(t);
  }


  template <typename T>
  inline
  void TriangleSystem<T>::remove( TSEdge<T> *e) {

    (_tv->_getEdges()).remove(e);
  }


  template <typename T>
  inline
  void TriangleSystem<T>::remove( TSTriangle<T> *t) {

    _tv->_removeTriangle(t);
  }


  template <typename T>
  inline
  void TriangleSystem<T>::set( TriangleFacets<T>& ts ) {

    _tv = &ts;
  }

  template <typename T>
  inline
  TSVertex<T>::TSVertex() : Arrow<T,3>(), _edges() {

    _const = false;
    _maxradius = _radius = 0.0;
  }


  template <typename T>
  inline
  TSVertex<T>::TSVertex( const Point<T,2>& v ) : Arrow<T,3>( Point<T,3>( v(0), v(1), 0 ) ) {

    _const = false;
    _maxradius = _radius = 0.0;
  }


  template <typename T>
  inline
  TSVertex<T>::TSVertex( const Point<T,3>& p ) : Arrow<T,3>( p ), _edges() {

    _const = false;
    _maxradius = _radius = 0.0;
  }


  template <typename T>
  inline
  TSVertex<T>::TSVertex( const Point<T,3>& p, const Vector<T,3>& n ) : Arrow<T,3>( p, n ), _edges() {

    _const = false;
    _maxradius = _radius = 0.0;
  }


  template <typename T>
  inline
  TSVertex<T>::TSVertex( const T& x, const T& y, const T& z ) : Arrow<T,3>( Point<T,3>( x, y, z ) ), _edges() {

    _const = false;
    _maxradius = _radius = 0.0;
  }


  template <typename T>
  TSVertex<T>::~TSVertex() {

    _edges.clear();
  }


  template <typename T>
  void TSVertex<T>::_computeNormal() {

    Array<TSTriangle<T>*> tris = getTriangles();

    Vector<T,3> nor(T(0));
    for( int i = 0; i < tris.getSize(); i++ )
      nor += tris[i]->getNormal();
    nor /= float(tris.getSize());

    this->setDir( Vector<T,3>(nor) );
  }


  template <typename T>
  void TSVertex<T>::_deleteEdges() {

    while( _edges.getSize() > 0 )
      delete _edges[0];

    _edges.clear();
  }


  template <typename T>
  inline
  bool TSVertex<T>::_insertEdge(TSEdge<T>* e)	{

    return _edges.insert( e );
  }


  template <typename T>
  inline
  bool TSVertex<T>::_removeEdge(TSEdge<T>* e)	{

    return _edges.remove( e );
  }


  template <typename T>
  inline
  void TSVertex<T>::_set( const Point<T,3>& p, const Vector<T,3>& n ) {

    this->setPos(p);
    this->setDir(n);
  }


  template <typename T>
  inline
  void TSVertex<T>::_set( const TSVertex<T>& v ) {

    _set( v.getPosition(), v.getNormal() );
    _const = v._const;
  }


  template <typename T>
  inline
  bool TSVertex<T>::boundary() const {

    for( int i = 0; i < _edges.getSize(); i++ )
      if( _edges(i)->boundary() )
        return true;

    return false;
  }


  template <typename T>
  inline
  Arrow<T,3> TSVertex<T>::getArrow() {

    Arrow<T,3> r( this->getPos(), this->getDir() );
    return r;
  }


  template <typename T>
  inline
  ArrayT<TSEdge<T>*>&	TSVertex<T>::getEdges() {

    return _edges;
  }


  template <typename T>
  inline
  Vector<T,3> TSVertex<T>::getNormal() const {

    return this->getDir();
  }


  template <typename T>
  Array<TSEdge<T>*> TSVertex<T>::getOuterEdges() const {

    Array<TSTriangle<T>*> tris  = getTriangles();
    Array<TSEdge<T>*>     s_edg = getEdges();
    Array<TSEdge<T>*>     o_edg( s_edg.getSize() );

    int i, j;
    for( i = 0; i < tris.getSize(); i++ ) {

      Array<TSEdge<T>*>	ee = tris[i]->getEdges();
      for( j = 0; j < ee.getSize(); j++ )
        if( !s_edg.exist( ee[j] ) )
          o_edg += ee[j];
    }

    return o_edg;
  }


  template <typename T>
  inline
  Point<T,2> TSVertex<T>::getParameter() const {

    return Point<T,2>( this->getPos() );
  }


  template <typename T>
  inline
  Point<T,3> TSVertex<T>::getPosition() const	{

    return this->getPos();
  }


  template <typename T>
  inline
  T TSVertex<T>::getRadius() {

    return _radius;
  }


  template <typename T>
  inline
  T TSVertex<T>::getRadiusMax() {

    return _maxradius;
  }


  template <typename T>
  inline
  T TSVertex<T>::getRadiusMin() {

    return _minradius;
  }


  template <typename T>
  Array<TSTriangle<T>*> TSVertex<T>::getTriangles() const {

    Array<TSTriangle<T>*> tris;
    for( int i = 0; i < _edges.getSize(); i++ )
      tris.insert( _edges(i)->getTriangle() );

    return tris;
  }


  template <typename T>
  inline
  bool TSVertex<T>::isConst() const {

    return _const;
  }


  template <typename T>
  int  TSVertex<T>::isInside( TSTriangle<T>* t ) const {

    Array<TSVertex<T>*> v = t->getVertices();
    Array<Point<T,2> >  a;

    for( int i = 0; i < 3; i++ )
      a += v[i]->getParameter();

    Point<T,2> p = this->getPos();

    return p.isInside( a );
  }


  template <typename T>
  inline
  void TSVertex<T>::setConst( bool c ) {

    _const = c;
  }


  template <typename T>
  inline
  void TSVertex<T>::setRadius( T r ) {

    _radius = r;
  }


  template <typename T>
  inline
  void TSVertex<T>::setRadiusMax( T r ) {

    _maxradius = r;
  }


  template <typename T>
  inline
  void TSVertex<T>::setRadiusMin( T r ) {

    _minradius = r;
  }


  template <typename T>
  void TSVertex<T>::setZ( T z ) {

    this->setPos( Point<T,3>( this->getPosition()[0], this->getPosition()[1], z ) );
  }


  template <typename T>
  TSVertex<T>& TSVertex<T>::operator = ( const TSVertex<T>& t ) {

    _edges = t._edges;
    _const = t._const;

    _set( t.getPosition(), t.getNormal() );

    return (*this);
  }


  template <typename T>
  inline
  bool TSVertex<T>::operator == (const TSVertex<T>& t) const {

    return getParameter() == t.getParameter();
  }


  template <typename T>
  inline
  bool TSVertex<T>::operator < (const TSVertex<T> &/*t*/) const {

    return true;
  }




  //
  //#if defined GM_STREAM
  //
  //
  //template <typename T>
  //inline
  //std::ostream& TSVertex<T>::operator<<(std::ostream& out, const TSVertex<T>& v) {
  //
  //  return v._prOut(out);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::ostream& TSVertex<T>::operator<<(std::ostream& out, const TSVertex<T>* v) {
  //
  //  return v->_prOut(out);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::istream& TSVertex<T>::operator>>(std::istream& in, TSVertex<T>& v)	{
  //
  //  return v._prIn(in);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::istream& TSVertex<T>::operator>>(std::istream& in, TSVertex<T>* v)	{
  //
  //  return v->_prIn(in);
  //}
  //
  //
  //
  //template <typename T>
  //inline
  //void TSVertex<T>::print(char prompt[]="TSVertex<T>", std::ostream & out = std::cout) const {
  //
  //  out << prompt << ": " << (*this) << "\n";
  //}
  //
  //
  //
  //
  //
  //template <typename T>
  //inline
  //std::ostream& TSVertex<T>::_prOut(std::ostream& out)const
  //{
  //	out << getPosition() << GMseparator::group();
  ////	if(  ) out << dir() << "\n ";
  //
  //	return out;
  //}
  //
  //
  //template <typename T>
  //inline
  //std::istream& TSVertex<T>::_prIn(std::istream& in)
  //{
  //	Point<T,3> p;
  //	in >> p; setPos(p);
  ////	if(  ) in >> p; setDir(p);
  //	return in;
  //}
  //
  //
  //#endif

  template <typename T>
  inline
  TSEdge<T>::TSEdge() {

    _vertex[0] = _vertex[1] = NULL;
    _triangle[0] = _triangle[1] = NULL;
    _const = false;
  }


  template <typename T>
  inline
  TSEdge<T>::TSEdge( TSVertex<T>& s, TSVertex<T>& e ) : _triangle() {

    _vertex[0] = &s;
    _vertex[1] = &e;
    _const = false;
    _upv();
  }


  template <typename T>
  inline
  TSEdge<T>::TSEdge( const TSEdge<T>& e ) {// : _vertex( e._vertex ), _triangle( e._triangle ) {

    for( int i = 0; i < 2; ++i ) {
      _vertex[i] = e._vertex[i];
      _triangle[i] = e._triangle[i];
    }

    _const = e._const;
    _upv();
  }


  template <typename T>
  TSEdge<T>::~TSEdge() {

    if( _triangle[0] != NULL )  delete _triangle[0];
    if( _triangle[1] != NULL )  delete _triangle[1];
    if( _vertex[0] != NULL )  _vertex[0]->_removeEdge(this);
    if( _vertex[1] != NULL )  _vertex[1]->_removeEdge(this);

    this->remove(this);
  }


  template <typename T>
  TSEdge<T>* TSEdge<T>::_getNext() {

    ArrayT< TSEdge<T>* > edg = _vertex[1]->getEdges();
    edg.remove(this);

    if( boundary() )
      for( int i = 0; i < edg.getSize(); i++ )
        if( edg[i]->boundary() )
          if( _vertex[1] == getCommonVertex(*(edg[i])) )
            return edg[i];

    return NULL;
  }


  template <typename T>
  bool TSEdge<T>::_hasPoints( const Point<T,3>& p1, const Point<T,3>& p2 ) const {

    TSVertex<T> * v1 = find(p1);
    ArrayT<TSEdge<T>*> edges = v1->getEdges();

    for( int i = 0; i < edges.getSize(); i++ )
      if( edges[i]->getOtherVertex(v1)->getPos() == p2 )
        return true;

    return false;
  /*
    TSVertex<T> * v2 = find(p2);
    if(_vertex[0] == v1 && _vertex[1] == v2)		return true;
    else if(_vertex[1] == v1 && _vertex[0] == v2)	return true;
    else											return false;
  */
  }


  template <typename T>
  inline
  bool TSEdge<T>::_isFirst( TSVertex<T>* v ) {

    return v == _vertex[0];
  }


  template <typename T>
  inline
  bool TSEdge<T>::_isLast( TSVertex<T>* v ) {

    return v == _vertex[1];
  }


  template <typename T>
  void TSEdge<T>::_okDelaunay() {

    if( _vertex[0]->isConst() && _vertex[1]->isConst() )
      setConst();

    if( _const || _triangle[0] == NULL || _triangle[1] == NULL )
      return;

    Array<TSVertex<T>*> a = _triangle[0]->getVertices();

    a.insert( _triangle[1]->getVertices() );

    a.remove( _vertex[0] );
    a.remove( _vertex[1] );

    Point<T,2> pt = a[0]->getParameter();

    if(
      pt.isInsideCircle(
        _vertex[0]->getParameter(),
        a[1]->getParameter(),
        _vertex[1]->getParameter()
      ) == 1
    )	this->_swap();
  }


  template <typename T>
  TSTriangle<T>* TSEdge<T>::_getOther( TSTriangle<T>* t ) {

    if( t == _triangle[0] )
      return _triangle[1];
    else if( t == _triangle[1] )
      return _triangle[0];
    else
      return NULL;
  }


  template <typename T>
  bool TSEdge<T>::_removeTriangle( TSTriangle<T>* t ) {

    if( _triangle[0] == t )
      _triangle[0] = NULL;
    else if( _triangle[1] == t )
      _triangle[1] = NULL;
    else
      return false;

    return true;
  }


  template <typename T>
  inline
  void TSEdge<T>::_reverse() {

    TSVertex<T> *v = _vertex[0];
    _vertex[0] = _vertex[1];
    _vertex[1] = v;
  }


  template <typename T>
  inline
  void TSEdge<T>::_setTriangle( TSTriangle<T>* t1, TSTriangle<T>* t2 ) {

    _triangle[0] = t1;
    _triangle[1] = t2;
  }


  template <typename T>
  bool TSEdge<T>::_split( TSVertex<T>& p ) {

    TSVertex<T>* v = NULL;
    TSEdge<T>*   e = NULL;
    TSTriangle<T> *t1 = NULL, *t2 = NULL;

    if( _triangle[0] != NULL )
      _triangle[0]->_reverse(this);
    if( _triangle[1] != NULL )
      _triangle[1]->_reverse(this);

//    Array<TSEdge<T>*> edg1 = _triangle[0] == NULL ? NULL : _triangle[0]->getEdges();
//    Array<TSEdge<T>*> edg2 = _triangle[1] == NULL ? NULL : _triangle[1]->getEdges();

      Array<TSEdge<T>*> edg1;
      Array<TSEdge<T>*> edg2;
      if( _triangle[0] != NULL ) edg1 = _triangle[0]->getEdges();
      if( _triangle[1] != NULL ) edg2 = _triangle[1]->getEdges();

    if( _triangle[0] != NULL ) { // swap triangles, _triang[0] should be on left
      if(_vertex[0] == getCommonVertex(*(edg1[1]))) {
        TSTriangle<T>* st = _triangle[0];
        _triangle[0] = _triangle[1];
        _triangle[1] = st;
        if( _triangle[0] != NULL ) edg1 = _triangle[0]->getEdges();
        if( _triangle[1] != NULL ) edg2 = _triangle[1]->getEdges();
      }
    }
    else { // swap triangles, _triang[0] should be on left
      if(_vertex[0] != getCommonVertex(*(edg2[1]))) {

        TSTriangle<T>* st = _triangle[0];
        _triangle[0] = _triangle[1];
        _triangle[1] = st;
        if( _triangle[0] != NULL ) edg1 = _triangle[0]->getEdges();
        if( _triangle[1] != NULL ) edg2 = _triangle[1]->getEdges();
      }
    }


    // Splitt edge in two
    TSEdge<T>* e1 = this;
    TSEdge<T>* e2 = new TSEdge<T>(*(_vertex[1]),p);
    _vertex[1]->_removeEdge(this);
    _vertex[1] = &p;
    p._insertEdge(this);
    e2->setConst(_const);
    this->insert(e2);

    // Splitt triangle 0
    if( _triangle[0] != NULL ) {

      v = edg1[1]->getCommonVertex(*(edg1[2]));
      e = new TSEdge<T>(p,*v);

      t1 = new TSTriangle<T>( e2, edg1[1], e );
      _triangle[0]->_setEdges( e1, e, edg1[2] );
      e->_setTriangle( t1, _triangle[0] );
      edg1[1]->_swapTriangle( _triangle[0], t1 );

      this->insert(e);
      this->insert(t1);

      this->adjust( _triangle[0] );
    }

    // Splitt triangle 1
    if( _triangle[1] != NULL ) {

      v = edg2[1]->getCommonVertex(*(edg2[2]));
      e = new TSEdge<T>(p,*v);

      t2 = new TSTriangle<T>(e2,e,edg2[2]);
      _triangle[1]->_setEdges(e1,edg2[1],e);
      e->_setTriangle(t2,_triangle[1]);

      e2->_setTriangle(t1,t2);
      edg2[2]->_swapTriangle(_triangle[1], t2);

      this->insert(e);
      this->insert(t2);

      this->adjust(_triangle[1]);
    }

    if( _triangle[0] != NULL ) {

      edg1[1]->_okDelaunay();
      edg1[2]->_okDelaunay();
    }

    if( _triangle[1] != NULL ) {

      edg2[1]->_okDelaunay();
      edg2[2]->_okDelaunay();
    }

    return true;
  }


  template <typename T>
  bool TSEdge<T>::_swap() {

    if( _const || _triangle[0] == NULL || _triangle[1] == NULL )
      return false;

    _triangle[0]->_reverse(this);
    _triangle[1]->_reverse(this);

    Array<TSEdge<T>*> edg1 = _triangle[0]->getEdges();
    Array<TSEdge<T>*> edg2 = _triangle[1]->getEdges();

    _vertex[0]->_removeEdge(this);
    _vertex[1]->_removeEdge(this);

    _vertex[0] = edg1[1]->getCommonVertex(*(edg1[2]));
    _vertex[1] = edg2[1]->getCommonVertex(*(edg2[2]));

    _vertex[0]->_insertEdge(this);
    _vertex[1]->_insertEdge(this);

    _triangle[0]->_setEdges( this, edg1[2], edg2[1] );
    _triangle[1]->_setEdges( this, edg2[2], edg1[1] );

    edg1[1]->_setTriangle( edg1[1]->_getOther( _triangle[0] ), _triangle[1] );
    edg2[1]->_setTriangle( edg2[1]->_getOther( _triangle[1] ), _triangle[0] );

    this->adjust( _triangle[0], true );
    this->adjust( _triangle[1], true );

    edg1[1]->_okDelaunay();
    edg1[2]->_okDelaunay();
    edg2[1]->_okDelaunay();
    edg2[2]->_okDelaunay();

    return true;
  }


  template <typename T>
  bool TSEdge<T>::_swapTriangle( TSTriangle<T>* ot, TSTriangle<T>* nt ) {

    if( ot == _triangle[0] ) {

      _triangle[0] = nt;
      return true;
    }
    else if( ot == _triangle[1] ) {

      _triangle[1] = nt;
      return true;
    }
    else
      return false;
  }


  template <typename T>
  bool TSEdge<T>::_swapVertex( TSVertex<T>& match_vtx, TSVertex<T>& rep_vtx ) {

    bool ret = false;

    if (_vertex[0] == &match_vtx) {

      _vertex[0] = &rep_vtx;
      ret = true;
    }

    if (_vertex[1] == &match_vtx) {

      _vertex[1] = &rep_vtx;
      ret = true;
    }

    if( _vertex[0]->isConst() && _vertex[1]->isConst() )
      _const = true;

    return ret;
  }


  template <typename T>
  inline
  void TSEdge<T>::_upv() {

    _vertex[0]->_insertEdge( this );
    _vertex[1]->_insertEdge(this);

    if( _vertex[0]->isConst() && _vertex[1]->isConst() )
      _const = true;
  }


  template <typename T>
  inline
  bool TSEdge<T>::boundary() const {

    return _triangle[0] == NULL || _triangle[1] == NULL;
  }


  template <typename T>
  TSVertex<T>* TSEdge<T>::getCommonVertex( const TSEdge<T>& e ) const {

    if( _vertex[0] == e._vertex[0] || _vertex[0] == e._vertex[1] )
      return _vertex[0];
    else if( _vertex[1] == e._vertex[0] || _vertex[1] == e._vertex[1] )
      return _vertex[1];

    return NULL;
  }


  template <typename T>
  inline
  TSVertex<T>* TSEdge<T>::getFirstVertex() const {

    return _vertex[0];
  }


  template <typename T>
  inline
  Point<T,3> TSEdge<T>::getCenterPos() {

    return (_vertex[1]->getPosition() + _vertex[0]->getPosition()) * 0.5;
  }


  template <typename T>
  inline
  Point<T,2> TSEdge<T>::getCenterPos2D() {

    return ( _vertex[1]->getParameter() + _vertex[0]->getParameter() ) * 0.5;
  }


  template <typename T>
  inline
  T TSEdge<T>::getLength() {

    return getVector().getLength();
  }


  template <typename T>
  inline
  T TSEdge<T>::getLength2D()	{

    return getVector2D().getLength();
  }


  template <typename T>
  Array<TSTriangle<T>*> TSEdge<T>::getTriangle() {

    Array<TSTriangle<T>*> arr;
    if( _triangle[0] != NULL ) arr.insert( _triangle[0] );
    if( _triangle[1] != NULL ) arr.insert( _triangle[1] );
    return arr;
  }


  template <typename T>
  inline
  Vector<T,3> TSEdge<T>::getVector() {

    return _vertex[1]->getPosition() - _vertex[0]->getPosition();
  }


  template <typename T>
  inline
  Vector<T,2> TSEdge<T>::getVector2D(){

    return _vertex[1]->getParameter() - _vertex[0]->getParameter();
  }


  template <typename T>
  inline
  TSVertex<T>* TSEdge<T>::getLastVertex() const {

    return _vertex[1];
  }


  template <typename T>
  TSVertex<T>* TSEdge<T>::getOtherVertex( const TSVertex<T>& v ) const {

    if( (*_vertex[0]) == v )
      return _vertex[1];
    else if( (*_vertex[1]) == v )
      return _vertex[0];
    else
      return NULL;
  }


  template <typename T>
  inline
  void TSEdge<T>::setConst( bool c ) {

    _const = c;
  }


  template <typename T>
  inline
  bool TSEdge<T>::operator <  (const TSEdge<T>& v) const {

    TSVertex<T>* c = (*this).commonVertex(v);
    Vector<T,3> a = (*this).otherVertex((*c))->getPosition()-c->getPosition();
    Vector<T,3> b = v.otherVertex((*c))->getPosition()-c->getPosition();

    if( a*b > 0 )
      return true;
    else
      return false;
  }


  template <typename T>
  inline
  bool TSEdge<T>::operator >  ( const TSEdge<T>& v ) const {

    return !((*this) < v);
  }


  template <typename T>
  inline
  bool TSEdge<T>::operator <= ( const TSEdge<T>& v ) const {

    return !((*this) > v);
  }


  template <typename T>
  inline
  bool TSEdge<T>::operator >= ( const TSEdge<T>& v ) const {

    return !((*this) < v);
  }







  //#if defined GM_STREAM
  //
  //
  //
  //template <template T>
  //inline
  //std::ostream& TSEdge<T>::_prOut(std::ostream& out)const
  //{
  //	out << (*_vertex[0]) << " " << (*_vertex[1])
  //		<< " " << _const << "\n ";
  //
  //	return out;
  //}
  //
  //template <template T>
  //inline
  //std::istream& TSEdge<T>::_prIn(std::istream& in)
  //{
  //	Point<T,3> p1, p2;
  //	in >> p1 >> p2 >> _const;
  //	_vertex[0] = find(p1);
  //	_vertex[1] = find(p2);
  //
  //	return in;
  //}
  //
  //
  //template <typename T>
  //inline
  //std::ostream& TSEdge<T>::operator<<( std::ostream& out, const TSEdge<T>& v ) {
  //
  //  return v._prOut(out);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::ostream& TSEdge<T>::operator<<( std::ostream& out, const TSEdge<T>* v ) {
  //
  //  return v->_prOut(out);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::istream& TSEdge<T>::operator >> ( std::istream& in, TSEdge<T>& v )	{
  //
  //  return v._prIn(in);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::istream& TSEdge<T>::operator>>( std::istream& in, TSEdge<T>* v ) {
  //
  //  return v->_prIn(in);
  //}
  //
  //
  //template <typename T>
  //inline
  //void print(char prompt[]="TSEdge<T>", std::ostream & out = std::cout) const {
  //
  //  out << prompt << ": " << (*this) << "\n";
  //}
  //
  //
  //
  //#endif

  template <typename T>
  inline
  TSTriangle<T>::TSTriangle() {

    _edge[0] = NULL;
    _edge[1] = NULL;
    _edge[2] = NULL;
  }


  template <typename T>
  inline
  TSTriangle<T>::TSTriangle( TSEdge<T>* e1, TSEdge<T>* e2, TSEdge<T>* e3 ) {

    _edge[0] = e1;
    _edge[1] = e2;
    _edge[2] = e3;
  }


  template <typename T>
  inline
  TSTriangle<T>::TSTriangle( const TSTriangle<T>& t ) : _box(t._box) {

    for( int i = 0; i < 3; ++i )
      _edge[i]  = t._edge[i];
  }


  template <typename T>
  TSTriangle<T>::~TSTriangle() {

    if( _edge[0] ) {

      _edge[0]->_removeTriangle( this );

      if( _edge[1] )
        _edge[1]->_removeTriangle( this );

      if( _edge[2] )
        _edge[2]->_removeTriangle( this );

      this->remove(this);
    }
  }


  template <typename T>
  T TSTriangle<T>::_evalZ( const Point<T,2>& p, int deg ) const {

    Array<TSVertex<T>*> ve = getVertices();
    Point<T,2> par[3];
    Point<T,3> pos[3];
    int i,j,k;

    for(i=0; i<3; i++) par[i] = pos[i] = ve[i]->getPosition();

    T a = p^par[0];
    T b = p^par[1];
    T c = p^par[2];
    T d = par[0]^par[1];
    T e = par[0]^par[2];
    T f = par[1]^par[2];
    T det = d-e+f;
    T u = (b-c+f)/det;
    T v = (-a+c-e)/det;
    T w = (a-b+d)/det;

    if (deg == 1)
      return u*pos[0](2) +v*pos[1](2) + w*pos[2](2);

    T pt[7];
    pt[6] = 0;

    for(i=0; i<3; i++) {

      if(i==2)	j = 0;
      else		j = i+1;
      k = 2*i;

      Vector<T,3>	  vec = pos[j] - pos[i];
      vec[2] = 0;
      UnitVector<T,3> uv  = vec;
      T		         vec2 = 0.33333333333333333333*(vec*vec);

      Vector<T,3> vv = ve[i]->getNormal()[2]*uv;
      vv[2]	-= ve[i]->getNormal()*uv;
      pt[k]			 = pos[i][2] + vec2/(vv*vec)*vv[2];

      vv			 = ve[j]->getNormal()[2]*uv;
      vv[2] -= ve[j]->getNormal()*uv;
      pt[k+1]		 = pos[j][2] - vec2/(vv*vec)*vv[2];

      pt[6]	+= pt[k] + pt[k+1];
    }

    pt[6] /= 6;

    a = u*u;
    b = v*v;
    c = w*w;
    return a*u*pos[0](2) +   3*a*v*pt[0] + 3*u*b*pt[1] + b*v*pos[1](2) +
        3*a*w*pt[5]    + 6*u*v*w*pt[6] + 3*b*w*pt[2] +
        3*u*c*pt[4]    +   3*v*c*pt[3] +
         c*w*pos[2](2);
  }


  template <typename T>
  inline
  Box<unsigned char,2>& TSTriangle<T>::_getBox() {

    return _box;
  }


 // #ifdef __gmOPENGL_H__

  template <typename T>
  void TSTriangle<T>::_render()  {

    Array<TSVertex<T>*> v = getVertices();
    //double a = (v[0]->position()[2]+v[1]->position()[2]+v[2]->position()[2])/3/-5 + 0.0;

    //glColor4f(0.0, 0.3, a, 1.0);
  /* NY KODE FOR TEXTUREBRUK 1/9 -2001 (MARTIN)*/
//    glTexCoord2f((v[0]->getPosition()[0])/100, (v[0]->getPosition()[1])/100);
//    glVertex((Arrow<T,3>*)v[0]);
//    glTexCoord2f((v[1]->getPosition()[0])/100, (v[1]->getPosition()[1])/100);
//    glVertex((Arrow<T,3>*)v[1]);
//    glTexCoord2f((v[2]->getPosition()[0])/100, (v[2]->getPosition()[1])/100);
//    glVertex((Arrow<T,3>*)v[2]);
  }

 // #endif


  template <typename T>
  bool TSTriangle<T>::_reverse( TSEdge<T>* a ) {

    int i,j;

    if(a == _edge[0])	return true;
    else if(a == _edge[1]) i = 1;
    else if(a == _edge[2]) i = 2;
    else			return false;

    j = (i==1? 2:1);

    TSEdge<T> *tmp = _edge[0];
    _edge[0] = _edge[i];
    _edge[i] = _edge[j];
    _edge[j] = tmp;

    return true;
  }


  template <typename T>
  inline
  void TSTriangle<T>::_setEdges( TSEdge<T>* e1, TSEdge<T>* e2, TSEdge<T>* e3 ) {

    _edge[0] = e1;
    _edge[1] = e2;
    _edge[2] = e3;
  }


  template <typename T>
  bool TSTriangle<T>::_split( TSVertex<T>& p ) {

    TSEdge<T>* edg1 = new TSEdge<T>(p,*(_edge[0]->getCommonVertex(*(_edge[1]))));
    TSEdge<T>* edg2 = new TSEdge<T>(p,*(_edge[1]->getCommonVertex(*(_edge[2]))));
    TSEdge<T>* edg3 = new TSEdge<T>(p,*(_edge[2]->getCommonVertex(*(_edge[0]))));

    TSTriangle<T>* t2 = new TSTriangle<T>( edg1, _edge[1], edg2 );
    TSTriangle<T>* t3 = new TSTriangle<T>( edg2, _edge[2], edg3 );

    edg1->_setTriangle( this, t2 );
    edg2->_setTriangle( t2, t3 );
    edg3->_setTriangle( t3, this );

    _edge[1]->_setTriangle( _edge[1]->_getOther(this), t2 );
    _edge[2]->_setTriangle( _edge[2]->_getOther(this), t3 );

    this->insert( edg1 );
    this->insert( edg2 );
    this->insert( edg3 );

    edg2 = _edge[1];
    _edge[1] = edg1;
    edg1 = _edge[2];
    _edge[2] = edg3;

    this->insert( t2 );
    this->insert( t3 );

    this->adjust( this );

    _edge[0]->_okDelaunay();
    edg1->_okDelaunay();
    edg2->_okDelaunay();

    return true;
  }


  template <typename T>
  void TSTriangle<T>::_updateBox( ArrayT<T>& u, ArrayT<T>& v, int d ) {

    int k,s,n = 1 << d;
    int i0=0, i1=n, j0=0, j1=n;
    Array<TSVertex<T>*>  ve = getVertices();
    Box<T,2> b( ve[0]->getParameter(), ve[1]->getParameter(), ve[2]->getParameter() );

    for( int i=1; i <= d; i++ ) {

      s = n >> i;
      k = i0 + s;
      if ( u[k] < b.getValueMin(0)) i0 = k;
      k = i1 - s;
      if ( u[k] > b.getValueMax(0)) i1 = k;
      k = j0 + s;
      if ( v[k] < b.getValueMin(1)) j0 = k;
      k = j1 - s;
      if ( v[k] > b.getValueMax(1)) j1 = k;
    }
    _box.reset(Point<unsigned char,2>(static_cast<unsigned char>(i0),static_cast<unsigned char>(j0)));
    _box.insert(Point<unsigned char,2>(static_cast<unsigned char>((i1-1)),static_cast<unsigned char>((j1-1))));
  }


  template <typename T>
  inline
  T TSTriangle<T>::getAngleLargest() {

    Array<Angle> a;
    a += this->_edges[0]->getVector().getAngle( this->_edges[1]->getVector() );
    a += this->_edges[1]->getVector().getAngle( this->_edges[2]->getVector() );
    a += this->_edges[2]->getVector().getAngle( this->_edges[0]->getVector() );
    a.sort();
    return a[2].getRad();
  }


  template <typename T>
  inline
  T TSTriangle<T>::getAngleSmallest() {

    Array<Angle> a;
    a += this->_edges[0]->getVector().getAngle( this->_edges[1]->getVector() );
    a += this->_edges[1]->getVector().getAngle( this->_edges[2]->getVector() );
    a += this->_edges[2]->getVector().getAngle( this->_edges[0]->getVector() );
    a.sort();
    return a[0].getRad();
  }


  template <typename T>
  inline
  T TSTriangle<T>::getArea() {

    return ( _edge[0]->getVector()^_edge[1]->getVector()).getLength() * 0.5;
  }


  template <typename T>
  inline
  T TSTriangle<T>::getArea2D() {

    T a=_edge[0]->getLength2D();
    T b=_edge[1]->getLength2D();
    T c=_edge[2]->getLength2D();
    T scf=0.5*(a+b+c);
    return sqrt(scf*(scf-a)*(scf-b)*(scf-c));
    //abs(_edge[0]->vector2D()^_edge[1]->vector2D())*0.5;
  }


  template <typename T>
  inline
  Point<T,3> TSTriangle<T>::getCenterPos() {

    Array<TSVertex<T>* > tmp= getVertices();
    return (tmp[0]->getPosition()+tmp[1]->getPosition()+tmp[2]->getPosition())/3.0;
  }


  template <typename T>
  inline
  Point<T,2> TSTriangle<T>::getCenterPos2D() {

    Array<TSVertex<T>* > tmp = getVertices();
    return (tmp[0]->getParameter() + tmp[1]->getParameter() + tmp[2]->getParameter())/3.0;
  }


  template <typename T>
  inline
  T TSTriangle<T>::getCircum() {

    return _edge[0]->getLength() + _edge[1]->getLength() + _edge[2]->getLength();
  }


  template <typename T>
  inline
  T TSTriangle<T>::getCircum2D() {

    return _edge[0]->getLength2D() + _edge[1]->getLength2D() + _edge[2]->getLength2D();
  }


  template <typename T>
  inline
  Array<TSEdge<T>*> TSTriangle<T>::getEdges() const	{

    return Array<TSEdge<T>*>(3,_edge);
  }


  template <typename T>
  Vector<T,3> TSTriangle<T>::getNormal() const {

    Array<TSVertex<T>*> v = getVertices();
    Vector<T,3> a = v[1]->getPosition() - v[0]->getPosition();
    Vector<T,3> b = v[2]->getPosition() - v[0]->getPosition();

    return  a^b;
  }


  template <typename T>
  Array<TSVertex<T>*>	TSTriangle<T>::getVertices() const {

    Array<TSVertex<T>*> r;
    r.setSize(3);

    r[1] = _edge[0]->getCommonVertex( *(_edge[1]) );
    r[2] = _edge[1]->getCommonVertex( *(_edge[2]) );
    r[0] = _edge[2]->getCommonVertex( *(_edge[0]) );

    return r;
  }


  template <typename T>
  inline
  int TSTriangle<T>::isAround( const TSVertex<T>& v ) const {

    return v.isInside(*this);
  }






  //#if defined GM_STREAM
  //
  //
  //
  //template <typename T>
  //inline
  //std::ostream& TSTriangle<T>::operator << ( std::ostream& out, const TSTriangle<T>& v ) {
  //
  //  return v._prOut(out);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::ostream& TSTriangle<T>::operator << ( std::ostream& out, const TSTriangle<T>* v ) {
  //
  //  return v->_prOut(out);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::istream& TSTriangle<T>::operator>>( std::istream& in, TSTriangle<T>& v ) {
  //
  //  return v._prIn(in);
  //}
  //
  //
  //template <typename T>
  //inline
  //std::istream& TSTriangle<T>::operator >> ( std::istream& in, TSTriangle<T>* v ) {
  //
  //  return v->_prIn(in);
  //}
  //
  //
  //template <typename T>
  //inline
  //void TSTriangle<T>::print( char prompt[] = "TSTriangle<T>", std::ostream & out = std::cout ) const {
  //
  //  out << prompt << ": " << (*this) << "\n";
  //}
  //
  //
  //
  //
  //template <class T>
  //inline
  //std::ostream& TSTriangle<T>::_prOut(std::ostream& out)const {
  //
  //	Array<TSVertex<T>*> v = getVertices();
  //	out << (*v[0]) << " " << (*v[1]) << " " << (*v[2]) << " ";
  //
  //	return out;
  //}
  //
  //
  //template <class T>
  //inline
  //std::istream& TSTriangle<T>::_prIn(std::istream& in) {
  //
  //	Point<T,3> p1, p2, p3;
  //	in >> p1 >> p2 >> p3;
  //	_edge[0] = find(p1,p2);
  //	_edge[1] = find(p2,p3);
  //
  //	_edge[2] = find(p3,p1);
  //
  //	return in;
  //}
  //
  //
  //
  //#endif

  template <typename T>
  inline
  TSTile<T>::TSTile() {}


  template <typename T>
  inline
  TSTile<T>::TSTile( TSVertex<T>* v, Box<T,3> domain ) {

    _vertex = v;

    Array<TSEdge<T> *> edges;
    edges = v->getEdges();

    Array<TSTriangle<T> *> triangles = v->getTriangles();
    for( int i = 0; i < triangles.getSize(); i++ ) {

      Array<TSVertex<T>* > vertices = triangles[i]->getVertices();

      if(vertices[0] != _vertex)
        swap( vertices[0], vertices[1]);

      if(vertices[0] != _vertex)
        swap(vertices[0],vertices[2]);

      Point<T,2> t;
      t = _voronoi(vertices[0]->getParameter(), vertices[1]->getParameter(), vertices[2]->getParameter());

      if( domain.isSurrounding( Point<T,3>(t[0],t[1],0.2) ) )
        _vorpts.insertAlways(t);
    }

    for( int i = 0; i < edges.getSize(); i++ )
    if(edges(i)->boundary()) _vorpts.insertAlways( edges(i)->getCenterPos2D() );


    // make polygon, maybe edges should have been sorted first
    _vorpts[0].setTestType( 3, _vertex->getPosition(), Vector<T,2>(0.0,1.0) );
//    _vorpts.sort();
///////////
    _circumscribed=0.0;
    _inscribed=1.0E9;
    // for(int i=0; i<vorpts.size(); i++)
    // {
    //	 Vector2D<T> tmp=(vorpts[i]-myvtx->parameter());
    //	 if (tmp.length()>circumscribed) circumscribed=tmp.length()*1.00;
    // }
    for( int i = 0; i< edges.getSize(); i++ )
      if( edges(i)->getLength2D()*1.0 < _inscribed )
        _inscribed = (edges(i)->getLength2D())*1.0; // smallest

    for( int i = 0; i < edges.getSize(); i++ )
      if( edges(i)->getLength2D()*1.0 > _circumscribed )
        _circumscribed = ( edges(i)->getLength2D())*1.0; // largest
///////////////////////
    setSurroundingSphere( Sphere<T,3>( Point<T,3>(_vertex->getParameter()), _circumscribed ) );
  }


  template <typename T>
  inline
  Point<T,2> TSTile<T>::_voronoi( const Point<T,2>& v1, const Point<T,2>& v2, const Point<T,2>& v3 ) {

    Vector<T,2> d1 = v2 - v1;
    Vector<T,2> d2 = v3 - v1;
    Vector<T,2> d3 = v3 - v2;
    d1 = d2 - ((d1*d2)/(d1*d1))*d1;
    return 0.5*(v1+v2 + ((d3*d2)/(d1*d2))*d1);
  }


  template <typename T>
  inline
  T TSTile<T>::getBigR() {

    return _circumscribed;
  }

  template <typename T>
  inline
  T TSTile<T>::getSmallR() {

    return _inscribed;
  }


  template <typename T>
  inline
  void TSTile<T>::localDisplay() {

    render();
  }


  template <typename T>
  inline
  void TSTile<T>::localSelect() {

    glBegin( GL_LINE_STRIP ); {

      //glBegin(GL_TRIANGLE_FAN);
      //glVertex((Point<T,3>)myvtx->parameter());
      for( int i = 0; i < _vorpts.getSize(); i++ )
        glVertex( Point<T,3>(_vorpts(i)) );

      if( !_vertex->boundary() )
        glVertex( Point<T,3>(_vorpts(0)) );

    } glEnd();
  }


  template <typename T>
  inline
  void TSTile<T>::render() {

    glPushAttrib( GL_LIGHTING_BIT ); {
      glBegin(GL_LINE_STRIP); {

        for( int i=0; i<_vorpts.size(); i++ )
          glVertex( Point<T,3>(_vorpts(i)) );

        if(!_vertex->boundary())  glVertex(Point<T,3>(_vorpts(0)));

      }glEnd();
    }glPopAttrib();
  }

  template <typename T>
  inline
  TSLine<T>::TSLine( int d ) : Array<TSVertex<T> >( d ) {}


  template <typename T>
  inline
  TSLine<T>::TSLine( const Array<TSVertex<T> >& v ) : Array<TSVertex<T> >(v) {}


  template <typename T>
  TSVertex<T> TSLine<T>::interpolate( int i, double t ) const {

  //	if( i < 0 || i > getSize() - 2 ) {
  //		// Errormessage for debug purpose
  //	}

    Point<T,3> pos = (1-t)*( (*this)(i).getPosition() ) + t*( (*this)(i+1).getPosition() );
    Point<T,3> nor = (1-t)*( (*this)(i).getNormal() ) + t*( (*this)(i+1).getNormal() );

    return TSVertex<T>( pos, nor );
  }

  template <typename T>
  inline
  TSVEdge<T>::TSVEdge() {}

  template <typename T>
  inline
  TSVEdge<T>::TSVEdge(const Point<T,2> &p, const Point<T,2> &q) {
    _pnts[0] = p;
    _pnts[1] = q;
  }

  template <typename T>
  inline
  bool TSVEdge<T>::operator==(const TSVEdge<T> &e) const {

    bool a=false ,b= false;
    if ((e._pnts[0] == _pnts[0])||(e._pnts[0] == _pnts[1])) a = true;
    if ((e._pnts[1] == _pnts[0])||(e._pnts[1] == _pnts[1])) b = true;

    return (a && b);
  }

  template <typename T>
  inline
  bool TSVEdge<T>::operator<(const TSVEdge<T> &/*e*/) const {
    return false;
  }

  template <typename T>
  inline
  Point<T,2> const &TSVEdge<T>::operator() (int i) const {
    return _pnts[i];
  }

} // END namespace GMlib

