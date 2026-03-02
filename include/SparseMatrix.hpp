#pragma once

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace SparseMatrix {

struct SparseMatrixCSR {
  std::vector<size_t> RowPtr;
  std::vector<size_t> ColIdx;
  std::vector<double> Values;
  size_t Dim = 0;

  double get(size_t Row, size_t Col) {
    for (size_t k = RowPtr[Row]; k < RowPtr[Row + 1]; ++k) {
      if (ColIdx[k] == Col) {
        return Values[k];
      }
    }
    return 0.0;
  }

  std::vector<double> operator*(const std::vector<double> Vec) const {
    if (Vec.size() != Dim) {
      throw(std::runtime_error(
          "SparseMatrix: Vector and matrix dimensions mismatch"));
    }
    std::vector<double> Res;
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
