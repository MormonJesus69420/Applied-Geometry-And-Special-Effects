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





#include "gmscene.h"

// gmlib
#include <core/utils/gmutils.h>

// local
#include "gmsceneobject.h"
#include "light/gmlight.h"
#include "camera/gmcamera.h"
#include "event/gmeventmanager.h"
#include "light/gmpointlight.h"
#include "light/gmspotlight.h"
#include "light/gmsun.h"


namespace GMlib {



  /*! Scene::Scene()
   *
   *  Default constructor
   */
  Scene::Scene() :
    _scene(),
    _matrix_stack(32),
    _event_manager(0) {

    _matrix_stack += HqMatrix<float,3>();

    init();
  }


  /*! Scene::Scene( SceneObject* obj )
   *
   *  Standard constructor
   */
  Scene::Scene( SceneObject* obj ) :
    _scene(),
    _matrix_stack(32),
    _event_manager(0) {

    _scene += obj;
    _matrix_stack += HqMatrix<float,3>();

    init();
  }

  Scene::Scene( const Scene&  s ) :
    _scene(s._scene),
    _matrix_stack(s._matrix_stack),
    _event_manager(0) {

    init();
  }

  Scene::~Scene() {

    clear();
  }

  SceneObject* Scene::find(unsigned int name) {
    SceneObject* obj;

    for(int i=0; i < _scene.getSize(); i++)
      if( (obj = _scene[i]->find(name)) ) return obj;
    return 0;
  }

  const SceneObject* Scene::find(unsigned int name) const {

    for(int i=0; i < _scene.getSize(); i++)
      if( const SceneObject* obj = _scene(i)->find(name) ) return obj;
    return 0x0;
  }

  void Scene::getRenderList( Array<const SceneObject*> &objs, const Camera *cam)  const {

    const bool is_culling = cam->isCulling();

    if(is_culling) {

      const_cast<Camera*>(cam)->computeFrustumBounds();

      for( int i = 0; i < _scene.getSize(); ++i )
        _scene(i)->getRenderList( objs, *cam );
    }
    else {
      for( int i = 0; i < _scene.getSize(); ++i )
        _scene(i)->getRenderList( objs );
    }
  }

  void Scene::clear() {

    const bool running = isRunning();
    if(running)
      stop();

    // Remove selections
    clearSelection();

    // Remove/(delete) sun
    removeSun();

    // Lights: if in scene remove, then delete
    for( int i = 0; i < _lights.getSize(); ++i ) {
      SceneObject *obj = dynamic_cast<SceneObject*>(_lights(i));
      _scene.remove(obj);
    }
    _lights.clear();

    // Cameras: if in scene remove, then delete
    for( int i = 0; i < _cameras.getSize(); ++i ) _scene.remove(_cameras(i));
    _cameras.clear();

    // Clear rest of scene (remove and delete)
    _scene.clear();

    if(running)
      start();
  }

  void Scene::clearSelection() {

    for(int i = 0; i < _sel_objs.getSize(); i++)
      _sel_objs[i]->setSelected( false );
    _sel_objs.clear();
  }

  int Scene::getSize() const {

    return _scene.getSize();
  }

  Sphere<float,3>  Scene::getSphere() const {

    Sphere<float,3> sp;
    for(int i=0; i< _scene.getSize(); i++)
      sp += _scene(i)->getSurroundingSphere();
    return sp;
  }

  Sphere<float,3>  Scene::getSphereClean() const {

    Sphere<float,3> sp;
    for(int i=0; i< _scene.getSize(); i++)
      sp += _scene(i)->getSurroundingSphereClean();
    return sp;
  }

  void Scene::updateSelection(SceneObject *obj) {

    if( obj->isSelected() )
      _sel_objs.insert(obj);
    else
      _sel_objs.remove(obj);
  }

  void Scene::insert(SceneObject* obj) {

    if(!obj)
      return;

    _scene.insert(obj);
    obj->setParent(0);
  }

  void Scene::insertCamera(Camera *cam, bool insert_in_scene) {

    cam->setScene(this);
    _cameras += cam;

    if(insert_in_scene) insert(cam);
  }

  bool Scene::removeCamera(Camera* cam) {

    if(_cameras.getSize() < 2)
      return false;

    int i = _cameras.getIndex(cam);
    if(i>0) {

      remove(cam);

      _cameras.removeIndex(i);
      return true;
    }
    return false;
  }

  void Scene::prepare() {

    int no_disp_obj = 0;

    for(int i=0; i < _scene.getSize(); i++)
      no_disp_obj += _scene[i]->prepare( _matrix_stack, this );
  }

  void Scene::remove( SceneObject* obj ) {

    if(obj) _scene.remove(obj);
  }

  void Scene::insertLight(Light* light, bool insert_in_scene ) {

    _lights += light;

    SceneObject *obj = dynamic_cast<SceneObject*>(light);

    if(insert_in_scene && obj)
      insert(obj);
  }

  bool Scene::removeLight(Light* light) {

    int i = _lights.getIndex(light);
    if(i>=0) {

      SceneObject* obj = dynamic_cast<SceneObject*>(light);

      if(obj)
        remove(obj);

      _lights.removeIndex(i);

      return true;
    }
    return false;
  }

  void Scene::insertSun() {

    if( _sun ) return;

    _sun = new Sun();
    _sun->scaleDayLight(1.0);
  }

  void Scene::removeSun() {

    if(!_sun) return;

    delete _sun;
    _sun = 0x0;
  }

  void Scene::scaleDayLight(double d) {

    _sun->scaleDayLight(d);
  }

  void Scene::setSunDirection(Angle d) {

    if(_sun)
      _sun->rotateGlobal(d, Vector<float,3>(1.0,1.0,0.0));
  }

  SceneObject* Scene::operator[]( int i ) {

    return _scene[i];
  }

  Scene& Scene::operator=(const Scene& other) {

    _timer_active         = false;
    _timer_time_elapsed   = other._timer_time_elapsed;
    _timer_time_scale     = other._timer_time_scale;

    _scene                = other._scene;
    _event_manager        = other._event_manager;

    _lights               = other._lights;
    _sun                  = other._sun;

    _cameras              = other._cameras;
    for(int i=0; i<_cameras.getSize(); i++)
      _cameras[i]->setScene(this);

    clearSelection();

    return *this;
  }

  bool Scene::isSelected(SceneObject *obj) const {

    return _sel_objs.exist( obj );
  }

  void
  Scene::setEventManager(EventManager *mgr) {
    _event_manager = mgr;
  }

  void Scene::enabledFixedDt() {

    _timer_fixed_dt_enabled = true;
  }

  void Scene::disableFixedDt() {

    _timer_fixed_dt_enabled = false;
  }

  void Scene::setFixedDt(double dt) {

    _timer_fixed_dt = dt;
  }

  void Scene::removeSelections() {

    // make a copy of the array instead of a reference as
    // SceneObject->setSelected(...) updates the selected objects array.
    Array<SceneObject*> sel_objs = getSelectedObjects();


    for( int i = 0; i < sel_objs.getSize(); i++ )
        sel_objs[i]->setSelected( false );
  }

  void Scene::simulate() {

    if( !_timer_active ) return;

    if( GMutils::compValueF(_timer_time_elapsed,0.0) ) prepare();

    double dt, timer_dt;
    timer_dt = _timer.getSec(true);
    if( _timer_fixed_dt_enabled ) dt = _timer_fixed_dt;
    else                          dt = _timer_time_scale * timer_dt;

    if(dt) {

      _timer_time_elapsed  += dt;
      if ( _event_manager ) _event_manager->processEvents(dt);
      for( int i=0; i< _scene.getSize(); i++ )
        _scene[i]->simulate(dt);
    }
  }

  void Scene::init() {

    _timer_active   = false;
    _timer_time_scale    = 1;
    _timer_time_elapsed  = 0;
    _timer_fixed_dt_enabled = false;
    _timer_fixed_dt = 0.25;

    _sun = 0x0;
  }

  const Array<Camera*>& Scene::getCameras() const {

    return _cameras;
  }



  void Scene::getEditedObjects(Array<const SceneObject*>& e_obj) const {

      for(int i=0; i< _scene.getSize(); i++)
          _scene(i)->getEditedObjects(e_obj);
  }



} // END namespace GMlib







