#include "defaulthidmanager.h"

// local
#include "../application/gmlibwrapper.h"
#include "hidaction.h"

// gmlib
#include <gmSceneModule>
#include <gmParametricsModule>
using namespace GMlib;

// qt
#include <QGuiApplication>

// Local Defines
#define SNAP 0.01



DefaultHidManager::DefaultHidManager(QObject* parent)
  : StandardHidManager(parent), _gmlib{nullptr} {}

DefaultHidManager::~DefaultHidManager() {

  _gmlib = nullptr;
}

void DefaultHidManager::triggerAction(const HidAction* action, const HidInputEvent::HidInputParams& params ) {

  if(action->getCustomTrigger() == OGL_TRIGGER)
    _ogl_actions.emplace(action,params);
  else
    HidManager::triggerAction(action,params);
}

void DefaultHidManager::triggerOGLActions() {

  while(!_ogl_actions.empty()) {

    auto action = _ogl_actions.front();
    HidManager::triggerAction(action.first,action.second);
    _ogl_actions.pop();
  }
}

void DefaultHidManager::heDeSelectAllObjects() {

  scene()->removeSelections();
}


void DefaultHidManager::heColapse() {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();

  for( int i = 0; i < sel_objs.getSize(); i++ )
    sel_objs(i)->toggleCollapsed();
}


void DefaultHidManager::heClose() {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();

  for( int i = 0; i < sel_objs.getSize(); i++ )
    sel_objs(i)->toggleClose();
}


void DefaultHidManager::heSelectors() {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();

  for( int i = 0; i < sel_objs.getSize(); i++ )
    sel_objs(i)->toggleSelectors();
}


void DefaultHidManager::heEdit() {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();
  for( int i = 0; i < sel_objs.getSize(); i++ ) {

    SceneObject *sel_obj = sel_objs(i);

    // ERBS
    PERBSCurve<float> *ecObj = dynamic_cast<PERBSCurve<float>*>( sel_obj );
    PERBSSurf<float> *esObj = dynamic_cast<PERBSSurf<float>*>( sel_obj );
    PERBSTriangle<float> *etObj = dynamic_cast<PERBSTriangle<float>*>( sel_obj );

    // Bezier
    PBezierCurve<float> *bcObj = dynamic_cast<PBezierCurve<float>*>( sel_obj );
    PBezierSurf<float> *bsObj = dynamic_cast<PBezierSurf<float>*>( sel_obj );
    PBezierTriangle<float> *btObj = dynamic_cast<PBezierTriangle<float>*>( sel_obj );

    // Arc
    PArc<float> *acObj = dynamic_cast<PArc<float>*>( sel_obj );

    // ERBS
    if( ecObj ) {

      if( ecObj->isLocalCurvesVisible() )
        ecObj->hideLocalCurves();
      else
        ecObj->showLocalCurves();
    }
    else if( esObj ) {

      if( esObj->isLocalPatchesVisible() )
        esObj->hideLocalPatches();
      else
        esObj->showLocalPatches();
    }
    else if( etObj ) {
      if( etObj->isLocalPatchesVisible() )
        etObj->hideLocalPatches();
      else
        etObj->showLocalPatches();
    }
    // Bezier
    else if( bcObj ) {

      PERBSCurve<float> *parent = dynamic_cast<PERBSCurve<float>*>( bcObj->getParent() );
      if( parent ) {

        if( bcObj->toggleCollapsed() )
          bcObj->hideSelectors();
        else
          bcObj->showSelectors();
      }
      else {

        if( bcObj->isSelectorsVisible() )
          bcObj->hideSelectors();
        else
          bcObj->showSelectors();
      }
    }
    else if( bsObj ) {

      PERBSSurf<float> *parent = dynamic_cast<PERBSSurf<float>*>( bsObj->getParent());
      if( parent ) {

        if( bsObj->toggleCollapsed() )
          bsObj->hideSelectors();
        else
          bsObj->showSelectors(true);
      }
      else {

        if( bsObj->isSelectorsVisible() )
          bsObj->hideSelectors();
        else
          bsObj->showSelectors(true);
      }
    }
    else if( btObj ) {

      PERBSTriangle<float> *parent = dynamic_cast<PERBSTriangle<float>*>( btObj->getParent() );
      if( parent ) {

        if( btObj->toggleCollapsed() )
          btObj->hideSelectors();
        else
          btObj->showSelectors(true);
      }
      else {

        if( btObj->isSelectorsVisible() )
          btObj->hideSelectors();
        else
          btObj->showSelectors(true);
      }
    }
    else if( acObj ) {

      acObj->toggleCollapsed();
    }

  }
}

void DefaultHidManager::heLockTo(const HidInputEvent::HidInputParams& params) {

  auto view_name = viewNameFromParams(params);
  auto pos       = toGMlibViewPoint(view_name, posFromParams(params));

  auto cam     = findCamera(view_name);
  auto sel_obj = findSceneObject(view_name,pos);

  if( sel_obj )
    cam->lock( sel_obj );
  else if(cam->isLocked())
    cam->unLock();
  else {

    cam->lock(
      ( scene()->getSphereClean().getPos() - cam->getPos() ) *
      cam->getDir() );
  }
}

void DefaultHidManager::heMoveCamera(const HidInputEvent::HidInputParams& params) {

  auto view_name = viewNameFromParams(params);
  auto pos       = toGMlibViewPoint(view_name, posFromParams(params));
  auto prev      = toGMlibViewPoint(view_name, prevPosFromParams(params));

  auto *cam = findCamera(view_name);
  if( !cam )
    return;

  const float scale = cameraSpeedScale( cam );
  const Vector<float,2> delta (
     (pos(0) - prev(0)) * scale / cam->getViewportW(),
     (prev(1) - pos(1)) * scale / cam->getViewportH()
  );
  cam->move( delta );
}

void DefaultHidManager::heMoveSelectedObjects( const HidInputEvent::HidInputParams& params ) {

  auto view_name = viewNameFromParams(params);
  auto pos       = toGMlibViewPoint(view_name, posFromParams(params));
  auto prev      = toGMlibViewPoint(view_name, prevPosFromParams(params));

  Camera *cam = findCamera(view_name);
  if( !cam )
    return;


  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();
  for( int i = 0; i < sel_objs.getSize(); i++ ) {

    SceneObject* obj = sel_objs(i);

    const double dh = cam->deltaTranslate( obj );
    const Vector<float,3> deltav(
      ( ( prev(0) - pos(0) ) * dh ) * cam->getSide() +
      ( ( pos(1) - prev(1) ) * dh ) * cam->getUp() );

    if( deltav.getLength() > SNAP && deltav.getLength() < 1000.0 ) {

      if( obj->getTypeId() != GM_SO_TYPE_SELECTOR )
        obj->translateGlobal( deltav );
      else if( obj->getTypeId()== GM_SO_TYPE_SELECTOR )
        obj->editPos(deltav);
    }
  }
}

void DefaultHidManager::hePanHorizontal(const HidInputEvent::HidInputParams& params) {

  auto view_name   = viewNameFromParams(params);
  auto wheel_delta = wheelDeltaFromParams(params);

  Camera *cam = findCamera(view_name);
  if( cam )
    cam->move(
      Vector<float,2>(
        wheel_delta * cameraSpeedScale(cam) / cam->getViewportH(),
        0.0f
        ));
}

void DefaultHidManager::hePanVertical(const HidInputEvent::HidInputParams& params) {

  auto view_name   = viewNameFromParams(params);
  auto wheel_delta = wheelDeltaFromParams(params);

  Camera *cam = findCamera(view_name);
  if( cam )
    cam->move(
      Vector<float,2>(
        0.0f,
        wheel_delta * cameraSpeedScale(cam) / cam->getViewportW()
            ));
}

void DefaultHidManager::heReplotQuick(int factor) {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();

  for( int i = 0; i < sel_objs.getSize(); i++ ) {

    GMlib::SceneObject *sel_obj = sel_objs(i);

    GMlib::PCurve<float,3> *curve = dynamic_cast<GMlib::PCurve<float,3>*>( sel_obj );
    GMlib::PSurf<float,3> *surf = dynamic_cast<GMlib::PSurf<float,3>*>( sel_obj );

    if( curve ) {

      GMlib::PERBSCurve<float> *erbs = dynamic_cast<GMlib::PERBSCurve<float>*>(curve);
      if( erbs )
        erbs->sample( (erbs->getLocalCurves().getDim()-1)*factor + 1, 1 );
      else
        curve->sample( std::pow<int>( factor, 2 ) * 100, 2 );
    }
    else if( surf ) {

      GMlib::PERBSSurf<float> *erbs = dynamic_cast<GMlib::PERBSSurf<float>*>(surf);
      if( erbs )
        erbs->replot(
          (erbs->getLocalPatches().getDim1()-1)*factor + 1,
          (erbs->getLocalPatches().getDim2()-1)*factor + 1,
          2, 2 );
      else {
        surf->replot( 10 * factor, 10 * factor, 2, 2 );
      }
    }
  }
}

void DefaultHidManager::heReplotQuickHigh() {

  heReplotQuick(20);
}

void DefaultHidManager::heReplotQuickLow() {

  heReplotQuick(1);
}

void DefaultHidManager::heReplotQuickMedium() {

  heReplotQuick(10);
}

void DefaultHidManager::heRotateSelectedObjects(const HidInputEvent::HidInputParams& params) {

  auto view_name = viewNameFromParams(params);
  auto pos       = toGMlibViewPoint(view_name, posFromParams(params));
  auto prev      = toGMlibViewPoint(view_name, prevPosFromParams(params));

  Camera *cam = findCamera(view_name);
  if( !cam )
    return;

  const Array<SceneObject*> &objs = scene()->getSelectedObjects();

  // Compute rotation axis and angle in respect to the camera and view.
  const UnitVector<float,3> rot_v =
    float( pos(0) - prev(0) ) * cam->getGlobalUp() -
    float( prev(1) - pos(1) ) * cam->getGlobalSide();
  const Angle ang(
    M_2PI * sqrt(
      pow( double( pos(0) - prev(0) ) / cam->getViewportW(), 2 ) +
      pow( double( prev(1) - pos(1) ) / cam->getViewportH(), 2 ) ) );


  int no_objs = 0;
  Sphere<float,3> sphere;
  for( int i = 0; i < objs.getSize(); ++i )
    if( objs(i)->getTypeId() != GM_SO_TYPE_SELECTOR ) {
      sphere += objs(i)->getSurroundingSphereClean();
      no_objs++;
    }

  for( int i = 0; i < objs.getSize(); ++i )
    if( objs(i)->getTypeId() != GM_SO_TYPE_SELECTOR )
      if( std::abs(pos(0)-prev(0)) > POS_TOLERANCE || std::abs(pos(1)-prev(1)) > POS_TOLERANCE )
  //        objs(i)->rotateGlobal( ang, sphere.getPos(), rot_v);
         no_objs > 1 ? objs(i)->rotateGlobal( ang, sphere.getPos(), rot_v) : objs(i)->rotateGlobal( ang, rot_v);
}

void DefaultHidManager::heScaleSelectedObjects(const HidInputEvent::HidInputParams& params) {

  auto view_name = viewNameFromParams(params);
  auto pos       = toGMlibViewPoint(view_name, posFromParams(params));
  auto prev      = toGMlibViewPoint(view_name, prevPosFromParams(params));

  Camera *cam = findCamera(view_name);
  if( !cam )
    return;

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();
  for( int i = 0; i < sel_objs.getSize(); i++ ) {

    SceneObject* obj = sel_objs(i);

    const double dh = cam->deltaTranslate( obj );
    const Vector<float,3> deltav(
      ( ( prev(0) - pos(0) ) * dh ) * cam->getSide() +
      ( ( pos(1) - prev(1) ) * dh ) * cam->getUp() );


    if( deltav.getLength() < 1000.0 )
      obj->scale( Vector<float,3>( 1.0f + deltav(1) ) );
  }
}

void DefaultHidManager::heSelectAllObjects() {

  Scene *scene = this->scene();
  for( int i = 0; i < scene->getSize(); ++i )
    heSelectObjectTree( (*scene)[i] );
}

void DefaultHidManager::heSelectObject(const HidInputEvent::HidInputParams& params) {

  auto view_name = viewNameFromParams(params);
  auto pos       = toGMlibViewPoint(view_name, posFromParams(params));

  auto obj = findSceneObject(view_name,pos);
  if( !obj )
    return;

  // Preserver object selection
  auto selected = obj->isSelected();
  heDeSelectAllObjects();
  obj->setSelected( !selected );
}

void DefaultHidManager::heSelectObjects(const HidInputEvent::HidInputParams& params) {

  auto view_name = viewNameFromParams(params);
  auto pos       = toGMlibViewPoint(view_name, posFromParams(params));

  if( auto obj = findSceneObject(view_name,pos) ) obj->toggleSelected();

//  if(obj) obj->toggleSelected();
}

void DefaultHidManager::heSelectObjectTree( SceneObject* obj ) {

  // Do not select cameras or lights
  GMlib::Camera *cam   = dynamic_cast<GMlib::Camera*>( obj );
  GMlib::Light  *light = dynamic_cast<GMlib::Light*>( obj );
  if( !cam && !light )
    obj->setSelected(true);

  // Recursive Propagation
  for( int i = 0; i < obj->getChildren().getSize(); i++ )
    heSelectObjectTree( (obj->getChildren())[i] );
}

void DefaultHidManager::heToggleObjectDisplayMode() {

  const Array<SceneObject*> &sel_objs = scene()->getSelectedObjects();

  for( int i = 0; i < sel_objs.getSize(); i++ ) {


    auto obj = sel_objs(i);
    GMlib::Array<GMlib::Visualizer*> &visus = obj->getVisualizers();
    for( int i = 0; i < visus.getSize(); i++ ) visus[i]->toggleDisplayMode();
  }
}

void DefaultHidManager::heToggleSimulation() {

  emit signToggleSimulation();
}

void DefaultHidManager::heToggleSelectAllObjects() {

  if( scene()->getSelectedObjects().getSize() > 0 )
    heDeSelectAllObjects();
  else
    heSelectAllObjects();
}

void DefaultHidManager::heZoom(const HidInputEvent::HidInputParams& params) {

  auto view_name   = viewNameFromParams(params);
  auto wheel_delta = wheelDeltaFromParams(params);

  // Qt comp scale
  wheel_delta /= 8;

  Camera *cam    = findCamera(view_name);
  Camera *isocam = dynamic_cast<IsoCamera*>( cam );

  if( isocam ) {

    if( wheel_delta < 0 ) isocam->zoom( 1.05 );
    if( wheel_delta > 0 ) isocam->zoom( 0.95 );
  }
  else if( cam ) {

    double scale;
    if( cam->isLocked() )
      scale = cam->getLockDist();
    else
      scale = scene()->getSphere().getRadius();

    cam->move( wheel_delta*scale / cam->getViewportH() );
  }
}

void DefaultHidManager::heLeftMouseReleaseStuff() {

  //  _move_border = false;
}

void DefaultHidManager::heOpenCloseHidHelp() {

  emit signOpenCloseHidHelp();
}

Camera* DefaultHidManager::findCamera( const QString& view_name ) const {

  return _gmlib->camera(view_name).get();
}


float DefaultHidManager::cameraSpeedScale(Camera *cam) const {

  if( !cam )
    return 1.0f;

  if(cam->isLocked())
    return M_2PI * cam->getLockDist();

  return scene()->getSphere().getRadius();
}



Scene* DefaultHidManager::scene() const {

  assert(_gmlib);

  return _gmlib->scene().get();
}

SceneObject* DefaultHidManager::findSceneObject( const QString& view_name, const GMlib::Point<int,2>& pos  ) {


  return _gmlib->findSceneObject( view_name, pos );
}

GMlib::Point<int,2> DefaultHidManager::toGMlibViewPoint(const QString& view_name, const QPoint &pos) {

  auto cam = findCamera(view_name);
  return Vector<int,2>( int(pos.x()), cam->getViewportH() - int(pos.y()) - 1 );
}

void DefaultHidManager::setupDefaultHidBindings() {

  //// Register Hid Actions

  // Camera
  QString ha_id_view_move_camera =
      registerHidAction("View",
                        "Move Camera",
                        "Move the camera. "
                        "If not locked to the scene, it will pan the camera in the view plane. "
                        "If locked it will rotate the camera about the center of the scene." ,
                        this, SLOT(heMoveCamera(HidInputEvent::HidInputParams)) );

  QString ha_id_view_pan_h =
      registerHidAction("View",
                        "Pan Horizontally",
                        "Pan horizontally",
                        this, SLOT(hePanHorizontal(HidInputEvent::HidInputParams)) );

  QString ha_id_view_pan_v =
      registerHidAction("View",
                        "Pan Vertically",
                        "Pan vertically",
                        this, SLOT(hePanVertical(HidInputEvent::HidInputParams)) );

  QString ha_id_view_zoom=
      registerHidAction("View",
                        "Zoom",
                        "Zoom",
                        this, SLOT(heZoom(HidInputEvent::HidInputParams)) );

  QString ha_id_view_lock_to =
      registerHidAction("View",
                        "Lock To ...",
                        "Lock camera to an object or to the scene.",
                        this, SLOT(heLockTo(HidInputEvent::HidInputParams)),
                        OGL_TRIGGER);

  // Object Transformation
  QString ha_id_objtrans_scale =
      registerHidAction("Object transformation",
                        "Scale Objects",
                        "Scale objects",
                        this, SLOT(heScaleSelectedObjects(HidInputEvent::HidInputParams)) );

  QString ha_id_objtrans_move =
      registerHidAction("Object transformation",
                        "Move Objects",
                        "Move objects",
                        this, SLOT(heMoveSelectedObjects(HidInputEvent::HidInputParams)),
                        OGL_TRIGGER);

  QString ha_id_objtrans_rotate =
      registerHidAction("Object transformation",
                        "Rotate Objects",
                        "Rotate objects",
                        this, SLOT(heRotateSelectedObjects(HidInputEvent::HidInputParams)) );

  // Object Selection
  QString ha_id_objsel_toggle_all =
      registerHidAction("Object selection",
                        "Toggle: (de)select all objects",
                        "Toggle selection on all objects",
                        this, SLOT(heToggleSelectAllObjects()) );

  QString ha_id_objsel_select =
      registerHidAction("Object selection",
                        "Toggle selection of one object",
                        "Toggle selection of object under cursor; deslecting all other objects",
                        this, SLOT(heSelectObject(HidInputEvent::HidInputParams)),
                        OGL_TRIGGER);

  QString ha_id_objsel_select_multi =
      registerHidAction("Object selection",
                        "Toggle selection of an object",
                        "Toggle selection of object under cursor; do not deselect other objects",
                        this, SLOT(heSelectObjects(HidInputEvent::HidInputParams)),
                        OGL_TRIGGER);

  // Object Interaction
  QString ha_id_objint_toggle_colaps =
      registerHidAction( "Object interaction",
                         "Toggle: Colapsed mode",
                         "Toggle colapsed mode for objects",
                         this, SLOT(heColapse()),
                         OGL_TRIGGER);

  QString ha_id_objint_toggle_close =
      registerHidAction( "Object interaction",
                         "Toggle: Close mode",
                         "Toggle close mode for objects",
                         this, SLOT(heClose()),
                         OGL_TRIGGER);

  QString ha_id_objint_toggle_selectors =
      registerHidAction( "Object interaction",
                         "Toggle: Selectors for objects",
                         "Toggle selectors for objects",
                         this, SLOT(heSelectors()),
                         OGL_TRIGGER);

  QString ha_id_objint_toggle_edit =
      registerHidAction( "Object interaction",
                         "Toggle: Edit mode",
                         "Toggle edit mode for editable objects",
                         this, SLOT(heEdit()),
                         OGL_TRIGGER);

  QString ha_id_objint_replot_high =
      registerHidAction( "Object interaction",
                         "Replot: QuickHigh",
                         "Replot with \"high\" resolution",
                         this, SLOT(heReplotQuickHigh()),
                         OGL_TRIGGER);

  QString ha_id_objint_replot_med =
      registerHidAction( "Object interaction",
                         "Replot: QuickMedium",
                         "Replot with \"medium\" resolution",
                         this, SLOT(heReplotQuickMedium()),
                         OGL_TRIGGER);

  QString ha_id_objint_replot_low =
      registerHidAction( "Object interaction",
                         "Replot: QuickLow",
                         "Replot with \"low\" resolution",
                         this, SLOT(heReplotQuickLow()),
                         OGL_TRIGGER);



  // Rendering
  QString ha_id_render_toggle_shademode =
      registerHidAction( "Rendering",
                         "Toggle: Shading mode",
                         "Toggle shadeing mode",
                         this, SLOT(heToggleObjectDisplayMode()),
                         OGL_TRIGGER);

  // Simulator
  QString ha_id_sim_toggle =
      registerHidAction( "Simulator",
                         "Toggle: Simulation",
                         "Toggle simulation",
                         this, SLOT(heToggleSimulation()) );

  // Open/Close HidBindings view
  QString ha_id_var_open_close_hbview =
      registerHidAction( "Application",
                         "Open/Close Hid help",
                         "Toggle open/close the Hid bindings help view",
                         this, SLOT(heOpenCloseHidHelp()) );

  // Various cleanup
  QString ha_id_var_lm_rel =
      registerHidAction( "Various",
                         "Left Mouse Release",
                         "Stuff that happens on left mouse release",
                         this, SLOT(heLeftMouseReleaseStuff()) );




  //// Set up initial mapping
  registerHidMapping( ha_id_objsel_toggle_all,            new KeyPressInput( Qt::Key_A ) ); 
  registerHidMapping( ha_id_objint_toggle_colaps,         new KeyPressInput( Qt::Key_Z, Qt::ControlModifier) );
  registerHidMapping( ha_id_objint_toggle_close,          new KeyPressInput( Qt::Key_O, Qt::ControlModifier) );
  registerHidMapping( ha_id_objint_toggle_selectors,      new KeyPressInput( Qt::Key_S, Qt::ControlModifier) );
  registerHidMapping( ha_id_objint_toggle_edit,           new KeyPressInput( Qt::Key_E ) );
  registerHidMapping( ha_id_objint_replot_high,           new KeyPressInput( Qt::Key_P, Qt::ShiftModifier ) );
  registerHidMapping( ha_id_objint_replot_med,            new KeyPressInput( Qt::Key_P ) );
  registerHidMapping( ha_id_objint_replot_low,            new KeyPressInput( Qt::Key_P, Qt::ControlModifier) );
  registerHidMapping( ha_id_sim_toggle,                   new KeyPressInput( Qt::Key_R ) );
  registerHidMapping( ha_id_render_toggle_shademode,      new KeyPressInput( Qt::Key_Z ) );

  registerHidMapping( ha_id_objsel_select,                new MousePressInput( Qt::RightButton ) );
  registerHidMapping( ha_id_view_lock_to,                 new MousePressInput( Qt::RightButton, Qt::ControlModifier ) );
  registerHidMapping( ha_id_objsel_select_multi,          new MousePressInput( Qt::RightButton, Qt::ShiftModifier ) );

  registerHidMapping( ha_id_view_move_camera,             new MouseMoveInput( Qt::LeftButton ) );
  registerHidMapping( ha_id_objtrans_scale,               new MouseMoveInput( Qt::LeftButton, Qt::ControlModifier | Qt::AltModifier ) );
  registerHidMapping( ha_id_objtrans_move,                new MouseMoveInput( Qt::LeftButton, Qt::ShiftModifier ) );
  registerHidMapping( ha_id_objtrans_rotate,              new MouseMoveInput( Qt::LeftButton, Qt::ControlModifier ) );

  registerHidMapping( ha_id_var_lm_rel,                   new MouseReleaseInput( Qt::LeftButton ) );
  registerHidMapping( ha_id_var_open_close_hbview,        new KeyPressInput( Qt::Key_Question, Qt::ShiftModifier ) );

  registerHidMapping( ha_id_view_pan_h,                   new WheelInput( Qt::ControlModifier ) );
  registerHidMapping( ha_id_view_pan_v,                   new WheelInput( Qt::ShiftModifier ) );
  registerHidMapping( ha_id_view_zoom,                    new WheelInput() );
}

void DefaultHidManager::init(GMlibWrapper& gmlib) {

  _gmlib = &gmlib;
}
