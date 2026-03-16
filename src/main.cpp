#include <CLI/App.hpp>
#include <CLI/CLI.hpp>
#include <CLI/Config.hpp>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Sparse>

#include "Eigen/SparseCore"
#include "Solvers.hpp"
#include "SparseMatrix.hpp"

int main(int argc, char **argv) {
  CLI::App CLIApp;
  double Omega;
  // Omega = 1 is Gauss-Seidel solver
  CLIApp.add_option("--omega", Omega, "Omega coefficient for SOR solver")
      ->default_val(1);
  CLI11_PARSE(CLIApp, argc, argv);

  std::vector<double> T{10, 0, 0, 0, 20, 0, 0, 0, 30};
  Interface::IMatrix<SparseMatrix::SparseMatrixCSR<double>, double> IMatr{T, 3};
  for (unsigned I = 0; I < IMatr.dim(); ++I) {
    for (unsigned J = 0; J < IMatr.dim(); ++J) {
      std::cout << IMatr.get(I, J) << std::endl;
    }
  }
  std::vector<double> B;
  B.resize(IMatr.dim());
  B.assign(B.size(), 1);
  auto Ans = Solvers::GaussSeidel<SparseMatrix::SparseMatrixCSR<double>>(
      IMatr, B, 0.1, 1000);

  std::cerr << "\nAnswers:\n";
  for (auto &Val : Ans) {
    std::cerr << Val << '\n';
  }
  std::cerr << std::endl;
  std::cerr << std::endl;

  Eigen::SparseMatrix<double> A{3, 3};
  std::vector<Eigen::Triplet<double>> VT;
  VT.push_back({0, 0, 10});
  VT.push_back({1, 1, 20});
  VT.push_back({2, 2, 30});
  std::cerr << "hoot\n";
  A.setFromTriplets(VT.begin(), VT.end());
  Eigen::ConjugateGradient<Eigen::SparseMatrix<double>,
                           Eigen::Lower | Eigen::Upper,
                           Eigen::DiagonalPreconditioner<double>>
      CGSolver;

  CGSolver.setTolerance(0.1);
  CGSolver.setMaxIterations(1000);
  Eigen::VectorXd EigenB;
  EigenB.resize(3);
  EigenB[0] = 1;
  EigenB[1] = 1;
  EigenB[2] = 1;
    std::cerr << "hoot\n";

  CGSolver.compute(A);
  std::cerr << "hoot\n";

  auto X = CGSolver.solve(EigenB);

  for (unsigned I = 0; I < IMatr.dim(); ++I) {
    std::cout << "x_" << I << " = " << X[I] << '\n';
  }
  return 0;
}
