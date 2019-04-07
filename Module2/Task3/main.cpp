#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
using std::string;
using std::queue;
using std::unordered_map;
using std::vector;
enum Colors {
    Left = 'L',
    Right = 'R',
    Down = 'D',
    Up = 'U',
    None = 'N'
};
class Node {
public:
    Node():movement(None), parent(nullptr), position("0"){}
    Node(Colors move,Node* parent_,string&& pos) : movement(move), parent(parent_), position(pos) {
    }
    string position;
    Colors movement; // U/D/R/L/N
    Node* parent;
    Node CalculateMovedNode(Colors where);
    int FindZeroPosition();
};
int Node::FindZeroPosition(){
    return position.find('0');
}
Node Node::CalculateMovedNode(Colors where){
    string newPosition = position;
    int pos = FindZeroPosition();
    if (where == Left) {
        std::swap(newPosition[pos], newPosition[pos - 1]);
    } else if (where == Right) {
        std::swap(newPosition[pos], newPosition[pos + 1]);
    } else if (where == Down) {
        std::swap(newPosition[pos], newPosition[pos + 3]);
    } else if (where == Up){
        std::swap(newPosition[pos], newPosition[pos - 3]);
    }
    return Node(where, this, std::move(newPosition));
}
bool FindPermutationParity(const string& str){ //чётность перестаноки
    int inversionNum = 0;
    for (size_t first = 0; first < str.size(); ++first) {
        for (size_t second = first + 1; second < str.size(); ++second) {
            if(str[first]=='0' || str[second]=='0'){
                continue;
            }
            if (str[first] > str[second]) {
                ++inversionNum;
            }
        }
    }
    return static_cast<bool>(inversionNum % 2);
}
bool SolveEightGame(string &basicPosition, string &ans) {
    //не использую const string&, т.к. поменяется при std::move
    if( FindPermutationParity(basicPosition)){
        return false;
    }
    unordered_map<string, Node> nodeMap;
    queue<Node> nodeQueue;        //Node&!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    nodeMap.emplace(basicPosition, Node(None, nullptr, std::move(basicPosition)));
    nodeQueue.push(nodeMap[basicPosition]);
    while (!nodeQueue.empty()) { // Он когда-то точно остановится, т.к. чётность к-ва беспорядков совпадает
        Node currentNode = std::move(nodeQueue.front());
        string curPos = currentNode.position;
        nodeQueue.pop();
        if (currentNode.position == "123456780") {
            string reversedAns;
            while (currentNode.parent) {
                reversedAns.push_back(currentNode.movement);
                currentNode = nodeMap[currentNode.parent->position];
            }
            while (!reversedAns.empty()) {
                ans.push_back(reversedAns.back());
                reversedAns.pop_back();
            }
            return true;
        }
        int zeroPosition = currentNode.FindZeroPosition();
        vector<Colors> possibleMoves;
        if (zeroPosition % 3 != 2){
            possibleMoves.push_back(Right);
        }
        if (zeroPosition % 3 != 0){
            possibleMoves.push_back(Left);
        }
        if (zeroPosition > 2){
            possibleMoves.push_back(Up);
        }
        if (zeroPosition < 6){
            possibleMoves.push_back(Down);
        }
        for(Colors color: possibleMoves){
            Node newNode = nodeMap[curPos].CalculateMovedNode(color);
            string newPosition = newNode.position;
            if(nodeMap.find(newPosition) == nodeMap.end()){
                nodeQueue.push(newNode);
                nodeMap.emplace(newPosition, newNode);
            }
        }
    }
}


int main() {
    const unsigned int size = 9;
    string firstPosition;
    for (int i = 0; i < size; ++i) {
        char next;
        std::cin >> next;
        firstPosition.push_back(next);
    }
    string answer;
    if (SolveEightGame(firstPosition, answer)) {
        std::cout << answer.size() << std::endl;
        std::cout << answer;
    } else {
        std::cout << -1;
    }

}