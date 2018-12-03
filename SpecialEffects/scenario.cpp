
#include <iostream>

#include "downeyeffects/bsplinecurve.h"
#include "mybsplinecurve.h"
#include "scenario.h"

//// hidmanager
//#include "hidmanager/defaulthidmanager.h"

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
  GMlib::Point<GLfloat, 3> init_light_pos(2.0, 4.0, 10);
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

  /* TEST BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& c);
  GMlib::DVector<GMlib::Vector<float, 3>> cp(8);
  cp[0] = GMlib::Vector<float, 3>(0, 0, 0);
  cp[1] = GMlib::Vector<float, 3>(1, 1, 0);
  cp[2] = GMlib::Vector<float, 3>(2, 0, 2);
  cp[3] = GMlib::Vector<float, 3>(3, 2, 0);
  cp[4] = GMlib::Vector<float, 3>(4, 1, 0);
  cp[5] = GMlib::Vector<float, 3>(5, 1, -2);
  cp[6] = GMlib::Vector<float, 3>(6, 2, 0);
  cp[7] = GMlib::Vector<float, 3>(7, 0, 0);

  auto myBSpline = new mybsplinecurve(cp, 3, false);
  myBSpline->toggleDefaultVisualizer();
  myBSpline->setColor(GMlib::GMcolor::blueViolet());
  myBSpline->showSelectors(0.5);
  myBSpline->sample(100, 4);
  this->scene()->insert(myBSpline);

  auto temp = new tardzone::BSplineCurve<float>(cp);
  temp->toggleDefaultVisualizer();
  temp->setColor(GMlib::GMcolor::darkMagenta());
  temp->sample(100, 4);
  this->scene()->insert(temp);
  //*/

  //* TEST BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& p, int n);
  GMlib::PCircle<float>* circle = new GMlib::PCircle<float>(5);
  circle->toggleDefaultVisualizer();
  circle->sample(100, 0);
  this->scene()->insert(circle);

  GMlib::DVector<GMlib::Vector<float, 3>> sample(250);
  float step = float(M_2PI / (sample.getDim() - 1));
  for (int i = 0; i < sample.getDim(); i++) {
    sample[i] = circle->getPosition(i * step);
  }

  auto temp = new tardzone::BSplineCurve<float>(sample, 15);
  temp->toggleDefaultVisualizer();
  temp->setColor(GMlib::GMcolor::darkMagenta());
  temp->showSelectors(0.5);
  temp->sample(100, 0);
  this->scene()->insert(temp);
  //*/
}

void Scenario::cleanupScenario()
{
}

void Scenario::callDefferedGL()
{

  GMlib::Array<const GMlib::SceneObject*> e_obj;
  this->scene()->getEditedObjects(e_obj);

  for (int i = 0; i < e_obj.getSize(); i++)
    if (e_obj(i)->isVisible())
      e_obj(i)->replot();
}
