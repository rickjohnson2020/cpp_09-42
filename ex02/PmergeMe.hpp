#ifndef PMERGEME_H
#define PMERGEME_H

#include <vector>
#include <iostream>

class PmergeMe {
private:
	std::vector<int> _vec;

	void parse(int ac, char** av);
	void sortVector();

	void mergeInsertVec(std::vector<int>& v);
	void binaryInsertVec(std::vector<int>& chain, int value, size_t limit);

	void printBefore() const;
	void printAfter() const;
	void printTimes() const;


public:
	void run(int ac, char** av);
};


#endif



