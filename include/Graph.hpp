#pragma once

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "SparceMatrix.hpp"

namespace Graph {

using NodeId = size_t;

struct Edge {
  NodeId From;
  NodeId To;
  double Weight;
};

struct EdgeKey {
  NodeId From;
  NodeId To;

  bool operator==(const EdgeKey &other) const {
    return From == other.From && To == other.To;
  }
};

struct EdgeKeyHash {
  size_t operator()(const EdgeKey &k) const {
    return std::hash<NodeId>()(k.From) ^ (std::hash<NodeId>()(k.To) << 1);
  }
};

class Node {
private:
  double Reward;
  NodeId ID;
  std::vector<NodeId> Successors;
  std::vector<NodeId> Predecessors;

public:
  double getReward() const { return Reward; };
  void setReward(int Reward_) { Reward = Reward_; }
  const std::vector<NodeId> &succ() const { return Successors; };
  const std::vector<NodeId> &pred() const { return Predecessors; };

  void addSucc(NodeId Succ) {
    if (std::find(Successors.begin(), Successors.end(), Succ) !=
        Successors.end()) {
      std::cout << "ExampleGraph warning: " << Succ
                << " is already a successor of " << ID << '\n';
    } else {
      Successors.push_back(Succ);
    }
  }

  void addPred(NodeId Pred) {
    if (std::find(Predecessors.begin(), Predecessors.end(), Pred) !=
        Predecessors.end()) {
      std::cout << "ExampleGraph warning: " << Pred
                << " is already a predecessor of " << ID << '\n';
    } else {
      Predecessors.push_back(Pred);
    }
  }

  void removeSucc(NodeId Succ) {
    auto ExistingSucc = std::find(Successors.begin(), Successors.end(), Succ);
    if (ExistingSucc == Successors.end()) {
      std::cout << "ExampleGraph error: tried to remove" << Succ
                << " which is not a successor of " << ID << '\n';
    } else {
      Successors.erase(ExistingSucc);
    }
  }

  void removePred(NodeId Pred) {
    auto ExistingPred =
        std::find(Predecessors.begin(), Predecessors.end(), Pred);
    if (ExistingPred == Predecessors.end()) {
      std::cout << "ExampleGraph error: tried to remove" << Pred
                << " which is not a successor of " << ID << '\n';
    } else {
      Predecessors.erase(ExistingPred);
    }
  }
};

class Graph {
private:
  std::vector<Node> Nodes;
  std::unordered_map<EdgeKey, double, EdgeKeyHash> Edges;
  NodeId Absorbing;

public:
  Graph(size_t N, NodeId Abs) : Nodes(N), Absorbing(Abs) {}

  Node &node(NodeId id) { return Nodes[id]; }

  void addEdge(NodeId from, NodeId to, double weight = 1.0) {
    Nodes[from].addSucc(to);
    Nodes[to].addPred(from);
    Edges[{from, to}] = weight;
  }

  double getEdgeWeight(NodeId from, NodeId to) const {
    return Edges.at({from, to});
  }

  SparseMatrix::SparseMatrixCSR buildSystemMatrix() const {
    size_t GraphSize = Nodes.size();
    SparseMatrix::SparseMatrixCSR SystemMatrix;
    SystemMatrix.Dim = GraphSize;
    SystemMatrix.RowPtr.resize(GraphSize + 1);

    for (NodeId i = 0; i < GraphSize; ++i) {

      SystemMatrix.RowPtr[i] = SystemMatrix.Values.size();

      SystemMatrix.ColIdx.push_back(i);
      SystemMatrix.Values.push_back(1.0);

      if (i == Absorbing) {
        continue;
      }

      double TotalWeight = 0.0;
      for (NodeId succ : Nodes[i].succ()) {
        TotalWeight += getEdgeWeight(i, succ);
      }

      if (TotalWeight == 0.0) {
        continue;
      }

      for (NodeId succ : Nodes[i].succ()) {
        if (succ == Absorbing) {
          continue;
        }

        double Probability = getEdgeWeight(i, succ) / TotalWeight;

        SystemMatrix.ColIdx.push_back(succ);
        SystemMatrix.Values.push_back(-Probability);
      }
    }

    SystemMatrix.RowPtr[GraphSize] = SystemMatrix.Values.size();
    return SystemMatrix;
  }

  std::vector<double> buildRewardVector() const {
    size_t GraphSize = Nodes.size();
    std::vector<double> RewardVector(GraphSize, 0.0);

    for (NodeId i = 0; i < GraphSize; ++i) {
      if (i != Absorbing) {
        RewardVector[i] = Nodes[i].getReward();
      }
    }

    return RewardVector;
  }
};

} // namespace Graph
