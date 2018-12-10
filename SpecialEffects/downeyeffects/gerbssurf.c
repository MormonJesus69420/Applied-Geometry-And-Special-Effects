#include <QtDebug>

#include "gerbssurf.h"

namespace tardzone {

template <typename T>
GERBSSurface<T>::GERBSSurface(GMlib::PSurf<T, 3>* model, int n1, int n2)
    : _modelSurface(model)
    , _n1(n1)
    , _n2(n2)
{
  // Needs to set the derivation method so that the eval function works correctly
  this->setDerivationMethod(GMlib::GM_DERIVATION_METHOD::GM_DERIVATION_DD);

  // Set size for the matrix of subsurfaces
  _c.setDim(n1 + 1, n2 + 1);

  // Create knot vectors
  createKnotsU();
  createKnotsV();

  // Create subsurfaces
  createSubSurfaces();

    for(auto i = 0; i <= _n1; ++i) {
      _c[i][4]->translateParent({0.0f, -1.0f, 0.0f});
      _c[i][12]->translateParent({0.0f, 1.0f, 0.0f});
    }

    if (isClosedU()) {
      _c[_n1][4]->translateParent({0.0f, 1.0f, 0.0f});
      _c[_n1][12]->translateParent({0.0f, -1.0f, 0.0f});
    }

}

template <typename T>
GERBSSurface<T>::GERBSSurface(const GERBSSurface<T>& replicant)
{
  //TODO
}

template <typename T>
GERBSSurface<T>::~GERBSSurface()
{
  //TODO
}

template <typename T>
bool GERBSSurface<T>::isClosedU() const
{
  return _modelSurface->isClosedU();
}

template <typename T>
bool GERBSSurface<T>::isClosedV() const
{
  return _modelSurface->isClosedV();
}

template <typename T>
void GERBSSurface<T>::eval(T u, T v, int d1, int d2, bool lu, bool lv) const
{
  // Set dimension for p
  this->_p.setDim(_n1 + 1, _n2 + 1);

  // Find iU and iV and blend at this point
  auto iU = iFinder(u, true);
  auto iV = iFinder(v, false);

  // Blend in U and V
  T bU = blendingFunction(wFunctionU(1, iU, u), false);
  T bV = blendingFunction(wFunctionV(1, iV, v), false);

  // Evaluate all 4 subsurfaces at given point
  auto c1 = _c(iU - 1)(iV - 1)->evaluateParent(u, v, d1, d2);
  auto c2 = _c(iU)(iV - 1)->evaluateParent(u, v, d1, d2);
  auto c3 = _c(iU - 1)(iV)->evaluateParent(u, v, d1, d2);
  auto c4 = _c(iU)(iV)->evaluateParent(u, v, d1, d2);

  // Calculate formula for the surface
  auto b1 = ((1 - bU) * c1) + (bU * c2);
  auto b2 = ((1 - bU) * c3) + (bU * c4);
  auto P = (1 - bV) * b1 + bV * b2;

  // Set Position
  this->_p[0][0] = P[0][0];
}

template <typename T>
void GERBSSurface<T>::localSimulate(double dt)
{
//  if(++_counter % 100 == 0) {
//    _up = !_up;
//    _counter = 0;
//  }

//  for(auto i = 0; i < _n1; ++i) {
//    _c[i][4]->translateParent({0.0f, _up ? -0.01f: 0.01f, 0.0f});
//    _c[i][12]->translateParent({0.0f, _up ? 0.01f: -0.01f, 0.0f});
//  }

//  this->replot(25,25,1,1);

}

template <typename T>
T GERBSSurface<T>::getStartPU() const
{
  return _tU[_d];
}

template <typename T>
T GERBSSurface<T>::getStartPV() const
{
  return _tV[_d];
}

template <typename T>
T GERBSSurface<T>::getEndPU() const
{
  return _tU[_tU.size() - _k];
}

template <typename T>
T GERBSSurface<T>::getEndPV() const
{
  return _tV[_tV.size() - _k];
}

template <typename T>
T GERBSSurface<T>::wFunctionU(const int& d, const int& i, T t) const
{
  // Calculate W function for the u knot vector
  return (t - _tU[i]) / (_tU[i + d] - _tU[i]);
}

template <typename T>
T GERBSSurface<T>::wFunctionV(const int& d, const int& i, T t) const
{
  // Calculate W function for the v knot vector
  return (t - _tV[i]) / (_tV[i + d] - _tV[i]);
}

template <typename T>
T GERBSSurface<T>::blendingFunction(T t, bool derivate) const
{
  if (!derivate) {
    // If not derivating, then return original polynomial blending function
    return (3 * t * t) - (2 * t * t * t);
  }
  else {
    // Else return derivated blending function
    return (6 * t) - (6 * t * t);
  }
}

template <typename T>
int GERBSSurface<T>::iFinder(T t, bool useU) const
{
  // Start searching at d
  auto i = _d;

  // Increment i while t at i till i reach t (ends up being one interval over)
  while (useU && _tU[i] <= t)
    i++;

  while (!useU && _tV[i] <= t)
    i++;

  // Check if i is larger or equal to n, set to n then (ends up being one interval over)
  if (useU && _n1 + 1 < i)
    i = _n1 + 1;

  if (!useU && _n2 + 1 < i)
    i = _n2 + 1;

  // Decrement by one, so i is in correct interval
  return --i;
}

template <typename T>
void GERBSSurface<T>::createSubSurfaces()
{
  int u, v;

  // Fill in all the rows except last one
  for (u = 0; u < _n1; u++) {
    // Fill in all columns except last one
    for (v = 0; v < _n2; v++) {
      createSubSurface(u, v);
    }

    if (isClosedV()) {
      // If closed in v then last subsurf in column is same as first one
      _c[u][_n2] = _c[u][0];
    }
    else {
      // If not closed then make new subsurface for the last column
      createSubSurface(u, _n2);
    }
  }

  // Fill in last row
  for (v = 0; v <= _n2; v++) {
    if (isClosedU()) {
      // If closed in u then subsurf in last row is same as in the first one
      _c[_n1][v] = _c[0][v];
    }
    else {
      // If not closed then make new subsurface for this row
      createSubSurface(_n1, v);
    }
  }
}

template <typename T>
void GERBSSurface<T>::createSubSurface(int u, int v)
{
  // Create local subsurface using the provided u and v indices
  _c[u][v] = new SimpleSubSurface<T>(_modelSurface,
      _tU[u], _tU[u + 2], _tU[u + 1],
      _tV[v], _tV[v + 2], _tV[v + 1]);

  _c[u][v]->toggleDefaultVisualizer();
  _c[u][v]->setCollapsed(true);
  _c[u][v]->replot(10, 10, 1, 1);
  this->insert(_c[u][v]);
}

template <typename T>
void GERBSSurface<T>::createKnotsU()
{
  // Set dimension for t, dim(t) = n+k+1
  _tU.resize(_n1 + _k + 1);

  // Initialize value for t at index i to start parameter value of model curve
  T ti = _modelSurface->getParStartU();

  // Initialize vars used to calculate values for knot vector.
  auto dt = _modelSurface->getParDeltaU() / _n1;
  T start = ti;

  // Calculate values for knot vector
  for (int i = 0; i < (int)_tU.size(); ++i) {
    // Before we reach k values are equal to start parameter of model curve.
    // From k to n values are calculated based on start param, i and scaled step (dt).
    // After n values are equal end parameter of model curve.
    if (i >= _k && i <= _n1) {
      ti = start + (i - 1) * dt;
    }
    else if (i == _n1 + 1) {
      ti = _modelSurface->getParEndU();
    }

    // Set t at i to ti
    _tU[i] = ti;
  }

  // If model surface is closed in V we need to adjust the start and end knots.
  if (isClosedV()) {
    // First knot needs to be equal to second knot minus distance between third and second knot from the end.
    _tU[0] = _tU[1] - (_tU[_n1 + 1] - _tU[_n1]);
    // Last knot needs to be equal to next to last knot minus distance between second and third knot from start
    _tU[_n1 + 2] = _tU[_n1 + 1] + _tU[2] - _tU[1];
  }

  qDebug() << "Tu" << _tU;
}

template <typename T>
void GERBSSurface<T>::createKnotsV()
{
  // Set dimension for t, dim(t) = n+k+1
  _tV.resize(_n2 + _k + 1);

  // Initialize value for t at index i to start parameter value of model curve
  T ti = _modelSurface->getParStartV();

  // Initialize vars used to calculate values for knot vector.
  auto dt = _modelSurface->getParDeltaV() / _n2;
  T start = ti;

  // Calculate values for knot vector
  for (int i = 0; i < (int)_tV.size(); ++i) {
    // Before we reach k values are equal to start parameter of model curve.
    // From k to n values are calculated based on start param, i and scaled step (dt).
    // After n values are equal end parameter of model curve.
    if (i >= _k && i <= _n2) {
      ti = start + (i - 1) * dt;
    }
    else if (i == _n2 + 1) {
      ti = _modelSurface->getParEndV();
    }

    // Set t at i to ti
    _tV[i] = ti;
  }

  // If model curve is closed in U we need to adjust the start and end knots.
  if (isClosedU()) {
    // First knot needs to be equal to second knot minus distance between third and second knot from the end.
    _tV[0] = _tV[1] - (_tV[_n2 + 1] - _tV[_n2]);
    // Last knot needs to be equal to next to last knot minus distance between second and third knot from start
    _tV[_n2 + 2] = _tV[_n2 + 1] + _tV[2] - _tV[1];
  }
  qDebug() << "Tv" << _tV;
}
} // namespace tardzone
