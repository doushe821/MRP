#pragma once

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "IMatrix.hpp"

namespace Solvers {

double EuclidianVectorNorm(std::vector<double> Vector);

// Eigenvalue finders.
template <class MatrixT>
double PowerIteration(const Interface::IMatrix<MatrixT, double> &Matrix,
                      size_t MaxIter = 20) {
  std::vector<double> RandB;
  RandB.resize(Matrix.dim());
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
  for (size_t I = 0; I < Matrix.dim(); ++I) {
    Rho += RandB[I] * LastB[I];
  }
  auto Norm = EuclidianVectorNorm(RandB);
  Rho /= Norm;
  return Rho;
}
template <class MatrixT> double GershgorinMethod(const MatrixT &Matrix) {
  double ApproxRho = 0.0;
  size_t Dim = Matrix.dim();
  for (size_t I = 0; I < Dim; ++I) {
    double RowSum = 0.0;
    for (size_t J = 0; J < Dim; ++J) {
      RowSum += Matrix.get(I, J);
    }
    ApproxRho = std::max(RowSum, ApproxRho);
  }
  return ApproxRho;
}

// If Omega == 1, that's Gauss Seidel solver, for any other omega (0; 2] that's
// SOR.

template <class MatrixT>
std::vector<double> GaussSeidel(const Interface::IMatrix<MatrixT, double> &A,
                                const std::vector<double> &B, double Tol,
                                size_t maxIter = 1000) {
  const size_t n = A.dim();

  std::vector<double> x(n, 0.0);

  // Precompute inverse diagonal
  std::vector<double> invDiag(n);

  for (size_t i = 0; i < n; ++i) {
    std::cout << "i = " << i << std::endl;
    double d = A.get(i, i);
    if (d == 0)
      throw std::runtime_error("Zero diagonal in Gauss-Seidel");

    invDiag[i] = 1.0 / d;
  }

  for (size_t iter = 0; iter < maxIter; ++iter) {
    double maxDiff = 0.0;

    for (size_t i = 0; i < n; ++i) {
      double sigma = 0.0;

      // j < i  (new values)
      for (size_t j = 0; j < i; ++j)
        sigma += A.get(i, j) * x[j];

      // j > i  (old values)
      for (size_t j = i + 1; j < n; ++j)
        sigma += A.get(i, j) * x[j];

      double newXi = (B[i] - sigma) * invDiag[i];

      double diff = std::abs(newXi - x[i]);
      if (diff > maxDiff)
        maxDiff = diff;

      x[i] = newXi;
    }

    if (maxDiff < Tol)
      return x;
  }

  std::cout << "Gauss-Seidel did not converge\n";
  return x;
}
} // namespace Solvers
