#include <cmath>
#include <iostream>

#include "IMatrix.hpp"
#include "Solvers.hpp"

namespace Solvers {

// template further
/*template <class MatrixT>
std::vector<double>
GaussSeidelSOR(const Interface::IMatrix<MatrixT, double> &Matrix,
               const std::vector<double> &B, double Tol, double Omega,
               double Threshold, size_t MaxIters) {
  size_t Dim = Matrix.dim();
  std::vector<double> Ans(Dim, 0.0);

  double Rho = Solvers::PowerIteration(Matrix);

  // TODO might also compare with Omega = 1.6, without additional computations
  Omega = 2 / (1 + std::sqrt(1 - Rho * Rho));

  for (size_t Iter = 0; Iter < MaxIters; ++Iter) {

    double MaxDiff = 0.0;

    for (size_t I = 0; I < Dim; ++I) {

      double Diag = 0.0;
      double Sum = 0.0;

      for (size_t J = 0; J < Dim; ++J) {
        auto Val = Matrix.get(I, J);
        if (I == J) {
          Diag = Val;
        } else {
          Sum += Val * Ans[J];
        }
      }

      if (Diag == 0) {
        std::cout
            << "Insufficient matrix: there is a zero element on diagonal\n";
      }

      double NewXi = (B[I] - Sum) / Diag;
      double Relaxed = (1.0 - Omega) * Ans[I] + Omega * NewXi;

      MaxDiff = std::max(MaxDiff, std::abs(Relaxed - Ans[I]));
      Ans[I] = Relaxed;

      if (Ans[I] >= Threshold) {
        break;
      }
    }

    if (MaxDiff < Tol) {
      return Ans;
    }
  }

  std::cout << "Gauss-Seidel did not converge\n";
  return Ans;
}*/

} // namespace Solvers
