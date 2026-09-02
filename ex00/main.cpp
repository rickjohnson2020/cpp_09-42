// #include "./BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1]);
    if (!ifs) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }

    // 1行読む → trim → | で分割 → 日付を検証 → 値を検証 → DB を検索 → 出力
    std::string line;
    while (std::getline(ifs, line)) {
        


    }
}