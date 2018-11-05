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



/*! \file gmwindow.cpp
 *  \brief Pending Description
 */


#include "gmwindow.h"

#include "../render/gmrenderer.h"
#include "../render/rendertargets/gmnativerendertarget.h"

// gmlib
#include <core/containers/gmdvector.h>
#include <opengl/gmopenglmanager.h>

// stl
#include <cstdio>
#include <iostream>
#include <cassert>


namespace GMlib {




  /*! GMWindow::GMWindow()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  GMWindow::GMWindow() {

    _border_prog.acquire("color");

    _std_rep_cube.acquire("std_rep_cube");
    _std_rep_cube_indices.acquire("std_rep_cube_indices");
    _std_rep_frame_indices.acquire("std_rep_frame_indices");

    _stereo     = false;
    _isbig      = false;
    _target     = NULL;
    _move       = 0.0;
    _active_cam = 0;
    stop();

    _rt = new NativeRenderTarget;
    _clear_color  = GMcolor::grey();
    _select_color = GMcolor::beige();



    // Build an orthogonal matrix
    float near_plane = -1.0f;
    float far_plane = 1.0f;

    float l, r, b, t, n, f;
    l = 0.0f;
    r = 1.0f;
    b = 0.0f;
    t = 1.0f;
    n = near_plane;
    f = far_plane;

    float A, B, C;
    A = - ( r + l ) / ( r - l );
    B = - ( t + b ) / ( t - b );
    C = - ( f + n ) / ( f - n );

    _ortho_mat[0][0] = 2 / (r - l);
    _ortho_mat[0][1] = 0.0f;
    _ortho_mat[0][2] = 0.0f;
    _ortho_mat[0][3] = A;

    _ortho_mat[1][0] = 0.0f;
    _ortho_mat[1][1] = 2 / ( t - b );
    _ortho_mat[1][2] = 0.0f;
    _ortho_mat[1][3] = B;

    _ortho_mat[2][0] = 0.0f;
    _ortho_mat[2][1] = 0.0f;
    _ortho_mat[2][2] = - 2.0f / (f-n);
    _ortho_mat[2][3] = C;

    _ortho_mat[3][0] = 0.0f;
    _ortho_mat[3][1] = 0.0f;
    _ortho_mat[3][2] = 0.0f;
    _ortho_mat[3][3] = 1.0f;
  }


  /*! GMWindow::GMWindow(const GMWindow&)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  GMWindow::GMWindow( const GMWindow& gw ) : Scene() {

    (*this) = gw;
  }


  /*! GMWindow::~GMWindow()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  GMWindow::~GMWindow() {}


  void GMWindow::addViewSet( int cam_idx ) {

    if( cam_idx < 1 || cam_idx >= _cameras.getSize() )
      return;

    _view_set_stack += ViewSet(_cameras[cam_idx]);
  }


  /*! void GMWindow::setViewSet(int new_c, int old_c, bool split_vertically, double d)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  void GMWindow::addToViewSet(int cam_idx, int split_cam_idx, bool split_vertically, double d) {

    if(cam_idx<1 || cam_idx>=_cameras.getSize() || split_cam_idx <1 || split_cam_idx>=_cameras.getSize() || d<=0 || d>=1) {

      return;
    }

    _view_set_stack.back().insertCamera(_cameras[split_cam_idx],_cameras[cam_idx], split_vertically,d);

    _view_set_stack.back().prepare(_w,_h);
  }

  void GMWindow::clearViewSetConfiguration() {

    bool running = isRunning();
    if( running )
      toggleRun();

    clearSelection();
//    for(int i = 0; i < _sel_objs.getSize(); i++)
//      _sel_objs[i]->setSelected( false );
//    _sel_objs.clear();

    Array<SceneObject*> rmobjs;
    for( int i = 0; i < getSize(); i++ )
      rmobjs += operator[](i);

    for( int i = 0; i < rmobjs.getSize(); i++ ) {

      Camera* cam   = dynamic_cast<Camera*>( rmobjs[i] );
      if( cam ) {
        removeCamera( cam );
        delete rmobjs[i];
      }
    }

    if( running )
      toggleRun();
  }

  const Color& GMWindow::getSelectColor() const {

    return _select_color;
  }


  /*! void GMWindow::popViewSet(int i)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  void GMWindow::popView(int cam_idx) {

    _view_set_stack.back().removeCamera(cam_idx);
  }


  void GMWindow::popViewSet() {

    if( getViewSetStackSize() > 2 )
      _view_set_stack.pop();
  }

  const Color& GMWindow::getClearColor() const {

    return _clear_color;
  }

  void GMWindow::setClearColor(const Color &color) {

//    _clear_color = color;
//    for( int i = 0; i < getCameras().getSize(); ++i)
//      getCameras()(i)->getRenderer()->setClearColor(color);
  }

  void GMWindow::setSelectColor( const Color& color ) {

    _clear_color = color;
    // update select renderer(s)
  }

  void GMWindow::setRenderTarget(RenderTarget *rt) {

    assert(rt);
    if( _rt ) delete _rt;

    _rt = rt;
  }

  void GMWindow::insertCamera(Camera* cam) {

    assert(cam);

//    cam->getRenderer()->setRenderTarget( new NativeRenderTarget );
//    cam->getRenderer()->setClearColor( getClearColor() );
    Scene::insertCamera(cam);
  }


  /*! bool GMWindow::toggleStereo()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  bool GMWindow::toggleStereo() {

    _stereo = !_stereo;
    return _stereo;
  }

  SceneObject *GMWindow::findSelectObject(Camera *cam, const Vector<int,2> &pos, int type_id) const {

    return findSelectObject( cam, pos(0), pos(1), type_id );
  }

  SceneObject *GMWindow::findSelectObject(Camera *cam, int x, int y, int type_id) const {

    assert(false);
//    RenderManager *rm = getRenderManager();
//    rm->select( cam, type_id );
//    return rm->findObject( x, y );
  }


  /*! GMWindow& GMWindow::operator=(const GMWindow& gw)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  GMWindow& GMWindow::operator=(const GMWindow& gw) {

    Scene::operator=(gw);
//    _cameras  = gw._cameras;
//    _lights  = gw._lights;
//    _sun  = gw._sun;
//    for(int i=0; i<_cameras.getSize(); i++)
//      _cameras[i]->setScene(this);

    _stereo  = false;
//    _running = false;
    _isbig   = false;
//    _sel_objs.clear();
    _view_set_stack = gw._view_set_stack;        // Active camera set
    reshape(_w,_h);
    return *this;
  }



  #ifndef GL_MULTISAMPLE
  #define GL_MULTISAMPLE  0x809D
  #endif
  /*! void GMWindow::init()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  void GMWindow::init() {

    insertCamera(new Camera(Point<float,3>(10,10,5),Point<float,3>(-10,-10,-5),Vector<float,3>(0,0,-1)));
    _view_set_stack += ViewSet(_cameras[0]);
    _view_set_stack.back().prepare(_w,_h);
  }

  void GMWindow::render() {

    if( _active_cam > -1 )
      _cameras[_active_cam]->move(_move);

//    simulate();
//    prepare();
//    if(_stereo) {

//      glDrawBuffer(GL_BACK_LEFT);
//      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//      _view_set_stack.back()._drawCamera();
//      swapBuffers();
//      glDrawBuffer(GL_BACK_RIGHT);
//      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//      _view_set_stack.back()._drawCamera(true);
//    }
//    else {

//      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//      GL::OGL::clearRenderBuffer();
//      GL::OGL::bindRenderBuffer();
//      _view_set_stack.back().drawCamera();
//      GL::OGL::unbindRenderBuffer();

    for( int i = 0; i < _view_set_stack.back().getCameras().getSize(); ++i ) {
      _view_set_stack.back().getCameras()(i)->updateLightUBO(this);
//      _view_set_stack.back().getCameras()(i)->getRenderer()->render(_view_set_stack.back().getCameras()(i));
      assert(false);
    }

    // Draw border
    {
      _rt->bind();



      // Draw what rendered by the camrea to the buffer
//      for( int i = 0; i < _view_set_stack.back().getCameras().getSize(); ++i )
//        _view_set_stack.back().getCameras()(i)->getRenderer()->renderTo();
      assert(false);



      GL_CHECK(::glViewport( 0, 0, _w, _h ));

      // Draw borders
      const ViewSet &top_view_set = getTopViewSet();
      _border_prog.bind();

      _border_prog.uniform( "u_mvpmat", _ortho_mat );
      _border_prog.uniform( "u_selected", false );
      _border_prog.uniform( "u_color", top_view_set.getBorderColor() );

      const GL::AttributeLocation vert_loc = _border_prog.getAttributeLocation( "in_vertex" );
      const GL::VertexBufferObject &border_vbo = top_view_set.getBorderVBO();

      border_vbo.bind();
      border_vbo.enable( vert_loc, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0x0) );

      GL_CHECK(::glPointSize( 10.0f ));
      GL_CHECK(::glDrawArrays( GL_QUADS, 0, top_view_set.getNoBorders() * 4 ));

      border_vbo.disable(vert_loc);
      border_vbo.unbind();

      _border_prog.unbind();

      _rt->unbind();
    }
  }

  void GMWindow::reshape(int w, int h) {

    _w = w; _h = h;
    prepareViewSets();
    _rt->resize(Vector<unsigned int,2>(_w,_h));
  }

} // END namespace GMlib
