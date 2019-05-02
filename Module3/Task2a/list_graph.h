#pragma once

#include <vector>
#include <utility>

using std::vector;
using std::pair;
class CListGraph {
public:
    CListGraph(int vertexCount);
    virtual void AddEdge(int from, std::pair<int , double> to);
    virtual int VerticesCount() const;
    virtual void GetOutcomingEdges(int vertex, vector<pair<int, double>> &vertices) const;
    virtual void GetIncomingEdges(int vertex, vector<pair<int, double>> &vertices) const;
    virtual double FindEdgeLength(int from, int to) const;

private:
    vector<vector<pair<int,double> > > adjacencyList; // pair<toVertex, wayLength>
};
