#include <cmath>
#include <iostream>
#include <vector>
#include <stack>

void CalcInOutTime(int vert, std::vector<std::vector<int>> &descendants, std::vector<std::pair<int, int>> &dfsTime,
                   unsigned int &moment) {
    dfsTime[vert].first = moment;
    ++moment;
    for (int elem: descendants[vert]) {
        CalcInOutTime(elem, descendants, dfsTime, moment);
    }
    dfsTime[vert].second = moment;
    ++moment;
}

std::vector<std::pair<int, int>> LaunchDFS(std::vector<int> &parents) {
    std::vector<std::pair<int, int>> dfsTime(parents.size()); // time of enter/leave
    std::vector<std::vector<int>> descendants(parents.size());
    for (int i = 1; i < parents.size(); ++i) {
        descendants[parents[i]].push_back(i);
    }
    unsigned int moment = 0;
    CalcInOutTime(0, descendants, dfsTime, moment);
    return dfsTime;
}

bool IsParent(int vert, int possiblePar, std::vector<std::pair<int, int>> dfsTime) {
    return (dfsTime[possiblePar].first < dfsTime[vert].first && dfsTime[vert].first < dfsTime[possiblePar].second);
}

int CalcLCA(int first, int second, std::vector<std::vector<int>> &table, std::vector<std::pair<int, int>> &dfsTime) {
    int power = table.front().size() - 1;
    if (first == second){
        return first;
    }
    if (IsParent(first, second, dfsTime)) {
        return second;
    }
    if (IsParent(second, first, dfsTime)) {
        return first;
    }
    while (power >= 0) {
        if (!IsParent(second, table[first][power], dfsTime)) {
            first = table[first][power];
        }
        --power;
    }
    return table[first][0];


}

long long ExecuteLCA(std::vector<int> &parents, int requestNum) {
    //preprocessing
    unsigned int power = 0;
    while (pow(2, power) < parents.size()) {
        ++power;
    }
    if(!power){
        return 0; // значит, только одна вершинка - только она может быть результатом запроса.
        //Дальнейшие действия бессмысленны ( создавние вектора размера 0 - ошибочка )
    }
    std::vector<std::vector<int>> table(parents.size(),
                                        std::vector<int>(power)); //<i, j> = 2^j - ый предок i-ой вершины
    for (int i = 0; i < parents.size(); ++i) {
        table[i][0] = parents[i];
    }
    for (int pow = 1; pow < power; ++pow) {
        for (int i = 0; i < parents.size(); ++i) {
            int prev = table[i][pow - 1];
            table[i][pow] = table[prev][pow - 1];
        }
    };
    std::vector<std::pair<int, int>> dfsTime = LaunchDFS(parents);
    //major part
    int firstElem, secondElem; //a1, a2
    std::cin >> firstElem >> secondElem;
    long long x, y, z; // magic numbers from the task
    std::cin >> x >> y >> z;
    int ans = CalcLCA(firstElem, secondElem, table, dfsTime);
    long long sum = ans;
    for (int i = 1; i < requestNum; ++i) {
        firstElem = (x * firstElem + y * secondElem + z) % parents.size();
        secondElem = (x * secondElem + y * firstElem + z) % parents.size();
        ans = CalcLCA((firstElem + ans) % parents.size(), secondElem, table, dfsTime);
        sum += ans;
    }
    return sum;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> parents{0};
    for (int i = 1; i < n; ++i) {
        int parent;
        std::cin >> parent;
        parents.push_back(parent);
    }
    std::cout << ExecuteLCA(parents, m);
}
