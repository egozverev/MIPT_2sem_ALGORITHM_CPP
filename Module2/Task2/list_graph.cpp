#include "list_graph.h"
using std::make_pair;
CListGraph::CListGraph(int vertexCount) : adjacencyList(vertexCount) {
}

void CListGraph::AddEdge(int from, std::pair<int , double> to) {
    adjacencyList[from].push_back(to);
}

int CListGraph::VerticesCount() const {
    return adjacencyList.size();
}

void CListGraph::GetOutcomingEdges(int vertex, vector<pair<int, double>> &vertices) const {
    vertices.clear();
    vertices = adjacencyList[vertex];
}

void CListGraph::GetIncomingEdges(int vertex, vector<pair<int, double>> &vertices) const {
    vertices.clear();
    for (int firstVertex=0; firstVertex<VerticesCount(); ++firstVertex) {
        for (pair<int, double> secondVertex : adjacencyList[firstVertex]) {
            if (secondVertex.first == vertex) {
                vertices.emplace_back(make_pair(firstVertex, secondVertex.second));
            }
        }
    }
}


