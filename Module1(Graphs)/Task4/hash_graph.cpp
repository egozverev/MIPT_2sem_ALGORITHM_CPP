#include "hash_graph.h"

CHashGraph::CHashGraph(int vertexCount) : adjacencyTable(vertexCount) {
}
void CHashGraph::AddEdge(int from, int to) {
    adjacencyTable[from].insert(to);
}

int CHashGraph::VerticesCount() const {
    return adjacencyTable.size();
}

void CHashGraph::GetNextVertices(int vertex, vector<int>& vertices) const {
    vertices.clear();
    for(int i: adjacencyTable[vertex]){
        vertices.push_back(i);
    }
}

void CHashGraph::GetPrevVertices(int vertex, vector<int>& vertices) const {
    vertices.clear();
    for (int i = 0; i < adjacencyTable.size(); i++) {
        for (int second : adjacencyTable[i]) {
            if (second == vertex) {
                vertices.push_back(i);
            }
        }
    }
}


