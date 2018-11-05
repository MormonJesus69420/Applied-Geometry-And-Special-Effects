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





#ifndef GM_SCENE_SCENE_H
#define GM_SCENE_SCENE_H


// gmlib
#include <core/types/gmpoint.h>
#include <core/utils/gmtimer.h>
#include <core/containers/gmarray.h>
#include <core/utils/gmsortobject.h>
#include <opengl/bufferobjects/gmuniformbufferobject.h>


namespace GMlib{

  class EventManager;
  class SceneObject;
  class Camera;
  class Light;
  class Sun;



  /*! \class Scene gmscene.h <gmscene>
   *  \brief Pending Documentation cleanup, and general documentation
   *
   *    Inneholder: Scenen er startnoden i ojecttreet
   *
   *        privat;
   *                En array av pekere til førstenode objektene i ojecttreet
   *        public:
   *                Konstuktør med mulige default verdier og sletter
   *                En display funksjon for displaying av objektet.
   *                En select funksjon for plukking(select) av objektet med mus.
   *                En simulate funksjon for at objektet kan bevege seg eller subobjekter.
   *                En find funksjon for å finne en funksjon med et gitt navn.
   *                En insert funksjon for å legge inn et nytt førstenode objekt
   *                En remove funksjon for å fjerne et førstenode objekt
   *                En operator [] som returnerer et gitt føstenode-element.
   *                En funksjon size() som gir antallet føstenode-element.
   *                En funksjon getMatrix(obj,mat,type) som gir transformasjonsmatrisen
   *                   fra scenen til og med obj hvis type er 1 og til obj hvis type er 2.
   */
  class Scene {
  public:
    Scene();
    Scene( SceneObject* obj );
    Scene( const Scene&  s );
    virtual ~Scene();

    SceneObject*                find(unsigned int name);
    const SceneObject*          find(unsigned int name) const;
    SceneObject*                getActiveObject();
    int                         getSize() const;
    virtual void                insert(SceneObject* obj);   //!< Inserts obj into the scene. Scene takes ownership.
    void                        remove(SceneObject* obj);   //!< Removes obj from scene if it exists. Scene releases ownership.

    void                        getRenderList(Array<const SceneObject*>& disp_objs, const Camera* cam) const;

    Array<Light*>&              getLights();
    const Array<Light*>&        getLights() const;
    void                        insertLight(Light* light, bool insert_in_scene = false);
    bool                        removeLight(Light* light);

    const Sun*                  getSun() const;
    void                        insertSun();
    void                        removeSun();
    void                        scaleDayLight(double d);
    void                        setSunDirection(Angle d);

    const Array<Camera*>&       getCameras() const;
    virtual void                insertCamera(Camera* cam, bool insert_in_scene = true);
    virtual bool                removeCamera(Camera* cam);

    void                        clear();

    void                        prepare();
    void                        simulate();
    bool                        isRunning() const;
    virtual bool                toggleRun();
    void                        enabledFixedDt();
    void                        disableFixedDt();
    void                        setFixedDt( double dt );
    double                      getElapsedTime() const;
    double                      getTimeScale() const;
    void                        setTimeScale(double s);
    void                        resetTime();
    void                        start();
    void                        stop();


    Sphere<float,3>             getSphere() const;
    Sphere<float,3>             getSphereClean() const;

    const Array<SceneObject*>&  getSelectedObjects() const;
    void                        updateSelection(SceneObject *obj );
    bool                        isSelected( SceneObject* obj ) const;
    void                        removeSelections();
    void                        clearSelection();

    void                        getEditedObjects(Array<const SceneObject*>& e_obj) const;
    void                        setEventManager( EventManager* mgr );


    SceneObject*                operator [] (int i);
    SceneObject*                operator () (int i);
    Scene&                      operator =  (const Scene& sc);

  private:
    Array<Camera*>              _cameras;
    Array<SceneObject*>         _scene;
    Array<Light*>               _lights;

    Sun*                        _sun;

    Array<SceneObject*>         _sel_objs;

    Array<HqMatrix<float,3> >   _matrix_stack;

    GMTimer                     _timer;
    bool                        _timer_active;
    double                      _timer_time_elapsed;
    double                      _timer_time_scale;
    double                      _timer_fixed_dt;
    bool                        _timer_fixed_dt_enabled;

    EventManager*               _event_manager;


    void                        init();




  }; // END class Scene






  inline
  double Scene::getElapsedTime() const {

    return _timer_time_elapsed;
  }

  /*! Array<Light*>& Scene::getLights()
   *  \brief Returns the scene's light sources.
   *
   *  Returns a reference to the Array containing the light sources
   *  in the scene
   *
   *  \return A reference to the scene's light source.
   */
  inline
  const Array<Light*>& Scene::getLights() const {

    return _lights;
  }

  inline
  const Sun* Scene::getSun() const {

    return _sun;
  }

  inline
  const Array<SceneObject*>& Scene::getSelectedObjects() const {

    return _sel_objs;
  }

  inline
  double Scene::getTimeScale() const {

    return _timer_time_scale;
  }

  inline
  bool Scene::isRunning() const {

    return _timer_active;
  }

  inline
  bool Scene::toggleRun() {

    if( !isRunning() )
      start();
    else
      stop();

    return isRunning();
  }

  inline
  void Scene::resetTime() {

    _timer_time_elapsed = 0;
  }

  inline
  void Scene::setTimeScale( double s ) {

    _timer_time_scale = s;
  }

  inline
  void Scene::start() {

    _timer_active = true;
    _timer.update();
  }

  inline
  void   Scene::stop() {

    _timer_active = false;
  }

} // END namespace GMlib

#endif // GM_SCENE_SCENE_H
