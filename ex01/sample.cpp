#include <iostream>
#include <string>
#include <sstream>

int main() {
    // int n;
    // std::cin >> n;

    // std::cout << n;

    std::string s = "10 20 30";
    std::istringstream iss(s);
    int a, b, c;
    iss >> a >> b >> c;

    return 0;
}