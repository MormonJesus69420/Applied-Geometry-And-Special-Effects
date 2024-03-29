#include <QtDebug>
#include <stdexcept>

#include "gerbscurve.h"

namespace tardzone {
template <typename T>
GERBSCurve<T>::GERBSCurve(GMlib::PCurve<T, 3>* model, int m)
    : _modelCurve(model)
    , _n(m)
{
  // Setup random generator and distribution units
  std::random_device rd;
  _gen = std::mt19937(rd());
  _dis = std::uniform_real_distribution<double>(0.0, 0.5);
  _step = std::uniform_real_distribution<double>(0.0, 0.005);

  // Set random start colour
  _hue = _dis(_gen);
  _colour = GMlib::Color(_hue, 1.0, 1.0);
  _colour.toHSV();
  this->setColor(_colour);

  // Setup curve
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
  // Set dimension for p
  this->_p.setDim(d + 1);

  // Find i and blend two curves at this point
  auto i = iFinder(t);
  T b1 = T(1 - blendingFunction(wFunction(1, i, t)));
  T b2 = T(blendingFunction(wFunction(1, i, t)));

  // Set position value based on curve at i and previous curve with their blend values.
  this->_p[0] = _c[i - 1]->evaluateParent(t, 0)[0] * b1 + _c[i]->evaluateParent(t, 0)[0] * b2;
}

template <typename T>
void GERBSCurve<T>::createKnots()
{
  // Set dimension for t, dim(t) = n+k+1
  _t.resize(_n + _k + 1);

  // Initialize value for t at index i to start parameter value of model curve
  T ti = _modelCurve->getParStart();

  // Initialize vars used to calculate values for knot vector.
  auto dt = _modelCurve->getParDelta() / _n;
  T start = ti;

  // Calculate values for knot vector
  for (int i = 0; i < (int)_t.size(); ++i) {
    // Before we reach k values are equal to start parameter of model curve.
    // From k to n values are calculated based on start param, i and scaled step (dt).
    // After n values are equal end parameter of model curve.
    if (i >= _k && i <= _n) {
      ti = start + (i - 1) * dt;
    }
    else if (i == _n + 1) {
      ti = _modelCurve->getParEnd();
    }

    // Set t at i to ti
    _t[i] = ti;
  }

  // If model curve is closed we need to adjust the start and end knots.
  if (isClosed()) {
    // First knot needs to be equal to second knot minus distance between third and second knot from the end.
    _t[0] = _t[1] - (_t[_n + 1] - _t[_n]);
    // Last knot needs to be equal to next to last knot minus distance between second and third knot from start
    _t[_n + 2] = _t[_n + 1] + (_t[2] - _t[1]);
  }
}

template <typename T>
void GERBSCurve<T>::createSubCurves()
{
  // Set dimension for c, dim(c) = n+1
  _c.resize(_n + 1);

  // Create curves from 0 to n
  for (auto i = 0; i <= _n; ++i) {
    // If curve is closed, last element must be the same as first one
    if (i == _n && isClosed()) {
      _c[_n] = _c[0];
      break;
    }

    // Make shared pointer to subcurve using model curve and knot vectors.
    _c[i] = std::make_shared<GMlib::PSubCurve<T>>(_modelCurve, _t[i], _t[i + 2], _t[i + 1]);
    this->insert(_c[i].get());
    _c[i]->toggleDefaultVisualizer();
    _c[i]->sample(100, 4);
    _c[i]->setVisible(true);
    _c[i]->setCollapsed(true);
  }

  // Used to colour curves different colours to differentiate them
  _c[0]->setColor(GMlib::GMcolor::aqua());
  _c[1]->setColor(GMlib::GMcolor::black());
  _c[2]->setColor(GMlib::GMcolor::brown());
  _c[3]->setColor(GMlib::GMcolor::red());
  _c[4]->setColor(GMlib::GMcolor::green());
  _c[5]->setColor(GMlib::GMcolor::yellow());
  _c[6]->setColor(GMlib::GMcolor::violet());
  _c[7]->setColor(GMlib::GMcolor::lightPink());
  _c[8]->setColor(GMlib::GMcolor::purple());
  _c[9]->setColor(GMlib::GMcolor::white());
}

template <typename T>
int GERBSCurve<T>::iFinder(T t) const
{
  // Check if t is in legal range
  if (t < getStartP() || t > getEndP()) {
    QString err = QString("Illegal t in iFinder. Got: %1, %2 - %3 expected").arg(t).arg(getStartP()).arg(getEndP());
    throw std::invalid_argument(err.toStdString());
  }

  // Start searching at d
  auto i = _d;

  // Increment i while t at i till i reach t (ends up being one interval over)
  while (_t[i] <= t)
    i++;

  // Check if i is larger or equal to n, set to n then (ends up being one interval over)
  if (_n + 1 < i)
    i = _n + 1;

  // Decrement by one, so i is in correct interval
  return --i;
}

template <typename T>
T GERBSCurve<T>::blendingFunction(T t) const
{
  // I am using Polynomial Blending function, since it easy to derivate it if I need to
  return (3 * t * t) - (2 * t * t * t);
}

template <typename T>
T GERBSCurve<T>::wFunction(const int& d, const int& i, T t) const
{
  // Check if t is between d and i arguments
  if (_t[i] <= t && t <= _t[i + d]) {
    // Calculate W_(d,i)(t) = (t - t_i)/(t_(i+d) - t_i)
    return (t - _t[i]) / (_t[i + d] - _t[i]);
  }

  QString err = QString("Illegal t wFunction. Got: %1, %2 - %3 expected").arg(t).arg(_t[i]).arg(_t[i + d]);
  throw std::invalid_argument(err.toStdString());
}

template <typename T>
void GERBSCurve<T>::localSimulate(double dt)
{
  // If hue is over 1 or under 0 switch the up variable
  if (_hue <= 0.0) {
    _hue = 0.0;
    _up = true;
  }
  else if (_hue >= 1.0) {
    _hue = 1.0;
    _up = false;
  }

  // Change colour value based on hue
  _hue += _up ? _step(_gen) : -_step(_gen);
  _colour.setRed(_hue);
  _colour.toHSV();
  this->setColor(_colour);

  // Calculate rotation and translation values based on dt
  _beat += _up ? 0.005 * dt : -0.005 * dt;
  float trans1 = _beat * 0.1f;
  float trans3 = _beat * 0.3f;
  float trans4 = _beat * 0.4f;
  float rot01 = _beat * 0.01f;

  // Translate subcurves
  _c[0]->translate({ 0.0f, 0.0f, _up ? -trans1 : trans1 });
  _c[1]->translate({ _up ? trans1 : -trans1, 0.0f, 0.0f });
  _c[2]->translate({ _up ? trans3 : -trans3, 0.0f, 0.0f });
  _c[3]->translate({ _up ? trans4 : -trans4, 0.0f, 0.0f });
  _c[4]->translate({ _up ? trans1 : -trans1, 0.0f, 0.0f });
  _c[5]->translate({ 0.0f, 0.0f, _up ? -trans1 : trans1 });
  _c[6]->translate({ _up ? -trans1 : trans1, 0.0f, 0.0f });
  _c[7]->translate({ _up ? -trans4 : trans4, 0.0f, 0.0f });
  _c[8]->translate({ _up ? -trans3 : trans3, 0.0f, 0.0f });
  _c[9]->translate({ _up ? -trans1 : trans1, 0.0f, 0.0f });

  // Rotate curves on top to smooth out the heart as it stretches out
  _c[1]->rotateParent(_up ? -rot01 : rot01, _rot1);
  _c[2]->rotateParent(_up ? -rot01 : rot01, _rot2);
  _c[8]->rotateParent(_up ? rot01 : -rot01, _rot2);
  _c[9]->rotateParent(_up ? rot01 : -rot01, _rot1);

  // Resample the curve
  this->sample(100, 0);
}
} // namespace tardzone
