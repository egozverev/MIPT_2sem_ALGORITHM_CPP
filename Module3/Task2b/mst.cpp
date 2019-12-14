#include "matrix_graph.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <unordered_map>


CMatrixGraph FindMST(CMatrixGraph& graph){
    //Using Prim`s algorithm
    std::vector<int> parents(graph.VerticesCount());
    std::vector<bool> wasUsed(graph.VerticesCount());
    std::set<std::pair<double, int>> queue; // distance, to
    std::vector<double> distancesToParents(graph.VerticesCount(), std::numeric_limits<double>::max());
    distancesToParents[0]=0;
    queue.emplace(std::make_pair(0,0));
    while(!queue.empty()){
        int currentVertex = queue.begin()->second;
        queue.erase(queue.begin());
        if(wasUsed[currentVertex]){
            continue;
        }
        wasUsed[currentVertex] = true ;
        std::vector<std::pair<int, double> > next;
        graph.GetNextVertices(currentVertex, next);
        for(auto edge: next){
            if( wasUsed[edge.first] || distancesToParents[edge.first]<= edge.second){
                continue;
            }
            queue.erase(std::make_pair(distancesToParents[edge.first], edge.first));
            queue.emplace(std::make_pair(edge.second, edge.first));
            distancesToParents[edge.first] = std::min(edge.second ,distancesToParents[edge.first]);
            parents[edge.first] = currentVertex;
        }
    }
    CMatrixGraph mstGraph(graph.VerticesCount());
    for(int i=1; i< graph.VerticesCount(); ++i){
        mstGraph.AddEdge(parents[i], i, distancesToParents[i]);
        mstGraph.AddEdge(i, parents[i], distancesToParents[i]);

    }
    return  mstGraph;
}
