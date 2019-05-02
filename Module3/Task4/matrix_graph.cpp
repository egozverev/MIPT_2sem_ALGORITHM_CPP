#include "matrix_graph.h"
#include <utility>
CMatrixGraph::CMatrixGraph(int vertexCount): adjacencyMatrix(std::vector<std::vector<int > >( vertexCount, std::vector<int>(vertexCount, 0))) {}

void CMatrixGraph::AddEdge(int from, int to, int length) {
    adjacencyMatrix[from][to]=length;
}
int CMatrixGraph::VerticesCount() const {
    return adjacencyMatrix.size();
}
int CMatrixGraph::GetEdgeLength(int from, int to) const{
    return adjacencyMatrix[from][to];
}
void CMatrixGraph::ChangeEdgeLength(int from, int to, int newLength){
    adjacencyMatrix[from][to] = newLength;
}
void CMatrixGraph::GetNextVertices(int firstVertex, std::vector<std::pair<int,int>> &vertices) const {
    //distance, to
    vertices.clear();
    for(int secondVertex=0; secondVertex<adjacencyMatrix.size(); ++secondVertex){
        if(adjacencyMatrix[firstVertex][secondVertex]){
            vertices.emplace_back(std::make_pair(adjacencyMatrix[firstVertex][secondVertex], secondVertex));
        }
    }
}
void CMatrixGraph::GetPrevVertices(int vertex, std::vector<std::pair<int,int>> &vertices) const {
    //distance, from
    vertices.clear();
    for(int i=0; i<adjacencyMatrix.size();++i){
        if(adjacencyMatrix[i][vertex]){
            vertices.emplace_back(std::make_pair(adjacencyMatrix[i][vertex], i));
        }
    }
}
