
#include <gtest/gtest.h>

#include <core/containers/gmarray.h>
using namespace GMlib;

namespace {

  TEST(Core, Containers__Array__SizeCheck) {


    Array<int> array;
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    array.push_back(4);
    array.push_back(5);
    array.push_back(6);

    EXPECT_EQ( 6, array.size() );
  }

  TEST(Core, Containers__Array__SizeCheck2) {

    Array<int> array;
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    array.push_back(4);
    array.push_back(5);
    array.push_back(6);

    EXPECT_NE( 3, array.getSize() );
  }

  TEST(Core, Containers__Array__SizeCheck3) {

    Array<int> array;
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);

    EXPECT_EQ( 3, array.getSize() );
  }

  TEST(Core, Containers__Array__Sort) {

    Array<int> array;
    array.push_back(45);
    array.push_back(5);
    array.push_back(4);
    array.push_back(60);
    array.push_back(19);
    array.push_back(500);
    array.push_back(5);
    array.push_back(3);
    array.push_back(-4);
    array.push_back(0);
    array.sort();

    EXPECT_EQ( array[0],  -4 );
    EXPECT_EQ( array[1],   0 );
    EXPECT_EQ( array[2],   3 );
    EXPECT_EQ( array[3],   4 );
    EXPECT_EQ( array[4],   5 );
    EXPECT_EQ( array[5],   5 );
    EXPECT_EQ( array[6],  19 );
    EXPECT_EQ( array[7],  45 );
    EXPECT_EQ( array[8],  60 );
    EXPECT_EQ( array[9], 500 );
  }

}
