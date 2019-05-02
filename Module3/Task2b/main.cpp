#include "matrix_graph.h"
#include "mst.h"

#include <iostream>
#include <cmath>
#include <random>
#include <stack>



void FillGraph(CMatrixGraph &graph, const unsigned int edgeNumber) {
    for (int i = 0; i < edgeNumber; ++i) {
        int from, to;
        double distance;
        std::cin >> from >> to >> distance;
        graph.AddEdge(from, to, distance);
        graph.AddEdge(to, from, distance);
    }
}


std::vector<int> WalkByPreOrder(CMatrixGraph &graph) {
    //returns vector with visited vertices in pre-order
    std::stack<int> vertices;
    vertices.push(0);
    std::vector<int> order;
    std::vector<bool> wasUsed(graph.VerticesCount());
    while (!vertices.empty()) {
        int current = vertices.top();
        vertices.pop();
        if(wasUsed[current]){
            continue;
        }
        wasUsed[current] = true;
        order.push_back(current);
        std::vector<std::pair<int, double>> next;
        graph.GetNextVertices(current, next);
        for (std::pair<int, double> edge: next) {
            int to = edge.first;
            if (wasUsed[to]) {
                continue;
            }
            vertices.push(to);
        }
    }
    return order;

}

double findWayLength(CMatrixGraph &graph, std::vector<int> way) {
    double length = 0;
    for (int i = 0; i < way.size() - 1; ++i) {
        length += graph.GetEdgeLength(way[i], way[i + 1]);
    }
    return length;
}


CMatrixGraph GetApproximateMinimalPerfectMatching(CMatrixGraph &graph, CMatrixGraph &forbiddenGraph, std::vector<int> &vertices) {
    if (vertices.empty()) {
        return CMatrixGraph(graph.VerticesCount());
    }
    std::vector<int> cycle;
    std::vector<bool> wasUsed(graph.VerticesCount());
    int cur = vertices[0];

    while (cycle.size() < vertices.size()) {
        double minWeight = std::numeric_limits<double>::max();
        int neighboursMinNumber = std::numeric_limits<int>::max();
        int nextVertex;
        for (int vertex: vertices) {
            if (vertex == cur || wasUsed[vertex]) {
                continue;
            }
            if (graph.GetEdgeLength(cur, vertex) < minWeight) {
                minWeight = graph.GetEdgeLength(cur, vertex);
                nextVertex = vertex;
            }
        }
        wasUsed[nextVertex] = true;
        cycle.push_back(nextVertex);
        cur = nextVertex;
    }
    double evenLength = 0;
    double oddLength = 0;
    for (int i = 0; i < cycle.size() - 1; ++i) {
        if (i % 2) {
            oddLength += graph.GetEdgeLength(vertices[i], vertices[i + 1]);
        } else {
            evenLength += graph.GetEdgeLength(vertices[i], vertices[i + 1]);
        }
    }
    oddLength += graph.GetEdgeLength(cycle.front(), cycle.back());
    CMatrixGraph newGraph(graph.VerticesCount());
    int firstToBeginWith = 0;
    if (oddLength < evenLength) {
        ++firstToBeginWith;
        newGraph.AddEdge(cycle.front(), cycle.back(), graph.GetEdgeLength(cycle.front(), cycle.back()));
        newGraph.AddEdge(cycle.back(), cycle.front(), graph.GetEdgeLength(cycle.front(), cycle.back()));

    }
    for (int i = firstToBeginWith; i < cycle.size() - 1; i += 2) {
        newGraph.AddEdge(cycle[i], cycle[i + 1], graph.GetEdgeLength(cycle[i], cycle[i + 1]));
        newGraph.AddEdge(cycle[i + 1], cycle[i], graph.GetEdgeLength(cycle[i], cycle[i + 1]));

    }
    return newGraph;

}

CMatrixGraph UniteGraphs(CMatrixGraph &firstGraph, CMatrixGraph &secondGraph) {
    CMatrixGraph newGraph(firstGraph.VerticesCount());
    for (int i = 0; i < firstGraph.VerticesCount(); ++i) {
        for (int j = 0; j < firstGraph.VerticesCount(); ++j) {
            newGraph.AddEdge(i, j, std::max(firstGraph.GetEdgeLength(i, j) , secondGraph.GetEdgeLength(i, j)));
        } // берём только одно из рёбер - чтобы не удваивалось понапрасну
    }
    return newGraph;
}

std::vector<int> FindChristofidesTravellingSalesman(CMatrixGraph &graph) {
    CMatrixGraph mstGraph = FindMST(graph);
    std::vector<int> oddVertices = mstGraph.GetOddVertices();
    CMatrixGraph oddSubgraph = graph.FindMinor(oddVertices);
    CMatrixGraph minimalPerfectMatching = GetApproximateMinimalPerfectMatching(oddSubgraph, mstGraph, oddVertices);
    CMatrixGraph unitedGraph = UniteGraphs(mstGraph, minimalPerfectMatching);
    std::vector<int> answer = WalkByPreOrder(unitedGraph);
    answer.push_back(answer.front());
    return answer;


}


double FindIdealTravellingSalesman(CMatrixGraph &graph, std::vector<int> &permute, int left, int right) {
    // O(n!) !!! Stupid solution
    double curSolution = std::numeric_limits<int>::max();
    if (left == right) {
        permute.push_back(0); // не забываем про последний элемент
        curSolution = findWayLength(graph, permute);
        permute.pop_back();
    } else {
        for (int i = left; i <= right; ++i) {
            std::swap(permute[i], permute[left]);
            curSolution = std::min(curSolution, FindIdealTravellingSalesman(graph, permute, left + 1, right));
            std::swap(permute[i], permute[left]);

        }
    }

    return curSolution;


}

double CalcEuclideanDistance(std::pair<double, double> firstPoint, std::pair<double, double> secondPoint) {
    return sqrt(pow((firstPoint.first - secondPoint.first), 2) + pow((firstPoint.second - secondPoint.second), 2));
}

CMatrixGraph GenerateRandomGraph(int vertNum) {
    std::random_device rd;
    std::mt19937_64 generator(rd());
    //std::default_random_engine generator;
    std::normal_distribution<> distribution(5, 1);
    CMatrixGraph graph(vertNum);
    std::vector<std::pair<double, double>> points;
    for (int i = 0; i < vertNum; ++i) {
        points.emplace_back(std::make_pair(distribution(generator), distribution(generator)));
    }
    for (int first = 0; first < vertNum; ++first) {
        for (int second = first + 1; second < vertNum; ++second) {
            double distance = CalcEuclideanDistance(points[first], points[second]);
            graph.AddEdge(first, second, distance);
            graph.AddEdge(second, first, distance);
        }
    }
    return graph;

}

void CheckEfficiency() {
    const unsigned int leftBorder = 2;
    const unsigned int rightBorder = 8;
    const unsigned int numberOfGraphs = 10;
    // В каких диапазонах создаём графы и сколько для каждой итерации

    std::cout << "Assessment of the quality of the approximation" << std::endl;
    for (int vertNum = leftBorder; vertNum <= rightBorder; ++vertNum) {
        std::vector<double> idealDistances; //отклонениея
        std::vector<double> christofidesDistances;
        for (int j = 0; j < numberOfGraphs; ++j) {
            CMatrixGraph graph = GenerateRandomGraph(vertNum);
            std::vector<int> permute(graph.VerticesCount());
            for (int i = 0; i < permute.size(); ++i) {
                permute[i] = i;
            }
            double idealResult = FindIdealTravellingSalesman(graph, permute, 1, graph.VerticesCount() - 1);
            idealDistances.push_back(idealResult);
            std::vector<int> christofidesWay = FindChristofidesTravellingSalesman(graph);
            double chrisofidesResult = findWayLength(graph, christofidesWay);
            christofidesDistances.push_back(chrisofidesResult);

        }
        std::cout << "Number of vertices = " << vertNum << std::endl;
        double idealMean = 0;
        double idealMeanSquare = 0;
        for (double elem: idealDistances) {
            idealMean += elem;
        }
        idealMean /= numberOfGraphs;
        for (double elem: idealDistances) {
            idealMeanSquare += pow((idealMean - elem), 2);
        }
        idealMeanSquare = sqrt(idealMeanSquare / numberOfGraphs);
        std::cout << "Mean(Ideal): " << idealMean << std::endl;
        std::cout << "Mean Square (Ideal): " << idealMeanSquare << std::endl;

        double chrisofidesMean = 0;
        double chrisofidesMeanSquare = 0;
        for (double elem: christofidesDistances) {
            chrisofidesMean += elem;
        }
        chrisofidesMean /= numberOfGraphs;
        for (double elem: christofidesDistances) {
            chrisofidesMeanSquare += pow((chrisofidesMean - elem), 2);
        }
        chrisofidesMeanSquare = sqrt(chrisofidesMeanSquare / numberOfGraphs);
        std::cout << "Mean(Chrisofides): " << chrisofidesMean << std::endl;
        std::cout << "Mean Square (Chrisofides): " << chrisofidesMeanSquare << std::endl;

    }


}


int main() {
    CheckEfficiency();


}