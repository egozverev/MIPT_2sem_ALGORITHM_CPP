#include "list_graph.h"

CListGraph::CListGraph(int vertexCount) : adjacencyList(vertexCount) {
}



void CListGraph::AddEdge(int from, int to) {
    adjacencyList[from].push_back(to);
}
bool CListGraph::CheckEdge(int from, int to){
    for(int vertex: adjacencyList[from]){
        if(vertex==to){
            return true;
        }
    }
    return false;
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

void CListGraph::DFSFromVertex(int vertex, vector<bool>& wasInUse, vector<int>& leave) const{
    if(wasInUse[vertex]){
        return;
    }
    wasInUse[vertex]=true;
    vector<int> next;
    GetNextVertices(vertex, next);
    for(int second: next){
        if(wasInUse[second]){
            continue;
        }

        DFSFromVertex(second, wasInUse, leave);
    }
    leave.push_back(vertex);
}
void CListGraph:: DFS(vector<int>& leave) const{
    vector<bool> wasInUse(VerticesCount());
    for(int vertex=0; vertex<VerticesCount();++vertex){
        if(wasInUse[vertex]){
            continue;
        }
        DFSFromVertex(vertex, wasInUse, leave);
    }
}
int CListGraph::MarkComponents(vector<int>& leave, vector<int>& marks) const {
    int mark=0;
    vector<bool> wasInUse(VerticesCount());
    while(!leave.empty()){
        int vertex= leave.back();
        leave.pop_back();
        if(wasInUse[vertex]){
            continue;
        }
        wasInUse[vertex]=true;
        vector<int> stack{vertex};
        marks[vertex]=mark;
        while(!stack.empty()){
            int cur=stack.back();
            stack.pop_back();
            vector<int> next;
            GetNextVertices(cur, next);
            for(int second: next){
                if(wasInUse[second]){
                    continue;
                }
                stack.push_back(second);
                wasInUse[second]=true;
                marks[second]=mark;
            }

        }
        ++mark;
    }
    return mark;
}