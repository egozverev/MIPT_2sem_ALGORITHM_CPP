#include "list_graph.h"
#include <iostream>
#include <algorithm>
#include <queue>
using std::queue;
int CountWays(const CListGraph* graph, int from, int to ){
    vector<bool> verticesStates(graph->VerticesCount());
    vector<int> countWays(graph->VerticesCount());
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
        vector<short> next;
        graph->GetNextVertices(cur, next);
        for(int neighbour: next){
            if(levels[neighbour]<=levels[cur]){
                continue;
            }
            countWays[neighbour]+=countWays[cur];
            if(!verticesStates[neighbour]){
                verticesStates[neighbour]=true;
                levels[neighbour]=levels[cur]+1;
                verticesQueue.push(neighbour);
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