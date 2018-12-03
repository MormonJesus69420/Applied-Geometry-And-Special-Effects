#ifndef TARDZONE_BSPLINECURVE_H
#define TARDZONE_BSPLINECURVE_H

#include <parametrics/gmpcurve.h>

namespace tardzone {

template <typename T>
class BSplineCurve : public GMlib::PCurve<T, 3> {
  GM_SCENEOBJECT(BSplineCurve)

  public:
  // Public functions for BSplineCurve
  // Use c as control points - and generate a knotvector
  BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& c);
  // Use least square to make n control points - and generate a knotvector
  BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& p, int n);
  BSplineCurve(const BSplineCurve<T>& replicant);
  virtual ~BSplineCurve();

  // Public virtual functions from PCurve
  bool isClosed() const override;

  // Public members for BSplineCurve

  protected:
  // Protected functions for BSplineCurve

  // Protected virtual functions from PCurve
  void eval(T t, int d, bool l) const override;
  T getStartP() const override;
  T getEndP() const override;

  // Protected members for BSplineCurve

  private:
  // Private functions for BSplineCurve
  void createCP(const GMlib::DVector<GMlib::Vector<T, 3>>& p);
  GMlib::DMatrix<T> calculateAMatrix();
  GMlib::Vector<T, 4> createBees(T t, int& i) const;
  T wFunction(int d, int i, T t) const;
  int iFinder(T t) const;
  void createKnots();
  // Private virtual functions from PCurve

  // Private members for BSplineCurve
  // Control points
  GMlib::DVector<GMlib::Vector<T, 3>> _c;
  // Knot vector
  std::vector<T> _t;
  // Dimensions for A matrix, n also # control points
  int _n, _m;
  // Degree and order
  const int _d = 3, _k = _d + 1;
};

} // namespace tardzone

// Include PCurve class function implementations
#include "bsplinecurve.c"

#endif
