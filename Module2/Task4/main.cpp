#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
#include <cmath>
using std::string;
using std::unordered_map;
using std::pair;
using std::priority_queue;
using std::make_pair;
using std::cin;
using std::cout;

char ConvertIntToChar(int number) {
    if (number <= 9) {
        return static_cast<char>(number + 48); //convert num to 'num' correctly
    } else {
        return static_cast<char>(55 + number); //convert 10 to A, 11 to B,... 15 to F
    } //Типа шестнадцатеричная система кодирования
}


int ConvertCharToInt(char sym) {
    if (sym < '9') {
        return sym - 48;
    } else {
        return sym - 55;
    }
}

class Node {
public:
    Node() : movement('N'), parent("0"), state("0") {}

    Node(string pos, char move, string parent_) : movement(move), parent(parent_), state(pos) {
    }

    string state;
    char movement;  // U/D/R/L/N
    string parent;

    bool CanBeSolved();

    int distanceFromRoot = 0;


private:

    static const unsigned int size = 16;
};

bool Node::CanBeSolved() {
    int inversionNum = 0;
    for (int first = 0; first < size; ++first) {
        for (int second = first + 1; second < size; ++second) {
            if (state[first] == '0' || state[second] == '0') {
                continue;
            }
            if (state[first] > state[second]) {
                ++inversionNum;
            }
        }
    }
    return (inversionNum + state.find('0') / 4) % 2;

}

int Heuristics(string &state) {
    const unsigned char size = 16;
    int heuristics = 0;
    for (int first = 0; first < size; ++first) {
        int idealPos = !first ? size - 1 : first - 1;
        int pos = state.find(ConvertIntToChar(first));
        heuristics += abs(pos % 4 - idealPos % 4) + abs(pos / 4 - idealPos / 4);

    }
    for (int first = 0; first < size; ++first) {
        int idealPosFirst = !first ? size - 1 : first - 1;
        int posFirst = state.find(ConvertIntToChar(first));
        if (idealPosFirst / 4 == posFirst / 4) {
            for (int posSecond = (posFirst / 4) * 4; posSecond < posFirst; ++posSecond) {
                int idealPosSecond = !ConvertCharToInt(state[posSecond]) ?
                                     size - 1 : ConvertCharToInt(state[posSecond]) - 1;
                if (idealPosSecond / 4 == posSecond / 4 &&(
                        !ConvertCharToInt(state[posSecond]) || (
                                ConvertCharToInt(state[posFirst]) &&
                                state[posSecond] > state[posFirst])) ){
                    heuristics += 2;
                }
            }
        }
        if (idealPosFirst % 4 == posFirst % 4) {
            for (int posSecond = posFirst % 4; posSecond < posFirst; posSecond += 4) {
                int idealPosSecond = !ConvertCharToInt(state[posSecond]) ?
                                     size - 1 : ConvertCharToInt(state[posSecond]) - 1;
                if (idealPosSecond %4 == posSecond % 4 &&(
                        !ConvertCharToInt(state[posSecond]) || (
                                ConvertCharToInt(state[posFirst]) &&
                                state[posSecond] > state[posFirst])) ){
                    heuristics += 2;
                }
            }
        }
    }


    return heuristics*117/80;
}


string moveBone(string str, int position, char where) {
    if (where == 'L') {
        std::swap(str[position], str[position - 1]);
    } else if (where == 'R') {
        std::swap(str[position], str[position + 1]);
    } else if (where == 'D') {
        std::swap(str[position], str[position + 4]);
    } else {
        std::swap(str[position], str[position - 4]);
    }
    return str;
}

class Compare {
public:
    bool operator()(pair<int, string> first, pair<int, string> second) {
        return first > second;
    }
};

bool FifteenGame(string firstPosition, string &answer, const unsigned int size) {

    Node firstNode(firstPosition, 'N', "0");
    if (!firstNode.CanBeSolved()) {
        return false;
    }

    unordered_map<string, int> distances;
    unordered_map<string, Node> nodeMap; // Храним использованные вершины ( инфу о них )
    unordered_map<string, bool> wasUsed;
    priority_queue<pair<int, string>, std::vector<pair<int, string>>, Compare> heap; // pair<distance, string>

    heap.push(make_pair(Heuristics(firstPosition), firstPosition));

    distances[firstPosition] = Heuristics(firstPosition);
    nodeMap.emplace(firstPosition, Node(firstPosition, 'N', "0"));
    while (!heap.empty()) {
        pair<int, string> current = heap.top();
        heap.pop();
        string currentState = current.second;
        if (currentState == "123456789ABCDEF0") {
            string reversedAns;
            while (currentState != firstPosition) {
                reversedAns.push_back(nodeMap[currentState].movement);
                currentState = nodeMap[currentState].parent;
            }
            while (!reversedAns.empty()) {
                answer.push_back(reversedAns.back());
                reversedAns.pop_back();
            }
            return true;
        }

        if (wasUsed[currentState]) {
            continue;
        }
        wasUsed[currentState] = true;
        int zeroPosition = currentState.find('0');
        if (zeroPosition % 4 != 3) {
            string newPosition = moveBone(currentState, zeroPosition, 'R');
            if (!wasUsed[newPosition] && (!distances.count(newPosition) ||
                                          distances[newPosition] >
                                          nodeMap[currentState].distanceFromRoot + 1 + Heuristics(newPosition))) {
                nodeMap[newPosition] = Node(newPosition, 'L', currentState);
                int newDistance = nodeMap[currentState].distanceFromRoot + 1;
                nodeMap[newPosition].distanceFromRoot = newDistance;
                newDistance += Heuristics(newPosition);
                heap.push(make_pair(newDistance, newPosition));
            }
        }
        if (zeroPosition % 4 != 0) {
            string newPosition = moveBone(currentState, zeroPosition, 'L');
            if (!wasUsed[newPosition] && (!distances.count(newPosition) ||
                                          distances[newPosition] >
                                          nodeMap[currentState].distanceFromRoot + 1 + Heuristics(newPosition))) {
                nodeMap[newPosition] = Node(newPosition, 'R', currentState);
                int newDistance = nodeMap[currentState].distanceFromRoot + 1;
                nodeMap[newPosition].distanceFromRoot = newDistance;
                newDistance += Heuristics(newPosition);
                heap.push(make_pair(newDistance, newPosition));
            }
        }
        if (zeroPosition > 3) {
            string newPosition = moveBone(currentState, zeroPosition, 'U');
            if (!wasUsed[newPosition] && (!distances.count(newPosition) ||
                                          distances[newPosition] >
                                          nodeMap[currentState].distanceFromRoot + 1 + Heuristics(newPosition))) {
                nodeMap[newPosition] = Node(newPosition, 'D', currentState);
                int newDistance = nodeMap[currentState].distanceFromRoot + 1;
                nodeMap[newPosition].distanceFromRoot = newDistance;
                newDistance += Heuristics(newPosition);
                heap.push(make_pair(newDistance, newPosition));
            }
        }
        if (zeroPosition < 12) {
            string newPosition = moveBone(currentState, zeroPosition, 'D');
            if (!wasUsed[newPosition] && (!distances.count(newPosition) ||
                                          distances[newPosition] >
                                          nodeMap[currentState].distanceFromRoot + 1 + Heuristics(newPosition))) {
                nodeMap[newPosition] = Node(newPosition, 'U', currentState);
                int newDistance = nodeMap[currentState].distanceFromRoot + 1;
                nodeMap[newPosition].distanceFromRoot = newDistance;
                newDistance += Heuristics(newPosition);
                heap.push(make_pair(newDistance, newPosition));
            }
        }

    }
}




int main() {
    const unsigned int size = 16;
    string firstPosition;
    for (int i = 0; i < size; ++i) {
        int next;
        cin >> next;
        firstPosition.push_back(ConvertIntToChar(next));
    }

    string answer;
    if (FifteenGame(firstPosition, answer, size)) {
        cout << answer.size() << "\n";
        cout << answer;
    } else {
        cout << -1;
    }


}