
#include <gtest/gtest.h>

#include <gmTestUtilsModule>
#include <core/types/gmmatrix.h>
using namespace GMlib;

namespace {




  TEST(Core, Types__Matrix__ConversionToFloat) {

    const int n = 3, m = 3;
    Matrix<float,n,m> mat_gold, fmat;
    Matrix<int,n,m> imat;
    Matrix<double,n,m> dmat;

    int rvalue;
    srand (time(NULL));
    for( int i = 0; i < n; ++ i ) {
      for( int j = 0; j < m; ++j ) {

        rvalue = rand() % 100 + 1;

        mat_gold[i][j]  = float(rvalue);
        imat[i][j]      = rvalue;
        dmat[i][j]      = double(rvalue);
      }
    }

    fmat = imat;
    ::testing::AssertionResult ires =
        GMlib::GMtest::assertArrays<float,n*m>( mat_gold.getPtr(),
                                                fmat.getPtr());

    fmat = dmat;
    ::testing::AssertionResult dres =
        GMlib::GMtest::assertArrays<float,n*m>( mat_gold.getPtr(),
                                                fmat.getPtr());

    EXPECT_TRUE( ires );
    EXPECT_TRUE( dres );
  }

  TEST(Core, Types__SqMatrix__ConversionToFloat) {

    const int n = 3;
    SqMatrix<float,n> mat_gold, fmat;
    SqMatrix<int,n> imat;
    SqMatrix<double,n> dmat;

    int rvalue;
    srand (time(NULL));
    for( int i = 0; i < n; ++ i ) {
      for( int j = 0; j < n; ++j ) {

        rvalue = rand() % 100 + 1;

        mat_gold[i][j]  = float(rvalue);
        imat[i][j]      = rvalue;
        dmat[i][j]      = double(rvalue);
      }
    }

    fmat = imat;
    ::testing::AssertionResult ires =
        GMlib::GMtest::assertArrays<float,n*n>( mat_gold.getPtr(),
                                                fmat.getPtr());

    fmat = dmat;
    ::testing::AssertionResult dres =
        GMlib::GMtest::assertArrays<float,n*n>( mat_gold.getPtr(),
                                                fmat.getPtr());

    EXPECT_TRUE( ires );
    EXPECT_TRUE( dres );
  }

//  TEST(Core, Types__HqMatrix__ConversionToFloat) {

//    const int n = 3;
//    HqMatrix<float,n> mat_gold, fmat;
//    HqMatrix<int,n> imat;
//    HqMatrix<double,n> dmat;

//    int rvalue;
//    srand (time(NULL));
//    for( int i = 0; i < n; ++ i ) {
//      for( int j = 0; j < n; ++j ) {

//        rvalue = rand() % 100 + 1;

//        mat_gold[i][j]  = float(rvalue);
//        imat[i][j]      = rvalue;
//        dmat[i][j]      = double(rvalue);
//      }
//    }

//    fmat = imat;
//    ::testing::AssertionResult ires =
//        GMlib::GMtest::assertArrays<float,n*n>( mat_gold.getPtr(),
//                                                fmat.getPtr());

//    fmat = dmat;
//    ::testing::AssertionResult dres =
//        GMlib::GMtest::assertArrays<float,n*n>( mat_gold.getPtr(),
//                                                fmat.getPtr());

//    EXPECT_TRUE( ires );
//    EXPECT_TRUE( dres );
//  }

}
