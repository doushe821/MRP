#pragma once

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace SparseMatrix {

// sparse vector needed.
// NOTE it's actually a NxN only matrix.
template <typename ValT> class SparseMatrixCSR {
private:
  const size_t Dim = 0;

public:
  // TODO think
  // thought.
  ~SparseMatrixCSR() = default;
  SparseMatrixCSR<ValT>(size_t InitDim) : Dim(InitDim){};
  SparseMatrixCSR<ValT>(std::vector<ValT> InitValues, size_t InitDim)
      : Dim(InitDim) {
    if ((InitDim * InitDim) != InitValues.size()) {
      throw(std::runtime_error("Invalid vector - dimension combination\n"));
    }
    RowPtr.resize(Dim + 1);
    RowPtr[0] = 0;
    size_t PseudoRowIdx {0};
    size_t PseudoColIdx {0};
    size_t NonZero {0};
    for (auto &Val :
         InitValues) { // there was something similar in std::algorithm
      if (Val) {
        Values.push_back(Val);
        ColIdx.push_back(PseudoRowIdx);
        ++NonZero;
      }
      if (PseudoRowIdx == (InitDim - 1)) {
        std::cout << "End of row " << PseudoColIdx << ", nonzero = " << NonZero << '\n';
        RowPtr[PseudoColIdx + 1] = NonZero;
        PseudoRowIdx = 0;
        ++PseudoColIdx;
      } else {
        ++PseudoRowIdx;
      }
    }
  };

  SparseMatrixCSR<ValT>(SparseMatrixCSR<ValT> &OtherMatrix)
      : Dim(OtherMatrix.dim()) {
    RowPtr.resize(Dim + 1);
    Values.assign(OtherMatrix.getValues());
    RowPtr.assign(OtherMatrix.getRowPtr());
    RowPtr.assign(OtherMatrix.getColIdx());
  };

  // Optimize
  std::vector<ValT> getValues() const { return Values; }

  std::vector<ValT> &getValues() { return Values; }

  std::vector<size_t> getRowPtr() const { return RowPtr; }

  std::vector<size_t> getColIdx() const { return ColIdx; }
  // Don't forget to move to private
  std::vector<size_t> RowPtr;
  std::vector<size_t> ColIdx;
  std::vector<ValT> Values;

  ValT get(size_t Row, size_t Col) const {
    for (size_t K = RowPtr[Row]; K < RowPtr[Row + 1]; ++K) {
      if (ColIdx[K] == Col) {
        return Values[K];
      }
    }
    ValT HopefullyZero{0};
    return HopefullyZero;
  }

  size_t dim() const { return Dim; }

  std::vector<ValT> operator*(const std::vector<ValT> Vec) const {
    if (Vec.size() != Dim) {
      throw(std::runtime_error(
          "SparseMatrix: Vector and matrix dimensions mismatch"));
    }
    std::vector<ValT> Res;
    Res.resize(Dim);
    for (size_t I = 0; I < Dim; ++I) {
      for (size_t K = RowPtr[I]; K < RowPtr[I + 1]; ++K) {
        Res[I] += Values[K] * Vec[K];
      }
    }
    return Res;
  }

  // This is shit, learn cpp please
  SparseMatrixCSR<ValT> operator*(const ValT Val) const {
    SparseMatrixCSR<ValT> NewMatr{*this};
    for (auto &NewVal : NewMatr.getValues()) {
      NewVal *= Val;
    }
    return NewMatr;
  }

  // TODO left side vector multiplication (maybe)
  // TODO transposition (conversion to CSC format) (maybe)
};

} // namespace SparseMatrix
