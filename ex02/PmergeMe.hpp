#ifndef PMERGEME_H
#define PMERGEME_H

#include <cstddef>
#include <deque>
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
	std::deque<int> _deq;
	double _vecTime;
	double _deqTime;

	bool containsDuplicate(int value) const;

	std::vector<Pair> makeVecPairs(const std::vector<int>& input,
			bool& hasStraggler, int& straggler);
	std::vector<int> fordJohnsonVec(const std::vector<int>& input);
	std::vector<size_t> makeJacobsthalOrderVec(size_t maxIndex);
	void binaryInsertVec(std::vector<int>& chain, const Pending& p);

	std::deque<Pair> makeDeqPairs(const std::deque<int>& input,
			bool& hasStraggler, int& straggler);
	std::deque<int> fordJohnsonDeq(const std::deque<int>& input);
	std::deque<size_t> makeJacobsthalOrderDeq(size_t maxIndex);
	void binaryInsertDeq(std::deque<int>& chain, const Pending& p);

public:
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();
	
	void parse(int ac, char** av);
	void sortVector();
	void sortDeque();
	void printBefore() const;
	void printAfter() const;
	void printTimes() const;
};

#endif