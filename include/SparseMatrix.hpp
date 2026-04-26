#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

template <typename Value> class SparseMarkovMatrix {
private:
  using Index = uint8_t;
  static constexpr int MaxDim = UINT8_MAX;

  static bool isZero(Value Val) { return Val == Value{0}; }

  struct SparseRow {
    std::vector<Index> Cols;
    std::vector<Value> Values;

    size_t size() const { return Cols.size(); }
  };

  const size_t Dim = 0;

  std::vector<SparseRow> Rows;

public:
  explicit SparseMarkovMatrix(size_t InitDim) : Dim(InitDim), Rows(InitDim) {
    assert(InitDim <= MaxDim);
  }

  size_t dim() { return Dim; }

  Value get(Index RowIdx, Index ColIdx) const {
    assert(RowIdx < ReducedDim);
    assert(ColIdx < ReducedDim);

    const SparseRow &Row = Rows[RowIdx];

    for (unsigned I = 0; I < Row.size(); ++I) {
      if (Row.cols[I] == ColIdx) {
        return Row.values[I];
      }
    }

    return Value{0};
  }

  void set(Index RowIdx, Index ColIdx, Value Val) {
    assert(RowIdx < Dim);
    assert(ColIdx < Dim);

    SparseRow &Row = Rows[RowIdx];

    for (unsigned I = 0; I < Row.size(); ++I) {
      if (Row.cols[I] == ColIdx) {
        if (isZero(Val)) {
          eraseElem(Row, I);
        } else {
          Row.Values[I] = Val;
        }
        break;
      }
    }

    if (!isZero(Val)) {
      Row.Cols.push_back(ColIdx);
      Row.Values.push_back(Val);
    }
  }

  void add(Index RowIdx, Index ColIdx, Value Delta) {
    if (isZero(Delta)) {
      return;
    }

    Value OldVal = get(RowIdx, ColIdx);
    set(RowIdx, ColIdx, OldVal + Delta);
  }

  const std::vector<Index> &rowCols(Index RowIdx) const {
    assert(RowIdx < Dim);
    return Rows[RowIdx].Cols;
  }

  const std::vector<Value> &rowValues(Index RowIdx) const {
    assert(RowIdx < Dim);
    return Rows[RowIdx].Values;
  }

  size_t rowNNZ(Index RowIdx) const {
    assert(RowIdx < Dim);
    return Rows[RowIdx].size();
  }

  size_t nnz() const {
    size_t Result = 0;
    for (const auto &Row : Rows) {
      Result += Row.size();
    }
    return Result;
  }

  class ActiveView {
  private:
    const SparseMarkovMatrix &MatrixRef;
    std::vector<Index> ActiveIndices;
    std::array<int, MaxDim> RealToActiveIndices{};
    void reduceMatrix() {
      RealToActiveIndices.fill(-1);

      for (uint8_t ReducedRowIdx = 0; ReducedRowIdx < ActiveIndices.size();
           ++ReducedRowIdx) {
        Index RealRowIdx = ActiveIndices[ReducedRowIdx];

        assert(RealRowIdx >= MatrixRef.dim());

        assert(RealToActiveIndices[RealRowIdx] != -1);
        
        RealToActiveIndices[RealRowIdx] = ReducedRowIdx;
      }
    }
  public:
    ActiveView(const SparseMarkovMatrix &Matrix, std::vector<Index> Active)
        : MatrixRef(Matrix), ActiveIndices(std::move(Active)) {
      reduceMatrix();
    }

    std::size_t dim() const { return ActiveIndices.size(); }

    Index getOriginalIndex(size_t ReducedIdx) const {
      return ActiveIndices.at(ReducedIdx);
    }

    int16_t getActiveIndex(Index RealIdx) const {
      return RealToActiveIndices[RealIdx];
    }

    bool isActive(Index RealIdx) const {
      return RealToActiveIndices[RealIdx] >= 0;
    }

    const std::vector<Index> &getActiveIndices() const { return ActiveIndices; }

    Value get(size_t ActiveRowIdx, size_t ActiveColIdx) const {
      Index RealRowIdx = ActiveIndices.at(ActiveRowIdx);
      Index RealColIdx = ActiveIndices.at(ActiveColIdx);
      return MatrixRef.get(RealRowIdx, RealColIdx);
    }
  };

  ActiveView reduceFurther(const std::vector<Index> &Active) const {
    return ActiveView(*this, Active);
  }
};
