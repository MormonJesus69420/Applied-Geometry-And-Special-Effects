#ifndef TARDZONE_BLENDINGCURVE_H
#define TARDZONE_BLENDINGCURVE_H

#include <parametrics/gmpcurve.h>
#include <memory>

namespace tardzone {

template <typename T>
class BlendingCurve : public GMlib::PCurve<T, 3> {
  GM_SCENEOBJECT(BlendingCurve)

  public:
  // Public functions for BlendingCurve
  BlendingCurve(std::shared_ptr<GMlib::PCurve<T,3>> c1, std::shared_ptr<GMlib::PCurve<T,3>> c2, T blendpoint);
  virtual ~BlendingCurve();

  // Public virtual functions from PCurve
  bool isClosed() const override;

  // Public members for BlendingCurve

  protected:
  // Protected functions for BlendingCurve
  void localSimulate(double dt) override;

  // Protected virtual functions from PCurve
  void eval(T t, int d, bool fromLeft) const override;
  T getStartP() const override;
  T getEndP() const override;

  // Protected members for BlendingCurve

  private:
  // Private functions for BlendingCurve
  T blendingFunction (T t) const;

  // Private virtual functions from PCurve

  // Private members for BlendingCurve
  // First curve
  std::shared_ptr<GMlib::PCurve<T,3>> _c1;
  // Second curve
  std::shared_ptr<GMlib::PCurve<T,3>> _c2;
  // Blending point
  T _blendpoint;
};

} // namespace tardzone

// Include BlendingCurve class function implementations
#include "blendingcurve.c"

#endif
