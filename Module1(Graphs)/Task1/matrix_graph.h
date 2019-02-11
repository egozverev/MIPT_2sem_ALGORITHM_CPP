#pragma once

#include "graph.h"

#include <vector>

using std::vector;

class CMatrixGraph : public IGraph {
public:
    CMatrixGraph(int vertexCount);
    CMatrixGraph(const IGraph* _graph);
    virtual void AddEdge(int from, int to);
    virtual int VerticesCount() const;
    virtual void GetNextVertices(int vertex, vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, vector<int>& vertices) const;

private:
    vector<vector<int> > adjacencyMatrix;
};