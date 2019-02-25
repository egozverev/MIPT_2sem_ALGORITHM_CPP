#include "arc_graph.h"
#include <set>
CArcGraph::CArcGraph(int vertexCount) : arcList() {
}
CArcGraph::CArcGraph(const IGraph *_graph) {
    for(int vertex=0; vertex<_graph->VerticesCount();++vertex){
        std::vector<int> nextVertices;
        _graph->GetNextVertices(vertex, nextVertices);
        for(int next : nextVertices){
            AddEdge(vertex, next);
        }
    }
}

void CArcGraph::AddEdge(int from, int to) {
    arcList.emplace_back(std::make_pair(from, to));


}

int CArcGraph::VerticesCount() const {
    return arcList.size();
}

void CArcGraph::GetNextVertices(int vertex, vector<int>& vertices) const {
    vertices.clear();
    for(pair<int, int> pair : arcList){
        if(pair.first==vertex){
            vertices.push_back(pair.second);
        }
    }
}

void CArcGraph::GetPrevVertices(int vertex, vector<int>& vertices) const {
    vertices.clear();
    for(pair<int, int> pair : arcList){
        if(pair.first==vertex){
            vertices.push_back(pair.second);
        }
    }
}


