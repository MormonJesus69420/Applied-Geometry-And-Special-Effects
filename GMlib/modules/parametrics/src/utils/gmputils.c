
#include "../gmpcurve.h"


template <typename T, int n>
void GMlib::GMPutils::printErrors(PCurve<T, n>& f, PCurve<T, n>& g) {
  float a = f.getParStart();
  float b = f.getParEnd();
  float eps = 10e-10;

  float l2 = GMlib::GMPutils::getL2(f, g, a, b, eps);
  float g2 = GMlib::GMPutils::getG2(f, g, a, b, eps);
  float l_inf = GMlib::GMPutils::getLInf(f, g, a, b, 1000);
  float g_inf = GMlib::GMPutils::getGInf(f, g, a, b, 1000);
  std::cout << " L2: " << l2 << " G2: " << g2 << " L_inf: " << l_inf << " G_inf: " << g_inf << std::endl;
}


template <typename T, int n>
T GMlib::GMPutils::getG2(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, double eps) {
 return std::sqrt( getIntegralClosestPointSqr(f, g, a, b, eps) / getIntegralDerivativeLength(f, a, b, eps) );
}

template <typename T, int n>
T GMlib::GMPutils::getGInf(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, int samples) {
 T delta = b - a;
 T delta_t = delta / T(samples);
 T err (0), g_t(0);
 Point<T, n> f_eval, g_closest;
 for (int i=0; i < samples; i++) {
   f_eval = f.evaluateGlobal(a + i*delta_t, 0)[0];
   bool ok = g.getClosestPoint(f_eval, g_t, g_closest);
   if (!ok) std::cout << "getGInf: closest point failed..." << std::endl;
   err = std::max( err, (f_eval - g_closest).getLength() );
 }
 return err;
}

/*!
* L^2 norm
*/
template <typename T, int n>
T GMlib::GMPutils::getL2(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, double eps) {
 return std::sqrt( (T(1) / (b - a)) * getIntegralDiffSqr(f, g, a, b, eps) );
}

/*!
* L^inf Max-norm
*/
template <typename T, int n>
T GMlib::GMPutils::getLInf(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, int samples) {
 T delta = b - a;
 T delta_t = delta / T(samples);
 T err (0), diff(0);
 for (int i=0; i < samples; i++) {
   diff = (f.evaluate(a + i*delta_t, 0)[0] - g.evaluate(a + i*delta_t, 0)[0]).getLength();
   err = std::max( err, diff );
 }
 return err;
}

template <typename T, int n>
T GMlib::GMPutils::getDiffCp(PCurve<T,n>& f, PCurve<T,n>& g, T t) {
 DVector<Vector<T,n> > f_val = f.evaluateParent(t, 0);
 Point<T, n> g_p;
 bool ok = g.getClosestPoint(f_val[0], t, g_p);
 if (!ok) std::cout << "_getDiffCp: closest point failed..." << std::endl;
 Vector<T,n>           x     = f_val[0] - g_p;
 return x * x;
}

template <typename T, int n>
T GMlib::GMPutils::getDiffSqr(PCurve<T,n>& f, PCurve<T,n>& g, T t) {
 DVector<Vector<T,n> > f_val = f.evaluateParent(t, 0);
 DVector<Vector<T,n> > g_val = g.evaluateParent(t, 0);
 Vector<T,n>           x     = f_val[0] - g_val[0];
 return x * x;
}


template <typename T, int n>
T GMlib::GMPutils::getIntegralClosestPointSqr(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, double eps) {

 T t = b - a;
 T sum =
     (getDiffCp(f, g, a) * f.evaluateGlobal(a, 1)[1].getLength() +
      getDiffCp(f, g, b) * f.evaluateGlobal(b, 1)[1].getLength()
     ) / T(2);

 T mat[16][16];
 mat[0][0] = sum * t;

 T s;
 int i,j,k;
 for( i = 1; i < 16; i++ ) {
   s = T(0);
   k = 1 << i;
   t /= 2.0;
   for( j = 1; j < k; j += 2 )
     s += getDiffCp(f, g, a + t*j) * f.evaluateGlobal(a + t*j, 1)[1].getLength();
   mat[0][i] = t * (sum += s);
   for (j=1; j<=i; j++) {
     b = 1 << (j << 1);
     mat[j][i-j] = (b*mat[j-1][i-j+1] - mat[j-1][i-j])/(b-1.0);
   }
   //if(std::abs(mat[i][0] - mat[i-1][0]) < eps) return mat[i][0];
 }
 return mat[15][0];

}

template <typename T, int n>
T GMlib::GMPutils::getIntegralDerivativeLength(PCurve<T, n>& f, T a, T b, double eps) {

 T t = b - a;
 T sum =
     (f.evaluateGlobal(a, 1)[1].getLength() +
      f.evaluateGlobal(b, 1)[1].getLength()
     ) / T(2);


 T mat[16][16];
 mat[0][0] = sum * t;

 T s;
 int i,j,k;
 for( i = 1; i < 16; i++ ) {
   s = T(0);
   k = 1 << i;
   t /= 2.0;
   for( j = 1; j < k; j += 2 )
     s += f.evaluateGlobal(a + t*j, 1)[1].getLength();
   mat[0][i] = t * (sum += s);
   for (j=1; j<=i; j++) {
     b = 1 << (j << 1);
     mat[j][i-j] = (b*mat[j-1][i-j+1] - mat[j-1][i-j])/(b-1.0);
   }
   //if(std::abs(mat[i][0] - mat[i-1][0]) < eps) return mat[i][0];
 }
 return mat[15][0];

}

template <typename T, int n>
T GMlib::GMPutils::getIntegralDiffSqr(PCurve<T, n>& f, PCurve<T, n>& g, T a, T b, double eps) {

 T t = b - a;
 T sum = (getDiffSqr(f, g, a)+getDiffSqr(f, g, b))/T(2);

 T mat[16][16];
 mat[0][0] = sum * t;

 T s;
 int i,j,k;
 for( i = 1; i < 16; i++ ) {
   s = T(0);
   k = 1 << i;
   t /= 2.0;
   for( j = 1; j < k; j += 2 )
     s += getDiffSqr(f, g, a + t*j);
   mat[0][i] = t * (sum += s);
   for (j=1; j<=i; j++) {
     b = 1 << (j << 1);
     mat[j][i-j] = (b*mat[j-1][i-j+1] - mat[j-1][i-j])/(b-1.0);
   }
   //if(std::abs(mat[i][0] - mat[i-1][0]) < eps) return mat[i][0];
 }
 return mat[15][0];

}
