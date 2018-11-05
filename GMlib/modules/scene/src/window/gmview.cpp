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



/*! \file gmview.cpp
 *
 */


#include "gmview.h"


namespace GMlib {



  /*! void View::_find(int x, int y, Camera*& cam, Array<ViewBorder*>& borders)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  void View::find(int x, int y, Camera*& cam, Array<ViewBorder*>& borders) {

    if( _border ) _border->find( x, y, cam, borders );
    else
    {
      int w1, w2, h1, h2;
      _camera->getViewport( w1, w2, h1, h2 );
      if( x >= w1 && x <= w2 && y >= h1 && y <= h2 ) cam=_camera;
    }
  }


  /*! void View::_prepare(int x1, int y1, int x2, int y2, Array<ViewBorder*>& borders)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  void View::prepare(int x1, int y1, int x2, int y2, Array<ViewBorder*>& borders) {

//    std::cout << "View::prepare (x1,y1,x2,y2): (" << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ")" << std::endl;
    if( _camera )
//      _camera->reshape( x1, y1, x2, y2 );
      _camera->reshape( x1, y1, x2, y2 );
    else
      _border->prepare( x1, y1, x2, y2, borders );
  }


  /*! void View::_removeCamera(Camera* cam)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   */
  void View::removeCamera(Camera* cam) {

    if( _border ) {

      View* view = _border->hasCamera( cam );
      if( view ) {

        ViewBorder* border = _border;
        (*this) = (*view);
        delete border;
      }
      else _border->removeCamera(cam);
    }
  }


  /*! void View::_splitCamera(Camera* cam_to_split, Camera* new_cam, bool split_vertically, double d)
   *	\brief Pending Documentation
   *
   *	Pending Documentation
   *	Recurciv seek to find object to split
   */
  void View::splitCamera(Camera* cam_to_split, Camera* new_cam, bool split_vertically, double d) {

    if(_camera == cam_to_split)
      split( new_cam, split_vertically, d );
    else if(_border)
      _border->splitCamera( cam_to_split, new_cam, split_vertically, d );
  }






} // END namespace GMlib
