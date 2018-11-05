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


#include "gmdefaultselectrenderer.h"

#include "../gmsceneobject.h"
#include "../camera/gmcamera.h"
#include "../visualizers/gmstdrepvisualizer.h"

// gmlib
#include <opengl/shaders/gmvertexshader.h>
#include <opengl/shaders/gmfragmentshader.h>
#include <opengl/gmprogram.h>

// stl
#include <cassert>

namespace GMlib {

  DefaultSelectRenderer::DefaultSelectRenderer() {

    _fbo.create();
    _rbo_color.create(GL_TEXTURE_2D);
    _rbo_depth.create(GL_TEXTURE_2D);

    // Color rbo texture
    _rbo_color.texParameteri( GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _rbo_color.texParameteri( GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _rbo_color.texParameterf( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _rbo_color.texParameterf( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Depth rbo texture
    _rbo_depth.texParameteri( GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _rbo_depth.texParameteri( GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _rbo_depth.texParameterf( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _rbo_depth.texParameterf( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    _fbo.attachTexture2D( _rbo_color,  GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 );
    _fbo.attachTexture2D( _rbo_depth, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT );
  }

  DefaultSelectRenderer::~DefaultSelectRenderer() {}

  const
  SceneObject*
  DefaultSelectRenderer::findObject(int x, int y) const {

    Color c;
    _fbo.bind();
    GL_CHECK(::glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,reinterpret_cast<GLubyte*>(&c)));
    _fbo.unbind();

    const SceneObject *obj = getCamera()->getScene()->find(c.get());

    return obj;
  }

  SceneObject*
  DefaultSelectRenderer::findObject(int x, int y) {


    Color c;
    _fbo.bind();
    GL_CHECK(::glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,reinterpret_cast<GLubyte*>(&c)));
    _fbo.unbind();

    return getCamera()->getScene()->find(c.get());
  }

  Array<const SceneObject*>
  DefaultSelectRenderer::findObjects(int xmin, int ymin, int xmax, int ymax) const {

    Array<const SceneObject* > sel;
    int dx=(xmax-xmin)+1;
    int dy=(ymax-ymin)+1;

    Color* pixels = new Color[dx*dy];
    _fbo.bind();
    GL_CHECK(::glReadPixels(xmin,ymin,dx-1,dy-1,GL_RGBA,GL_UNSIGNED_BYTE,reinterpret_cast<GLubyte*>(pixels)));
    _fbo.unbind();

    int ct = 0;
    Color c;
    for(int i = ymin; i < ymax; ++i) {
      for(int j = xmin; j < xmax; ++j) {
        c = pixels[ct++];
        const SceneObject *tmp = getCamera()->getScene()->find(c.get());
        if(tmp && !tmp->isSelected())
          sel.insertAlways(tmp);
      }
    }
    delete [] pixels;

    return sel;
  }

  Array<SceneObject*>
  DefaultSelectRenderer::findObjects(int xmin, int ymin, int xmax, int ymax) {

    Array<SceneObject* > sel;
    int dx=(xmax-xmin)+1;
    int dy=(ymax-ymin)+1;

    Color* pixels = new Color[dx*dy];
    _fbo.bind();
    GL_CHECK(::glReadPixels(xmin,ymin,dx-1,dy-1,GL_RGBA,GL_UNSIGNED_BYTE,reinterpret_cast<GLubyte*>(pixels)));
    _fbo.unbind();

    int ct = 0;
    Color c;
    for(int i = ymin; i < ymax; ++i) {
      for(int j = xmin; j < xmax; ++j) {
        c = pixels[ct++];

        SceneObject *tmp = getCamera()->getScene()->find(c.get());
        if(tmp && !tmp->isSelected())
          sel.insertAlways(tmp);
      }
    }
    delete [] pixels;

    return sel;
  }

  void DefaultSelectRenderer::select(int what) {

    const Camera *cam = getCamera();

    GL_CHECK(::glViewport(0,0,_size(0),_size(1)));

    // Clear buffers
    _fbo.clear( GL_DEPTH_BUFFER_BIT );
    _fbo.clearColorBuffer( GMcolor::black() );

    // Render selection
    GLboolean depth_test_state;
    GL_CHECK(::glGetBooleanv( GL_DEPTH_TEST, &depth_test_state ));
    GL_CHECK(::glEnable( GL_DEPTH_TEST ));

    GL_CHECK(::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL));

    _fbo.bind(); {

      for( int i=0; i < _objs.getSize(); ++i ) {

        const SceneObject *obj = _objs(i);
        if( obj != cam && ( what == 0 || what == obj->getTypeId() || ( what < 0 && what + obj->getTypeId() != 0 ) ) ) {

          if(obj->isCollapsed()) {

            VisualizerStdRep::getInstance()->renderGeometry(obj,this,obj->getVirtualName());
          }
          else {

            const Array<Visualizer*>& visus = obj->getVisualizers();
            for( int j = 0; j < visus.getSize(); ++j )
              visus(j)->renderGeometry(obj,this,obj->getVirtualName());

            obj->localSelect(this,obj->getVirtualName());
          }
        }
      }

    }  _fbo.unbind();

    if( !depth_test_state )
      GL_CHECK(::glDisable( GL_DEPTH_TEST ));
  }

  void
  DefaultSelectRenderer::prepare() {

    Camera* cam = getCamera();

    // Get displayable objects
    _objs.resetSize();
    cam->getScene()->getRenderList( _objs, cam );
  }

  void
  DefaultSelectRenderer::reshape(const Vector<int,2>& size) {

    _size = size;

    _rbo_color.texImage2D( 0, GL_RGBA8, _size(0), _size(1), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0 );
    _rbo_depth.texImage2D( 0, GL_DEPTH_COMPONENT, _size(0), _size(1), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0 );
  }

} // END namespace GMlib
