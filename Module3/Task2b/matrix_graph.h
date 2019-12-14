#pragma once


#include <vector>


class CMatrixGraph  {
public:
    CMatrixGraph(int vertexCount);
    virtual void AddEdge(int from, int to, double length);
    virtual int VerticesCount() const;
    virtual double GetEdgeLength(int from, int to) const;
    virtual void ChangeEdgeLength(int from, int to, double newLength);
    virtual void GetNextVertices(int firstVertex, std::vector<std::pair<int,double>>& vertices) const;
    virtual void GetPrevVertices(int vertex, std::vector<std::pair<int,double>>& vertices) const;
    virtual std::vector<int> GetOddVertices();
    virtual CMatrixGraph FindMinor(std::vector<int>& vertices);
private:
    std::vector<std::vector<double> > adjacencyMatrix;
};