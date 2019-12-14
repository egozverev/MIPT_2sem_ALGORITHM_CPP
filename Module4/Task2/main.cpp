#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct Node {
    int maxSequence;
    int maxPrefix;
    int maxSuffix;
    int segmentLength;
};

class SegmentTree {
public:
    explicit SegmentTree(std::vector<int> &values);

    Node GetNode(int index);

private:
    std::vector<Node> treeVector;
};

Node MergeNodes(Node &left, Node &right) {
    Node newNode;
    newNode.maxSequence = std::max(left.maxSequence,
                                   std::max(right.maxSequence, left.maxSuffix + right.maxPrefix));
    if (left.maxPrefix == left.segmentLength) {
        newNode.maxPrefix = left.segmentLength + right.maxPrefix;
    } else {
        newNode.maxPrefix = left.maxPrefix;
    }
    if (right.maxSuffix == right.segmentLength) {
        newNode.maxSuffix = right.segmentLength + left.maxSuffix;
    } else {
        newNode.maxSuffix = right.maxSuffix;
    }
    newNode.segmentLength = left.segmentLength + right.segmentLength;
    return newNode;
}

SegmentTree::SegmentTree(std::vector<int> &values) {

    int powerOfTwo = 1;
    while (powerOfTwo < values.size()) {
        powerOfTwo *= 2;
    }
    treeVector = std::vector<Node>(powerOfTwo * 2 - 1);
    for (int i = powerOfTwo - 1; i < powerOfTwo - 1 + values.size(); ++i) {
        treeVector[i].maxSequence = values[i - (powerOfTwo - 1)];
        treeVector[i].maxPrefix = values[i - (powerOfTwo - 1)];
        treeVector[i].maxSuffix = values[i - (powerOfTwo - 1)];
        treeVector[i].segmentLength = 1;

    }
    for (int i = powerOfTwo - 1 + values.size(); i < treeVector.size(); ++i) {
        treeVector[i].maxSequence = 0;
        treeVector[i].maxPrefix = 0;
        treeVector[i].maxSuffix = 0;
        treeVector[i].segmentLength = 1; // Фиктивные нолики, очев. , не повлияют на решение задачи.

    }
    powerOfTwo /= 2;
    while (powerOfTwo > 0) {
        for (int i = powerOfTwo - 1; i < powerOfTwo * 2 - 1; ++i) {
            Node &left = treeVector[2 * i + 1];
            Node &right = treeVector[2 * i + 2];
            treeVector[i] = MergeNodes(left, right);
        }
        powerOfTwo /= 2;
    }


}

Node SegmentTree::GetNode(int index) {
    return treeVector[index];
}

int GetParentIndex(int index) {
    return (index - 1) / 2;
}
Node GetResultNode(SegmentTree tree, int left, int right, int root){
    //предполагается, что left и right для этого сегмента
    Node rootNode = tree.GetNode(root);
    if ( rootNode.segmentLength == 1){
        return rootNode;
    }
    int mid = rootNode.segmentLength / 2 - 1 ;
    if(right <= mid){
        return GetResultNode(tree, left, right, root * 2 + 1);
    }
    else if (left > mid ){
        return GetResultNode(tree, left - mid - 1, right - mid - 1, root * 2 + 2);
    } else {
        Node leftPart = GetResultNode(tree, left, mid, root * 2 + 1);
        Node rightPart = GetResultNode(tree, 0 , right - mid - 1 , root * 2 + 2);
        return MergeNodes(leftPart, rightPart);
    }
}
void FindSequenceLength(std::vector<int> values, int requestNum) {
    SegmentTree tree(values);
    for (int k = 0; k < requestNum; ++k) {
        int left, right;
        std::cin >> left >> right;
        if (left == right) {
            std::cout << values[left] << std::endl;
            continue;
        }
        std::cout << GetResultNode(tree, left, right, 0).maxSequence << std::endl;

    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> vec;
    vec.reserve(n);
    for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        vec.push_back(x);
    }
    FindSequenceLength(vec, m);
}