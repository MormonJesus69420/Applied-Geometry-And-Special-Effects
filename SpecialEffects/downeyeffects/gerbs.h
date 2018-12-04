#ifndef TARDZONE_GERBS_H
#define TARDZONE_GERBS_H

#include <parametrics/gmpcurve.h>

namespace tardzone {

template <typename T>
class GERBSCurve : public GMlib::PCurve<T, 3> {
  GM_SCENEOBJECT(GERBSCurve)

  public:
  // Public functions for BSplineCurve
  GERBSCurve(GMlib::PCurve<T, 3>* model, int m);
  GERBSCurve(const GERBSCurve<T>& replicant);
  virtual ~GERBSCurve();

  // Public virtual functions from PCurve
  bool isClosed() const override;

  // Public members for BSplineCurve

  protected:
  // Protected functions for BSplineCurve

  // Protected virtual functions from PCurve
  void eval(T t, int d, bool fromLeft) const override;
  T getStartP() const override;
  T getEndP() const override;

  // Protected members for BSplineCurve

  private:
  // Private functions for BSplineCurve
  T createBee(T t) const;
  T wFunction(int d, int i, T t) const;
  int iFinder(T t) const;
  void createSubCurves();
  void createKnots();

  // Private virtual functions from PCurve

  // Private members for BSplineCurve
  std::vector<GMlib::PCurve<T, 3>*> _C;
  GMlib::PCurve<T, 3>* _modelCurve;
  std::vector<T> _knotVector;
  const int _d = 1, _k = _d + 1;
  int _n;
};

} // namespace tardzone

// Include GERBS class function implementations
#include "gerbs.c"

#endif
