#pragma once

#include <vector>

using std::vector;

class CListGraph {
public:
    CListGraph(int vertexCount);
    virtual void AddEdge(int from, int to);
    bool CheckEdge(int from, int to);
    virtual int VerticesCount() const;
    virtual void GetNextVertices(int vertex, vector<int>& vertices) const;
    virtual void GetPrevVertices(int vertex, vector<int>& vertices) const;
    void  DFS(vector<int>& leave) const;
    int  MarkComponents(vector<int>& leave, vector<int>& marks) const;

private:
    void  DFSFromVertex(int vertex, vector<bool>& wasInUse, vector<int>& leave) const;
    vector<vector<int> > adjacencyList;
};
