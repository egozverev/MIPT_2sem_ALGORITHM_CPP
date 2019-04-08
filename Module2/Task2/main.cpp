#include <iostream>
#include "list_graph.h"
#include <limits>
#include <algorithm>
void FillArbitrageGraph(CListGraph &graph) {
    for (int first = 0; first < graph.VerticesCount(); ++first) {
        for (int second = 0; second < graph.VerticesCount(); ++second) {
            if (first == second) {
                continue;
            }
            double coeff;
            std::cin>>coeff;
            if(coeff==-1){
                continue;
            }
            graph.AddEdge(first, std::make_pair(second, coeff));
        }
    }
}

bool IsArbitrageTradePossible(const CListGraph &graph) {
    for (int vertex = 0; vertex < graph.VerticesCount(); ++vertex) {
        vector<double> distances(graph.VerticesCount(), std::numeric_limits<double>::min());
        distances[vertex] = 1;
        for (int son = 0; son < graph.VerticesCount(); ++son) {
            vector<std::pair<int, double>> prev;
            graph.GetIncomingEdges(son, prev);
            for (std::pair<int, double> parent: prev) {
                distances[son] = std::max(distances[son], distances[parent.first] * parent.second);
            }
        }

        for (int son = 0; son < graph.VerticesCount(); ++son) {
            vector<std::pair<int, double>> next;
            graph.GetIncomingEdges(son, next);
            for (std::pair<int, double> parent: next) {
                if(distances[son]<distances[parent.first] * parent.second){
                    return true;
                }
            }
        }

    }
    return false;
}

int main() {
    int n;
    std::cin >> n;
    CListGraph graph(n);
    FillArbitrageGraph(graph);
    std::cout<<(IsArbitrageTradePossible(graph)? "YES" : "NO");

}