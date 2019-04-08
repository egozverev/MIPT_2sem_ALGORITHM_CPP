#include <algorithm>
#include <iostream>
#include <vector>

void InputSquareMatrix(std::vector<std::vector<int>> &matrix) {
    for (size_t first = 0; first < matrix.size(); ++first) {
        for (size_t second = 0; second < matrix.size(); ++second) {
            std::cin >> matrix[first][second];
        }
    }
}

void CalculateShortestPaths(std::vector<std::vector<int>> &adjacencyMatrix) {
    for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
        for (size_t first = 0; first < adjacencyMatrix.size(); ++first) {
            for (size_t second = 0; second < adjacencyMatrix.size(); ++second) {
                int oldPath = adjacencyMatrix[first][second];
                int newPath = adjacencyMatrix[first][i] + adjacencyMatrix[i][second];
                adjacencyMatrix[first][second] = std::min(oldPath, newPath);
            }
        }
    }
}

void PrintMatrix(const std::vector<std::vector<int>> &matrix) {
    for (auto first: matrix) {
        for (auto second: first) {
            std::cout << second << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    unsigned int n;
    std::cin >> n;
    std::vector<std::vector<int>> adjacencyMatrix(n, std::vector<int>(n));
    InputSquareMatrix(adjacencyMatrix);
    CalculateShortestPaths(adjacencyMatrix);
    PrintMatrix(adjacencyMatrix);
}
