#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
class SparseTable{
public:
    explicit SparseTable(const std::vector<int>& values);
    int GetFirstStat(int elem, int power);
    int GetSecondStat(int elem, int power);
private:
    std::vector<std::vector<int> > tableFirst;
    std::vector<std::vector<int> > tableSecond;

};

SparseTable::SparseTable(const std::vector<int>& values){
    tableFirst.clear();
    int power = 1;
    std::vector<int> curLevel = values;
    while(!curLevel.empty()){
        tableFirst.push_back(curLevel);
        if(power >= curLevel.size()){
            break;
        }
        std::vector<int> nextLevel;
        nextLevel.reserve(curLevel.size() - power);
        for(int j=0; j < curLevel.size() - power; ++j){
            nextLevel.push_back(std::min(curLevel[j], curLevel[j+power]));
        }
        curLevel = nextLevel;
        power *= 2;
    }

    // теперь построение для второй пор. статистики
    curLevel = tableFirst.front();
    std::vector<int> nextLevel;
    nextLevel.reserve(curLevel.size()-1);
    for(int i=0; i < curLevel.size() -1;++i){
        nextLevel.push_back(std::max(curLevel[i], curLevel[i+1]));
    }
    tableSecond.emplace_back(std::vector<int>(0)); // просто пустой вектор - это таблица вторых пор. статистик
    curLevel = nextLevel;
    int prevLayer = 1;
    while(!curLevel.empty()){
        tableSecond.push_back(curLevel);
        int power = pow(2, prevLayer);
        if(power >= curLevel.size()){
            break;
        }

        std::vector<int> nextLevel;
        nextLevel.reserve(curLevel.size() - power);
        for(int j=0; j < curLevel.size() - power; ++j){
            std::vector<int> variants;
            variants.push_back(curLevel[j]);
            variants.push_back( curLevel[j+power]);
            variants.push_back(tableFirst[prevLayer][j]);
            variants.push_back(tableFirst[prevLayer][j+power]);
            std::sort(variants.begin(), variants.end());
            for(int i=1; i <= variants.size(); ++i){
                if(variants[i] != variants.front()){
                    nextLevel.push_back(variants[i]);
                    break;
                }
            }
        }
        curLevel = nextLevel;
        ++ prevLayer;

    }
}
int SparseTable::GetFirstStat(int elem, int power){
    return tableFirst[elem][power];
}
int SparseTable::GetSecondStat(int elem, int power){
    return tableSecond[elem][power];
}

void FindSecondOrderStat(std::vector<int>& values, int requestNum){
    SparseTable sparseTable(values);
    for(int i=0; i < requestNum; ++i){
        int first, last;
        std::cin >> first >> last;
        --first;
        --last;
        if(first == last - 1){
            std::cout << std::max(values[first], values[last]) << std::endl;
            continue;
        }
        int power = 0;
        while(first + pow(2, power) <= last){
            ++ power;
        }
        -- power;
        std::vector<int> variants;
        variants.push_back(sparseTable.GetFirstStat(power, first));
        variants.push_back(sparseTable.GetFirstStat(power,last +1 - static_cast<int>(pow(2, power))));
        variants.push_back(sparseTable.GetSecondStat(power, first));
        variants.push_back(sparseTable.GetSecondStat(power,last +1 - static_cast<int>(pow(2, power))));
        std::sort(variants.begin(), variants.end());
        for(int j = 1; j < variants.size(); ++j){
            if(variants[j] != variants.front()){
                std::cout << variants[j] << std::endl;
                break;
            }
        }


    }
}
int main() {
    unsigned int n, m;
    std::cin >> n >> m;
    std::vector<int> values;
    values.reserve(n);
    for(int i=0; i< n; ++i){
        int x;
        std::cin >> x;
        values.push_back(x);
    }
    FindSecondOrderStat(values, m);


}