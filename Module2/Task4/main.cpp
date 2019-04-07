#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <functional>
using std::string;
using std::unordered_map;
using std::pair;
using std::priority_queue;
using std::make_pair;
using std::cin;
using std::cout;
using std::pair;
using std::vector;
enum Colors {
    Left = 'L',
    Right = 'R',
    Down = 'D',
    Up = 'U',
    None = 'N'
};
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
    Node():movement(None), parent(nullptr), position("0"){}
    Node(Colors move,Node* parent_,const string& pos) : movement(move), parent(parent_), position(pos) {
    }
    string position;
    Colors movement; // U/D/R/L/N
    Node* parent;
    int distanceFromRoot = 0;

    bool CanBeSolved();
    Node CalculateMovedNode(Colors where);
    int FindZeroPosition();
    int Heuristics();
private:
    static const unsigned int size = 16;
};
int Node::FindZeroPosition(){
    return position.find('0');
}

bool Node::CanBeSolved() {
    int inversionNum = 0;
    for (int first = 0; first < size; ++first) {
        for (int second = first + 1; second < size; ++second) {
            if (position[first] == '0' || position[second] == '0') {
                continue;
            }
            if (position[first] > position[second]) {
                ++inversionNum;
            }
        }
    }
    return (inversionNum + position.find('0') / 4) % 2;

}

int Node::Heuristics() {
    const unsigned char size = 16;
    int heuristics = 0;
    for (int first = 0; first < size; ++first) {
        int idealPos = !first ? size - 1 : first - 1;
        int pos = position.find(ConvertIntToChar(first));
        heuristics += abs(pos % 4 - idealPos % 4) + abs(pos / 4 - idealPos / 4);

    }
    for (int first = 0; first < size; ++first) {
        int idealPosFirst = !first ? size - 1 : first - 1;
        int posFirst = position.find(ConvertIntToChar(first));
        if (idealPosFirst / 4 == posFirst / 4) {
            for (int posSecond = (posFirst / 4) * 4; posSecond < posFirst; ++posSecond) {
                int idealPosSecond = !ConvertCharToInt(position[posSecond]) ?
                                     size - 1 : ConvertCharToInt(position[posSecond]) - 1;
                if (idealPosSecond / 4 == posSecond / 4 &&(
                        !ConvertCharToInt(position[posSecond]) || (
                                ConvertCharToInt(position[posFirst]) &&
                                position[posSecond] > position[posFirst])) ){
                    heuristics += 2;
                }
            }
        }
        if (idealPosFirst % 4 == posFirst % 4) {
            for (int posSecond = posFirst % 4; posSecond < posFirst; posSecond += 4) {
                int idealPosSecond = !ConvertCharToInt(position[posSecond]) ?
                                     size - 1 : ConvertCharToInt(position[posSecond]) - 1;
                if (idealPosSecond %4 == posSecond % 4 &&(
                        !ConvertCharToInt(position[posSecond]) || (
                                ConvertCharToInt(position[posFirst]) &&
                                position[posSecond] > position[posFirst])) ){
                    heuristics += 2;
                }
            }
        }
    }


    return heuristics*117/80;
}
Node Node::CalculateMovedNode(Colors where){
    string newPosition = position;
    int pos = FindZeroPosition();
    if (where == Left) {
        std::swap(newPosition[pos], newPosition[pos - 1]);
    } else if (where == Right) {
        std::swap(newPosition[pos], newPosition[pos + 1]);
    } else if (where == Down) {
        std::swap(newPosition[pos], newPosition[pos + 4]);
    } else if (where == Up){
        std::swap(newPosition[pos], newPosition[pos - 4]);
    }
    return Node(where, this, newPosition);
}




bool FifteenGame(string firstPosition, string &answer) {

    Node firstNode(None, nullptr, firstPosition);
    if (!firstNode.CanBeSolved()) {
        return false;
    }

    unordered_map<string, int> distances;
    unordered_map<string, Node> nodeMap; // Храним использованные вершины ( инфу о них )
    unordered_map<string, bool> wasUsed;
    priority_queue<pair<int, string>, std::vector<pair<int, string>>, std::greater<>> heap; // pair<distance, string>

    heap.push(make_pair(firstNode.Heuristics(), firstPosition));

    distances[firstPosition] = firstNode.Heuristics();
    nodeMap.emplace(firstPosition, firstNode);
    while (!heap.empty()) {
        pair<int, string> current = heap.top();
        heap.pop();
        string currentState = current.second;
        Node* currentNode = &nodeMap[currentState];
        if (currentState == "123456789ABCDEF0") {
            string reversedAns;
            while (currentNode->position != firstPosition) {
                reversedAns.push_back(currentNode->movement);
                currentNode = currentNode->parent;
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
        int zeroPosition = currentNode->FindZeroPosition();
        vector<pair<Colors, Colors> > possibleMoves;
        if (zeroPosition % 4 != 3){
            possibleMoves.emplace_back(make_pair(Right, Left));
        }
        if (zeroPosition % 4 != 0) {
            possibleMoves.emplace_back(make_pair(Left, Right));
        }
        if (zeroPosition > 3){
            possibleMoves.emplace_back(make_pair(Up, Down));
        }
        if (zeroPosition < 12){
            possibleMoves.emplace_back(make_pair(Down, Up));
        }
        for(auto move: possibleMoves){
            Node newNode = currentNode->CalculateMovedNode(move.first);
            string newPosition = newNode.position;
            if (!wasUsed[newPosition] && (!distances.count(newPosition) ||
                                          distances[newPosition] >
                                          nodeMap[currentState].distanceFromRoot + 1 + newNode.Heuristics())) {
                nodeMap[newPosition] = Node(move.second, currentNode , newPosition);
                int newDistance = nodeMap[currentState].distanceFromRoot + 1;
                nodeMap[newPosition].distanceFromRoot = newDistance;
                newDistance += newNode.Heuristics();
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
    if (FifteenGame(firstPosition, answer)) {
        cout << answer.size() << "\n";
        cout << answer;
    } else {
        cout << -1;
    }


}