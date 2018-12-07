#ifndef TARDZONE_MODELCURVE_H
#define TARDZONE_MODELCURVE_H

#include <parametrics/gmpcurve.h>

namespace tardzone {

template <typename T>
class ModelCurve : public GMlib::PCurve<T, 3> {
  GM_SCENEOBJECT(ModelCurve)

  public:
  // Public functions for ModelCurve
  ModelCurve(T size = T(4));
  ModelCurve(const ModelCurve<T>& replicant);
  virtual ~ModelCurve();

  void setSize(T size);
  T getSize() const;

  // Public virtual functions from PCurve
  bool isClosed() const override;

  // Public members for ModelCurve

  protected:
  // Protected functions for ModelCurve

  // Protected virtual functions from PCurve
  void eval(T t, int d, bool fromLeft) const override;
  T getStartP() const override;
  T getEndP() const override;

  // Protected members for ModelCurve

  private:
  // Private functions for ModelCurve

  // Private virtual functions from PCurve

  // Private members for ModelCurve
  T _size;
};

} // namespace tardzone

// Include ModelCurve class function implementations
#include "modelcurve.c"

#endif
