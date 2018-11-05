

#include <gtest/gtest.h>

#include <core/containers/gmdvectorn.h>
using namespace GMlib;

#include <vector>

namespace {

//TEST(Core_Containers, DVectorN__Default_Constructor) {


//  const int n = 1;
//  int d_size = 100;

//  int data_size = n * d_size;

//  int k = 0;
//  std::vector<float> gold(data_size);
//  for( std::vector<float>::iterator itr = gold.begin(); itr != gold.end(); ++itr )
//    *itr = k++;


//  GMlib::DVectorN<float,n> vec(&gold[0]);

//  int vec_size = 0;
//  GMlib::Vector<int,n> vec_dim = vec.getDim();
//  for( int i = 0; i < n; ++i )
//    vec_size += vec[i];

//  EXPECT_EQ( data_size, vec_size );

//}


//TEST(Core_Containers, DVectorN__Fill_1D) {


//  const int n = 1;
//  int d_size = 100;

//  int data_size = n * d_size;

//  int k = 0;
//  std::vector<float> gold(data_size);
//  for( std::vector<float>::iterator itr = gold.begin(); itr != gold.end(); ++itr )
//    *itr = k++;


//  GMlib::DVectorN<float,n> vec(&gold[0]);

//  int vec_size = 0;
//  GMlib::Vector<int,n> vec_dim = vec.getDim();
//  for( int i = 0; i < n; ++i )
//    vec_size += vec[i];

//  EXPECT_EQ( data_size, vec_size );

//}

TEST(Core_Containers, DVectorN_Dimensions_constructor) {

  const int n = 4;
  Vector<int,n> gold_dims(1,2,3,4);

//  GMlib::DVectorN<double,n> dv( gold_dims, 1.0 );

//  Vector<int,n> dims = dv.getDim();

//  for( int i = 0; i < n; ++i )
//    EXPECT_EQ( dims[i], gold_dims[i] );
}


TEST(Core_Containers, DVectorN_setDim) {

  const int n = 4;
  Vector<int,n> gold_dims(1,2,3,4);

  DVectorN<double,n> dv;
  dv.setDim(gold_dims);

  Vector<int,n> dims = dv.getDim();

  for( int i = 0; i < n; ++i )
    EXPECT_EQ( dims[i], gold_dims[i] );
}

}

























