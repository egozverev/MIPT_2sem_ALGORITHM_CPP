#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
using std::string;
using std::queue;
using std::unordered_map;

struct Node {
public:
    Node():movement('N'), parent("0"){}
    Node(char move, string parent_) : movement(move), parent(parent_) {
    }

    char movement;  // U/D/R/L/N
    string parent;
};

string moveBone(string str, int position, char where) {
    if (where == 'L') {
        std::swap(str[position], str[position - 1]);
    } else if (where == 'R') {
        std::swap(str[position], str[position + 1]);
    } else if (where == 'D') {
        std::swap(str[position], str[position + 3]);
    } else {
        std::swap(str[position], str[position - 3]);
    }
    return str;
}

bool EightGame(string basicPosition, string &ans, const unsigned int size) {
    int inversionNum = 0;
    for (int first = 0; first < size; ++first) {
        for (int second = first + 1; second < size; ++second) {
            if(basicPosition[first]=='0' || basicPosition[second]=='0'){
                continue;
            }
            if (basicPosition[first] > basicPosition[second]) {
                ++inversionNum;
            }
        }
    }
    if (inversionNum % 2) {
        return false;
    }
    unordered_map<string, Node> nodeMap;
    nodeMap.emplace(basicPosition, Node('N', "0"));
    queue<string> nodeQueue;
    nodeQueue.push(basicPosition);
    while (!nodeQueue.empty()) { // Он когда-то точно остановится, т.к. чётность к-ва беспорядков совпадает
        string currentPosition = nodeQueue.front();
        nodeQueue.pop();
        if (currentPosition == "123456780") {
            string reversedAns;
            Node currentNode = nodeMap[currentPosition];
            while (currentNode.parent!= "0") {
                reversedAns.push_back(currentNode.movement);
                currentNode = nodeMap[currentNode.parent];
            }
            while (!reversedAns.empty()) {
                ans.push_back(reversedAns.back());
                reversedAns.pop_back();
            }
            return true;
        }
        Node currentNode = nodeMap[currentPosition];
        int zeroPosition = currentPosition.find('0');
        if (zeroPosition % 3 != 2) {
            string newPosition = moveBone(currentPosition, zeroPosition, 'R');
            if(! nodeMap.count(newPosition)){
                nodeMap.emplace(newPosition, Node('R', currentPosition));
                nodeQueue.push(newPosition);
            }
        }
        if (zeroPosition % 3 != 0 ){
            string newPosition = moveBone(currentPosition, zeroPosition, 'L');
            if(! nodeMap.count(newPosition)){
                nodeMap.emplace(newPosition, Node('L', currentPosition));
                nodeQueue.push(newPosition);
            }
        }
        if (zeroPosition > 2) {
            string newPosition = moveBone(currentPosition, zeroPosition, 'U');
            if(! nodeMap.count(newPosition)){
                nodeMap.emplace(newPosition, Node('U', currentPosition));
                nodeQueue.push(newPosition);
            }
        }
        if (zeroPosition < 6) {
            string newPosition = moveBone(currentPosition, zeroPosition, 'D');
            if(! nodeMap.count(newPosition)){
                nodeMap.emplace(newPosition, Node('D', currentPosition));
                nodeQueue.push(newPosition);
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
    if (EightGame(firstPosition, answer, size)) {
        std::cout << answer.size() << "\n";
        std::cout << answer;
    } else {
        std::cout << -1;
    }

}