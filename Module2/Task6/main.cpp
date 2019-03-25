#include <iostream>
#include <vector>
#include <algorithm>
void InputSquareMatrix(std::vector<std::vector<int>> &matrix, int order) {
    for (int first = 0; first < order; ++first) {
        for (int second = 0; second < order; ++second) {
            std::cin >> matrix[first][second];
        }
    }
}

void Floid(std::vector<std::vector<int>> &adjacencyMatrix) {
    for (int stage = 0; stage < adjacencyMatrix.size(); ++stage) {
        for (int first = 0; first < adjacencyMatrix.size(); ++first) {
            for (int second = 0; second < adjacencyMatrix[0].size(); ++second) {
                adjacencyMatrix[first][second]=std::min(adjacencyMatrix[first][second], adjacencyMatrix[first][stage]+adjacencyMatrix[stage][second]);
            }
        }
    }
}
void PrintMatrix(std::vector<std::vector<int>> &matrix){
    for(int first=0; first< matrix.size();++first){
        for(int second=0; second<matrix[0].size(); ++second){
            std::cout<<matrix[first][second]<<" ";
        }
        std::cout<<"\n";
    }
}
int main() {
    unsigned int n;
    std::cin >> n;
    std::vector<std::vector<int>> adjacencyMatrix(n, std::vector<int>(n, 0));
    InputSquareMatrix(adjacencyMatrix, n);
    Floid(adjacencyMatrix);
    PrintMatrix(adjacencyMatrix);


}