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



/*! \file gmview.h
 */

#ifndef __gmVIEW_H__
#define __gmVIEW_H__



// local
#include "../camera/gmcamera.h"

// gmlib
#include <core/types/gmpoint.h>


namespace GMlib {

  class ViewSet;
  class ViewBorder;

  /*! \class View gmview.h <gmView>
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  class View {
  public:
    View( Camera* cam = 0x0 );
    View( ViewBorder* border );
    View( const View& view );

  private:
    friend class ViewSet;
    friend class ViewBorder;

    Camera*        _camera;
    ViewBorder*    _border;


    void           corr(Array<ViewBorder*>& nw, const Array<ViewBorder*>& ow);
    void           find(int x, int y, Camera*& cam, Array<ViewBorder*>& borders);
    int            getSize(bool split_vertically = true);
    bool           isCamera(Camera* cam);
    void           prepare(int x1, int y1, int x2, int y2, Array<ViewBorder*>& borders);
    void           removeCamera(Camera* cam);
    void           split(Camera* new_cam, bool split_vertically, double d);
    void           splitCamera(Camera* cam_to_split, Camera* new_cam, bool split_vertically, double d);

  }; // END class View





  /*! \class ViewBorder gmview.h <gmView>
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  class ViewBorder {
  public:
    ViewBorder(Camera* cam_to_split, Camera* new_cam, bool v, double r);
    ViewBorder(const ViewBorder& w);

    Box<int,2>    getBounds() const;
  private:
    friend class ViewSet;
    friend class View;

    bool      _vertical;    /// True if border is vertical
    double    _displacement;
    int        _x1, _x2, _y1, _y2;  /// Pixel-index of the border
    View      _child_lt;
    View      _child_rb;

    void       corr(Array<ViewBorder*>& nw, const Array<ViewBorder*>& ow);
    void       display();
    void       find(int x, int y, Camera*& cam, Array<ViewBorder*>& borders);
    int        getSize(bool vertical=true);
    View*     hasCamera(Camera* cam);
    bool       isVertical();
    void       move(int x, int y);
    void       prepare( int x1, int y1, int x2, int y2, Array<ViewBorder*>& borders );
    void       removeCamera(Camera* cam);
    void       splitCamera(Camera* cam_to_split, Camera* new_cam, bool vertical, double d);


  }; // END class ViewBorder

























  /*! View::View(Camera* cam)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  View::View(Camera* cam) {

    _camera = cam;
    _border = 0;
  }


  /*! View::View(ViewBorder* border)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  View::View(ViewBorder* border) {

    _camera = 0;
    _border = border;
  }


  /*! View::View(const View& view)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  View::View(const View& view) {

    _camera = view._camera;
    _border = view._border;
  }


  /*! void View::_corr(Array<ViewBorder*>& nw, const Array<ViewBorder*>& ow)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void View::corr(Array<ViewBorder*>& nw, const Array<ViewBorder*>& ow) {

    if( _border )
      _border = nw[ow.index(_border)];
  }


  /*! int View::_getSize( bool split_vertically )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  int View::getSize( bool split_vertically ) {

    if( _border )
      return _border->getSize( split_vertically );
    else {

      int w1, w2, h1, h2;
      _camera->getViewport( w1, w2, h1, h2 );
      if( split_vertically )
        return w2 - w1;
      else
        return h2 - h1;
    }
  }


  /*! bool View::_isCamera(Camera* cam)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool View::isCamera(Camera* cam)  {

    return _camera == cam;
  }


  /*! void View::_split(Camera* new_cam, bool split_vertically, double d)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  Splitting this in two
   */
  inline
  void View::split(Camera* new_cam, bool split_vertically, double d) {

    if(_camera) {

      _border = new ViewBorder( _camera, new_cam, split_vertically, d );
      _camera = 0;
    }
    else {

      _border = 0;
      _camera = new_cam;
    }
  }

  /*! ViewBorder::ViewBorder( Camera* cam_to_split, Camera* new_cam, bool v, double r )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  ViewBorder::ViewBorder( Camera* cam_to_split, Camera* new_cam, bool v, double r )
    : _vertical(v), _displacement(r), _child_lt(cam_to_split), _child_rb(new_cam)
  {}


  /*! ViewBorder::ViewBorder(const ViewBorder& w)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  ViewBorder::ViewBorder(const ViewBorder& w)
    : _vertical(w._vertical), _displacement(w._displacement), _child_lt(w._child_lt), _child_rb(w._child_rb)
  {}


  /*! void ViewBorder::corr(Array<ViewBorder*>& nw, const Array<ViewBorder*>& ow)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewBorder::corr(Array<ViewBorder*>& nw, const Array<ViewBorder*>& ow) {

    _child_lt.corr( nw, ow );
    _child_rb.corr( nw, ow );
  }


  /*! void ViewBorder::display()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewBorder::display() {

    glRecti( _x1, _y1, _x2, _y2 );
  }


  /*! void ViewBorder::find(int x, int y, Camera*& cam, Array<ViewBorder*>& borders)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewBorder::find(int x, int y, Camera*& cam, Array<ViewBorder*>& borders) {

    if(x>(_x1-5) && x<(_x2+5) && y>(_y1-5) && y<(_y2+5)) borders += this;

    _child_lt.find(x,y,cam,borders);
    _child_rb.find(x,y,cam,borders);
  }

  inline
  Box<int,2>  ViewBorder::getBounds() const {

    Box<int,2> box( Point<int,2>( _x1, _y1 ) );
    box.insert( Point<int,2>( _x2, _y2 ) );
    return box;
//    return Box<float,3>( Point<float,2>(_x1,_y1), Point<float,2>(_x2,_y2) );
  }

  /*! int  ViewBorder::getSize(bool vertical)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  int  ViewBorder::getSize(bool vertical) {

    if(vertical == _vertical)
      return _child_lt.getSize(vertical) + _child_rb.getSize(vertical);
    else
      return std::min<int>( _child_lt.getSize(vertical), _child_rb.getSize(vertical) );
  }


  /*! View* ViewBorder::hasCamera(Camera* cam)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  View* ViewBorder::hasCamera(Camera* cam) {

    if(_child_lt.isCamera(cam))
      return &_child_rb;
    else if(_child_rb.isCamera(cam))
      return &_child_lt;
    else
      return 0;
  }


  /*! bool ViewBorder::isVertical()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool ViewBorder::isVertical() {

    return _vertical;
  }


  /*! void ViewBorder::move(int x, int y)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewBorder::move(int x, int y) {

    int l_size = _child_lt.getSize(_vertical);
    int r_size = _child_rb.getSize(_vertical);

    if(_vertical) {

      l_size = _x1-l_size;
      r_size += _x2;
      if((x > l_size && x < _x1)||(x > _x2 && x < r_size))
        _displacement = (double)(x-l_size)/(r_size-l_size);
    }
    else
    {
      l_size = _y1-l_size;
      r_size += _y2;
      if((y > l_size && y < _y1)||(y > _y2 && y < r_size))
        _displacement = (double)(y-l_size)/(r_size-l_size);
    }
  }


  /*! void ViewBorder::prepare(int x1, int y1, int x2, int y2, Array<ViewBorder*>& borders)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewBorder::prepare(int x1, int y1, int x2, int y2, Array<ViewBorder*>& borders) {

    borders.insert(this);
    if(_vertical) {

      _x1 = x1 + int(_displacement*(x2-x1)) - 1;
      _x2 = _x1 + 2;
      _y1 = y1-2;
      _y2 = y2+2;
      _child_lt.prepare( x1, y1, _x1, y2, borders );
      _child_rb.prepare( _x2, y1, x2, y2, borders );
    }
    else {

      _y1 = y1 + int( _displacement * (y2-y1) ) - 1;
      _y2 = _y1 + 2;
      _x1 = x1-2;
      _x2 = x2+2;
      _child_lt.prepare( x1, y1, x2, _y1, borders );
      _child_rb.prepare( x1, _y2, x2, y2, borders );
    }
  }


  /*! void ViewBorder::removeCamera(Camera* cam)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewBorder::removeCamera(Camera* cam) {

    _child_lt.removeCamera(cam);
    _child_rb.removeCamera(cam);
  }


  /*! void ViewBorder::splitCamera(Camera* cam_to_split, Camera* new_cam, bool split_vertically, double d)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ViewBorder::splitCamera(Camera* cam_to_split, Camera* new_cam, bool split_vertically, double d) {

    _child_lt.splitCamera( cam_to_split, new_cam, split_vertically, d );
    _child_rb.splitCamera( cam_to_split, new_cam, split_vertically, d );
  }



} // END namespace GMlib


#endif // __gmVIEW_H__
