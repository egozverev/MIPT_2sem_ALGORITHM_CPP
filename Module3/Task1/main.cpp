#include "list_graph.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <unordered_map>

void FillGraph(CListGraph& graph, const unsigned int edgeNumber){
    for(int i=0; i<edgeNumber; ++i){
        int from, to, distance;
        std::cin>>from>>to>>distance;
        --from;
        --to;
        graph.AddEdge(from, std::make_pair(to, distance));
        graph.AddEdge(to, std::make_pair(from, distance));
    }
}
int FindMSTLength(CListGraph& graph){
    //Using Prim`s algorithm
    std::vector<bool> wasUsed(graph.VerticesCount());
    std::set<std::pair<int, int>> queue; // distance, to
    std::vector<int> distancesToParents(graph.VerticesCount(), std::numeric_limits<int>::max());
    distancesToParents[0]=0;
    queue.emplace(std::make_pair(0,0));
    while(!queue.empty()){
        int currentVertex = queue.begin()->second;
        queue.erase(queue.begin());
        if(wasUsed[currentVertex]){
            continue;
        }
        wasUsed[currentVertex] = true ;
        std::vector<std::pair<int, int> > next;
        graph.GetOutcomingEdges(currentVertex, next);
        for(auto edge: next){
            if( wasUsed[edge.first] || distancesToParents[edge.first]<= edge.second){
                continue;
            }
            queue.erase(std::make_pair(distancesToParents[edge.first], edge.first));
            queue.emplace(std::make_pair(edge.second, edge.first));
            distancesToParents[edge.first] = std::min(edge.second ,distancesToParents[edge.first]);

        }
    }
    int ans = 0;
    for(int elem: distancesToParents){
        ans += elem;
    }
    return ans;
}
int main() {
    unsigned int n, m;
    std::cin>>n>>m;
    CListGraph graph(n);
    FillGraph(graph, m);
    std::cout<<FindMSTLength(graph);

}