#ifndef TARDZONE_GERBSSURF_H
#define TARDZONE_GERBSSURF_H

#include <memory>
#include <parametrics/gmpsurf.h>
#include <random>

#include "simplesubsurf.h"

namespace tardzone {

template <typename T>
class GERBSSurface : public GMlib::PSurf<T, 3> {
  GM_SCENEOBJECT(GERBSSurface)

  public:
  // Public functions for GERBSSurface
  GERBSSurface(GMlib::PSurf<T, 3>* model, int n1, int n2);
  GERBSSurface(const GERBSSurface<T>& replicant);
  virtual ~GERBSSurface();

  // Public virtual functions from PSurf
  bool isClosedU() const override;
  bool isClosedV() const override;

  // Public members for GERBSSurface

  protected:
  // Protected functions for GERBSSurface

  // Protected virtual functions from PSurf
  void eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true) const;
  void localSimulate(double dt) override;
  T getStartPU() const override;
  T getStartPV() const override;
  T getEndPU() const override;
  T getEndPV() const override;

  // Protected members for GERBSSurface

  private:
  // Private functions for GERBSSurface
  T wFunctionU(const int& d, const int& i, T t) const;
  T wFunctionV(const int& d, const int& i, T t) const;
  T blendingFunction(T t, bool derivate) const;
  int iFinder(T t, bool useU) const;
  void createSubSurfaces();
  void createSubSurface(int u, int v);
  void createKnotsU();
  void createKnotsV();

  // Private virtual functions from PSurf

  // Private members for GERBSSurface
  GMlib::DMatrix<GMlib::PSurf<T, 3>*> _c;
  GMlib::PSurf<T, 3>* _modelSurface;
  const int _d = 1, _k = _d + 1;
  int _n1, _n2;
  std::vector<T> _tU;
  std::vector<T> _tV;

  int _counter = 0;
  bool _up = false;
};

} // namespace tardzone

// Include GERBSSurface class function implementations
#include "gerbssurf.c"

#endif
