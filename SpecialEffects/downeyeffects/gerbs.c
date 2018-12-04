#include <QtDebug>
#include <stdexcept>

#include "gerbs.h"

namespace tardzone {
template <typename T>
GERBSCurve<T>::GERBSCurve(GMlib::PCurve<T, 3>* model, int m)
    : GMlib::PCurve<T, 3>(50, 0, 1)
    , _n(m)
    , _modelCurve(model)
{

  createKnots();
  createSubCurves();
}

template <typename T>
GERBSCurve<T>::GERBSCurve(const GERBSCurve<T>& replicant)
{
  //TODO
}

template <typename T>
GERBSCurve<T>::~GERBSCurve()
{
  //TODO
}

template <typename T>
bool GERBSCurve<T>::isClosed() const
{
  return _modelCurve->isClosed();
}

template <typename T>
T GERBSCurve<T>::getStartP() const
{
  return _t[_d];
}

template <typename T>
T GERBSCurve<T>::getEndP() const
{
  return _t[_t.size() - _k];
}

template <typename T>
void GERBSCurve<T>::eval(T t, int d, bool) const
{
  this->_p.setDim(d + 1);

  int i = iFinder(t);
  auto b = createBee(wFunction(1, i, t));

  this->_p = (1 - b) * _C[i - 1]->evaluateParent(t, d) + _C[i]->evaluateParent(t, d) * b;
}

} // namespace tardzone
