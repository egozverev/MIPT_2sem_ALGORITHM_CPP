#include <algorithm>
#include <bitset>
#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <functional>
using namespace std;
enum Directions {
    Left = 'L',
    Right = 'R',
    Down = 'D',
    Up = 'U',
    None = 'N'
};

bitset<64> EncodeVectorToBitset(vector<int>& vec){
    bitset<64> encoded;
    int i=0;
    for(int number: vec){
        for(int j=3; j>=0; --j){
            encoded[i+j] = number % 2;
            number /= 2;
        }
        i += 4;
    }
    return encoded;
}

vector<int> DecodeVectorFromBitset(bitset<64>& encoded){
    vector<int> decoded;
    int i=0;
    bitset<4> cur;
    while(i<64){
        cur[3-i%4] = encoded[i];
        if(i%4==3){
            decoded.push_back(static_cast<int>(cur.to_ulong()));
        }
        ++i;

    }
    return decoded;
}

class Node {
public:
    Node():movement(None), parent(nullptr){}
    Node(Directions move,Node* parent_,bitset<64> pos) : movement(move), parent(parent_), position(pos) {
    }
    Node(Directions move, Node* parent_, vector<int> pos ): movement(move), parent(parent_){
        position = EncodeVectorToBitset(pos);
    }
    bitset<64> position;
    Directions movement; // U/D/R/L/N
    Node* parent;
    int distanceFromRoot = 0;

    bool CanBeSolved();
    Node CalculateMovedNode(Directions where);
    int FindZeroPosition();
    int Heuristics();
};
int Node::FindZeroPosition(){
    vector<int> vec =DecodeVectorFromBitset(position);
    int cur =0;
    while(cur<16 && vec[cur]!= 0){
        ++cur;
    }
    return cur;
}

bool Node::CanBeSolved() {
    const unsigned int size = 16;
    int inversionNum = 0;
    vector<int> vecPos = DecodeVectorFromBitset(position);
    for (int first = 0; first < size; ++first) {
        for (int second = first + 1; second < size; ++second) {
            if (vecPos[first] == 0 || vecPos[second] == 0) {
                continue;
            }
            if (vecPos[first] > vecPos[second]) {
                ++inversionNum;
            }
        }
    }
    return (inversionNum + FindZeroPosition() / 4) % 2 == 1;

}

int Node::Heuristics() {
    const unsigned char size = 16;
    int heuristics = 0;
    vector<int> vecPos = DecodeVectorFromBitset(position);
    for (int first = 0; first < size; ++first) {
        int idealPos = !first ? size - 1 : first - 1;
        int pos = find(vecPos.begin(), vecPos.end(),first)- vecPos.begin();
        heuristics += abs(pos % 4 - idealPos % 4) + abs(pos / 4 - idealPos / 4);

    }
    for (int first = 0; first < size; ++first) {
        int idealPosFirst = !first ? size - 1 : first - 1;
        int posFirst = find(vecPos.begin(), vecPos.end(), first)-vecPos.begin();
        if (idealPosFirst / 4 == posFirst / 4) {
            for (int posSecond = (posFirst / 4) * 4; posSecond < posFirst; ++posSecond) {
                int idealPosSecond = !vecPos[posSecond] ?
                                     size - 1 : vecPos[posSecond] - 1;
                if (idealPosSecond / 4 == posSecond / 4 &&(
                        !vecPos[posSecond] || (vecPos[posFirst] &&
                                vecPos[posSecond] > vecPos[posFirst])) ){
                    heuristics += 2;
                }
            }
        }
        if (idealPosFirst % 4 == posFirst % 4) {
            for (int posSecond = posFirst % 4; posSecond < posFirst; posSecond += 4) {
                int idealPosSecond = !vecPos[posSecond] ?
                                     size - 1 : vecPos[posSecond] - 1;
                if (idealPosSecond %4 == posSecond % 4 &&(
                        !vecPos[posSecond] || (
                                vecPos[posFirst] &&
                                        vecPos[posSecond] > vecPos[posFirst])) ){
                    heuristics += 2;
                }
            }
        }
    }


    return heuristics*117/80;
}
Node Node::CalculateMovedNode(Directions where){
    vector<int> newPosition = DecodeVectorFromBitset(position);
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


class Compare {
public:
    bool operator()(pair<int, bitset<64>> first, pair<int, bitset<64>> second) {
        return first.first > second.first;
    }
};

bool FifteenGame(vector<int> firstVecPosition, string &answer) {
    bitset<64> firstPosition = EncodeVectorToBitset(firstVecPosition);
    vector<int> goalPositionVector {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
    const bitset<64> goalPositionSet = EncodeVectorToBitset(goalPositionVector);
    Node firstNode(None, nullptr, firstPosition);
    if (!firstNode.CanBeSolved()) {
        return false;
    }
    unordered_map<bitset<64>, int> distances;
    unordered_map<bitset<64>, Node> nodeMap; // Храним использованные вершины ( инфу о них )
    unordered_map<bitset<64>, bool> wasUsed;
    priority_queue<pair<int, bitset<64>>, std::vector<pair<int, bitset<64>>>, Compare> heap; // pair<distance, string>

    heap.push(make_pair(firstNode.Heuristics(), firstPosition));

    distances[firstPosition] = firstNode.Heuristics();
    nodeMap.emplace(firstPosition, firstNode);
    while (!heap.empty()) {
        pair<int, bitset<64>> current = heap.top();
        heap.pop();
        bitset<64> currentState = current.second;
        Node* currentNode = &nodeMap[currentState];
        if (currentState == goalPositionSet) {
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
        vector<pair<Directions, Directions> > possibleMoves;
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
            bitset<64> newPosition = newNode.position;
            int newDis = nodeMap[currentState].distanceFromRoot + 1 + newNode.Heuristics();
            if (!wasUsed[newPosition] && (!distances.count(newPosition) || distances[newPosition] > newDis)) {
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
    vector<int> firstPosition;
    for (int i = 0; i < size; ++i) {
        int next;
        cin >> next;
        firstPosition.push_back(next);
    }

    string answer;
    if (FifteenGame(firstPosition, answer)) {
        cout << answer.size() << "\n";
        cout << answer;
    } else {
        cout << -1;
    }


}