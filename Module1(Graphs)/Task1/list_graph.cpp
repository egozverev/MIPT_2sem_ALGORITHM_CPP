#include "list_graph.h"

CListGraph::CListGraph(int vertexCount) : adjacencyList(vertexCount) {
}

CListGraph::CListGraph(const IGraph *_graph) {
    for(int vertex=0; vertex<_graph->VerticesCount();++vertex){
        std::vector<int> nextVertices;
        _graph->GetNextVertices(vertex, nextVertices);
        for(int next : nextVertices){
            AddEdge(vertex, next);
        }
    }
}

void CListGraph::AddEdge(int from, int to) {
    adjacencyList[from].push_back(to);
}

int CListGraph::VerticesCount() const {
    return adjacencyList.size();
}

void CListGraph::GetNextVertices(int vertex, vector<int>& vertices) const {
    vertices.clear();
    vertices = adjacencyList[vertex];
}

void CListGraph::GetPrevVertices(int vertex, vector<int>& vertices) const {
    vertices.clear();
    for (int i = 0; i < adjacencyList.size(); i++) {
        for (int second : adjacencyList[i]) {
            if (second == vertex) {
                vertices.push_back(i);
            }
        }
    }
}


