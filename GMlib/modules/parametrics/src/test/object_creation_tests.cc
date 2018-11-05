
#include <gtest/gtest.h>

#include <gmTestUtilsModule>
#include <gmParametricsModule>
using namespace GMlib;


namespace {


  template <typename T>
  void createAndDestroyBasicCurves() {

//    GMtest::createDestroyObject< PArc<T> >();
    GMtest::createDestroyObject< PBasisCurve<T> >();
    GMtest::createDestroyObject< PButterfly<T> >();
    GMtest::createDestroyObject< PChrysanthemumCurve<T> >();
    GMtest::createDestroyObject< PCircle<T> >();
    GMtest::createDestroyObject< PHelicoid<T> >();
    GMtest::createDestroyObject< PRoseCurve<T> >();
  }


  template <typename T>
  void createAndDestroyBasicSurfaces() {

    GMtest::createDestroyObject< PApple<T> >();
    GMtest::createDestroyObject< PApple2<T> >();
    GMtest::createDestroyObject< PAsteroidalSphere<T> >();
    GMtest::createDestroyObject< PBentHorns<T> >();
    GMtest::createDestroyObject< PBohemianDome<T> >();
    GMtest::createDestroyObject< PBoysSurface<T> >();
    GMtest::createDestroyObject< PCircularSurface<T> >();
    GMtest::createDestroyObject< PCone<T> >();
    GMtest::createDestroyObject< PCrossCap<T> >();
    GMtest::createDestroyObject< PCylinder<T> >();
    GMtest::createDestroyObject< PDiniSurface<T> >();
    GMtest::createDestroyObject< PEightSurface<T> >();
    GMtest::createDestroyObject< PEnnepersSurface<T> >();
    GMtest::createDestroyObject< PHeart<T> >();
    GMtest::createDestroyObject< PInsideOutTorus<T> >();
    GMtest::createDestroyObject< PKleinsBottle<T> >();
    GMtest::createDestroyObject< PMoebiusStrip<T> >();
    GMtest::createDestroyObject< PSeashell<T> >();
    GMtest::createDestroyObject< PSlippersSurface<T> >();
    GMtest::createDestroyObject< PSphere<T> >();
    GMtest::createDestroyObject< PSteinerSurf<T> >();
    GMtest::createDestroyObject< PSwallowTail<T> >();
    GMtest::createDestroyObject< PTorus<T> >();
    GMtest::createDestroyObject< PTrianguloidTrefoil<T> >();
    GMtest::createDestroyObject< PWhitneyUmbrella<T> >();
  }

  template <typename T>
  void createAndDestroyTriangles() {

    DVector< Vector<T,3> > cp(3);
    cp[0] = Vector<T,3>( 1.0f, 0.0f, 0.0f );
    cp[1] = Vector<T,3>( 0.0f, 1.0f, 0.0f );
    cp[2] = Vector<T,3>( 1.0f, 1.0f, 0.0f );

    PBezierTriangle<T> *test_triangle_float = new PBezierTriangle<T>(cp);
    delete test_triangle_float;
  }


  TEST(Parametrics, ObjectCreation__PCurves) {

    createAndDestroyBasicCurves<float>();
    createAndDestroyBasicCurves<double>();
    createAndDestroyBasicCurves<int>();
  }

  TEST(Parametrics, ObjectCreation__PSurfs) {

    createAndDestroyBasicSurfaces<float>();
    createAndDestroyBasicSurfaces<double>();
    createAndDestroyBasicSurfaces<int>();
  }

  TEST(Parametrics, ObjectCreation__PTriangle) {

    createAndDestroyTriangles<float>();
    createAndDestroyTriangles<double>();
    createAndDestroyTriangles<int>();
  }

}
