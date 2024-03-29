#pragma once

#include "graph.h"

#include <vector>
#include <utility>
using std::vector;
using std::pair;
class CArcGraph : public IGraph {
public:
    CArcGraph(int vertexCount);
    CArcGraph(const IGraph* _graph);
    virtual void AddEdge(int from, int to);
    virtual int VerticesCount() const;
    virtual void GetNextVertices(int vertex, vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, vector<int>& vertices) const;

private:
    vector<pair<int, int>> arcList;
};