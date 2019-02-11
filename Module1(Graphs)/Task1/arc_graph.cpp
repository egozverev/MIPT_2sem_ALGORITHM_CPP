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
    arcList.push_back(vector<int> {from, to});

}

int CArcGraph::VerticesCount() const {
    std::set<int> vertices;
    for(vector< int >pair : arcList){
        vertices.insert(pair[0]);
        vertices.insert(pair[1]);
    }
    return vertices.size();
}

void CArcGraph::GetNextVertices(int vertex, vector<int>& vertices) const {
    vertices.clear();
    for(vector< int >pair : arcList){
        if(pair[0]==vertex){
            vertices.push_back(pair[1]);
        }
    }
}

void CArcGraph::GetPrevVertices(int vertex, vector<int>& vertices) const {
    vertices.clear();
    for(vector< int >pair : arcList){
        if(pair[1]==vertex){
            vertices.push_back(pair[0]);
        }
    }
}


