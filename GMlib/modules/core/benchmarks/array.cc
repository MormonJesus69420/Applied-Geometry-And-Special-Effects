#include <benchmark/benchmark.h>

#include <containers/gmarray.h>
using namespace GMlib;

#include <vector>
#include <limits>
#include <random>
#include <algorithm>

/*!
 * \brief BM_ArrayAlwaysInsert
 * Testing 'alwaysInsert's into an array of some size
 */


static void BM_Array_insertAlways_increasing(benchmark::State& state)
{
  // Setup
  Array<int> test_array(0);

  // The test loop
  while (state.KeepRunning())
    for (int i = 0; i < state.range(0); ++i) test_array.insertAlways(i);
}
BENCHMARK(BM_Array_insertAlways_increasing)
  ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Ranges({{1, 2 << 15}});


static void BM_Array_insertAlways_random(benchmark::State& state)
{
  // Setup
  Array<int> test_array(0);

  std::default_random_engine         generator;
  std::uniform_int_distribution<int> distribution(
    std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());

  const auto       no_values = size_t(state.range(0));
  std::vector<int> values(no_values);
  for (size_t i = 0; i < no_values; ++i) values.at(i) = distribution(generator);

  // The test loop
  while (state.KeepRunning())
    for (size_t i = 0; i < no_values; ++i)
      test_array.insertAlways(values.at(i));
}
BENCHMARK(BM_Array_insertAlways_random)
  ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Ranges({{1, 2 << 15}});

static void BM_Array_sort(benchmark::State& state)
{
  // Setup
  Array<int> test_array(0);
  test_array.setSorted(false);

  std::default_random_engine         generator;
  std::uniform_int_distribution<int> distribution(
    std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());

  const auto no_values = size_t(state.range(0));
  for (size_t i = 0; i < no_values; ++i)
    test_array.insertAlways(distribution(generator));

  // The test loop
  while (state.KeepRunning()) test_array.sort();
}
BENCHMARK(BM_Array_sort)
  ->Unit(benchmark::kMillisecond)
  ->RangeMultiplier(2)
  ->Ranges({{1, 2 << 15}});


BENCHMARK_MAIN()
