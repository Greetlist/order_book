#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <benchmark/benchmark.h>
#include <algorithm>

static constexpr int size = 128;
static constexpr int loop = 100000000;

std::random_device rd;
std::mt19937 engine{rd()};
std::uniform_int_distribution<> dist{0, size};

static std::vector<int> v(size+1, 0);
static std::map<int, int> m;

static void vector_insert(benchmark::State& state) {
  for (auto _ : state) {
    for (int i = 0; i < loop; ++i) {
      int target = dist(engine);
      v[target] = target;
    }
  }
}
BENCHMARK(vector_insert)->Unit(benchmark::kMillisecond);

static void vector_find(benchmark::State& state) {
  for (auto _ : state) {
    for (int i = 0; i < loop; ++i) {
      int target = dist(engine);
      auto it = std::find(v.begin(), v.end(), target);
      if (it == v.end()) { [[unlikely]]
        std::cout << "cannot find: " << target << std::endl;
      }
    }
  }
}
BENCHMARK(vector_find)->Unit(benchmark::kMillisecond);

static void vector_find_binary_search(benchmark::State& state) {
  for (auto _ : state) {
    for (int i = 0; i < loop; ++i) {
      int target = dist(engine);
      bool exist = std::binary_search(v.begin(), v.end(), target);
      if (!exist) { [[unlikely]]
        std::cout << "cannot find: " << target << std::endl;
      }
    }
  }
}
BENCHMARK(vector_find_binary_search)->Unit(benchmark::kMillisecond);

static void map_insert(benchmark::State& state) {
  for (auto _ : state) {
    for (int i = 0; i < loop; ++i) {
      m[dist(engine)] += 1;
    }
  }
}
BENCHMARK(map_insert)->Unit(benchmark::kMillisecond);

static void map_find(benchmark::State& state) {
  for (auto _ : state) {
    for (int i = 0; i < loop; ++i) {
      int target = dist(engine);
      m.find(target);
    }
  }
}
BENCHMARK(map_find)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

//int main(int argc, char** argv) {
//  return 0;
//}
