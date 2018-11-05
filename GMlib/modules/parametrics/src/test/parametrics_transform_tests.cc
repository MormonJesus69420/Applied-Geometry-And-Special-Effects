
#include <gtest/gtest.h>

#include <gmTestUtilsModule>
#include <gmParametricsModule>
using namespace GMlib;



namespace {



  template <typename T_Param>
  ::testing::AssertionResult transformTest( T_Param* p ) {

//    HqMatrix<float,3> mat_gold, mat_result;

    // Translate
//    Vector<float,3> v(4.0f, 3.0f, 1.0f);
//    mat_gold.translate(v);
//    p->translate(v);
//    mat_result = p->getMatrix();


    // Dummy tests (make sure stuff compiles)
    p->rotate(        Angle(90), Vector<float,3>( 1.0f, 1.0f, 0.0f ) );
    p->rotateGlobal(  Angle(90), Vector<float,3>( 1.0f, 1.0f, 0.0f ) );

    p->rotate(        Angle(90), Point<float,3>(  1.0f, 1.0f, 1.0f ), UnitVector<float,3>( 1.0, 1.0, 0.0 ) );
    p->rotateGlobal(  Angle(90), Point<float,3>(  1.0f, 1.0f, 1.0f ), UnitVector<float,3>( 1.0, 1.0, 0.0 ) );

    p->translate(       Vector<float,3>(4.0f, 3.0f, 1.0f) );
    p->translateGlobal( Vector<float,3>(4.0f, 3.0f, 1.0f) );


    return ::testing::AssertionSuccess();
  }



  TEST(Parametrics, Parametrics__Transforms__PCurves) {


    PCircle<float> *test_curve = new PCircle<float>; {
      ::testing::AssertionResult res = transformTest( test_curve );
      EXPECT_TRUE( res );
    } delete test_curve;

    //
  }

  TEST(Parametrics, Parametrics__Transforms__PSurfs ) {


    PTorus<float> *test_surf = new PTorus<float>; {
      ::testing::AssertionResult res = transformTest( test_surf );
      EXPECT_TRUE( res );
    } delete test_surf;

    //
  }

  TEST(Parametrics, Parametrics__Transforms__PTriangle ) {

    DVector< Vector<float,3> > cp(3);
    cp[0] = Vector<float,3>( 1.0f, 0.0f, 0.0f );
    cp[1] = Vector<float,3>( 0.0f, 1.0f, 0.0f );
    cp[2] = Vector<float,3>( 1.0f, 1.0f, 0.0f );

    PBezierTriangle<float> *test_triangle = new PBezierTriangle<float>(cp); {
      ::testing::AssertionResult res = transformTest( test_triangle );
      EXPECT_TRUE( res );
    } delete test_triangle;

    //
  }

}
