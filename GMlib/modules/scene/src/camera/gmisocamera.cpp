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





#include "gmisocamera.h"

namespace GMlib {


  /*! IsoCamera::IsoCamera(Scene* s)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  IsoCamera::IsoCamera(Scene* s) : Camera(s) {

    resetC();
  }


  /*!IsoCamera::IsoCamera( const Point<float,3>&  pos, const Point<float,3>&  look_pos )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  IsoCamera::IsoCamera(
    const Point<float,3>&  pos,
    const Point<float,3>&  look_pos
  ) : Camera(pos,look_pos) {

    resetC();
  }


  /*! IsoCamera::IsoCamera( const Point<float,3>&  pos, const Vector<float,3>& dir, const Vector<float,3>& up, float zoom )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  IsoCamera::IsoCamera(
    const Point<float,3>&  pos,
    const Vector<float,3>& dir,
    const Vector<float,3>& up,
    float zoom
  ) : Camera(pos,dir,up,zoom) {

    resetC(zoom);
  }


  /*! IsoCamera::~IsoCamera()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  IsoCamera::~IsoCamera() {}


  /*! double IsoCamera::deltaTranslate(SceneObject *)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  double IsoCamera::deltaTranslate(SceneObject * obj) {

    if(obj)
      return 2*_horizontal/getViewportH();
    else
      return 0.0;
  }


//  /*! void IsoCamera::go(bool stereo)
//   *  \brief Pending Documentation
//   *
//   *  Pending Documentation
//   *  Running the IsoCamera.
//   */
//  void IsoCamera::go(bool /*stereo*/) {

//    _active = true;
//    display();
//    _active = false;
//  }




  /*! void IsoCamera::resetC(float z)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  Only for the constructors.
   */
  void IsoCamera::resetC(float z) {

    _horizontal  = z;
    _gw  = _gh  = 0.1;
    updateFrustum();
  }


  /*! void IsoCamera::zoom(float z)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  void IsoCamera::zoom(float z) {

    _horizontal *= z;
    updateFrustum();
  }

}
