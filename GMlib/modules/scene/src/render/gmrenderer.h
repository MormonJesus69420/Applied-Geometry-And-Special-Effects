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




#ifndef GM_SCENE_RENDER_RENDERER
#define GM_SCENE_RENDER_RENDERER


// GMlib
#include <core/types/gmpoint.h>
#include <core/containers/gmarray.h>
#include <core/utils/gmcolor.h>

namespace GMlib {

  class Camera;

  class Renderer {
  public:
    Renderer();
    virtual ~Renderer();

    Camera*           getCamera();
    const Camera*     getCamera() const;
    void              setCamera( Camera* cam );
    void              releaseCamera();

    /* prepare stuff */
    virtual void      prepare() = 0;
    virtual void      render() = 0;
    virtual void      swap() = 0;
    virtual void      reshape( const Vector<int,2>& size ) = 0;

  private:
    Camera*           _camera;

  }; // END class Renderer


} // END namespace GMlib

#endif // GM_SCENE_RENDER_RENDERER
