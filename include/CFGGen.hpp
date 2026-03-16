#pragma once

#include "Graph.hpp"
#include "RNG.hpp"

namespace Graph {

template<typename NodeT, class MatrixT>
AbsorbingMarkovChain<NodeT, MatrixT>
generateRandomCFG(size_t N, NodeId absorbing, unsigned seed = 0)
{
  RNG rng(seed);

  AbsorbingMarkovChain<NodeT, MatrixT> g(N, absorbing);

  for(NodeId i = 0; i < N; ++i)
  {
    if(i == absorbing)
      continue;

    g.node(i).setReward(1.0);

    // fallthrough edge
    if(i + 1 < N)
      g.addEdge(i, i+1);

    double p = rng.prob();

    // conditional branch
    if(p < 0.4 && i+2 < N)
    {
      NodeId target = std::min(i + 2 + rng.index(5), N-1);
      g.addEdge(i, target);
    }

    // loop backedge
    if(p > 0.8 && i > 2)
    {
      NodeId target = rng.index(i);
      g.addEdge(i, target);
    }
  }

  return g;
}

template<typename NodeT, class MatrixT>
AbsorbingMarkovChain<NodeT, MatrixT>
generateLoopCFG(size_t N, NodeId absorbing, unsigned seed = 0)
{
  RNG rng(seed);

  AbsorbingMarkovChain<NodeT, MatrixT> g(N, absorbing);

  for(NodeId i = 0; i < N; ++i)
  {
    if(i == absorbing)
      continue;

    g.node(i).setReward(1.0);

    if(i + 1 < N)
      g.addEdge(i, i+1);

    // strong probability of loop
    if(i > 3 && rng.prob() < 0.6)
    {
      NodeId back = rng.index(i);
      g.addEdge(i, back);
    }
  }

  return g;
}

template<typename NodeT, class MatrixT>
AbsorbingMarkovChain<NodeT, MatrixT>
generateStructuredCFG(size_t N, NodeId absorbing, unsigned seed = 0)
{
  RNG rng(seed);

  AbsorbingMarkovChain<NodeT, MatrixT> g(N, absorbing);

  NodeId i = 0;

  while(i + 4 < N)
  {
    g.node(i).setReward(1.0);

    double p = rng.prob();

    // diamond
    if(p < 0.4)
    {
      NodeId a = i;
      NodeId b = i+1;
      NodeId c = i+2;
      NodeId d = i+3;

      g.addEdge(a,b);
      g.addEdge(a,c);
      g.addEdge(b,d);
      g.addEdge(c,d);

      i += 3;
    }
    // loop
    else if(p < 0.7)
    {
      NodeId start = i;
      NodeId body = i+1;
      NodeId end = i+2;

      g.addEdge(start,body);
      g.addEdge(body,start);
      g.addEdge(body,end);

      i += 2;
    }
    else
    {
      g.addEdge(i,i+1);
      i++;
    }
  }

  for(; i < N-1; ++i)
    g.addEdge(i,i+1);

  return g;
}
} // namespace Graph
