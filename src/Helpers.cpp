#include "Solvers.hpp"
#include <numeric>

namespace Solvers {

double EuclidianVectorNorm(std::vector<double> Vector) {
  return std::inner_product(Vector.begin(), Vector.end(), Vector.begin(), 0);
}



} // namespace Solvers
