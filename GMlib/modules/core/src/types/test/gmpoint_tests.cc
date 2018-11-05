
#include <gtest/gtest.h>

#include <gmTestUtilsModule>
#include <core/types/gmpoint.h>
using namespace GMlib;

namespace {




  TEST(Core, Types__APoint__ConversionToFloat) {

    const int n = 3;

    APoint<float,n> point_gold, fp;
    APoint<int,n> ip;
    APoint<double,n> dp;

    int rvalue;
    srand (time(NULL));
    for( int i = 0; i < n; ++ i ) {

      rvalue = rand() % 100 + 1;

      point_gold[i] = float(rvalue);
      ip[i]         = rvalue;
      dp[i]         = double(rvalue);
    }

    fp= ip;
    ::testing::AssertionResult res =
        GMlib::GMtest::assertArrays<float,n>( point_gold.getPtr(),
                                              fp.getPtr());

    EXPECT_TRUE( res );


  }

  TEST(Core, Types__Point__ConversionToFloat) {

    const int n = 3;

    Point<float,n> point_gold, fp;
    Point<int,n> ip;
    Point<double,n> dp;

    int rvalue;
    srand (time(NULL));
    for( int i = 0; i < n; ++ i ) {

      rvalue = rand() % 100 + 1;

      point_gold[i] = float(rvalue);
      ip[i]         = rvalue;
      dp[i]         = double(rvalue);
    }

    fp= ip.toType<float>();
    ::testing::AssertionResult res =
        GMlib::GMtest::assertArrays<float,n>( point_gold.getPtr(),
                                              fp.getPtr());

    EXPECT_TRUE( res );


  }

  TEST(Core, Types__Vector__ConversionToFloat) {

    const int n = 3;

    Vector<float,n> point_gold, fp;
    Vector<int,n> ip;
    Vector<double,n> dp;

    int rvalue;
    srand (time(NULL));
    for( int i = 0; i < n; ++ i ) {

      rvalue = rand() % 100 + 1;

      point_gold[i] = float(rvalue);
      ip[i]         = rvalue;
      dp[i]         = double(rvalue);
    }

    fp= ip.toType<float>();
    ::testing::AssertionResult res =
        GMlib::GMtest::assertArrays<float,n>( point_gold.getPtr(),
                                              fp.getPtr());

    EXPECT_TRUE( res );


  }

}
