#include "matrix_graph.h"
#include <utility>

CMatrixGraph::CMatrixGraph(int vertexCount) : adjacencyMatrix(
        std::vector<std::vector<double> >(vertexCount, std::vector<double>(vertexCount, 0))) {}

void CMatrixGraph::AddEdge(int from, int to, double length) {
    adjacencyMatrix[from][to] = length;
}

int CMatrixGraph::VerticesCount() const {
    return adjacencyMatrix.size();
}

double CMatrixGraph::GetEdgeLength(int from, int to) const {
    return adjacencyMatrix[from][to];
}

void CMatrixGraph::ChangeEdgeLength(int from, int to, double newLength) {
    adjacencyMatrix[from][to] = newLength;
}

void CMatrixGraph::GetNextVertices(int firstVertex, std::vector<std::pair<int, double>> &vertices) const {
    //to, distance
    vertices.clear();
    for (int secondVertex = 0; secondVertex < adjacencyMatrix.size(); ++secondVertex) {
        if (adjacencyMatrix[firstVertex][secondVertex] != 0) {
            vertices.emplace_back(std::make_pair(secondVertex, adjacencyMatrix[firstVertex][secondVertex]));
        }
    }
}

void CMatrixGraph::GetPrevVertices(int vertex, std::vector<std::pair<int, double>> &vertices) const {
    //distance, from
    vertices.clear();
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
        if (adjacencyMatrix[i][vertex] != 0) {
            vertices.emplace_back(std::make_pair(i, adjacencyMatrix[i][vertex]));
        }
    }
}

std::vector<int> CMatrixGraph::GetOddVertices() {
    std::vector<int> oddVertices;
    for (int vert = 0; vert < VerticesCount(); ++vert) {
        std::vector<std::pair<int, double>> next;
        GetNextVertices(vert, next);
        if(next.size() % 2){
            oddVertices.push_back(vert);
        }
    }
    return oddVertices;
}

CMatrixGraph CMatrixGraph::FindMinor(std::vector<int>& vertices){
    // находит минор на пересечении строк и столбцов vertices
    CMatrixGraph minorGraph(VerticesCount());
    for(int i=0; i < vertices.size(); ++i){
        for(int j=0; j < vertices.size(); ++j){
            if(i == j){
                continue;
            }
            minorGraph.AddEdge(vertices[i], vertices[j], GetEdgeLength(vertices[i], vertices[j]));
        }
    }
    return minorGraph;
}
