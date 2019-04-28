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
    for (int first = 0; first < graph.VerticesCount(); ++first) {
        std::vector<std::pair<int, int>> next;
        graph.GetNextVertices(first, next);
        for (std::pair<int, int> second: next) {
            int to = second.second;
            if (level[to] == level[first] + 1) {
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


void PrintGraph(CMatrixGraph &graph) {
    std::cout << "-------\n";
    for (int first = 0; first < graph.VerticesCount(); ++first) {
        for (int second = 0; second< graph.VerticesCount(); ++second){
            std::cout<<graph.GetEdgeLength(first, second)<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"======\n";
}

//Код текущей задачи

CMatrixGraph CreateBaseTeaGraph(int peopleNumber, int teaNumber) {
    // Создаёт каркас для графа по входным данным. Пустые рёбра между истоком и голубчиками
    CMatrixGraph baseGraph(peopleNumber + teaNumber + 2); // 2 фиктивные вершины
    for (int i = 0; i < teaNumber; ++i) {
        int currentSortNumber;
        std::cin >> currentSortNumber;
        baseGraph.AddEdge(peopleNumber + i + 1, peopleNumber + teaNumber + 1, currentSortNumber);
    }
    for (int i = 0; i < peopleNumber; ++i) {
        int numberOfSorts;
        std::cin >> numberOfSorts;
        for (int j = 0; j < numberOfSorts; ++j) {
            int curSort;
            std::cin >> curSort;
            baseGraph.AddEdge(1 + i, peopleNumber + curSort, std::numeric_limits<int>::max());
        }

    }
    return baseGraph;

}

bool IsTeaPartyAccepted(int numOfDays, CMatrixGraph baseGraph, int peopleNum) {
    //нужно именно копирование - на основе базового графа построится текущий.
    for (int guy = 1; guy <= peopleNum; ++guy) {
        baseGraph.AddEdge(0, guy, numOfDays);
    }
    return (CalculateMaxFlow(baseGraph) == numOfDays * peopleNum);
}

int CalculateDaysOfTeaParty() {
    int peopleNumber, teaNumber;
    std::cin >> peopleNumber >> teaNumber;
    CMatrixGraph baseGraph = CreateBaseTeaGraph(peopleNumber, teaNumber);
    const int stock = peopleNumber + teaNumber + 1; // 2 фиктивные вершины - сток и исток
    int maxNumOfDays = 0;
    for (int i = peopleNumber + 1; i < baseGraph.VerticesCount() - 1; ++i) {
        maxNumOfDays += baseGraph.GetEdgeLength(i, stock);
    }
    int first = 0;
    int last = maxNumOfDays + 1;
    while (first < last - 1) {
        int mid = last - (last - first)/2;
        if (IsTeaPartyAccepted(mid, baseGraph, peopleNumber)) {
            first = mid;
        } else {
            last = mid;
        }

    }
    return first;

}

int main() {
    std::cout << CalculateDaysOfTeaParty();
}