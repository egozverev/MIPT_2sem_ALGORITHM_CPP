#include "matrix_graph.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>

void FillGraph(CMatrixGraph &graph, unsigned int edgeNumber) {
    for (int i = 0; i < edgeNumber; ++i) {
        int from, to, distance;
        std::cin >> from >> to >> distance;
        --from;
        --to;
        graph.AddEdge(from, to, distance);
    }
}

CMatrixGraph GetResidualNetwork(const CMatrixGraph &graph, const CMatrixGraph &flow) {
    // Получить дополняющую сеть
    CMatrixGraph resGraph(graph.VerticesCount());
    for (int first = 0; first < graph.VerticesCount(); ++first) {
        for (int second = 0; second < graph.VerticesCount(); ++second) {
            if (first == second) {
                continue;
            }
            resGraph.AddEdge(first, second, graph.GetEdgeLength(first, second) - flow.GetEdgeLength(first, second));
        }
    }
    return resGraph;
}

CMatrixGraph GetAuxiliaryNetwork(const CMatrixGraph &graph) {
    //получить вспомогательную ( слоистую ) сеть
    std::vector<int> level(graph.VerticesCount(), std::numeric_limits<int>::max());
    std::queue<int> vertices;
    vertices.push(0);
    level[0] = 0;
    while (!vertices.empty()) {
        int current = vertices.front();
        vertices.pop();
        std::vector<std::pair<int, int>> next;
        graph.GetNextVertices(current, next);
        for (std::pair<int, int> edge: next) {
            int to = edge.second;
            if (level[to] > level[current] + 1) {
                level[to] = level[current] + 1;
                vertices.push(to);
            }
        }
    }
    CMatrixGraph auxGraph(graph.VerticesCount());
    for(int first=0; first< graph.VerticesCount(); ++first){
        std::vector<std::pair<int,int>> next;
        graph.GetNextVertices(first, next);
        for(std::pair<int, int> second: next){
            int to = second.second;
            if (level[to] == level[first] + 1){
                auxGraph.AddEdge(first, to, second.first);
            }
        }
    }

    return auxGraph;

}

int dfs(int from, int curFlow, std::vector<int> &firstExistingSon, const CMatrixGraph &graph, CMatrixGraph &flow) {
    if (!curFlow) {
        return 0;
    }
    if (from == graph.VerticesCount() - 1) {
        return curFlow;
    }
    std::vector<std::pair<int, int> > next;
    graph.GetNextVertices(from, next);
    for (int i = firstExistingSon[from]; i < next.size(); ++i) {
        std::pair<int, int> vertex = next[i];
        int givenFlow = dfs(vertex.second, std::min(curFlow, vertex.first - flow.GetEdgeLength(from, vertex.second)),
                            firstExistingSon, graph, flow);
        if (!givenFlow) {
            ++firstExistingSon[from];
            continue;
        }
        flow.ChangeEdgeLength(from, vertex.second, flow.GetEdgeLength(from, vertex.second) + givenFlow);
        flow.ChangeEdgeLength(vertex.second, from, flow.GetEdgeLength(vertex.second, from) - givenFlow);
        return givenFlow;
    }
    return 0;
}


int GetBlockingFlow(CMatrixGraph &graph, CMatrixGraph &blockingFlow) {
    std::vector<int> firstExistingSon(graph.VerticesCount());
    int flowThroughBlockingGraph = 0;
    int givenFlow = 0;
    do {
        givenFlow = dfs(0, std::numeric_limits<int>::max(), firstExistingSon, graph, blockingFlow);
        flowThroughBlockingGraph += givenFlow;
    } while (givenFlow > 0);
    return flowThroughBlockingGraph;

}

int CalculateMaxFlow(CMatrixGraph &graph) {
    CMatrixGraph flow(graph.VerticesCount());
    int answer = 0;
    while (true) {
        CMatrixGraph resGraph = GetResidualNetwork(graph, flow);
        CMatrixGraph auxGraph = GetAuxiliaryNetwork(resGraph);
        CMatrixGraph blockingFlow(graph.VerticesCount());
        int flowProfit = GetBlockingFlow(auxGraph, blockingFlow);

        if (!flowProfit) {
            break;
        }
        answer += flowProfit;
        for (int first = 0; first < graph.VerticesCount(); ++first) {
            for (int second = 0; second < graph.VerticesCount(); ++second) {
                flow.ChangeEdgeLength(first, second,
                                      flow.GetEdgeLength(first, second) + blockingFlow.GetEdgeLength(first, second));

            }
        }
    }
    return answer;


}

int main() {
    unsigned int n, m;
    std::cin >> n >> m;
    CMatrixGraph graph(n);
    FillGraph(graph, m);
    std::cout << CalculateMaxFlow(graph);

}