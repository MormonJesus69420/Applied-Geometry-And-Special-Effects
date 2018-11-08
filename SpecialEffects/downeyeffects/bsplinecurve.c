#include <stdexcept>

namespace tardzone {

template <class T>
BSplineCurve<T>::BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& c)
    : _c(c)
{
  knotMeDaddy();
}

template <class T>
BSplineCurve<T>::BSplineCurve(const BSplineCurve<T>& replicant)
{
  //TODO
}

template <class T>
BSplineCurve<T>::BSplineCurve(const GMlib::DVector<GMlib::Vector<T, 3>>& p, int n)
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
void BSplineCurve<T>::knotMeDaddy()
{
  // Set dimension for t, dim(t) = n+k
  _t.resize(_c.getDim() + _k);

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
T BSplineCurve<T>::wFunction(int d, int i, T t)
{
  // Check if t is between d and i arguments
  if (_t[i] > t || t >= _t[i + d])
    throw std::invalid_argument("Illegal t value received for wFunction. Got: " + t + " legal bounds: " + _t[i] + "-" + _t[i + d]);

  // Calculate W_(d,i)(t) = (t - t_i)/(t_(i+d) - t_i)
  return (t - _t[i]) / (_t[i + d] - _t[i]);
}

template <class T>
int BSplineCurve<T>::findI(T t)
{
  // Check if t is in legal range
  if (t < getStartP() || t > getEndP())
    throw std::invalid_argument("Illegal t value received for findI. Got: " + t + " legal bounds: " + getStartP() + "-" + getEndP());

  // Start searching at d and get value for n
  int i = _d, n = _c.getDim();

  // Increment i while t at i is less than or equal t
  while (_t[i] <= t)
    ++i;

  // Check if i is larger or equal to n, decrement by one if so
  if (n <= i)
    i = n - 1;

  return i;
}

template <typename T>
void BSplineCurve<T>::eval(T t, int d, bool l) const
{
}

template <typename T>
GMlib::Vector<T, 4> BSplineCurve<T>::releaseTheBees(T t, int& i)
{
  return GMlib::Vector<T, 4>();
}

} // namespace tardzone
