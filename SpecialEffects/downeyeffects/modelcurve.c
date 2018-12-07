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

  // Based on parametric heart formula found here https://tpenguinltg.wordpress.com/2014/02/15/representing-the-heart-shape-precisely/
  const double x = 16 * std::pow(std::sin(t), 3);
  const double z = 13 * std::cos(t) - 5 * std::cos(2 * t) - 2 * std::cos(3 * t) - std::cos(4 * t);

  this->_p[0] = { _size * T(x), 0, _size * T(z) };
}

template <typename T>
T ModelCurve<T>::getStartP() const
{
  return T(0);
}

template <typename T>
T ModelCurve<T>::getEndP() const
{
  return T(M_2PI);
}
} // namespace tardzone
