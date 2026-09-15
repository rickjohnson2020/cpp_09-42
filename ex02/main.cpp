#include "./PmergeMe.hpp"
#include <exception>
#include <iostream>


int main(int argc, char** argv) {
	try {
		PmergeMe p;
		p.parse(argc, argv);
		p.printBefore();
		p.sortVector();
		p.sortDeque();
		p.printAfter();
		p.printTimes();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}