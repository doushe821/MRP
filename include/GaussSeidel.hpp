#pragma once

#include <cmath>
#include <vector>
#include <iostream>

#include "SparseMatrix.hpp"

namespace Solvers { 

  std::vector<double>
  gaussSeidel(const SparseMatrix::SparseMatrixCSR& A,
    const std::vector<double>& b,
    size_t maxIters = 10000,
    double tol = 1e-10);

} // namespace Solvers
