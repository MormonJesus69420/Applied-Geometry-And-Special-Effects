#ifndef SCENARIO_H
#define SCENARIO_H

#include "application/gmlibwrapper.h"

// qt
#include <QObject>

// Forward declaration of classes used from tardzone
namespace tardzone {
template <typename T>
class BSplineCurve;
template <typename T>
class BlendingCurve;
template <typename T>
class GERBSSurface;
template <typename T>
class GERBSCurve;
template <typename T>
class ModelCurve;
}

// Forward declaration of classes used from GMlib
namespace GMlib {
template <typename T>
class PBezierCurve;
template <typename T>
class PCircle;
template <typename T>
class PTorus;
}

// Forward declaration of classes used from source folder of project
class defaultsplinecurve;

class Scenario : public GMlibWrapper {
  Q_OBJECT
  public:
  using GMlibWrapper::GMlibWrapper;

  void initializeScenario() override;
  void cleanupScenario() override;

  public slots:
  void callDefferedGL();
  void toggleSimulation();

  private:
  void initBSplineControl();
  void initBSplineSampling();
  void initBlending();
  void initGoebbels();
  void initSurface();

  std::shared_ptr<defaultsplinecurve> _controlDefCurve;
  std::shared_ptr<tardzone::BSplineCurve<float>> _controlMyCurve;

  std::shared_ptr<GMlib::PCircle<float>> _samplingCircle;
  std::shared_ptr<tardzone::BSplineCurve<float>> _samplingCurve;

  std::shared_ptr<GMlib::PBezierCurve<float>> _blendCurve1;
  std::shared_ptr<GMlib::PBezierCurve<float>> _blendCurve2;
  std::shared_ptr<tardzone::BlendingCurve<float>> _blending80Curve;
  std::shared_ptr<tardzone::BlendingCurve<float>> _blending50Curve;
  std::shared_ptr<tardzone::BlendingCurve<float>> _blending20Curve;

  std::shared_ptr<tardzone::GERBSCurve<float>> _gerbsCurve;
  std::shared_ptr<tardzone::ModelCurve<float>> _heart;

  std::shared_ptr<tardzone::GERBSSurface<float>> _gerbsSurface;
  std::shared_ptr<GMlib::PTorus<float>> _torus;

  int counter = 0;
  const int max = 5;
};

#endif // SCENARIO_H
