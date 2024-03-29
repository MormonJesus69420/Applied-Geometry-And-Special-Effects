#include <iostream>

#include "defaultsplinecurve.h"
#include "downeyeffects/blendingcurve.h"
#include "downeyeffects/bsplinecurve.h"
#include "downeyeffects/gerbscurve.h"
#include "downeyeffects/gerbssurf.h"
#include "downeyeffects/modelcurve.h"
#include "scenario.h"

// hidmanager
#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <gmCoreModule>
#include <gmOpenglModule>
#include <gmParametricsModule>
#include <gmSceneModule>

// qt
#include <QQuickItem>

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
{
  out << v.size() << std::endl;
  for (uint i = 0; i < v.size(); i++)
    out << " " << v[i];
  out << std::endl;
  return out;
}

void Scenario::initializeScenario()
{

  // Insert a light
  GMlib::Point<GLfloat, 3> init_light_pos(2.0, -4.0, 10);
  GMlib::PointLight* light = new GMlib::PointLight(GMlib::GMcolor::white(), GMlib::GMcolor::white(),
      GMlib::GMcolor::white(), init_light_pos);
  light->setAttenuation(0.8f, 0.002f, 0.0008f);
  this->scene()->insertLight(light, false);

  // Insert Sun
  this->scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float, 3> init_cam_pos(0.0f, 0.0f, 0.0f);
  GMlib::Vector<float, 3> init_cam_dir(0.0f, 1.0f, 0.0f);
  GMlib::Vector<float, 3> init_cam_up(0.0f, 0.0f, 1.0f);

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos, init_cam_dir, init_cam_up);
  proj_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  proj_rcpair.camera->rotateGlobal(GMlib::Angle(-45), GMlib::Vector<float, 3>(1.0f, 0.0f, 0.0f));
  proj_rcpair.camera->translateGlobal(GMlib::Vector<float, 3>(0.0f, -20.0f, 20.0f));
  scene()->insertCamera(proj_rcpair.camera.get());
  proj_rcpair.renderer->reshape(GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  // Front cam
  auto front_rcpair = createRCPair("Front");
  front_rcpair.camera->set(init_cam_pos + GMlib::Vector<float, 3>(0.0f, -50.0f, 0.0f), init_cam_dir, init_cam_up);
  front_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  scene()->insertCamera(front_rcpair.camera.get());
  front_rcpair.renderer->reshape(GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  // Side cam
  auto side_rcpair = createRCPair("Side");
  side_rcpair.camera->set(init_cam_pos + GMlib::Vector<float, 3>(-50.0f, 0.0f, 0.0f), GMlib::Vector<float, 3>(1.0f, 0.0f, 0.0f), init_cam_up);
  side_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  scene()->insertCamera(side_rcpair.camera.get());
  side_rcpair.renderer->reshape(GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  // Top cam
  auto top_rcpair = createRCPair("Top");
  top_rcpair.camera->set(init_cam_pos + GMlib::Vector<float, 3>(0.0f, 0.0f, 50.0f), -init_cam_up, init_cam_dir);
  top_rcpair.camera->setCuttingPlanes(1.0f, 8000.0f);
  scene()->insertCamera(top_rcpair.camera.get());
  top_rcpair.renderer->reshape(GMlib::Vector<int, 2>(init_viewport_size, init_viewport_size));

  initBSplineControl();
  initBSplineSampling();
  initBlending();
  initGoebbels();
  initSurface();

  toggleSimulation();
}

void Scenario::cleanupScenario()
{
  if (counter == 1) {
    scene()->remove(_controlDefCurve.get());
    scene()->remove(_controlMyCurve.get());
  }
  else if (counter == 2) {
    scene()->remove(_samplingCircle.get());
    scene()->remove(_samplingCurve.get());
  }
  else if (counter == 3) {
    scene()->remove(_blendCurve1.get());
    scene()->remove(_blendCurve2.get());
    scene()->remove(_blending80Curve.get());
    scene()->remove(_blending50Curve.get());
    scene()->remove(_blending20Curve.get());
  }
  else if (counter == 4) {
    scene()->remove(_heart.get());
    scene()->remove(_gerbsCurve.get());
  }
  else if (counter == 5) {
    scene()->remove(_cylinder.get());
    scene()->remove(_plane.get());
    scene()->remove(_torus.get());
    scene()->remove(_cylinderSurface.get());
    scene()->remove(_planeSurface.get());
    scene()->remove(_torusSurface.get());
  }
}

void Scenario::toggleSimulation()
{
  cleanupScenario();

  counter = ++counter > max ? 1 : counter;

  if (counter == 1) {
    scene()->insert(_controlDefCurve.get());
    scene()->insert(_controlMyCurve.get());
  }
  else if (counter == 2) {
    scene()->insert(_samplingCircle.get());
    scene()->insert(_samplingCurve.get());
  }
  else if (counter == 3) {
    scene()->insert(_blendCurve1.get());
    scene()->insert(_blendCurve2.get());
    scene()->insert(_blending80Curve.get());
    scene()->insert(_blending50Curve.get());
    scene()->insert(_blending20Curve.get());
  }
  else if (counter == 4) {
    scene()->insert(_heart.get());
    scene()->insert(_gerbsCurve.get());
  }
  else if (counter == 5) {
    scene()->insert(_cylinder.get());
    scene()->insert(_plane.get());
    scene()->insert(_torus.get());
    scene()->insert(_cylinderSurface.get());
    scene()->insert(_planeSurface.get());
    scene()->insert(_torusSurface.get());
  }
}

void Scenario::initBSplineControl()
{
  GMlib::DVector<GMlib::Vector<float, 3>> cp(8);
  cp[0] = GMlib::Vector<float, 3>(-10, 2, 1);
  cp[1] = GMlib::Vector<float, 3>(-7, -3, 0);
  cp[2] = GMlib::Vector<float, 3>(-4, 2, 2);
  cp[3] = GMlib::Vector<float, 3>(-1, -2, -3);
  cp[4] = GMlib::Vector<float, 3>(1, 3, 5);
  cp[5] = GMlib::Vector<float, 3>(4, -4, -3);
  cp[6] = GMlib::Vector<float, 3>(5, 3, 0);
  cp[7] = GMlib::Vector<float, 3>(10, -1, 0);

  _controlDefCurve = std::make_shared<defaultsplinecurve>(cp, 3, false);
  _controlDefCurve->toggleDefaultVisualizer();
  _controlDefCurve->setColor(GMlib::GMcolor::cyan());
  _controlDefCurve->showSelectors(0.5);
  _controlDefCurve->sample(100, 0);

  _controlMyCurve = std::make_shared<tardzone::BSplineCurve<float>>(cp);
  _controlMyCurve->toggleDefaultVisualizer();
  _controlMyCurve->setColor(GMlib::GMcolor::darkMagenta());
  _controlMyCurve->sample(100, 0);
}

void Scenario::initBSplineSampling()
{
  _samplingCircle = std::make_shared<GMlib::PCircle<float>>(9);
  _samplingCircle->setColor(GMlib::GMcolor::cyan());
  _samplingCircle->rotateGlobal(M_PI / 2.0, { 1.0f, 0.0f, 0.0f });
  _samplingCircle->toggleDefaultVisualizer();
  _samplingCircle->sample(100, 0);

  GMlib::DVector<GMlib::Vector<float, 3>> sample(100);
  float step = float(M_2PI / (sample.getDim() - 1));
  for (int i = 0; i < sample.getDim(); i++) {
    sample[i] = _samplingCircle->getPosition(i * step);
  }

  _samplingCurve = std::make_shared<tardzone::BSplineCurve<float>>(sample, 10);
  _samplingCurve->rotate(GMlib::Angle(M_PI / 2.0), GMlib::Vector<float, 3>(1, 0, 0), false);
  _samplingCurve->toggleDefaultVisualizer();
  _samplingCurve->setColor(GMlib::GMcolor::darkMagenta());
  _samplingCurve->sample(100, 0);
}

void Scenario::initBlending()
{
  GMlib::DVector<GMlib::Vector<float, 3>> cp1(4);
  cp1[0] = GMlib::Vector<float, 3>(-12, -1, -3);
  cp1[1] = GMlib::Vector<float, 3>(-9, 1, 3);
  cp1[2] = GMlib::Vector<float, 3>(-6, 2, 3);
  cp1[3] = GMlib::Vector<float, 3>(-3, -3, -2);

  _blendCurve1 = std::make_shared<GMlib::PBezierCurve<float>>(cp1);
  _blendCurve1->toggleDefaultVisualizer();
  _blendCurve1->setColor(GMlib::GMcolor::cyan());
  _blendCurve1->sample(100, 3);
  _blendCurve1->showSelectors(0.5);

  GMlib::DVector<GMlib::Vector<float, 3>> cp2(4);
  cp2[0] = GMlib::Vector<float, 3>(3, 0, 3);
  cp2[1] = GMlib::Vector<float, 3>(6, -1, -3);
  cp2[2] = GMlib::Vector<float, 3>(9, -1, -3);
  cp2[3] = GMlib::Vector<float, 3>(12, 0, 3);

  _blendCurve2 = std::make_shared<GMlib::PBezierCurve<float>>(cp2);
  _blendCurve2->toggleDefaultVisualizer();
  _blendCurve2->setColor(GMlib::GMcolor::limeGreen());
  _blendCurve2->sample(100, 3);
  _blendCurve2->showSelectors(0.5);

  _blending80Curve = std::make_shared<tardzone::BlendingCurve<float>>(_blendCurve1, _blendCurve2, 0.8f);
  _blending80Curve->toggleDefaultVisualizer();
  _blending80Curve->setColor(GMlib::GMcolor::darkMagenta());
  _blending80Curve->sample(100, 0);

  _blending50Curve = std::make_shared<tardzone::BlendingCurve<float>>(_blendCurve1, _blendCurve2, 0.5f);
  _blending50Curve->toggleDefaultVisualizer();
  _blending50Curve->setColor(GMlib::GMcolor::pink());
  _blending50Curve->sample(100, 0);

  _blending20Curve = std::make_shared<tardzone::BlendingCurve<float>>(_blendCurve1, _blendCurve2, 0.2f);
  _blending20Curve->toggleDefaultVisualizer();
  _blending20Curve->setColor(GMlib::GMcolor::crimson());
  _blending20Curve->sample(100, 0);
}

void Scenario::initGoebbels()
{
  _heart = std::make_shared<tardzone::ModelCurve<float>>(0.75);
  _heart->translateGlobal({ 0.0f, 0.0f, 1.5f });
  _heart->setColor(GMlib::GMcolor::cyan());
  _heart->toggleDefaultVisualizer();
  _heart->setCollapsed(true);
  _heart->sample(100, 0);

  _gerbsCurve = std::make_shared<tardzone::GERBSCurve<float>>(_heart.get(), 10);
  _gerbsCurve->toggleDefaultVisualizer();
  _gerbsCurve->sample(100, 0);
}

void Scenario::initSurface()
{
  _cylinder = std::make_shared<GMlib::PCylinder<float>>(1.5f,1.5f,5.0f);
  _cylinder->setMaterial(GMlib::GMmaterial::ruby());
  _cylinder->toggleDefaultVisualizer();
  _cylinder->replot(25, 25, 3, 3);

  _plane = std::make_shared<GMlib::PPlane<float>>(
      GMlib::Point<float, 3>(-15.0f, 0.0f, -5.0f),
      GMlib::Vector<float, 3>(10.0f, 0.0f, 0.0f),
      GMlib::Vector<float, 3>(0.0f, 0.0f, 10.0f));
  _plane->setMaterial(GMlib::GMmaterial::ruby());
  _plane->toggleDefaultVisualizer();
  _plane->replot(25, 25, 3, 3);

  _torus = std::make_shared<GMlib::PTorus<float>>(3.5f, 1.5f, 1.5f);
  _torus->rotateGlobal(M_PI / 2.0, { 1.0f, 0.0f, 0.0f });
  _torus->setMaterial(GMlib::GMmaterial::ruby());
  _torus->translateGlobal({10.0f, 0.0f, 0.0f});
  _torus->toggleDefaultVisualizer();
  _torus->replot(25, 25, 3, 3);

  _cylinderSurface = std::make_shared<tardzone::GERBSSurface<float>>(_cylinder.get(), 16, 16);
  _cylinderSurface->setMaterial(GMlib::GMmaterial::emerald());
  _cylinderSurface->toggleDefaultVisualizer();
  _cylinderSurface->replot(25, 25, 3, 3);

  _planeSurface = std::make_shared<tardzone::GERBSSurface<float>>(_plane.get(), 16, 16);
  _planeSurface->setMaterial(GMlib::GMmaterial::emerald());
  _planeSurface->toggleDefaultVisualizer();
  _planeSurface->replot(25, 25, 3, 3);

  _torusSurface = std::make_shared<tardzone::GERBSSurface<float>>(_torus.get(), 16, 16);
  _torusSurface->setMaterial(GMlib::GMmaterial::emerald());
  _torusSurface->toggleDefaultVisualizer();
  _torusSurface->replot(25, 25, 3, 3);
}

void Scenario::callDefferedGL()
{
  GMlib::Array<const GMlib::SceneObject*> e_obj;
  this->scene()->getEditedObjects(e_obj);

  for (int i = 0; i < e_obj.getSize(); i++)
    if (e_obj(i)->isVisible())
      e_obj(i)->replot();
}
