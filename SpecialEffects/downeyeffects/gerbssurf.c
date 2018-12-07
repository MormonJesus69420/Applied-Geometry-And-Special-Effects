#include "gerbssurf.h"

namespace tardzone {

template <typename T>
GERBSSurface<T>::GERBSSurface(GMlib::PSurf<T, 3>* model, int n1, int n2) {

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
void GERBSSurface<T>::eval(T u, T v, int d1, int d2, bool lu, bool lv) const {}

template <typename T>
void GERBSSurface<T>::localSimulate(double dt) {}

template <typename T>
void GERBSSurface<T>::eval(T t, int d, bool fromLeft) const {}

template <typename T>
T GERBSSurface<T>::getStartPU() const
{
  return _tu[_d];
}

template <typename T>
T GERBSSurface<T>::getStartPV() const
{
  return _tu[_tu.size() - _k];
}

template <typename T>
T GERBSSurface<T>::getEndPU() const
{
  return _tv[_d];
}

template <typename T>
T GERBSSurface<T>::getEndPV() const
{
  return _tv[_tv.size() - _k];
}

template <typename T>
T GERBSSurface<T>::wFunction(const int& d, const int& i, T t) const {}

template <typename T>
T GERBSSurface<T>::blendingFunction(T t) const {}

template <typename T>
int GERBSSurface<T>::iFinder(T t) const {}

template <typename T>
void GERBSSurface<T>::createSubSurfaces() {}

template <typename T>
void GERBSSurface<T>::createKnots() {}
} // namespace tardzone
