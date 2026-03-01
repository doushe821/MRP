#include "GaussSeidel.hpp"

namespace Solvers {

std::vector<double> gaussSeidel(const SparseMatrix::SparseMatrixCSR &Matrix,
                                const std::vector<double> &b, size_t MaxIters,
                                double Tol) {
  size_t Dim = Matrix.Dim;
  std::vector<double> Ans(Dim, 0.0);

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
        }
        else {
          Sum += Val * Ans[j];
        }
      }

      double NewXi = (b[i] - Sum) / Diag;

      MaxDiff = std::max(MaxDiff, std::abs(NewXi - Ans[i]));

      Ans[i] = NewXi;
    }

    if (MaxDiff < Tol) {
      return Ans;
    }
  }

  std::cout << "Gauss-Seidel did not converge\n";
  return Ans;
}

} // namespace Solvers
