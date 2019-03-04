#include <iostream>
#include <algorithm>
#include "list_graph.h"

void GraphCondensation(const CListGraph* graph, CListGraph* newGraph, vector<int>& marks){
    for(int first=0; first<graph->VerticesCount();++first){
        vector<int> next;
        graph->GetNextVertices(first, next);
        for(int second: next){
            if(marks[first]==marks[second] || newGraph->CheckEdge(marks[first], marks[second])){
                continue;
            }
            newGraph->AddEdge(marks[first], marks[second]);
        }
    }
}

int CountRequiredEdges(CListGraph* graph){
    if(graph->VerticesCount()<=1){
        return 0;
    }
    int noIncomingCount=0;
    int noOutcomingCount=0;
    for(int vertex=0; vertex<graph->VerticesCount();++vertex){
        vector<int> next;
        graph->GetNextVertices(vertex,next);
        if(next.empty()){
            ++noOutcomingCount;
        }
        graph->GetPrevVertices(vertex, next);
        if(next.empty()){
            ++noIncomingCount;
        }
    }
    return std::max(noIncomingCount, noOutcomingCount);
}
int main() {
    int v, e;
    std::cin>>v>>e;
    CListGraph graph(v);
    CListGraph transGraph(v);
    for(int i=0; i<e;++i){
        int from, to;
        std::cin>>from>>to;
        if(from==to){
            continue;
        }
        --from;
        --to;
        graph.AddEdge(from, to);
    }
    graph.GetTransposed(transGraph);
    vector<int> leave;
    graph.DFS(leave);
    vector<int> marks(graph.VerticesCount());
    int componentsNum;
    componentsNum = transGraph.MarkComponents(leave, marks);
    CListGraph condensedGraph(componentsNum);
    GraphCondensation(&graph, &condensedGraph, marks);
    std::cout<<CountRequiredEdges(&condensedGraph);
}
