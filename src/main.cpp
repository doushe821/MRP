#include <CLI/App.hpp>
#include <CLI/CLI.hpp>
#include <CLI/Config.hpp>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Sparse>

#include "Eigen/SparseCore"
#include "Graph.hpp"
#include "CFGGen.hpp"
#include "Solvers.hpp"
#include "SparseMatrix.hpp"


constexpr size_t kNSmall = 100;
constexpr size_t kNMedium = 100;
constexpr size_t kNBig = 100;

constexpr size_t kSizeSmall = 20;
constexpr size_t kSizeMedium = 100;
constexpr size_t kSizeBig = 1000;


int main(int argc, char **argv) {
  CLI::App CLIApp;
  double Omega;
  // Omega = 1 is Gauss-Seidel solver
  CLIApp.add_option("--omega", Omega, "Omega coefficient for SOR solver")
      ->default_val(1);
  CLI11_PARSE(CLIApp, argc, argv);

  std::vector<std::pair<
      Interface::IMatrix<SparseMatrix::SparseMatrixCSR<double>, double>,
      std::vector<double>>>
      SmallMatrices;
  std::vector<std::pair<
      Interface::IMatrix<SparseMatrix::SparseMatrixCSR<double>, double>,
      std::vector<double>>>
      MediumMatrices;
  std::vector<std::pair<
      Interface::IMatrix<SparseMatrix::SparseMatrixCSR<double>, double>,
      std::vector<double>>>
      BigMatrices;
  for (size_t I = 0; I < kNSmall; ++I) {
    auto RandomCFG =
        Graph::generateRandomCFG<Graph::Node,
                                 SparseMatrix::SparseMatrixCSR<double>>(
            kSizeSmall, kSizeSmall - 1);
    SmallMatrices.push_back(std::make_pair(std::move(RandomCFG.buildSystemMatrix()),
                                           std::move(RandomCFG.buildRewardVector()));
  }
  double Sum{0};
  std::cout << SmallMatrices[1].first.get(0, 0) << std::endl;
  for (auto &System : SmallMatrices) {
    std::cout << System.first.dim() << std::endl;
    std::cout << System.second.size() << std::endl;
    std::cout << System.first.getNonZeroValues().size() << std::endl;

    auto X = Solvers::GaussSeidel(System.first, System.second, 0.1);
    Sum += X[0];
  }
  std::cout << Sum << std::endl;

  //  Eigen::SparseMatrix<double> A{3, 3};
  //  std::vector<Eigen::Triplet<double>> VT;
  //  VT.push_back({0, 0, 10});
  //  VT.push_back({1, 1, 20});
  //  VT.push_back({2, 2, 30});
  //  std::cerr << "hoot\n";
  //  A.setFromTriplets(VT.begin(), VT.end());
  //  Eigen::ConjugateGradient<Eigen::SparseMatrix<double>,
  //                           Eigen::Lower | Eigen::Upper,
  //                           Eigen::DiagonalPreconditioner<double>>
  //      CGSolver;
  //
  //  CGSolver.setTolerance(0.1);
  //  CGSolver.setMaxIterations(1000);
  //  Eigen::VectorXd EigenB;
  //  EigenB.resize(3);
  //  EigenB[0] = 1;
  //  EigenB[1] = 1;
  //  EigenB[2] = 1;
  //    std::cerr << "hoot\n";
  //
  //  CGSolver.compute(A);
  //  std::cerr << "hoot\n";
  //
  //  auto X = CGSolver.solve(EigenB);
  //
  //  for (unsigned I = 0; I < X.size(); ++I) {
  //    std::cout << "x_" << I << " = " << X[I] << '\n';
  //  }
  return 0;
}
