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



/*! \file gmviewset.h
 *
 */

#ifndef __gmVIEWSET_H__
#define __gmVIEWSET_H__


#include "gmview.h"

// gmlib
#include <core/containers/gmarray.h>
#include <core/types/gmpoint.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/gmprogram.h>

namespace GMlib {


  class Camera;
  class Renderer;


  /*! \class ViewSet gmwindow.h <gmWindow>
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  class ViewSet {
    struct GLBorder {
      GL::GLViewVertex    v[4]; // corners
    }; // END struct GLBorder

  public:
    ViewSet(Camera* cam=0);
    ViewSet(const ViewSet& viewset);
    ~ViewSet();

    const Array<Camera*>&             getCameras() const;
    int                               getSize();
    Point<int,2>                      getViewPortSize();

    const Color&                      getBorderColor() const;
    const GL::VertexBufferObject&     getBorderVBO() const;
    int                               getNoBorders() const;

    void                              prepare(int w, int h);
    void                              removeCamera(Camera*);

    ViewSet&                          operator=(const ViewSet& viewset);
    bool                              operator<(const ViewSet& viewset)const;

  protected:
    void                              prepareGraphics();

    GL::VertexBufferObject            _vbo;
    int                               _no_borders;

  private:
    friend class GMWindow;

    int                               _vp_w; // ViewPort size ( Width )
    int                               _vp_h;  // Viewport size ( height )
    Array<Camera*>                    _cameras;
    Array<ViewBorder*>                _borders;
    View                              _root;
    Color                             _border_color;

    Array<ViewBorder*>                _selected_borders;


    void                              drawBorder();

  public:
//    void                            drawCamera();
    Camera*                           operator[](int i);

  private:
    bool           find(int x, int y, Camera*& cam);
    bool           insertCamera(Camera* cam_to_split, Camera* new_cam, bool split_vertically=true, double d=0.5);
    void           moveBorder(int x, int y);
    void           removeCamera(int i);
    void           reset();
    void           setBorderColor(const Color& bc);

  }; // END class ViewSet


















  inline
  const GL::VertexBufferObject& ViewSet::getBorderVBO() const {

    return _vbo;
  }

  inline
  int ViewSet::getNoBorders() const {

    return _no_borders;
  }

  inline
  const Array<Camera*>& ViewSet::getCameras() const {

    return _cameras;
  }


  /*! Point<int,2> ViewSet::getViewPortSize()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Point<int,2> ViewSet::getViewPortSize() {

    return Point<int,2>(_vp_w, _vp_h);
  }

  inline
  const Color &ViewSet::getBorderColor() const {

    return _border_color;
  }

  /*! bool ViewSet::operator<(const ViewSet& viewset) const
   *  \brief DUMMY!!
   *
   *  Pending Documentation
   *
   *  \todo Remove if not needed!
   */
  inline
  bool ViewSet::operator < ( const ViewSet& /* viewset */ ) const {

    return true;
  }


  /*! void ViewSet::_drawBorder()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewSet::drawBorder() {

    if( _no_borders > 0 ) {


      GL::Program prog;
      prog.acquire("color");

      prog.uniform( "u_color", _border_color );

      GL::AttributeLocation vert_loc = prog.getAttributeLocation( "in_vertex" );
      _vbo.bind();
//      _vbo.enable( vert_loc, 2, GL_FLOAT, GL_FALSE, (const GLvoid*)0x0 );
      glVertexAttribPointer( vert_loc(), 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0x0 );
      glEnableVertexAttribArray( vert_loc() );
      glPointSize( 10.0f );
      glDrawArrays( GL_QUADS, 0, _no_borders * 4 );

      glDisableVertexAttribArray( vert_loc() );

      _vbo.unbind();

    }
  }


  /*! bool ViewSet::_insertCamera(Camera* cam_to_split, Camera* new_cam, bool split_verticaly, double d)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool ViewSet::insertCamera(Camera* cam_to_split, Camera* new_cam, bool split_vertically, double d) {

    if(_cameras.exist(new_cam))
      return false;
    else if(!_cameras.exist(cam_to_split))
      return false;
    else
    {
      _cameras += new_cam;
      if(_cameras.getSize()>2)
        _root.splitCamera(cam_to_split,new_cam,split_vertically,d);
      else
        _root.split( new_cam,split_vertically,d);
      _borders.resetSize();
      _root.prepare( 0, 0,_vp_w, _vp_h, _borders);
      prepareGraphics();
      return true;
    }
  }


  /*! void ViewSet::_moveBorder(int x, int y)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewSet::moveBorder(int x, int y) {

    for( int i = 0; i < _selected_borders.getSize(); i++ )
      _selected_borders[i]->move(x,y);
    prepare(_vp_w,_vp_h);
    prepareGraphics();
  }


  /*! void ViewSet::_prepare(int w, int h)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewSet::prepare(int w, int h) {

    std::cout << "ViewSet::prepare (w,h): (" << w << "," << h << ")" << std::endl;
    _vp_w = w; _vp_h = h;
    _borders.resetSize();
    _root.prepare(0,0, _vp_w, _vp_h, _borders );

//    for( int i = 0; i < getCameras().getSize(); ++i )
//      getCameras()(i)->reshape( 0, 0, _vp_w, _vp_h );
  }


  /*! void ViewSet::removeCamera(Camera* cam)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewSet::removeCamera(Camera* cam) {

    if(_cameras.getSize() > 1)
    {
      int i = _cameras.getIndex(cam);
      if(i>=0) removeCamera(i);
    }

    prepareGraphics();
  }


  /*! void ViewSet::_removeCamera(int i)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewSet::removeCamera(int i) {

    if( _cameras.getSize() > 1 ){

      _root.removeCamera(_cameras[i]);
      _cameras.removeIndex(i);
      prepare( _vp_w,_vp_h );
    }
  }

} // END namespace GMlib


#endif // __gmVIEWSET_H__
