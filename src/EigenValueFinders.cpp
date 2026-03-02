#include "Solvers.hpp"
#include "SparseMatrix.hpp"

namespace Solvers {

double PowerIteration(const SparseMatrix::SparseMatrixCSR &Matrix,
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

} // namespace Solvers
