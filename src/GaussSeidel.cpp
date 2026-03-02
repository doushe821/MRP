#include "Solvers.hpp"
#include <cmath>

namespace Solvers {


// TODO make templated with spectral radius finder
template <typename SpectralRadiusFinderFn>
std::vector<double> GaussSeidelSOR(const SparseMatrix::SparseMatrixCSR<double> &Matrix,
                                   const std::vector<double> &B,
                                   size_t MaxIters, double Tol,
                                   double Omega = 1,
                                   double Threshold = SIZE_MAX) {
  size_t Dim = Matrix.Dim;
  std::vector<double> Ans(Dim, 0.0);

  double Rho = SpectralRadiusFinderFn(Matrix);

  // TODO might also compare with Omega = 1.6, without additional computations
  Omega = 2 / (1 + std::sqrt(1 - Rho * Rho));

  for (size_t Iter = 0; Iter < MaxIters; ++Iter) {

    double MaxDiff = 0.0;

    for (size_t i = 0; i < Dim; ++i) {

      double Diag = 0.0;
      double Sum = 0.0;

      for (size_t k = Matrix.RowPtr[i]; k < Matrix.RowPtr[i + 1]; ++k) {
        size_t j = Matrix.ColIdx[k];
        double Val = Matrix.Values[k];

        if (j == i) {
          Diag = Val;
        } else {
          Sum += Val * Ans[j];
        }
      }

      if (Diag == 0) {
        std::cout << "Insufficient matrix: there is zero element on diagonal\n";
      }

      double NewXi = (B[i] - Sum) / Diag;
      double Relaxed = (1.0 - Omega) * Ans[i] + Omega * NewXi;

      MaxDiff = std::max(MaxDiff, std::abs(Relaxed - Ans[i]));
      Ans[i] = Relaxed;

      if (Ans[i] >= Threshold) {
        break;
      }
    }

    if (MaxDiff < Tol) {
      return Ans;
    }
  }

  std::cout << "Gauss-Seidel did not converge\n";
  return Ans;
}

} // namespace Solvers
