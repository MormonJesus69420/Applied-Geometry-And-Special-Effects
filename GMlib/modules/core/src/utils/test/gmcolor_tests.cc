
#include <gtest/gtest.h>

#include <core/utils/gmcolor.h>
using namespace GMlib;

namespace {

  TEST(Core, Utils__Color__StreamInCheck) {

    std::stringstream ss( std::stringstream::in | std::stringstream::out );
    ss << "0.5 0.3 0.789 0.9";

    Color c;
    ss >> c;

    Color c_gold( 0.5, 0.3, 0.789, 0.9 );

    EXPECT_EQ( c, c_gold );
  }

}
