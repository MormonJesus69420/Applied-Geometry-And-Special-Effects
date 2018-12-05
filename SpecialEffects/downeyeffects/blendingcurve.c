#include <QtDebug>
#include <stdexcept>

#include "blendingcurve.h"

namespace tardzone {

template <typename T>
BlendingCurve<T>::BlendingCurve(std::shared_ptr<GMlib::PCurve<T, 3>> c1, std::shared_ptr<GMlib::PCurve<T, 3>> c2, T blendpoint)
    : _c1(c1)
    , _c2(c2)
    , _blendpoint(blendpoint)
{
}

template <typename T>
BlendingCurve<T>::~BlendingCurve()
{
  //TODO
}

template <typename T>
T BlendingCurve<T>::getStartP() const
{
  // Blending start at 0
  return T(0);
}

template <typename T>
T BlendingCurve<T>::getEndP() const
{
  // Blending ends at 1 + blendpoint, for example 1.2 if blendpoint is 0.2
  return T(1) + _blendpoint;
}

template <typename T>
bool BlendingCurve<T>::isClosed() const
{
  //TODO
  return false;
}

template <typename T>
T BlendingCurve<T>::blendingFunction(T t) const
{
  // I am using Polynomial Blending function, since it easy to derivate it if I need to

  return (3 * t * t) - (2 * t * t * t);
}

template <typename T>
void BlendingCurve<T>::eval(T t, int d, bool fromLeft) const
{
  this->_p.setDim(d + 1);
  auto posC1 = _c1->evaluate(t * _c1->getParDelta(), 0)[0];
  auto posC2 = _c2->evaluate((t - _blendpoint) * _c2->getParDelta(), 0)[0];

  // First curve, since t is between start point and blendpoint.
  if (getStartP() <= t && t < _blendpoint) {
    // GetParDelta -> Return the length of the domain of the curve
    this->_p[0] = posC1;
  } // Blending point
  else if (_blendpoint <= t && t < T(1)) {
    // Credit to Kent Arne Larsen for finding formula for blend function letting us change blending point
    this->_p[0] = posC1 + blendingFunction(t / (1 - _blendpoint) - _blendpoint / (1 - _blendpoint)) * (posC2 - posC1);
  } // Second curve, since t is beetween 1 and end point
  else if (T(1) <= t && t <= getEndP()) {
    this->_p[0] = posC2;
  }
}

template <typename T>
void BlendingCurve<T>::localSimulate(double dt [[maybe_unused]])
{
  this->resample();
}

} // namespace tardzone
