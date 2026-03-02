#pragma once

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace SparseMatrix {

// sparse vector needed.
template<typename ValT>
struct SparseMatrixCSR {
  // TODO think
  SparseMatrixCSR<ValT>() = default;
  SparseMatrixCSR<ValT>(std::vector<ValT> InitValues) {
    Dim = InitValues.size();
    RowPtr.resize(Dim + 1);
    Values.assign(InitValues);
    for (size_t I = 0; I < InitValues.size(); ++I) {
      RowPtr[I] = Values.size();

      ColIdx.push_back(I);
    }
  };

  std::vector<size_t> RowPtr;
  std::vector<size_t> ColIdx;
  std::vector<ValT> Values;
  size_t Dim = 0;

  double get(size_t Row, size_t Col) const {
    for (size_t k = RowPtr[Row]; k < RowPtr[Row + 1]; ++k) {
      if (ColIdx[k] == Col) {
        return Values[k];
      }
    }
    return 0.0;
  }

  std::vector<ValT> operator*(const std::vector<ValT> Vec) const {
    if (Vec.size() != Dim) {
      throw(std::runtime_error(
          "SparseMatrix: Vector and matrix dimensions mismatch"));
    }
    std::vector<ValT> Res;
    Res.resize(Dim);
    for (size_t I = 0; I < Dim; ++I) {
      for (size_t K = RowPtr[I]; K < RowPtr[I+1]; ++K) {
        Res[I] += Values[K] * Vec[K];
      }
    }
    return Res;
  }

// TODO left side vector multiplication (maybe)
// TODO transposition (conversion to CSC format) (maybe)
};

} // namespace SparseMatrix
