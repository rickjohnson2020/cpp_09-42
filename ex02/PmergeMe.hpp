#ifndef PMERGEME_H
#define PMERGEME_H

#include <cstddef>
#include <vector>
#include <iostream>

class PmergeMe {
private:
	struct Pair {
		int low;
		int high;
	};

	struct Pending {
		int value;
		int partner;
		bool hasPartner;
	};

	std::vector<int> _vec;
	double _vecTime;

	void binaryInsertVec(std::vector<int>& chain, const Pending& p);
	std::vector<int> fordJohnsonVector(const std::vector<int>& input);
	std::vector<Pair> makeVecPairs(const std::vector<int>& input,
		bool& hasStraggler, int& straggler);
	bool containsDuplicate(int value) const;
	std::vector<size_t> makeJacobsthalOrder(size_t maxIndex);
	
public:
	void parse(int ac, char** av);
	void sortVector();
	void printBefore() const;
	void printAfter() const;
	void printTimes() const;
};


#endif



