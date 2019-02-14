#include <iostream>
#include <algorithm>
#include <queue>
#include "hash_graph.h"
using std::queue;
int BFS_from_vertex(const CHashGraph* graph, int rootVertex ){
    vector<bool> verticesState(graph->VerticesCount(), false);
    vector<int> parents(graph->VerticesCount(), -1);
    vector<int> depths(graph->VerticesCount(), 0);
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
                depths[vertex]=depths[cur]+1;
                queue.push(vertex);
            }
            else{
                return depths[cur]+depths[vertex]+1;
            }

        }
    }

    return -1;

}

int findMinLength(const CHashGraph* graph){
    int minLength=-1;
    for(int i=0;i<graph->VerticesCount();++i){
        int length = BFS_from_vertex(graph,i);
        if(length!=-1){
            minLength = minLength==-1 ? length : std::min(minLength, length);
        }
    }
    return minLength;
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
    std::cout<<findMinLength(&graph);



return 0;
}