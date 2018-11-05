#include "gmlibwrapper.h"

#include "utils.h"


// GMlib
#include <gmOpenglModule>
#include <gmSceneModule>
#include <gmParametricsModule>

// Qt
#include <QTimerEvent>
#include <QRectF>
#include <QMouseEvent>
#include <QDebug>

// stl
#include <stdexcept>
#include <thread>
#include <mutex>





std::unique_ptr<GMlibWrapper> GMlibWrapper::_instance {nullptr};

GMlibWrapper&
GMlibWrapper::instance() { return *_instance; }


GMlibWrapper::GMlibWrapper() : QObject(), _timer_id{0}/*, _select_renderer{nullptr}*/ {

  if(_instance != nullptr) {

    std::cerr << "This version of the GMlibWrapper only supports a single instance of the GMlibWraper..." << std::endl;
    std::cerr << "Only one of me(0x" << this << ") please!! ^^" << std::endl;
    assert(!_instance);
    exit(666);
  }


  _instance = std::unique_ptr<GMlibWrapper>(this);
}

GMlibWrapper::~GMlibWrapper() {

  _instance.release();
}

void GMlibWrapper::toggleSimulation() {  _scene->toggleRun(); }


void GMlibWrapper::render( const QString& name, const QRect& viewport_in, GMlib::RenderTarget& target ) {

  auto&        rc_pair = rcPair(name);
  auto&         camera = rc_pair.camera;
  auto&       renderer = rc_pair.renderer;
  auto&       viewport = rc_pair.viewport;

  // Update viewport
  if(viewport != viewport_in) {

    viewport = viewport_in;

    const auto& size = viewport.size();
    renderer->reshape( GMlib::Vector<int,2>(size.width(),size.height()));
    camera->reshape( 0, 0, size.width(), size.height() );
  }

  // Render and swap buffers
  renderer->render(target);
}


void GMlibWrapper::timerEvent(QTimerEvent* e) {

  e->accept();

  _scene->simulate();
  prepare();
}

void GMlibWrapper::start() {

  if( _timer_id || _scene->isRunning() )
    return;

  _timer_id = startTimer(16, Qt::PreciseTimer);
  _scene->start();
}

void GMlibWrapper::stop() {

  if( !_timer_id || !_scene->isRunning() )
    return;

  _scene->stop();
  killTimer(_timer_id);
  _timer_id = 0;
}

void GMlibWrapper::initialize() {

  // Setup and initialized GMlib GL backend
  GMlib::GL::OpenGLManager::init();

  // Setup and init the GMlib GMWindow
  _scene = std::make_shared<GMlib::Scene>();

  // Setup Select Renderer
  _select_renderer = std::make_shared<GMlib::DefaultSelectRenderer>();
//  _select_renderer->setSelectRboName("select_render_color_rbo");
}

void GMlibWrapper::cleanUp() {

  stop();

  cleanupScenario();

  _select_renderer.reset();

  for( auto& rc_pair : _rc_pairs ) {

    rc_pair.second.renderer->releaseCamera();
    _scene->removeCamera( rc_pair.second.camera.get() );
  }
  _rc_pairs.clear();

  _scene->clear();
  _scene.reset();

  // Clean up GMlib GL backend
//  GMlib::GL::OpenGLManager::cleanUp();   // NO IMPLEMENTED IN GMlib
}

GMlib::SceneObject*
GMlibWrapper::findSceneObject(const QString& rc_name, const GMlib::Point<int,2>& pos) {


  if(!_rc_pairs.count(rc_name.toStdString()))
    throw std::invalid_argument("[][]Render/Camera pair '" + rc_name.toStdString() + "'  does not exist in [" + __FILE__ + " on line " + std::to_string(__LINE__) + "]!");

  auto rc_pair = _rc_pairs.at(rc_name.toStdString());
  auto cam = rc_pair.camera;
  auto viewport = rc_pair.viewport;
  GMlib::Vector<int,2> size( viewport.width(), viewport.height() );

  GMlib::SceneObject* sel_obj = nullptr;

  // Setup select renderer to match current view and prepare
  _select_renderer->setCamera(cam.get());
  {
    _select_renderer->reshape( size );
    _select_renderer->prepare();

    // Render selectors and find object ad pos
    _select_renderer->select( GMlib::GM_SO_TYPE_SELECTOR );
    sel_obj = _select_renderer->findObject(pos(0),pos(1));


    // Render other objects and find object ad pos
    if(!sel_obj) {

      _select_renderer->select( -GMlib::GM_SO_TYPE_SELECTOR );
      sel_obj = _select_renderer->findObject(pos(0),pos(1));
    }
  }
  _select_renderer->releaseCamera();

  return sel_obj;
}

RenderCamPair&
GMlibWrapper::rcPair(const QString& name) {

  if(!_rc_pairs.count(name.toStdString())) throw std::invalid_argument("[][]Render/Camera pair '" + name.toStdString() + "'  does not exist!");
  return _rc_pairs.at(name.toStdString());
}

const RenderCamPair&
GMlibWrapper::rcPair(const QString& name) const {

  if(!_rc_pairs.count(name.toStdString())) throw std::invalid_argument("[][]Render/Camera pair '" + name.toStdString() + "'  does not exist!");
  return _rc_pairs.at(name.toStdString());
}

RenderCamPair& GMlibWrapper::createRCPair(const QString& name) {

  auto rc_pair = RenderCamPair {};

  rc_pair.renderer = std::make_shared<GMlib::DefaultRenderer>();
  rc_pair.camera = std::make_shared<GMlib::Camera>();
  rc_pair.renderer->setCamera(rc_pair.camera.get());

  return _rc_pairs[name.toStdString()] = rc_pair;
}

void
GMlibWrapper::updateRCPairNameModel() {

  QStringList names;
  for( auto& rc_pair : _rc_pairs )
    names << QString(rc_pair.first.c_str());

  std::reverse(names.begin(),names.end());

  _rc_name_model.setStringList(names);
}

QStringListModel&
GMlibWrapper::rcNameModel() {

  return _rc_name_model;
}

const std::shared_ptr<GMlib::Scene>&
GMlibWrapper::scene() const {

  return _scene;
}

const std::shared_ptr<GMlib::Camera>&
GMlibWrapper::camera(const QString& name) const {

  return rcPair(name).camera;
}

void  GMlibWrapper::prepare() {  _scene->prepare(); }
