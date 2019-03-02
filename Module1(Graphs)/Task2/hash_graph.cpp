#include "hash_graph.h"
#include <algorithm>
using std::copy;
CHashGraph::CHashGraph(int vertexCount) : adjacencyTable(vertexCount) {
}
CHashGraph::CHashGraph(const IGraph *_graph) {
    for(int vertex=0; vertex<_graph->VerticesCount();++vertex){
        std::vector<int> nextVertices(0);
        _graph->GetNextVertices(vertex, nextVertices);
        for(int next : nextVertices){
            AddEdge(vertex, next);
        }
    }
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
    copy(adjacencyTable.begin(), adjacencyTable.end(), vertices.begin());
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


