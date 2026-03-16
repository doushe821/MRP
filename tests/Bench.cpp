#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Sparse>
#include <utility>

#include "CFGGen.hpp"
#include "IMatrix.hpp"
#include "Solvers.hpp"
#include "SparseMatrix.hpp"

#define CATCH_CONFIG_MAIN

#include <Eigen/Core>

constexpr size_t kNSmall = 100;
constexpr size_t kNMedium = 100;
constexpr size_t kNBig = 100;

constexpr size_t kSizeSmall = 20;
constexpr size_t kSizeMedium = 100;
constexpr size_t kSizeBig = 1000;

TEST_CASE("Eigen vs Primitive solver", "[!benchmark]") {

  // Matrix generation
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
    SmallMatrices.push_back(std::make_pair(RandomCFG.buildSystemMatrix(),
                                           RandomCFG.buildRewardVector()));
  }
  //for (size_t I = 0; I < kNMedium; ++I) {
  //  auto RandomCFG =
  //      Graph::generateRandomCFG<Graph::Node,
  //                               SparseMatrix::SparseMatrixCSR<double>>(
  //          kSizeMedium, kSizeMedium - 1);
  //  MediumMatrices.push_back(std::make_pair(RandomCFG.buildSystemMatrix(),
  //                                          RandomCFG.buildRewardVector()));
  //}
  //for (size_t I = 0; I < kNBig; ++I) {
  //  auto RandomCFG =
  //      Graph::generateRandomCFG<Graph::Node,
  //                               SparseMatrix::SparseMatrixCSR<double>>(
  //          kSizeBig, kSizeBig - 1);
  //  BigMatrices.push_back(std::make_pair(RandomCFG.buildSystemMatrix(),
  //                                       RandomCFG.buildRewardVector()));
  //}
  double Sum {0};
  //  for (auto &System : SmallMatrices) {
  //    auto X = Solvers::GaussSeidel(System.first, System.second, 0.1);
  //    Sum += X[0];
  //  }
  //  return Sum;
  BENCHMARK("Small matrices") {

  };

}