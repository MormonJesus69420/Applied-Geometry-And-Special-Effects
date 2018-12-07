#ifndef TARDZONE_GERBSCURVE_H
#define TARDZONE_GERBSCURVE_H

#include <memory>
#include <parametrics/gmpcurve.h>
#include <random>

namespace tardzone {

template <typename T>
class GERBSCurve : public GMlib::PCurve<T, 3> {
  GM_SCENEOBJECT(GERBSCurve)

  public:
  // Public functions for GERBSCurve
  GERBSCurve(GMlib::PCurve<T, 3>* model, int m);
  GERBSCurve(const GERBSCurve<T>& replicant);
  virtual ~GERBSCurve();

  // Public virtual functions from PCurve
  bool isClosed() const override;

  // Public members for GERBSCurve

  protected:
  // Protected functions for GERBSCurve
  void localSimulate(double dt) override;

  // Protected virtual functions from PCurve
  void eval(T t, int d, bool fromLeft) const override;
  T getStartP() const override;
  T getEndP() const override;

  // Protected members for GERBSCurve

  private:
  // Private functions for GERBSCurve
  T wFunction(const int& d, const int& i, T t) const;
  T blendingFunction(T t) const;
  int iFinder(T t) const;
  void createSubCurves();
  void createKnots();

  // Private virtual functions from PCurve

  // Private members for GERBSCurve
  std::vector<std::shared_ptr<GMlib::PCurve<T, 3>>> _c;
  GMlib::PCurve<T, 3>* _modelCurve;
  const int _d = 1, _k = _d + 1;
  std::vector<T> _t;
  int _n;

  // Animation variables
  bool _up = true;
  std::mt19937 _gen;
  double _hue, _beat;
  GMlib::Color _colour;
  std::uniform_real_distribution<double> _dis;
  std::uniform_real_distribution<double> _step;
  const GMlib::Vector<float, 3> _rot1 = { 1.2f, 1.0f, 0.0f };
  const GMlib::Vector<float, 3> _rot2 = { -1.2f, 1.0f, 0.0f };
};

} // namespace tardzone

// Include GERBSCurve class function implementations
#include "gerbscurve.c"

#endif
