#include "./RPN.hpp"
#include <exception>
#include <iostream>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Error" << std::endl;
		return 1;
	}
	try {
		RPN rpn;
		std::cout << rpn.calculate(argv[1]) << std::endl;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}