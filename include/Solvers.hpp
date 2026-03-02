#pragma once

#include <algorithm>
#include <cstdint>

#include "SparseMatrix.hpp"

namespace Solvers {
std::vector<double> gaussSeidel(const SparseMatrix::SparseMatrixCSR<double> &A,
                                const std::vector<double> &b,
                                size_t maxIters = 10000, double tol = 1e-10);

double EuclidianVectorNorm(std::vector<double> Vector);

double PowerIteration(const SparseMatrix::SparseMatrixCSR<double> &Matrix,
                      size_t MaxIter = 20);

double GershgorinMethod(const SparseMatrix::SparseMatrixCSR<double> &Matrix);

} // namespace Solvers
