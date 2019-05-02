#pragma once

#include <vector>
#include <utility>

using std::vector;
using std::pair;
class CListGraph {
public:
    CListGraph(int vertexCount);
    virtual void AddEdge(int from, std::pair<int , int> to);
    virtual int VerticesCount() const;
    virtual void GetOutcomingEdges(int vertex, vector<pair<int, int>> &vertices) const;
    virtual void GetIncomingEdges(int vertex, vector<pair<int, int>> &vertices) const;

private:
    vector<vector<pair<int,int> > > adjacencyList; // pair<toVertex, wayLength>
};
