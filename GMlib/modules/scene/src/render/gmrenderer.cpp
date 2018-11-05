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



#include "gmrenderer.h"

// local
#include "../camera/gmcamera.h"
#include "gmrendertarget.h"
#include "rendertargets/gmnativerendertarget.h"

// gmlib
#include <opengl/utils/gmutils.h>

// stl
#include <cassert>


namespace GMlib {

  Renderer::Renderer() : _camera(0x0) {}

  Renderer::~Renderer() {}

  Camera*
  Renderer::getCamera() {

    return _camera;
  }

  const Camera*
  Renderer::getCamera() const {

    return _camera;
  }

  void
  Renderer::setCamera(Camera* cam) {

    _camera = cam;
  }

  void Renderer::releaseCamera() {
    _camera = 0x0;
  }

//  void Renderer::setViewport(int x, int y, int w, int h) {

//    _x = x;
//    _y = y;
//    _w = w;
//    _h = h;

//    reshape();
//  }

//  void Renderer::setRenderTarget(RenderTarget* rt) {

//    assert(rt);
//    delete _rt;
//    _rt = rt;
//  }

//  void Renderer::reshape() {

//    _camera->reshape(0, 0, _w, _h);
//    _rt->resize( Vector<unsigned int,2>( _w, _h ) );
//  }




} // END namespace GMlib
