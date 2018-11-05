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





#include "gmcamera.h"

// local
#include "../utils/gmmaterial.h"
//#include "../render/gmrendermanager.h"
#include "../render/gmdefaultrenderer.h"
#include "../light/gmspotlight.h"
#include "../light/gmpointlight.h"
#include "../light/gmsun.h"

// gmlib
#include <core/types/gmpoint.h>
#include <core/containers/gmdvector.h>
#include <opengl/gmprogram.h>

//stl
#include <iostream>
#include <cassert>

namespace GMlib {


  Scene  Camera::_default_scene;
  unsigned int  Camera::_display_list = 0;




  /*! Camera::Camera( Scene& s = _default_scene )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  Camera::Camera( Scene& s ) : SceneObject(),_scene(&s) {

    resetC();
  }


  /*! Camera::Camera( Scene* s )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  Camera::Camera( Scene* s ) : SceneObject(),_scene(s) {

    resetC();
  }


  /*! Camera::Camera( const Point<float,3>& pos, const Point<float,3>& look_at_pos )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  Camera::Camera(
    const Point<float,3>&  pos,
    const Point<float,3>&  look_pos
  ) : SceneObject(look_pos,pos), _scene(&_default_scene) {

    resetC();
  }

  /*! Camera::Camera( const Point<float,3>&  pos,  const Vector<float,3>& dir,  const Vector<float,3>& up, float zoom )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  Camera::Camera(
    const Point<float,3>&  pos,
    const Vector<float,3>& dir,
    const Vector<float,3>& up,
    float zoom
  ) : SceneObject(pos,dir,up),_scene(&_default_scene) {

    resetC(zoom);
  }


  /*! Camera::~Camera()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  Camera::~Camera() {}


  /*! double Camera::deltaTranslate(SceneObject *)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  double Camera::deltaTranslate(SceneObject* obj) {

    if(obj)
      return 26*(getDir()*(obj->getPos()-getPos()))*getNearPlane()/(getFocalLength()*getViewportH());
    else
      return 0.0;
  }

  /*! double Camera::getDistanceToObject(int x, int y)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  double Camera::getDistanceToObject(int /*x*/, int /*y*/) {

    assert(false);
    return getDistanceToObject(0x0);
//    SceneObject*   obj = getScene()->getRenderManager()->findObject(x,y);
//    return getDistanceToObject(obj);
  }


  /*! double Camera::getDistanceToObject(SceneObject*)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  double Camera::getDistanceToObject(SceneObject* obj) {

    if(obj) {

      const HqMatrix<float,3> mat = _matrix*_matrix_scene_inv;
      const Vector<float,3> dv = mat*Point<float,3>(obj->getCenterPos());
      return dv.getLength();
    }
    else
      return 0.0;
  }


  /*! HqMatrix<float,3>& Camera::getMatrix()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  HqMatrix<float,3>& Camera::getMatrix() {

    /*! \todo fix how the matrix is returned */
    static HqMatrix<float,3> retmat;
    retmat = _matrix;
    retmat.invertOrthoNormal();
    return retmat;
  }


  const HqMatrix<float,3>& Camera::getMatrix() const {

    /*! \todo fix how the matrix is returned */
    static HqMatrix<float,3> retmat;
    retmat = _matrix;
    retmat.invertOrthoNormal();
    return retmat;
  }


//  /*! void Camera::go(bool stereo)
//   *  \brief Pending Documentation
//   *
//   *  Pending Documentation
//   *  Running the Camera.
//   */
//  void Camera::go(bool stereo) {



//    _active = true;
////    if (stereo)
////    {
////      Point3D<float>    tmp_pos  = _pos  - _eye_dist*_side;
////      UnitVector3D<float>  tmp_dir  = _dir  + _ed_fd*_side; //tmp_dir  = _pos + _focus_dist*_dir - tmp_pos;
////      UnitVector3D<float>  tmp_side = _side - _ed_fd*_dir;  //tmp_side = _up^tmp_dir;
////      basisChange(tmp_side, _up, tmp_dir, tmp_pos);      // Change to right eye
////      display();
////      basisChange(_side, _up, _dir, _pos);            // Back to left eye
////    }
////    else
//    {
//      Point<float,3>    tmp_pos  = _pos  - _eye_dist*_side;
//      UnitVector<float,3>  tmp_dir  = _dir  + _ed_fd*_side; //tmp_dir  = _pos + _focus_dist*_dir - tmp_pos;
//      UnitVector<float,3>  tmp_side = _side - _ed_fd*_dir;  //tmp_side = _up^tmp_dir;
//      basisChange(tmp_side, _up, tmp_dir, tmp_pos);      // Change to right eye
//      display();
//    }

//    _active = false;
//  }



  /*! SceneObject* Camera::lockTargetAtPixel(int x, int y)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  SceneObject* Camera::lockTargetAtPixel(int /*x*/, int /*y*/) {

    assert(false);
    return 0x0;
//    SceneObject*   obj = getScene()->getRenderManager()->findObject(x,y);
//    if(obj)
//    {
//      lock(obj);
//      return obj;
//    }
//    return 0;
  }


  /*! void Camera::reshape(int w1, inth1, int w2, int h2)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  To be used when changing size of window
   */
  void Camera::reshape(int x1, int y1, int x2, int y2) {

    _x = x1;
    _y = y1;
    _w = x2-x1;
    _h = y2-y1;
    updateFrustum();

  }


  /*! void Camera::zoom(float z)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  void Camera::zoom(float z) {

    setFocalLength(z*_focal_length);
  }


  /*! void Camera::resetC(float z)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  NB!!! Only for the constructors.
   */
  void Camera::resetC(float z) {

    basisChange(_side, _up, _dir, _pos);  // Must repete because the virtual functions do not work proper jet.
    _frustum_near = 1.0;
    _frustum_far  = 100.0;
    _eye_dist  = 0.08;
    setFocalLength(z*50);
    setSurroundingSphere(Sphere<float,3>(Point<float,3>(0,0,0),1.0));
    setCoordSysVisible();
    setFrustumVisible();
    _type_id  = GM_SO_TYPE_CAMERA;
    _culling = true;
  }


  void Camera::updateFrustum() {

    computeProjectionMatrix();
    computeFrustumBounds();
    computeFrustumFrame();
  }















































} // END namespace GMlib
