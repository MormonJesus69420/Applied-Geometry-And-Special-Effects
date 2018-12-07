#ifndef TARDZONE_PSIMPLESUBSURF_H
#define TARDZONE_PSIMPLESUBSURF_H

#include <parametrics/gmpsurf.h>

namespace tardzone {

template <typename T>
class SimpleSubSurface : public GMlib::PSurf<T, 3> {
  GM_SCENEOBJECT(SimpleSubSurface)

  public:
  // Public functions for SimpleSubSurface
  SimpleSubSurface(GMlib::PSurf<T, 3>* s, T su, T eu, T sv, T ev);
  SimpleSubSurface(GMlib::PSurf<T, 3>* s, T su, T eu, T u, T sv, T ev, T v);
  SimpleSubSurface(const SimpleSubSurface<T>& replicant);
  virtual ~SimpleSubSurface() {}

  // Public virtual functions from PSurf

  // Public members for SimpleSubSurface

  protected:
  // Protected functions for ModelCurve

  // Protected virtual functions from PCurve
  void eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true) const override;
  T getStartPU() const override;
  T getEndPU() const override;
  T getStartPV() const override;
  T getEndPV() const override;

  // Protected members for ModelCurve
  GMlib::PSurf<T, 3>* _s; // The original surface
  T _su; // Start parameter value in u-direction
  T _sv; // Start parameter value in v-direction
  T _eu; // End parameter value in u-direction
  T _ev; // End parameter value in v-direction
  T _u; // Center parameter value in u-direction
  T _v; // Center parameter value in v-direction
  GMlib::Vector<float, 3> _trans; // Translation so center is origin in local coordinate system

  private:
  // Private functions for ModelCurve
  void set(GMlib::PSurf<T, 3>* s, T su, T eu, T u, T sv, T ev, T v);

  // Private virtual functions from PCurve

  // Private members for ModelCurve

}; // END class PSimpleSubSurf
} // namespace tardzone

// Include SimpleSubSurface class function implementations
#include "simplesubsurf.c"

#endif
