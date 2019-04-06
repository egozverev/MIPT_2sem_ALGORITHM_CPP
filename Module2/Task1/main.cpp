#include <iostream>
#include "list_graph.h"
#include <queue>
#include <algorithm>
#include <limits.h>
#include <functional>
using std::priority_queue;
using std::make_pair;


int Dijkstra(CListGraph &graph, int from, int to){
    vector<int> distances(graph.VerticesCount(), INT_MAX);
    priority_queue<pair<int, int>, std::vector<pair<int, int>>, std::greater<> > heap; // pair<distance, node>
    //std::greater используется с пустым шаблоном - он умный и сам поймёт, что нужно подставить.
    //Если вручную ставить, будет clang-tidy.
    distances[from]=0;
    heap.push(make_pair(0, from));
    while(!heap.empty()){
        pair<int,int> current = heap.top();
        heap.pop();
        int curNode= current.second;
        if(distances[curNode]!= current.first){
            continue;
        } // Тут фигня в том, что может несколько раз одна вершина попасть в очередь - дистанция в ней может
        // быть уже устаревшейй. В то же время массив distances всегда поддерживает актуальную инфу.
        // Просто проверяем, что если мы наткнулись на такой случай - скипаем.
        vector<pair<int,int>> next;
        graph.GetOutcomingEdges(curNode,next);
        for(pair<int,int> vertex: next){
            if(distances[vertex.first]> distances[curNode] + vertex.second){
                distances[vertex.first]= distances[curNode] + vertex.second;
                heap.push(make_pair(distances[vertex.first], vertex.first));
            }
        }
    }
    return distances[to];
}
int main() {
    int n, m;
    std::cin >> n >> m;
    CListGraph graph(n);
    for (int i = 0; i < m; ++i) {
        int from, to, length;
        std::cin >> from >> to >> length;
        graph.AddEdge(from, pair<int, int>(to, length));
        graph.AddEdge(to, pair<int, int>(from, length));
    }
    int from, to;
    std::cin>>from>>to;
    std::cout<< Dijkstra(graph, from, to);

}