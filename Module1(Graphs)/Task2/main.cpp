#include <iostream>
#include <algorithm>
#include <queue>
#include <optional>
#include "hash_graph.h"
using std::queue;
using std::optional;
using std::nullopt;
optional<int> BFS(const CHashGraph *graph, int rootVertex){
    vector<bool> verticesState(graph->VerticesCount());
    vector<optional<int>> parents (graph->VerticesCount());
    vector<int> depths(graph->VerticesCount());
    queue<int> queue;
    queue.push(rootVertex);
    verticesState[rootVertex]=true;
    while(!queue.empty()){
        int cur=queue.front();
        queue.pop();
        vector<int> next;
        graph->GetNextVertices(cur,next);
        for(int neighbour: next){
            if(neighbour==parents[cur]){
                continue;
            }
            if(!verticesState[neighbour]){
                parents[neighbour]=cur;
                verticesState[neighbour]=true;
                depths[neighbour]=depths[cur]+1;
                queue.push(neighbour);
            }
            else{
                return depths[cur]+depths[neighbour]+1;
            }

        }
    }

    return nullopt;

}

int FindMinCycleLength(const CHashGraph *graph){
    vector<bool> verticesState(graph->VerticesCount(), false);
    int minLength=-1;
    for(int i=0;i<graph->VerticesCount();++i){
        optional<int> length = BFS(graph, i);
        if(length!=nullopt){
            minLength = minLength==-1 ? *length : std::min(minLength, *length);
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
    std::cout<< FindMinCycleLength(&graph);



}
