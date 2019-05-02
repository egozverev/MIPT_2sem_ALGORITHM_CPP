#include "list_graph.h"
#include "mst.h"

#include <iostream>
#include <cmath>
#include <random>
#include <stack>

using std::stack;

void FillGraph(CListGraph &graph, const unsigned int edgeNumber) {
    for (int i = 0; i < edgeNumber; ++i) {
        int from, to;
        double distance;
        std::cin >> from >> to >> distance;
        graph.AddEdge(from, std::make_pair(to, distance));
        graph.AddEdge(to, std::make_pair(from, distance));
    }
}


vector<int> WalkByPreOrder(CListGraph &graph) {
    //returns vector with visited vertices in pre-order
    stack<int> vertices;
    vertices.push(0);
    vector<int> order;
    vector<bool> wasUsed(graph.VerticesCount());
    while (!vertices.empty()) {
        int current = vertices.top();
        wasUsed[current] = true;
        vertices.pop();
        order.push_back(current);
        vector<pair<int, double>> next;
        graph.GetOutcomingEdges(current, next);
        for (pair<int, double> edge: next) {
            int to = edge.first;
            if (wasUsed[to]) {
                continue;
            }
            vertices.push(to);
        }
    }
    return order;

}

double findWayLength(CListGraph &graph, vector<int> way) {
    double length = 0;
    for (int i = 0; i < way.size() - 1; ++i) {
        length += graph.FindEdgeLength(way[i], way[i + 1]);
    }
    return length;
}

vector<int> FindApproximateTravellingSalesman(CListGraph &graph) {
    CListGraph mstGraph = FindMST(graph);
    vector<int> answer = WalkByPreOrder(mstGraph);
    answer.push_back(0); // формально обход должен завершиться в нуле - цикл же!
    return answer;
}

double FindIdealTravellingSalesman(CListGraph &graph, vector<int> &permute, int left, int right) {
    // O(n!) !!! Stupid solution
    double curSolution = std::numeric_limits<int>::max();
    if (left == right) {
        permute.push_back(0); // не забываем про последний элемент
        curSolution = findWayLength(graph, permute);
        permute.pop_back();
    }
    else{
        for (int i = left; i <= right; ++i) {
            std::swap(permute[i], permute[left]);
            curSolution = std::min(curSolution, FindIdealTravellingSalesman(graph, permute, left + 1, right));
            std::swap(permute[i], permute[left]);

        }
    }

    return curSolution;


}

double CalcEuclideanDistance(pair<double, double> firstPoint, pair<double, double> secondPoint) {
    return sqrt(pow((firstPoint.first - secondPoint.first), 2) + pow((firstPoint.second - secondPoint.second), 2));
}

CListGraph GenerateRandomGraph(int vertNum) {
    std::random_device rd;
    std::mt19937_64 generator(rd());
    //std::default_random_engine generator;
    std::normal_distribution<> distribution(5, 1);
    CListGraph graph(vertNum);
    vector<pair<double, double>> points;
    for (int i = 0; i < vertNum; ++i) {
        points.emplace_back(std::make_pair(distribution(generator), distribution(generator)));
    }
    for (int first = 0; first < vertNum; ++first) {
        for (int second = first + 1; second < vertNum; ++second) {
            double distance = CalcEuclideanDistance(points[first], points[second]);
            graph.AddEdge(first, std::make_pair(second, distance));
            graph.AddEdge(second, std::make_pair(first, distance));
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
        vector<double> idealDistances; //отклонениея
        vector<double> approximateDistances;
        for (int j = 0; j < numberOfGraphs; ++j) {
            CListGraph graph = GenerateRandomGraph(vertNum);
            vector<int> permute(graph.VerticesCount());
            for (int i = 0; i < permute.size(); ++i) {
                permute[i] = i;
            }
            double idealResult = FindIdealTravellingSalesman(graph, permute, 1, graph.VerticesCount() - 1);
            vector<int> approximateWay = FindApproximateTravellingSalesman(graph);
            double approximateResult = findWayLength(graph, approximateWay);
            idealDistances.push_back(idealResult);
            approximateDistances.push_back(approximateResult);

        }
        std::cout << "Number of vertices = " << vertNum << std::endl;
        double idealMean = 0;
        double idealMeanSquare = 0;
        for (double elem: idealDistances) {
            idealMean += elem;
        }
        idealMean /= numberOfGraphs;
        for(double elem: idealDistances){
            idealMeanSquare += pow( (idealMean - elem), 2);
        }
        idealMeanSquare = sqrt(idealMeanSquare / numberOfGraphs);
        std::cout << "Mean(Ideal): " << idealMean << std::endl;
        std::cout << "Mean Square (Ideal): " << idealMeanSquare << std::endl;

        double approximateMean = 0;
        double approximateMeanSquare = 0;
        for (double elem: approximateDistances) {
            approximateMean += elem;
        }
        approximateMean /= numberOfGraphs;
        for(double elem: approximateDistances){
            approximateMeanSquare += pow( (approximateMean - elem), 2);
        }
        approximateMeanSquare = sqrt(approximateMeanSquare / numberOfGraphs);
        std::cout << "Mean(Approximate): " << approximateMean << std::endl;
        std::cout << "Mean Square (Approximate): " << approximateMeanSquare << std::endl;

    }


}

int main() {

    CheckEfficiency();


}