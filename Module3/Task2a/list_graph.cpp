#include "list_graph.h"

CListGraph::CListGraph(int vertexCount) : adjacencyList(vertexCount) {
}

void CListGraph::AddEdge(int from, std::pair<int, double> to) {
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
    for (vector<pair<int, double>> first: adjacencyList) {
        for (pair<int, double > second : first) {
            if (second.first == vertex) {
                vertices.push_back(second);
            }
        }
    }
}

double CListGraph::FindEdgeLength(int from, int to) const{
    vector<pair<int, double>> next;
    GetOutcomingEdges(from, next);
    for (pair<int, double > edge: next) {
        if (edge.first == to) {
            return edge.second;
        }
    }
    return -1;
}


