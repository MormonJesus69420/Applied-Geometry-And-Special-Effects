

#include <gtest/gtest.h>

#include <static/gmstaticproc.h>
using namespace GMlib;

#include <array>

namespace {


TEST(Core_Static, StaticProc_GM_Static__CompileTorture) {

  using T = float;
  std::array<T,3> a { {0.0f,0.0f,0.0f} }, b { {0.0f,0.0f,0.0f} };
  T c {0.0f};
  double d {0.0};
  int e {0};

  GM_Static_<T,3>::dpr(a.data(),b.data());
  GM_Static_<T,3>::dprm(a.data(),b.data());

  GM_Static_<T,3>::eq(a.data(),c);
  GM_Static_<T,3>::eqm(a.data(),b.data());

  GM_Static_<T,3>::sc(a.data(),d);
  GM_Static_<T,3>::sc_r(a.data(),b.data(),d);

  GM_Static_<T,3>::peq(a.data(),b.data());
  GM_Static_<T,3>::meq(a.data(),b.data());
  GM_Static_<T,3>::xeq(a.data(),b.data());

  GM_Static_<T,3>::rot_xy(a.data(),b.data(), c, c);

  GM_Static_<T,3>::decomp(a.data(),b.data(), c);
  GM_Static_<T,3>::normalizeHq(a.data(),b.data(), c);

  GM_Static_<T,3>::maxIndex(a.data());
  GM_Static_<T,3>::maxIdx(a.data(),e);
}

TEST(Core_Static, StaticProc_GM_Static1__CompileTorture) {

  using T = float;
  using G = float;
  std::array<T,3> a { {0.0f,0.0f,0.0f} };
  std::array<G,3> b { {0.0f,0.0f,0.0f} };

  GM_Static1_<T,G,3>::eq(a.data(),b.data());
}


TEST(Core_Static, StaticProc_GM_Static2__CompileTorture) {

  using T = float;
  std::array<std::array<T, 3>, 3> a{
    {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}}};

  std::array<std::array<T, 3>, 3> b{
    {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}}};

  T c {0.0f};
  double d {0.0};


  GM_Static2_<T,3,3>::dpr(a.data()->data(),b.data()->data());
  GM_Static2_<T,3,3>::dpc(a.data()->data(),b.data()->data());

  GM_Static2_<T,3,3>::eq1(a.data()->data(),b.data()->data());
  GM_Static2_<T,3,3>::eq2(a.data()->data(),b.data()->data());
  GM_Static2_<T,3,3>::eq3(a.data()->data(),b.data()->data());

  GM_Static2_<T,3,3>::sc(a.data()->data(),d);
  GM_Static2_<T,3,3>::decomp(a.data()->data(),b.data()->data());
  GM_Static2_<T,3,3>::decomp1(a.data()->data(),b.data()->data(),c);
}

TEST(Core_Static, StaticProc_GM_Static3__CompileTorture) {

  using T = float;

  std::array<std::array<T, 3>, 3> a{
    {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}}};

  std::array<std::array<T, 3>, 3> b{
    {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}}};

  std::array<std::array<T, 3>, 3> c{
    {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}}};

  T d {0.0f};
  double e {0.0};

  GM_Static3_<T,3,3>::dpr(a.data()->data(),b.data()->data());
  GM_Static3_<T,3,3>::eq1(a.data()->data(),b.data()->data());
  GM_Static3_<T,3,3>::eq2(a.data()->data(),b.data()->data());
  GM_Static3_<T,3,3>::eq3(a.data()->data(),b.data()->data());

  GM_Static3_<T,3,3>::sc(a.data()->data(),e);

  GM_Static3_<T,3,3>::swp(a.data()->data(),b.data()->data());
  GM_Static3_<T,3,3>::trn(a.data()->data(),b.data()->data());

  GM_Static3_<T,3,3>::cm_x(a.data()->data(),b.data()->data(),c.data()->data());

  GM_Static3_<T,3,3>::decomp(a.data()->data(),b.data()->data());
  GM_Static3_<T,3,3>::decomp1(a.data()->data(),b.data()->data(),d);
}

TEST(Core_Static, StaticProc_GM_Std__CompileTorture) {

  using T = float;

  std::array<std::array<T, 3>, 3> a{
    {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}}};

  std::array<std::array<T, 3>, 3> b{
    {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}}};

  std::array<std::array<T, 3>, 3> c{
    {{{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}}};

  T d {0.0f};

  GM_Std_<T,3>::b_min_d_c(a.data()->data(),b.data()->data(),c.data()->data(),d);
  GM_Std_<T,3>::orto(a.data()->data(),b.data()->data(),c.data()->data());
  GM_Std_<T,3>::ortoNormal(a.data()->data(),b.data()->data(),c.data()->data());
}


}

























