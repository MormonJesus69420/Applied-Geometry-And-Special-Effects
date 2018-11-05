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





#ifndef GM_SCENE_CAMERA_ISOCAMERA_H
#define GM_SCENE_CAMERA_ISOCAMERA_H


#include "gmcamera.h"

// gmlib
#include <core/utils/gmstream.h>

namespace GMlib {



  /*! \class IsoCamera gmisocamera.h <gmIsoCamera>
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  class IsoCamera : public Camera {
    GM_SCENEOBJECT(IsoCamera)
  public:
    IsoCamera(Scene* s);
    IsoCamera(
      const Point<float,3>&  pos,
      const Point<float,3>&  look_pos
    );
    IsoCamera(
      const Point<float,3>&  pos,
      const Vector<float,3>& dir,
      const Vector<float,3>& up,
      float zoom=1
    );
    IsoCamera( const IsoCamera& copy ) = default;
    ~IsoCamera();

    double          deltaTranslate(SceneObject *) override;

//    void             go(bool stereo=false);
    void            lock(SceneObject* /*obj*/) override {}         //!< Disable locking
    void            lock(const Point<float,3>& /*p*/) override {}  //!< Disable locking
    void            lock(double /*d*/) override {}                 //!< Disable locking
    void            move(float d, bool propagate = true) override;
    void            move(const Vector<float,3>& t, bool propagate = true) override;


    void            rotate(Angle, const Vector<float,3>&, bool propagate = true) override;
    void            rotate(Angle, const Point<float,3>&,const UnitVector<float,3>&, bool propagate = true) override;
    void            rotateParent(Angle, const Vector<float,3>&, bool propagate = true) override;
    void            rotateParent(Angle, const Point<float,3>&,const UnitVector<float,3>&, bool propagate = true) override;
    void            rotateGlobal(Angle, const Vector<float,3>&, bool propagate = true) override;
    void            rotateGlobal(Angle, const Point<float,3>&,const UnitVector<float,3>&, bool propagate = true) override;

    void            setGrid(double w, double h);
    void            tilt(Angle a, bool propagate = true) override;
    void            translate(const Vector<float,3>& trans_vector, bool propagate = true) override;
    void            translateParent(const Vector<float,3>& trans_vector, bool propagate = true) override;
    void            translateGlobal(const Vector<float,3>&, bool propagate = true) override;
    void            turn(Angle a, bool propagate = true) override;
    void            zoom(float z) override;





    void            computeProjectionMatrix() override {

      float  hh = _horizontal;
      float  rr = getAspectRatio()*_horizontal;

      float l, r, b, t, n, f;
      l = -rr;
      r = rr;
      b = -hh;
      t = hh;
      n = getNearPlane();
      f = getFarPlane();

      float tx, ty, tz;
      tx = -(r+l)/(r-l);
      ty = -(t+b)/(t-b);
      tz = -(f+n)/(f-n);

      _projection_matrix[0][0] = 2.0f / (r - l);
      _projection_matrix[0][1] = 0.0f;
      _projection_matrix[0][2] = 0.0f;
      _projection_matrix[0][3] = tx;

      _projection_matrix[1][0] = 0.0f;
      _projection_matrix[1][1] = 2.0f / ( t - b );
      _projection_matrix[1][2] = 0.0f;
      _projection_matrix[1][3] = ty;

      _projection_matrix[2][0] = 0.0f;
      _projection_matrix[2][1] = 0.0f;
      _projection_matrix[2][2] = (-2.0f)/(f-n);
      _projection_matrix[2][3] = tz;

      _projection_matrix[3][0] = 0.0f;
      _projection_matrix[3][1] = 0.0f;
      _projection_matrix[3][2] = 0.0f;
      _projection_matrix[3][3] = 1.0f;
    }


    void            computeFrustumBounds() override {

      float ratio = getAspectRatio();

      Vector<float,3> f  = _matrix_scene*_dir;
      Vector<float,3> oe = _matrix_scene*_up;
      Vector<float,3> ve = _matrix_scene*_side;
      _frustum_p[0] = _matrix_scene*_pos;
      _frustum_p[1] = _frustum_p[0]+_frustum_far*f+_horizontal*(ratio*ve-oe);
      _frustum_p[0] += _frustum_near*f-_horizontal*(ratio*ve-oe);
      _frustum_v[0] = ve;        // Venstre  (normal)
      _frustum_v[1] = -ve;        // Høyre  (normal)
      _frustum_v[2] = oe;        // Opp    (normal)
      _frustum_v[3] = -oe;        // ned    (normal)
      _frustum_v[4] = -f;        // Bak    (normal)
      _frustum_v[5] = f;        // Fram    (normal)
    }


    void            computeFrustumFrame() override {

      float ratio = getAspectRatio();

      Point<float,3>  p1(0,0,-_frustum_near);
      Point<float,3>  p2(0,0,-_frustum_far);

      Vector<float,3> v1(0,_horizontal,0);
      Vector<float,3> v2(-ratio*_horizontal,0,0);
      Vector<float,3> v3(0,_horizontal,0);
      Vector<float,3> v4(-ratio*_horizontal,0,0);


      Point<float,3>  p1m(p1-v1);
      Point<float,3>  p2m(p2-v3);
      p1 += v1;
      p2 += v3;

      _frustum_frame[0] = p1  + v2;
      _frustum_frame[1] = p1  - v2;
      _frustum_frame[2] = p1m - v2;
      _frustum_frame[3] = p1m + v2;
      _frustum_frame[4] = p2  + v4;
      _frustum_frame[5] = p2  - v4;
      _frustum_frame[6] = p2m - v4;
      _frustum_frame[7] = p2m + v4;

    }




  protected:

  private:
    double          _horizontal;
    double          _gh,_gw;

    // ***********
    // From Camera
    void            drawActiveCam();
    SceneObject*    find(unsigned int name);        /// \todo ????? NOT IMPLEMENTED !!`????? why??
    void            resetC(float z=1);


  };  // END class IsoCamera


  /*! void IsoCamera::drawActivCam()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void IsoCamera::drawActiveCam() {

//    int i,ih,iw;
//    iw = _horisontal/_gw;
//    ih = _ratio*_horisontal/_gh;
//    Point<float,3> cp=_pos+(0.99*_near_plane+0.01*_far_plane)*_dir;
//    GLboolean lg;
//    glGetBooleanv(GL_LIGHTING,&lg);
//    if(lg) glDisable(GL_LIGHTING);
//    Color gr(120,120,120);
//    glColor(gr);
//    glBegin(GL_LINES);
//      Vector<float,3> dside = _gw*_side;
//      Vector<float,3> dup   = _gh*_up;
//      Vector<float,3> side  = ih*dside;
//      Vector<float,3> up    = iw*dup;
//      for(i=-iw; i<=iw; i++)
//      {
//        glPoint(cp+double(i)*dup-side);
//        glPoint(cp+double(i)*dup+side);
//      }
//      for(i=-ih; i<=ih; i++)
//      {
//        glPoint(cp-up+double(i)*dside);
//        glPoint(cp+up+double(i)*dside);
//      }
//    glEnd();
//    if (lg) glEnable(GL_LIGHTING);
  }


  /*! void IsoCamera::move(float d)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  Moveing in shooting direction
   */
  inline
  void IsoCamera::move(float /* delta */, bool /*propagate*/) {}


  /*! void IsoCamera::move(const Vector<float,3>& t)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  Move in direction t.
   */
  inline
  void IsoCamera::move( const Vector<float,3>& /* delta */, bool /*propagate*/ ) {}

  inline
  void IsoCamera::rotate( Angle /*angle*/, const Vector<float,3>& /* axel */, bool /*propagate*/ ) {}

  inline
  void IsoCamera::rotate(Angle, const Point<float,3>&,const UnitVector<float,3>&, bool /*propagate*/) {}

  inline
  void IsoCamera::rotateParent( Angle /*angle*/, const Vector<float,3>& /* axel */, bool /*propagate*/ ) {}

  inline
  void IsoCamera::rotateParent(Angle, const Point<float,3>&,const UnitVector<float,3>&, bool /*propagate*/) {}

  inline
  void IsoCamera::rotateGlobal(Angle, const Vector<float,3>&, bool /*propagate*/) {}

  inline
  void IsoCamera::rotateGlobal(Angle, const Point<float,3>&,const UnitVector<float,3>&, bool /*propagate*/) {}

  inline
  void IsoCamera::setGrid(double w, double h) {
    _gw=w;
    _gh=h;
  }

  inline
  void IsoCamera::tilt( Angle /* angle */, bool /*propagate*/ ) {}

  inline
  void IsoCamera::translate( const Vector<float,3>& /*trans_vector*/, bool /*propagate*/ ) {}

  inline
  void IsoCamera::translateParent( const Vector<float,3>& /*trans_vector*/, bool /*propagate*/ ) {}

  inline
  void IsoCamera::translateGlobal( const Vector<float,3>& /*trans_vector*/, bool /*propagate*/ ) {}

  inline
  void IsoCamera::turn( Angle /*angle*/, bool /*propagate*/ ) {}






}  // END namespace GMlib

#endif // GM_SCENE_CAMERA_ISOCAMERA_H
