#pragma once
#include <iostream>
#include <vector>
namespace Interface {

template <class MatrixT, typename ValT> class IMatrix {
private:
  MatrixT ExactMatrix;
  // might store something later
public:
  IMatrix(size_t NewDim) : ExactMatrix(NewDim){};
  IMatrix(const std::vector<ValT> Vec, size_t InitDim)
      : ExactMatrix(Vec, InitDim){};
  IMatrix(const MatrixT &InitMatr) : ExactMatrix(InitMatr) {
    std::cout << "constructing from other MatrixT\n";
  };
  IMatrix(const IMatrix &OtherImatrix) : ExactMatrix(OtherImatrix.ExactMatrix) {
    std::cout << "constructing from other IMatrix\n";
  };

  ValT get(size_t I, size_t J) const { return ExactMatrix.get(I, J); }

  void set(size_t I, size_t J, ValT NewVal) { ExactMatrix.set(I, J, NewVal); }

  size_t dim() const { return ExactMatrix.dim(); }

  std::vector<ValT> getNonZeroValues() const { return ExactMatrix.getValues(); }

  std::vector<ValT> &getNonZeroValues() { return ExactMatrix.getValues(); }

  std::vector<ValT> operator*(const std::vector<ValT> Vec) const {
    return ExactMatrix.operator*(Vec);
  }

  std::vector<ValT> operator*(const MatrixT Other) const {
    return ExactMatrix.operator*(Other);
  }

  std::vector<ValT> operator*(const ValT Val) const {
    return ExactMatrix.operator*(Val);
  }

  std::vector<ValT> operator+(const MatrixT Other) const {
    return ExactMatrix.operator+(Other);
  }
};

} // namespace Interface
