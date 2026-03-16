#pragma once
#include <random>

namespace Graph {

struct RNG {
  std::mt19937 Gen;

  RNG(unsigned seed = std::random_device{}()) : Gen(seed) {}

  double prob() {
    return std::uniform_real_distribution<>(0.0,1.0)(Gen);
  }

  size_t index(size_t max) {
    return std::uniform_int_distribution<size_t>(0,max-1)(Gen);
  }
};

} // namespace Graph
