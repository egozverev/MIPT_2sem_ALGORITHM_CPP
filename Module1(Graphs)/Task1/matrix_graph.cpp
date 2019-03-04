#include "matrix_graph.h"

CMatrixGraph::CMatrixGraph(int vertexCount): adjacencyMatrix(vector<vector<int > >( vertexCount, vector<int>(vertexCount, 0))) {}
CMatrixGraph::CMatrixGraph(const IGraph *_graph) {
    for(int vertex=0; vertex<_graph->VerticesCount();++vertex){
        std::vector<int> nextVertices;
        _graph->GetNextVertices(vertex, nextVertices);
        for(int next : nextVertices){
            AddEdge(vertex, next);
        }
    }
}
void CMatrixGraph::AddEdge(int from, int to) {
    adjacencyMatrix[from][to]=1;
}
int CMatrixGraph::VerticesCount() const {
    return adjacencyMatrix.size();
}
void CMatrixGraph::GetNextVertices(int firstVertex, vector<int> &vertices) const {
    vertices.clear();
    for(int secondVertex=0; secondVertex<adjacencyMatrix.size(); ++secondVertex){
        if(adjacencyMatrix[firstVertex][secondVertex]){
            vertices.push_back(secondVertex);
        }
    }
}
void CMatrixGraph::GetPrevVertices(int vertex, vector<int> &vertices) const {
    vertices.clear();
    for(int i=0; i<adjacencyMatrix.size();++i){
        if(adjacencyMatrix[i][vertex]){
            vertices.push_back(i);
        }
    }
}