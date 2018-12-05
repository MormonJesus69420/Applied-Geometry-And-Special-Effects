#ifndef SCENARIO_H
#define SCENARIO_H

#include "application/gmlibwrapper.h"

// qt
#include <QObject>

namespace tardzone {
template <typename T>
class BSplineCurve;
template <typename T>
class BlendingCurve;
template <typename T>
class GERBSCurve;
template <typename T>
class ModelCurve;
}

namespace GMlib {
template <typename T>
class PBezierCurve;
template <typename T>
class PCircle;
}

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
  std::shared_ptr<tardzone::ModelCurve<float>> _butterfly;

  int counter = 0, max = 4;
};

#endif // SCENARIO_H
