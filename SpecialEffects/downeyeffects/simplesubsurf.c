#include "simplesubsurf.h"

namespace tardzone {
template <typename T>
SimpleSubSurface<T>::SimpleSubSurface(GMlib::PSurf<T, 3>* s, T su, T eu, T sv, T ev)
{
  set(s, su, eu, (su + eu) / 2, sv, ev, (sv + ev) / 2);
  // Set local coordinate system, origin in center point
  GMlib::DMatrix<GMlib::Vector<T, 3>> tr = _s->evaluateParent(_u, _v, 0, 0);
  _trans = tr[0][0];
  this->translateParent(_trans);

  this->setMaterial(GMlib::GMmaterial::polishedGreen());
}

template <typename T>
SimpleSubSurface<T>::SimpleSubSurface(GMlib::PSurf<T, 3>* s, T su, T eu, T u, T sv, T ev, T v)
{
  set(s, su, eu, u, sv, ev, v);
  // Set local coordinate system, origin in center point
  GMlib::DMatrix<GMlib::Vector<T, 3>> tr = _s->evaluateParent(_u, _v, 0, 0);
  _trans = tr[0][0];
  this->translateParent(_trans);

  this->setMaterial(GMlib::GMmaterial::polishedGreen());
}

template <typename T>
SimpleSubSurface<T>::SimpleSubSurface(const SimpleSubSurface<T>& replicant)
    : GMlib::PSurf<T, 3>(replicant)
{
  set(replicant._s, replicant._su, replicant._eu, replicant._u, replicant._sv, replicant._ev, replicant._v);
  // Set local coordinate system, origin in center point
  _trans = replicant._trans;
}

template <typename T>
void SimpleSubSurface<T>::eval(T u, T v, int d1, int d2, bool lu, bool lv) const
{
  this->_p.setDim(3, 3);
  this->_p = _s->evaluateParent(u, v, d1, d2);
  this->_p[0][0] -= _trans;
}

template <typename T>
T SimpleSubSurface<T>::getStartPU() const
{
  return _su;
}

template <typename T>
T SimpleSubSurface<T>::getEndPU() const
{
  return _eu;
}

template <typename T>
T SimpleSubSurface<T>::getStartPV() const
{
  return _sv;
}

template <typename T>
T SimpleSubSurface<T>::getEndPV() const
{
  return _ev;
}

template <typename T>
void SimpleSubSurface<T>::set(GMlib::PSurf<T, 3>* s, T su, T eu, T u, T sv, T ev, T v)
{
  _s = s;
  _su = su;
  _sv = sv;
  _eu = eu;
  _ev = ev;
  _u = u;
  _v = v;
}
} // namespace tardzone
