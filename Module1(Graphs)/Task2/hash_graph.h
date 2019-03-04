#pragma once

#include "graph.h"

#include <unordered_set>
#include <vector>
using std::vector;
using std::unordered_set;
class CHashGraph : public IGraph {
public:
    CHashGraph(int vertexCount);
    CHashGraph(const IGraph* _graph);
    virtual void AddEdge(int from, int to);
    virtual int VerticesCount() const;
    virtual void GetNextVertices(int vertex, vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, vector<int>& vertices) const;

private:
    vector<unordered_set<int> > adjacencyTable;
};