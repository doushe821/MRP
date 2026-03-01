#pragma once

#include <iostream>
#include <vector>

namespace SparseMatrix {

struct SparseMatrixCSR {
  std::vector<size_t> RowPtr;
  std::vector<size_t> ColIdx;
  std::vector<double> Values;
  size_t Dim = 0;
};

} // namespace SparseMatrix
