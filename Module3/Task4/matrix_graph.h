#pragma once


#include <vector>


class CMatrixGraph  {
public:
    CMatrixGraph(int vertexCount);
    virtual void AddEdge(int from, int to, int length);
    virtual int VerticesCount() const;
    virtual int GetEdgeLength(int from, int to) const;
    virtual void ChangeEdgeLength(int from, int to, int newLength);
    virtual void GetNextVertices(int firstVertex, std::vector<std::pair<int,int>>& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<std::pair<int,int>>& vertices) const;
private:
    std::vector<std::vector<int> > adjacencyMatrix;
};