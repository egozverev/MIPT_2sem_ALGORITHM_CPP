#include "hash_graph.h"
#include <iostream>
#include <algorithm>
#include <queue>
using std::queue;
int CountWays(const CListGraph* graph, int from, int to ){
    vector<bool> verticesStates(graph->VerticesCount(), false);
    vector<int> countWays(graph->VerticesCount(), 0);
    vector<int> levels(graph->VerticesCount(), graph->VerticesCount());

    queue<int> verticesQueue;
    verticesQueue.push(from);
    countWays[from]=1;
    levels[from]=0;
    while(!verticesQueue.empty()){
        int cur=verticesQueue.front();
        verticesQueue.pop();
        if(levels[cur]>=levels[to]){
            continue;
        }
        verticesStates[cur]=true;
        vector<int> next;
        graph->GetNextVertices(cur, next);
        for(int vertex: next){
            if(levels[vertex]<=levels[cur]){
                continue;
            }
            countWays[vertex]+=countWays[cur];
            if(!verticesStates[vertex]){
                verticesStates[vertex]=true;
                levels[vertex]=levels[cur]+1;
                verticesQueue.push(vertex);
            }

        }
    }
    return countWays[to];
}

int main(){
    int v,n;
    std::cin>>v>>n;
    int u,w;
    CListGraph graph(v);
    for(int i=0; i<n;++i){
        int from, to;
        std::cin>>from>>to;
        graph.AddEdge(from,to);
        graph.AddEdge(to, from);
    }
    std::cin>>u>>w;
    std::cout<<CountWays(&graph, u,w);



    return 0;
}