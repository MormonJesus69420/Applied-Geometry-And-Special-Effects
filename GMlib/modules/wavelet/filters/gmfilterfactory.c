
#include "gmfilterfactory.h"

namespace GMlib {
namespace Wavelet {

template<typename T>
OrthoFilter<T>*
FilterFactory<T>::createHaarFilter() {
  GMlib::DVector<float> coefs;
  coefs.append(0.5);
  coefs.append(0.5);
  GMlib::Wavelet::OrthoFilter<float>* f = new OrthoFilter<float>;
  f->setFilterCoefs(coefs);
  return f;
}

template<typename T>
OrthoFilter<T>*
FilterFactory<T>::createDaub4Filter() {
  GMlib::DVector<float> coefs;
  coefs.append(0.34150635094622);
  coefs.append(0.59150635094587);
  coefs.append(0.15849364905378);
  coefs.append(-0.09150635094587);
  GMlib::Wavelet::OrthoFilter<float>* f = new OrthoFilter<float>;
  f->setFilterCoefs(coefs);
  return f;
}


}
}
