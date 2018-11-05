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



/*! \file gmwindow.h
 *
 *  Interface for the Window and "Camera" View classes.
 */


#ifndef __gmWINDOW_H__
#define __gmWINDOW_H__


// local
#include "gmview.h"
#include "gmviewset.h"

// local
#include "../camera/gmcamera.h"

// gmlib
#include <opengl/bufferobjects/gmvertexbufferobject.h>
#include <opengl/bufferobjects/gmindexbufferobject.h>

// stl
#include <iostream>



namespace GMlib {


  // local
  class RenderTarget;




  /* \class GMWindow gmWindow.h <gmWindow>
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  class GMWindow : public Scene {
  public:
    GMWindow();
    GMWindow(const GMWindow&);

    virtual ~GMWindow();


    void                    addToViewSet( int cam_idx, int split_cam_idx, bool split_vertically=true, double d=0.5 );
    void                    addViewSet( int cam_idx );
    int                     getViewSetStackSize() const;
    void                    popView(int cam_idx);
    void                    popViewSet();
    void                    prepareViewSets();


    Camera*                 findCamera( int x, int y );
    Camera*                 findCamera( const Vector<int,2>& pos );
    int                     getCameraIndex( Camera* cam ) const;

    Vector<int,2>           mapToCamera( int x, int y );
    Vector<int,2>           mapToCamera( const Vector<int,2>& pos );

    const Color&            getClearColor() const;
    void                    setClearColor( const Color& color );
    const Color&            getSelectColor() const;
    void                    setSelectColor( const Color& color );

    int                     getViewportHeight() const;
    int                     getViewportWidth() const;

    void                    moveBorder(int x, int y);

    void                    clearViewSetConfiguration();
    void                    reset();

    bool                    isStereoEnabled() const;
    bool                    toggleStereo();

    GMWindow&               operator=(const GMWindow& gw);


    void                    render();
    void                    reshape(int w, int h);

    void                    setRenderTarget( RenderTarget* rt );


    /* Virtual from Scene */
    void                    insertCamera(Camera *cam);


  protected:
    RenderTarget*           _rt;
    Color                   _clear_color;
    Color                   _select_color;

    GL::Program             _border_prog;
    HqMatrix<float,3>       _ortho_mat;

    // Viewport size
    int                     _w;
    int                     _h;          // GMWindow size in viewport coordinates.


    Array<ViewSet>          _view_set_stack;  /// Active camera set

    // index of active camera, old/previous x and y mouse positions
    int                     _active_cam;

    double                  _move;

    bool                    find(int x, int y, int& index);
    virtual void            message( const std::string& str);
    virtual void            message( SceneObject* d);
    virtual void            message( Point<float,3>& p);

  public:
    virtual void            init();


  protected:
    void                    updateMaxObjects(int no_objects);

  private:
    bool                    _stereo;

    SceneObject*            _target;    /// NB!!!! take a look at this variable not used proper today.....
    bool                    _isbig;      /// State of one window functionality have been used (see _mouseDoubleClick on right knob)

    GL::VertexBufferObject  _std_rep_cube;
    GL::IndexBufferObject   _std_rep_cube_indices;
    GL::IndexBufferObject   _std_rep_frame_indices;

  public:
    SceneObject*            findSelectObject( Camera* cam, const Vector<int,2>& pos, int type_id ) const;
    SceneObject*            findSelectObject( Camera* cam, int x, int y, int type_id ) const;


    const ViewSet&          getTopViewSet() const;



  }; // END class GMWindow

















  inline
  const ViewSet& GMWindow::getTopViewSet() const {

    return _view_set_stack.back();
  }




  inline
  Camera* GMWindow::findCamera( int x, int y ) {

    int i;
    find( x, y, i );
    if( i > 0 )
      return _cameras[i];

    return 0x0;
  }

  inline
  Camera* GMWindow::findCamera( const Vector<int,2>& pos ) {

    return findCamera( pos(0), pos(1) );
  }

  inline
  int GMWindow::getCameraIndex( Camera* cam ) const {

    if( !_cameras.exist( cam ) )
      return -1;

    return _cameras.index( cam );
  }

  inline
  Vector<int,2>
  GMWindow::mapToCamera(int x, int y) {

    Camera *cam = findCamera(x,y);
    if(!cam)
      return Vector<int,2>(0);

    return Vector<int,2>( x, y) - cam->getViewport().getPos();
  }

  inline
  Vector<int,2>
  GMWindow::mapToCamera(const Vector<int,2>& pos) {

    return mapToCamera( pos(0), pos(1) );
  }

  inline
  int GMWindow::getViewportHeight() const {

    return _h;
  }

  inline
  int GMWindow::getViewportWidth() const {

    return _w;
  }

  inline
  int GMWindow::getViewSetStackSize() const {

    return _view_set_stack.getSize();
  }


  /*! bool GMWindow::isStereoEnabled()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool GMWindow::isStereoEnabled() const {

    return _stereo;
  }

  inline
  void GMWindow::prepareViewSets() {

    _view_set_stack.back().prepare( _w, _h );
    _view_set_stack.back().prepareGraphics();
  }


  /*! void GMWindow::reset()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void GMWindow::reset() {

    init();
  }

  inline
  void GMWindow::updateMaxObjects(int no_objects) {

//    _rm->updateMaxObjects(no_objects);
  }


  /*! void GMWindow::message( const std::string& str)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void GMWindow::message( const std::string& /* str */ ) {}


  /*! void GMWindow::message( SceneObject* d)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void GMWindow::message( SceneObject* /* obj */ ) {}


  /*! void GMWindow::message( Point<float,3>& p)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void GMWindow::message( Point<float,3>& /* p */ ) {}


  /*! bool GMWindow::find(int x, int y, int& index)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool GMWindow::find(int x, int y, int& index) {

    if(x<0 || x>_w || y<0 || y>_h)    // Outside window
    {
      index = 0;
      _view_set_stack.back().reset();
      return false;
    }

    Camera* cam;
    if(_view_set_stack.back().find(x,y,cam))    // Camera found
    {
      index = _cameras.index(cam);
      return true;
    }
    else                // border found
    {
      index = -1;
      return false;
    }
  }


  /*! void GMWindow::moveBorder(int x, int y)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void GMWindow::moveBorder(int x, int y) {

    if(x>0 && x <_w && y>0 && y<_h)
      (_view_set_stack.back()).moveBorder(x,y);
  }

} // END namespace GMlib



#endif // __gmWINDOW_H__
