#include "hash_graph.h"
#include <optional>
#include <iostream>
#include <algorithm>
#include <queue>
using std::queue;
using std::optional;
bool BFS_from_vertex(const CHashGraph* graph, vector<bool>& verticesState, int rootVertex ){
    vector<optional<int>> parents(graph->VerticesCount());
    vector<bool> marks(graph->VerticesCount());
    queue<int> queue;
    queue.push(rootVertex);
    verticesState[rootVertex]=true;
    while(!queue.empty()){

        int cur=queue.front();
        queue.pop();
        vector<int> next;
        graph->GetNextVertices(cur,next);
        for(int vertex: next){
            if(vertex==parents[cur]){
                continue;
            }
            if(!verticesState[vertex]){
                parents[vertex]=cur;
                verticesState[vertex]=true;
                queue.push(vertex);
                marks[vertex]=!marks[cur];
            }
            else if(marks[cur]==marks[vertex]){
                    return true;
                }

        }
    }

    return false;

}
bool Check(CHashGraph* graph){
    vector<bool> verticesState(graph->VerticesCount());
    for(int i=0; i<graph->VerticesCount();++i){
        if(verticesState[i]){
            continue;
        }
        if (BFS_from_vertex(graph, verticesState, i)){
            return false;
        }
    }
    return true;
}

int main(){
    int n,m;
    std::cin>>n>>m;
    CHashGraph graph(n);
    for(int i=0; i<m;++i){
        int from, to;
        std::cin>>from>>to;
        graph.AddEdge(from,to);
        graph.AddEdge(to, from);
    }
    std::cout<<(Check(&graph) ? "YES" : "NO ");



    return 0;
}