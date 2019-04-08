#include "list_graph.h"
CListGraph::CListGraph(int vertexCount) : adjacencyList(vertexCount) {
}

void CListGraph::AddEdge(int from, std::pair<int , int> to) {
    adjacencyList[from].push_back(to);
}

int CListGraph::VerticesCount() const {
    return adjacencyList.size();
}

void CListGraph::GetOutcomingEdges(int vertex, vector<pair<int, int>> &vertices) const {
    vertices.clear();
    vertices = adjacencyList[vertex];
}

void CListGraph::GetIncomingEdges(int vertex, vector<pair<int, int>> &vertices) const {
    vertices.clear();
    for (vector<pair<int,int>> first: adjacencyList) {
        for (pair<int, int> second : first) {
            if (second.first == vertex) {
                vertices.push_back(second);
            }
        }
    }
}


