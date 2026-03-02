#include "Solvers.hpp"
#include "SparseMatrix.hpp"
#include <numeric>

namespace Solvers {
// TODO implement other algos, do tests, compare.
double PowerIteration(const SparseMatrix::SparseMatrixCSR<double> &Matrix,
                      size_t MaxIter) {
  std::vector<double> RandB;
  RandB.resize(Matrix.Dim);
  std::fill(RandB.begin(), RandB.end(), 1); // TODO acceleration
  for (size_t I = 0; I < MaxIter; ++I) {
    RandB = Matrix * RandB;
    auto Norm = EuclidianVectorNorm(RandB);
    std::for_each(RandB.begin(), RandB.end(),
                  [Norm](auto &Val) { Val /= Norm; });
  }
  // TODO clean up
  // Rayleigh quotient
  double Rho{0.0};
  auto LastB = Matrix * RandB;
  for (size_t I = 0; I < Matrix.Dim; ++I) {
    Rho += RandB[I] * LastB[I];
  }
  auto Norm = EuclidianVectorNorm(RandB);
  Rho /= Norm;
  return Rho;
}

double GershgorinUpperBoundMethod(const SparseMatrix::SparseMatrixCSR<double> &Matrix) {
  double ApproxRho = 0.0;
  for (size_t I = 0; I < Matrix.Dim; ++I) {
    double RowSum = 0.0;
    for (size_t J = Matrix.RowPtr[I]; J < Matrix.RowPtr[I + 1]; ++J) {
      RowSum += Matrix.Values[J];
    }
    ApproxRho = std::max(RowSum, ApproxRho);
  }
  return ApproxRho;
}

} // namespace Solvers
