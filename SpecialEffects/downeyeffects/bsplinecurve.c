#include <QtDebug>
#include <stdexcept>

#include "bsplinecurve.h"

namespace tardzone {

template <typename T>
BSplineCurve<T>::BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& c)
    : _c(c)
    , _n(c.getDim())
{
  createKnots();
}

template <typename T>
BSplineCurve<T>::BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& p, int n)
    : _m(p.getDim())
{
  // Check if n is valid, if not change its value
  _n = _m <= _n ? _m - 1 : n;

  // Create a knot vector for given n
  createKnots();

  // Create control points based on p
  createCP(p);
}

template <typename T>
BSplineCurve<T>::BSplineCurve(const BSplineCurve<T>& replicant)
{
  //TODO
}

template <typename T>
BSplineCurve<T>::~BSplineCurve()
{
  //TODO
}

template <typename T>
T BSplineCurve<T>::getStartP() const
{
  return _t[_d];
}

template <typename T>
T BSplineCurve<T>::getEndP() const
{
  return _t[_n];
}

template <typename T>
bool BSplineCurve<T>::isClosed() const
{
  // TODO
  return false;
}

template <typename T>
void BSplineCurve<T>::createKnots()
{
  // Set dimension for t, dim(t) = n+k
  _t.resize(_n + _k);

  // Initialize value for t at index i to 1
  T ti = T(1);

  // For elements in t from k to end
  for (std::size_t i = _k; i < _t.size(); ++i) {
    // Set t at i to ti
    _t[i] = ti;

    // If i less than t size - k increment ti
    if (i < _t.size() - _k)
      ti += T(1);
  }
}

template <typename T>
void BSplineCurve<T>::createCP(const GMlib::DVector<GMlib::Vector<T, 3>>& p)
{
  // Calculate A matrix
  auto A = calculateAMatrix();

  // Create a copy of a so we can transpose it.
  auto At = A;
  At.transpose();

  // Get the D and b so we can get control points
  auto D = At * A;
  auto b = At * p;
  _c = D.invert() * b;
}

template <typename T>
GMlib::DMatrix<T> BSplineCurve<T>::calculateAMatrix()
{
  // Create A matrix, base, and some helper values.
  auto A = GMlib::DMatrix<T>(_m, _n, T(0));
  GMlib::Vector<T, 4> base;
  const T start = getStartP(), dt = (getEndP() - start) / (_m - 1);
  int i;

  // Calculate values for A
  for (int j = 0; j < _m; j++) {
    base = createBees(start + j * dt, i);
    for (int z = i - _d; z <= i; z++) {
      A[j][z] = base[z - i + _d];
    }
  }

  return A;
}

template <typename T>
void BSplineCurve<T>::eval(T t, int d, bool fromLeft) const
{
  // Firstly set dimension for p
  this->_p.setDim(d + 1);

  // Calculate basis functions and set i based on t
  int i;
  auto basis = createBees(t, i);

  // Set value for p[0]
  this->_p[0] = basis[0] * _c[i - 3] + basis[1] * _c[i - 2] + basis[2] * _c[i - 1] + basis[3] * _c[i];
}

template <typename T>
GMlib::Vector<T, 4> BSplineCurve<T>::createBees(T t, int& i) const
{
  // Calculate i based on t
  i = iFinder(t);

  // Calculate values for W's used in defining basis functions
  auto W1i = wFunction(1, i, t), W2i = wFunction(2, i, t), W2i1 = wFunction(2, i - 1, t);
  auto W3i2 = wFunction(3, i - 2, t), W3i1 = wFunction(3, i - 1, t), W3i = wFunction(3, i, t);

  // Create container for and set in calculated basis functions
  GMlib::Vector<T, 4> basis;
  basis[0] = (1 - W1i) * (1 - W2i1) * (1 - W3i2);
  basis[1] = ((1 - W1i) * (1 - W2i1) * W3i2) + ((((1 - W1i) * W2i1) + (W1i * (1 - W2i))) * (1 - W3i1));
  basis[2] = ((((1 - W1i) * W2i1) + (W1i * (1 - W2i))) * W3i1) + (W1i * W2i * (1 - W3i));
  basis[3] = W1i * W2i * W3i;

  return basis;
}

template <typename T>
int BSplineCurve<T>::iFinder(T t) const
{
  // Check if t is in legal range
  if (t < getStartP() || t > getEndP()) {
    QString err = QString("Illegal t in iFinder. Got: %1, %2 - %3 expected").arg(t).arg(getStartP()).arg(getEndP());
    throw std::invalid_argument(err.toStdString());
  }

  // Start searching at d and get value for n
  auto i = _d;

  // Increment i while t at i till i reach t (ends up being one interval over)
  while (_t[i] <= t)
    i++;

  // Check if i is larger or equal to n, set to n then (ends up being one interval over)
  if (_n <= i)
    i = _n;

  // Decrement by one, so i is in correct interval
  return --i;
}

template <typename T>
T BSplineCurve<T>::wFunction(const int& d, const int& i, T t) const
{
  // Check if t is between d and i arguments
  if (_t[i] <= t && t <= _t[i + d]) {
    // Calculate W_(d,i)(t) = (t - t_i)/(t_(i+d) - t_i)
    return (t - _t[i]) / (_t[i + d] - _t[i]);
  }

  QString err = QString("Illegal t wFunction. Got: %1, %2 - %3 expected").arg(t).arg(_t[i]).arg(_t[i + d]);
  throw std::invalid_argument(err.toStdString());
}

} // namespace tardzone
