#include <QtDebug>
#include <stdexcept>

#include "modelcurve.h"

namespace tardzone {
template <typename T>
ModelCurve<T>::ModelCurve(T size)
    : GMlib::PCurve<T, 3>(0, 0, 2)
    , _size(size)
{
}

template <typename T>
ModelCurve<T>::ModelCurve(const ModelCurve<T>& replicant)
    : GMlib::PCurve<T, 3>(replicant)
{
  //TODO
}

template <typename T>
ModelCurve<T>::~ModelCurve()
{
  //TODO
}

template <typename T>
void ModelCurve<T>::setSize(T size)
{
  _size = size;
}

template <typename T>
T ModelCurve<T>::getSize() const
{
  return _size;
}

template <typename T>
bool ModelCurve<T>::isClosed() const
{
  // Butterfly curve is closed.
  return true;
}

template <typename T>
void ModelCurve<T>::eval(T t, int d, bool fromLeft) const
{
  // Set dimension for p
  this->_p.setDim(d + 1);

  // Calculate value for a given t based on formula 4.3, page 70 in Blending technics[sic] for Curve and Surface constructions by Arne Lakså
  const double cosT = std::cos(t);
  const double sinT = std::sin(t);
  const double sinT12 = std::sin(t / 12);
  const double inner = std::exp(cosT) - (2 * std::cos(4 * t)) - std::pow(sinT12, 5.0);

  // Set point's xyz position
  this->_p[0]= {_size * T(inner * cosT), 0 ,_size * T(inner * sinT)};
}

template <typename T>
T ModelCurve<T>::getStartP() const {
  return T(0);
}

template <typename T>
T ModelCurve<T>::getEndP() const {
  return T( M_PI * 24.0 );
}
} // namespace tardzone
