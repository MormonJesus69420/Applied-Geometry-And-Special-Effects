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





#ifndef GM_SCENE_CAMERA_CAMERA_H
#define GM_SCENE_CAMERA_CAMERA_H

#include "../gmsceneobject.h"
#include "../gmscene.h"

// gmlib
#include <core/utils/gmstream.h>

// stl
#include <iostream>




namespace GMlib {



  class Renderer;
  class SelectRenderer;



  /*!  \class Camera gmcamera.h <gmCamera>
   *  \brief  The Camera class
   *
   *  Inneholder:
   *    privat;
   *        Et navn for identifikasjon i select funksjonaliteten.
   *        To matriser for lokal transformasjon av objektet.
   *        En Omsluttende kule til objektet som må lages.
   *        En array av pekere til alle sub-objektene til dette objektet
   *    public:
   *        Konstuktør med mulige default verdier og sletter
   *        En display funksjon for displaying av objektet.
   *        En select funksjon for plukking(select) av objektet med mus.
   *        En simulate funksjon for at objektet kan bevege subobjekter.
   *        En find funksjon for å finne en funksjon med et gitt navn.
   *        En rotate, scale og translate funksjon for å
   *           posisjonere objektet relativt i forhold til morsobjektet
   *    protected:
   *        Det er to virtuelle funksjoner localDisplay og localSelect
   *          som alle avledede klasser må ha sin versjon av.
   *        Arbeidsdelingen er slik at display og select foretar
   *          transformasjonen lagret i matrix, og displayer alle
   *          sub-objektene til objektet. Men etter transformasjonen
   *          og før sub-objektene displayes kalles localDisplay
   *          (eventuelt localSelect). I localDisplay kan en så
   *          displaye det som ikke displayes i sub-objektene og/eller
   *          foreta transformasjoner av sub-objekter for
   *          å simulere bevegelser.
   *        Det er også en insert-funksjon som avledede klasser
   *            kan bruke for å lage sub-objekt. Hvis en avledet klasse
   *          trenger tilgang til et objekt for bevegelsessimulering
   *          bør den lage en egen peker for det.
   */
  class Camera : public SceneObject {
    GM_SCENEOBJECT(Camera)
  public:
    Camera( Scene& s = _default_scene );
    Camera( Scene* s );

    Camera( const Point<float,3>&  pos,
            const Point<float,3>&  look_at_pos);

    Camera( const Point<float,3>&  pos,
            const Vector<float,3>& dir,
            const Vector<float,3>& up,
            float zoom=1);

   Camera( const Camera& copy ) = default;
    virtual ~Camera();

    SceneObject*                findSelectObject(int, int, int type_id=0);
//    SceneObject*                findSelectObject( const Vector<int,2>& pos, int type_id=0);
//    Array<SceneObject* >        findSelectObjects(int xmin, int ymin, int xmax, int ymax, int type_id=0);

    HqMatrix<float,3>&          getMatrix() override;
    const HqMatrix<float,3>&    getMatrix() const override;
    const HqMatrix<float,3>&    getProjectionMatrix() const;
    Arrow<int,2>                getViewport() const;
    void                        getViewport(int& w1, int& w2, int& h1, int& h2) const;
    int                         getViewportW() const;
    int                         getViewportH() const;

//    virtual void                go(bool stereo=false);  // Running the Camera.

    bool                        isCoordSysVisible() const;
    void                        setCoordSysVisible(bool visible=true);


    virtual double              deltaTranslate(SceneObject * obj);
    double                      getDistanceToObject(int, int);
    double                      getDistanceToObject(SceneObject* obj);
    virtual SceneObject*        lockTargetAtPixel(int,int);

    bool                        isCulling() const;
    void                        enableCulling( bool enable = true );

    float                       getAspectRatio() const;
    float                       getNearPlane() const;
    void                        setCuttingPlanes(float near_plane, float far_plane);
    float                       getAngleTan() const;
    float                       getFarPlane() const;

    float                       getFocalLength() const;
    void                        setFocalLength(double focal=50.0);
    void                        decreaseFocalLength(double delta=1);
    void                        increaseFocalLength(double delta=1);

    void                        increaseEyeDist(double delta=0.01);
    void                        decreaseEyeDist(double delta=0.01);
    void                        setEyeDist(double eye_dist=0.08);

    void                        setFrustumVisible(bool visible=true);
    bool                        isFrustumVisible() const;

    void                        setScene(Scene& s);
    void                        setScene(Scene *s);
    virtual void                zoom(float z);


    void                        reset();            // To be used when changing Camera.
    virtual void                reshape(int x1, int y1, int x2, int y2);    // To be used when changing size of window
    void                        updateCameraOrientation();

//  protected:
  public:


    void                        basisChange( const Vector<float,3>& x,
                                             const Vector<float,3>& y,
                                             const Vector<float,3>& z,
                                             const Vector<float,3>& p
                                             ) override;

//    virtual void                display();
    SceneObject*                find(unsigned int name);
    void                        resetC(float z = 1);
//    void                        select(int type_id);
    virtual void                updateFrustum();


//  private:
    static Scene                _default_scene;
    static unsigned int         _display_list;

    Scene*                      _scene;
    int                         _x,_y,_w,_h;          // Viewport position and size.

    bool                        _coord_sys_visible;
    bool                        _frustum_visible;
//    PerspectiveFrustum          _frustum;

    double                      _focal_length;
    double                      _eye_dist;
    double                      _ed_fd;

    bool                        _culling;







    Point<float,3>              _frustum_p[2];    // p[0]: høyre/opp/bak-hjørne  p[1]: venstre/ned/foran-hjørne
    Vector<float,3>             _frustum_v[6];    // normal: venstre, høyre, opp, ned, bak, fram.

    HqMatrix<float,3>           _projection_matrix;

    Point<float,3>              _frustum_frame[8];

    float                       _frustum_near;
    float                       _frustum_far;
    float                       _frustum_angle_tan;


    int isInsideFrustum(const Sphere<float,3>& s) const {

      if(!s.isValid())  return -1;
      int ret = 1;

      Vector<float,3> d = s.getPos()-_frustum_p[0];
      float dv = d*_frustum_v[1];          // Høyre
      if(dv >= s.getRadius())    return -1;
      else if(dv > -s.getRadius())  ret = 0;
      dv = d*_frustum_v[2];                // Opp
      if(dv >= s.getRadius())    return -1;
      else if(dv > -s.getRadius())  ret = 0;
      dv = d*_frustum_v[4];                // Bak
      if(dv >= s.getRadius())    return -1;
      else if(dv > -s.getRadius())  ret = 0;

      d = s.getPos()-_frustum_p[1];
      dv = d*_frustum_v[0];                // Venstre
      if(dv >= s.getRadius())    return -1;
      else if(dv > -s.getRadius())  ret = 0;
      dv = d*_frustum_v[3];                // Ned
      if(dv >= s.getRadius())    return -1;
      else if(dv > -s.getRadius())  ret = 0;
      dv = d*_frustum_v[5];                // Fram
      if(dv >= s.getRadius())    return -1;
      else if(dv > -s.getRadius())  ret = 0;
      return ret;
    }

    const Point<float,3>*     getFrustumFramePtr() const { return _frustum_frame; }

    virtual void              computeProjectionMatrix() {

      float  hh = _frustum_near* _frustum_angle_tan;
      float  rr = getAspectRatio()*hh;

      float l, r, b, t, n, f;
      l = -rr;
      r = rr;
      b = -hh;
      t = hh;
      n = _frustum_near;
      f = _frustum_far;

      float A, B, C, D;
      A =  ( r + l ) / ( r - l );
      B =  ( t + b ) / ( t - b );
      C = - ( f + n ) / ( f - n );
      D = - 2 * f * n / ( f - n );

      _projection_matrix[0][0] = 2.0f / (r - l);
      _projection_matrix[0][1] = 0.0f;
      _projection_matrix[0][2] = A;
      _projection_matrix[0][3] = 0.0f;

      _projection_matrix[1][0] = 0.0f;
      _projection_matrix[1][1] = 2.0f / ( t - b );
      _projection_matrix[1][2] = B;
      _projection_matrix[1][3] = 0.0f;

      _projection_matrix[2][0] = 0.0f;
      _projection_matrix[2][1] = 0.0f;
      _projection_matrix[2][2] = C;
      _projection_matrix[2][3] = D;

      _projection_matrix[3][0] = 0.0f;
      _projection_matrix[3][1] = 0.0f;
      _projection_matrix[3][2] = -1.0f;
      _projection_matrix[3][3] = 0.0f;
    }

    virtual void              computeFrustumBounds() {

      float ratio = getAspectRatio() * _frustum_angle_tan;
      double rr = sqrt(1+ratio*ratio);
      double tt = sqrt(1+_frustum_angle_tan*_frustum_angle_tan);
      Vector<float,3> f  = _matrix_scene*_dir;
      Vector<float,3> oe = _matrix_scene*_up;
      Vector<float,3> ve = _matrix_scene*_side;
      Vector<float,3> pp = ratio*ve-_frustum_angle_tan*oe;
      _frustum_p[0] = _matrix_scene*_pos;      // Venstre, høyre, opp, ned (posisjon)
      _frustum_p[1] = _frustum_p[0]+_frustum_far*(f+pp);
      _frustum_p[0] += _frustum_near*(f-pp);

      _frustum_v[0] = ve-ratio*f;      // Venstre  (normal)
      _frustum_v[1] = (_frustum_v[0]-2*ve)/rr;  // Høyre  (normal)
      _frustum_v[0] /= rr;
      _frustum_v[2] = oe-_frustum_angle_tan*f;      // Opp    (normal)
      _frustum_v[3] = (_frustum_v[2]-2*oe)/tt;  // ned    (normal)
      _frustum_v[2] /= tt;
      _frustum_v[4] = -f;        // Bak    (normal)
      _frustum_v[5] = f;        // Fram    (normal)

    }

    virtual void              computeFrustumFrame() {

      float ratio = getAspectRatio();

      Point<float,3>  p1(0,0,-_frustum_near);
      Point<float,3>  p2(0,0,-_frustum_far);

      Vector<float,3> v1(0,_frustum_angle_tan*_frustum_near,0);
      Vector<float,3> v2(-ratio*_frustum_angle_tan*_frustum_near,0,0);
      Vector<float,3> v3(0,_frustum_angle_tan*_frustum_far,0);
      Vector<float,3> v4(-ratio*_frustum_angle_tan*_frustum_far,0,0);

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



  public:
    void    switchToLeftEye() { basisChange( _side, _up, _dir, _pos ); }

    void    switchToRightEye() {

      Point<float,3>         tmp_pos  = _pos  - _eye_dist*_side;
      UnitVector<float,3>    tmp_dir  = _dir  + _ed_fd*_side; //tmp_dir  = _pos + _focus_dist*_dir - tmp_pos;
      UnitVector<float,3>    tmp_side = _side - _ed_fd*_dir;  //tmp_side = _up^tmp_dir;
      basisChange(tmp_side, _up, tmp_dir, tmp_pos);            // Change to right eye
    }


  }; // END class Camera









  /*! void Camera::basisChange(const Vector<float,3>& x, const Vector<float,3>& y, const Vector<float,3>& z, const Vector<float,3>& p)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::basisChange( const Vector<float,3>& x,
                            const Vector<float,3>& y,
                            const Vector<float,3>& z,
                            const Vector<float,3>& p ) {

    static Vector<float,4> nx, ny, nz, nw(0.0f);
    nx = -x;
    ny =  y;
    nz = -z;

    nx[3] = x*p;
    ny[3] = -(y*p);
    nz[3] = z*p;
    nw[3] = 1.0f;

    _matrix.setRow( nx, 0 );
    _matrix.setRow( ny, 1 );
    _matrix.setRow( nz, 2 );
    _matrix.setRow( nw, 3 );
  }


  /*! void Camera::decreaseEyeDist(double delta)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::decreaseEyeDist(double delta) {

    _eye_dist-=delta;
    _ed_fd=_eye_dist/_focal_length;
  }


  /*! void Camera::decreaseFocalDist(double delta)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::decreaseFocalLength(double delta) {

    increaseFocalLength(-delta);
  }


//  /*! void Camera::display()
//   *  \brief Pending Documentation
//   *
//   *  Pending Documentation
//   */
//  inline
//  void Camera::display() {

//    setupDisplay();

//    // Cull the scene using the camera's frustum
//    _scene->culling( _frustum, _culling );

//    // Render scene
//    _scene->display( this );

//    if(_coord_sys_visible)
//      drawActiveCam();
//  }


  /*! void Camera::enableCulling( bool enable )
   *  \brief Enable or disable culling
   *
   *  Enable or disable Culling for this camera
   */
  inline
  void Camera::enableCulling( bool enable ) {

    _culling = enable;
  }


  /*! SceneObject* Camera::find(unsigned int name
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  SceneObject* Camera::find(unsigned int name) {

    return _scene->find(name);
  }


  /*! float Camera::getAngleTan() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  float Camera::getAngleTan() const {

    return _frustum_angle_tan;
  }


  /*! float Camera::getFarPlane() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  float Camera::getFarPlane() const  {

    return _frustum_far;
  }

  /*! float Camera::getFocalLength() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  float Camera::getFocalLength() const {

    return _focal_length;
  }


  /*! float Camera::getNearPlane() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  float Camera::getNearPlane() const {

    return _frustum_near;
  }


  inline
  const HqMatrix<float,3>& Camera::getProjectionMatrix() const {

    return _projection_matrix;
  }


  /*! float Camera::getRatio() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  float Camera::getAspectRatio() const {

    return float(_w)/float(_h);
  }

  inline
  Arrow<int,2>
  Camera::getViewport() const {

    Arrow<int,2> vp;
    vp.setPos( Point<int,2>(_x,_y) );
    vp.setDir( Vector<int,2>(_w,_h) );
    return vp;
  }

  /*!
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::getViewport(int& w1, int& w2, int& h1, int& h2) const {

    w1 = _x;
    h1 = _y;
    w2 = _x+_w;
    h2 = _y+_h;
  }


  /*! int Camera::getViewPortW() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  int Camera::getViewportW() const {

    return _w;
  }


  /*! int Camera::getViewPortH() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  int Camera::getViewportH() const {

    return _h;
  }


  /*! void Camera::increaseEyeDist(double delta)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::increaseEyeDist(double delta) {

    _eye_dist+=delta;
    _ed_fd=_eye_dist/_focal_length;
  }


  /*! void Camera::increaseFocalDist(double delta)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::increaseFocalLength(double delta) {

    setFocalLength(_focal_length+delta);
  }


  /*! bool Camera::isCoordSysVisible() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool Camera::isCoordSysVisible() const {

    return _coord_sys_visible;
  }


  /*! bool Camera::isCulling() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool Camera::isCulling() const {

    return _culling;
  }


  /*! bool Camera::isFrustumVisible() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool Camera::isFrustumVisible() const {

    return _frustum_visible;
  }


  /*! void Camera::reset()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  To be used when changing Camera.
   */
  inline
  void Camera::reset() {

    int  wp[4];
    glGetIntegerv(GL_VIEWPORT,wp);
    reshape(wp[0],wp[1],wp[2],wp[3]);
  }


  /*! void Camera::setCuttingPlanes(float near_plane, float far_plane)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::setCuttingPlanes(float near_plane, float far_plane) {

    _frustum_near      = near_plane;
    _frustum_far       = far_plane;
    _frustum_angle_tan = 13.0f*_frustum_near/_focal_length;
    updateFrustum();
  }


  /*! void Camera::setCoordSysVisible(bool visible)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::setCoordSysVisible(bool visible) {

    _coord_sys_visible=visible;
  }


  /*! void Camera::setEyeDist(double eye_dist)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::setEyeDist(double eye_dist) {

    _eye_dist=eye_dist;
    _ed_fd=_eye_dist/_focal_length;
  }


  /*! void Camera::setFocalDist(double focal)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::setFocalLength(double focal)  {

    _focal_length      = focal;
    _ed_fd             = _eye_dist/_focal_length;
    _frustum_angle_tan = 13.0f*_frustum_near/_focal_length;
    updateFrustum();
  }


  /*! void Camera::setFrustumVisible(bool visible=true)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::setFrustumVisible(bool visible) {

    _frustum_visible = visible;
  }


  /*! void Camera::setScene(Scene& s)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::setScene(Scene& s) {

    _scene = &s;
  }


  /*! void Camera::setScene(Scene *s)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Camera::setScene(Scene *s) {

    _scene = s;
  }


  /*! void Camera::updateCameraOrientation()
   *    \brief Pending Documentation
   *
   *    Pending Documentation
   */
  inline
  void Camera::updateCameraOrientation() {

    if( _locked ) {

      updateOrientation( getSceneLockPos() );
      basisChange( _side, _up, _dir, _pos );
    }
  }


} // END namespace GMlib


#endif // GM_SCENE_CAMERA_CAMERA_H
