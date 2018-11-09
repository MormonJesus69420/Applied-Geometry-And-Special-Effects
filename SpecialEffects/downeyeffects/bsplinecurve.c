#include <QtDebug>
#include <stdexcept>

namespace tardzone {

template <class T>
BSplineCurve<T>::BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& c)
    : _c(c)
{
  knotMeDaddy(c.getDim());
}

template <class T>
BSplineCurve<T>::BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& p, int n)
{
  //TODO
}

template <class T>
BSplineCurve<T>::BSplineCurve(const BSplineCurve<T>& replicant)
{
  //TODO
}

template <class T>
BSplineCurve<T>::~BSplineCurve()
{
  //TODO
}

template <class T>
T BSplineCurve<T>::getStartP() const
{
  return _t[_d];
}

template <class T>
T BSplineCurve<T>::getEndP() const
{
  return _t[_c.getDim()];
}

template <class T>
bool BSplineCurve<T>::isClosed() const
{
  // TODO
  return false;
}

template <class T>
void BSplineCurve<T>::knotMeDaddy(int n)
{
  // Set dimension for t, dim(t) = n+k
  _t.resize(n + _k);

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

template <class T>
T BSplineCurve<T>::wtf(int d, int i, T t) const
{
  // Check if t is between d and i arguments
  if (_t[i] <= t && t <= _t[i + d]) {
    // Calculate W_(d,i)(t) = (t - t_i)/(t_(i+d) - t_i)
    return (t - _t[i]) / (_t[i + d] - _t[i]);
  }

  QString err = QString("Illegal t value received for wFunction. Got: %1 legal bounds: %2-%3").arg(t).arg(_t[i]).arg(_t[i + d]);
  throw std::invalid_argument(err.toStdString());
}

template <class T>
int BSplineCurve<T>::theyAreInMyEyes(T t) const
{
  // Check if t is in legal range
  if (t < getStartP() || t > getEndP()) {
    QString err = QString("Illegal t value received for findI. Got: %1 legal bounds: %2-%3").arg(t).arg(getStartP()).arg(getEndP());
    throw std::invalid_argument(err.toStdString());
  }

  // Start searching at d and get value for n
  auto i = _d, n = _c.getDim();

  // Increment i while t at i is less than t (ends up being one interval over)
  while (_t[i] < t)
      i++;

  // Check if i is larger or equal to n, set to n then (ends up being one interval over)
  if (n <= i)
    i = n;

  // Decrement by one, so i is in correct interval
  return --i;
}

template <typename T>
void BSplineCurve<T>::eval(T t, int d, bool l) const
{
  this->_p.setDim(d + 1);

  int i;
  auto basis = releaseTheBees(t, i);

  this->_p[0] = basis[0] * _c[i - 3] + basis[1] * _c[i - 2] + basis[2] * _c[i - 1] + basis[3] * _c[i];
}

template <typename T>
GMlib::Vector<T, 4> BSplineCurve<T>::releaseTheBees(T t, int& i) const
{
  i = theyAreInMyEyes(t);

  auto W1i = wtf(1, i, t), W2i = wtf(2, i, t), W2i1 = wtf(2, i - 1, t);
  auto W3i2 = wtf(3, i - 2, t), W3i1 = wtf(3, i - 1, t), W3i = wtf(3, i, t);

  auto a = (1 - W1i) * (1 - W2i1);
  auto b = ((1 - W1i) * W2i1) + (W1i * (1 - W2i));
  auto c = W1i * W2i;

  GMlib::Vector<T, 4> basis;
  basis[0] = a * (1 - W3i2);
  basis[1] = (a * W3i2) + (b * (1 - W3i1));
  basis[2] = (b * W3i1) + (c * (1 - W3i));
  basis[3] = c * W3i;

  return basis;
}

} // namespace tardzone
