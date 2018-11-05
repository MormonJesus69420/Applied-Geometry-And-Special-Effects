
#include <gtest/gtest.h>

#include <gmTestUtilsModule>
#include <core/types/gmmatrix.h>
using namespace GMlib;

namespace {





  TEST(Core, Types__Matrix__Arithmetics__pluss) {

    Matrix<float,2,2> A, B;

    A[0][0] =  1; A[0][1] =  1;
    A[1][0] = -2; A[1][1] =  0;

    B[0][0] =  3; B[0][1] =  0;
    B[1][0] = -2; B[1][1] = -0;

    EXPECT_EQ(  4, (A+B)(0)(0) );  EXPECT_EQ( 1, (A+B)(0)(1) );
    EXPECT_EQ( -4, (A+B)(1)(0) );  EXPECT_EQ( 0, (A+B)(1)(1) );
  }

  TEST(Core, Types__Matrix__Arithmetics__pluss_eq) {

    Matrix<float,2,2> A, B;

    A[0][0] =  1; A[0][1] =  1;
    A[1][0] = -2; A[1][1] =  0;

    B[0][0] =  3; B[0][1] =  0;
    B[1][0] = -2; B[1][1] = -0;

    A += B;

    EXPECT_EQ(  4, A(0)(0) );  EXPECT_EQ( 1, A(0)(1) );
    EXPECT_EQ( -4, A(1)(0) );  EXPECT_EQ( 0, A(1)(1) );
  }

  TEST(Core, Types__Matrix__Arithmetics__minus) {

    Matrix<float,2,2> A, B;

    A[0][0] =  1; A[0][1] =  1;
    A[1][0] = -2; A[1][1] =  0;

    B[0][0] =  3; B[0][1] =  0;
    B[1][0] = -2; B[1][1] = -0;

    EXPECT_EQ( -2, (A-B)(0)(0) );  EXPECT_EQ( 1, (A-B)(0)(1) );
    EXPECT_EQ(  0, (A-B)(1)(0) );  EXPECT_EQ( 0, (A-B)(1)(1) );
  }

  TEST(Core, Types__Matrix__Arithmetics__minus_eq) {

    Matrix<float,2,2> A, B;

    A[0][0] =  1; A[0][1] =  1;
    A[1][0] = -2; A[1][1] =  0;

    B[0][0] =  3; B[0][1] =  0;
    B[1][0] = -2; B[1][1] = -0;

    A -= B;

    EXPECT_EQ( -2, A(0)(0) );  EXPECT_EQ( 1, A(0)(1) );
    EXPECT_EQ(  0, A(1)(0) );  EXPECT_EQ( 0, A(1)(1) );
  }

  TEST(Core, Types__Matrix__Arithmetics__mult_by_double) {

    Matrix<float,2,2> A;

    A[0][0] =  1; A[0][1] =  4;
    A[1][0] = -2; A[1][1] =  0;

    double d = 2.0;

    EXPECT_EQ(  2.0, (A*d)(0)(0) );  EXPECT_EQ( 8.0, (A*d)(0)(1) );
    EXPECT_EQ( -4.0, (A*d)(1)(0) );  EXPECT_EQ( 0.0, (A*d)(1)(1) );
  }

  TEST(Core, Types__Matrix__Arithmetics__mult_eq_by_double) {

    Matrix<float,2,2> A;

    A[0][0] =  1; A[0][1] =  4;
    A[1][0] = -2; A[1][1] =  0;

    double d = 2.0;

    A *= d;

    EXPECT_EQ(  2.0, A(0)(0) );  EXPECT_EQ( 8.0, A(0)(1) );
    EXPECT_EQ( -4.0, A(1)(0) );  EXPECT_EQ( 0.0, A(1)(1) );
  }

  TEST(Core, Types__Matrix__Arithmetics__division_by_double) {

    Matrix<float,2,2> A;

    A[0][0] =  1; A[0][1] =  4;
    A[1][0] = -2; A[1][1] =  0;

    double d = 2.0;

    EXPECT_EQ(  0.5, (A/d)(0)(0) );  EXPECT_EQ( 2.0, (A/d)(0)(1) );
    EXPECT_EQ( -1.0, (A/d)(1)(0) );  EXPECT_EQ( 0.0, (A/d)(1)(1) );
  }

  TEST(Core, Types__Matrix__Arithmetics__division_eq_by_double) {

    Matrix<float,2,2> A;

    A[0][0] =  1; A[0][1] =  4;
    A[1][0] = -2; A[1][1] =  0;

    double d = 2.0;

    A /= d;

    EXPECT_EQ(  0.5, A(0)(0) );  EXPECT_EQ( 2.0, A(0)(1) );
    EXPECT_EQ( -1.0, A(1)(0) );  EXPECT_EQ( 0.0, A(1)(1) );
  }

  TEST(Core, Types__Matrix__Arithmetics__minus_unary) {

    Matrix<float,2,2> A;

    A[0][0] =  1; A[0][1] =  1;
    A[1][0] = -2; A[1][1] =  0;

    EXPECT_EQ( -1, (-A)(0)(0) );  EXPECT_EQ( -1, (-A)(0)(1) );
    EXPECT_EQ(  2, (-A)(1)(0) );  EXPECT_EQ(  0, (-A)(1)(1) );
  }

//  TEST(Core, Types__Matrix__Arithmetics__scale) {}
//  TEST(Core, Types__Matrix__Arithmetics__scale_eq) {}
//  TEST(Core, Types__Matrix__Arithmetics__scale_inv) {}
//  TEST(Core, Types__Matrix__Arithmetics__scale_inv_eq) {}
}
