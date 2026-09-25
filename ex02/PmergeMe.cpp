#include "./PmergeMe.hpp"
#include <algorithm>
#include <cerrno>
#include <cfloat>
#include <cstddef>
#include <ctime>
#include <deque>
#include <stdexcept>
#include <climits>
#include <vector>

PmergeMe::PmergeMe() : _vecTime(0.0), _deqTime(0.0) {}

PmergeMe::PmergeMe(const PmergeMe& other) : _vec(other._vec),
		_vecTime(other._vecTime), _deq(other._deq), _deqTime(other._deqTime) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	if (this != &other) {
		_vec = other._vec;
		_deq = other._deq;
		_vecTime = other._vecTime;
		_deqTime = other._deqTime;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::parse(int ac, char** av) {
	if (ac < 2)
		throw std::runtime_error("Error");
	
	for (int i = 1; i < ac; ++i) {
		char* end;
		errno = 0;
		long v = std::strtol(av[i], &end, 10);
		if (end == av[i] || *end != '\0')
			throw std::runtime_error("Error");
		if (errno == ERANGE)
			throw std::runtime_error("Error");
		if (v > INT_MAX || v < 1)
			throw std::runtime_error("Error");

		if (containsDuplicate(static_cast<int>(v)))
			throw std::runtime_error("Error");

		_vec.push_back(static_cast<int>(v));
		_deq.push_back(static_cast<int>(v));
	}
}

void PmergeMe::sortVector() {
	clock_t start = clock();
	_vec = fordJohnsonVec(_vec);
	clock_t end = clock();
	_vecTime = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * 1000000.0;
}

void PmergeMe::sortDeque() {
	clock_t start = clock();
	_deq = fordJohnsonDeq(_deq);
	clock_t end = clock();
	_deqTime = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * 1000000.0;
}

void PmergeMe::printBefore() const {
	std::cout << "Before: ";
	for (size_t i = 0; i < _vec.size(); ++i) {
		std::cout << _vec[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::printAfter() const {
	std::cout << "After: ";
	for (size_t i = 0; i < _vec.size(); ++i) {
		std::cout << _vec[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::printTimes() const {
	std::cout << "Time to process a range of " << _vec.size() <<
			" elements with std::vector: " << _vecTime << " us" << std::endl;
	
	std::cout << "Time to process a range of " << _deq.size() <<
			" elements with std::deque: " << _deqTime << " us" << std::endl;
}

bool PmergeMe::containsDuplicate(int value) const {
	for (size_t i = 0; i < _vec.size(); ++i) {
		if (_vec[i] == value)
			return true;
	}
	return false;
}

std::vector<int> PmergeMe::fordJohnsonVec(const std::vector<int>& input) {
	if (input.size() <= 1)
		return input;

	bool hasStraggler = false;
	int straggler = 0;
	std::vector<Pair> pairs = makeVecPairs(input, hasStraggler, straggler);

	//highだけ取り出して再帰でソートする
	std::vector<int> highs;
	for (size_t i = 0; i < pairs.size(); ++i)
		highs.push_back(pairs[i].high);
	
	std::vector<int> sortedHighs = fordJohnsonVec(highs);

	//sortedHighsの順番にpairを並べ直す。
	std::vector<Pair> reorderedPairs;
	for (size_t i = 0; i < sortedHighs.size(); ++i) {
		for (size_t j = 0; j < pairs.size(); ++j) {
			if (pairs[j].high == sortedHighs[i]) {
				reorderedPairs.push_back(pairs[j]);
				break;
			}
		}
	}

	//main chainを作る(sortedHighsにreorderedPairsのhighが一番小さいペアのlowを入れる)
	std::vector<int> mainChain = sortedHighs;
	mainChain.insert(mainChain.begin(), reorderedPairs[0].low);

	//pendingに格納
	std::vector<Pending> pending;
	for (size_t i = 1; i < reorderedPairs.size(); ++i) {
		Pending p;
		p.value = reorderedPairs[i].low;
		p.partner = reorderedPairs[i].high;
		p.hasPartner = true;
		pending.push_back(p);
	}
	if (hasStraggler) {
		Pending p;
		p.value = straggler;
		p.partner = -1;
		p.hasPartner = false;
		pending.push_back(p);
	}

	//Jacobsthal順を生成
	//pendingにはa1が含まれていないのでpending.size()+1 (a1はmain chainに最初から入っている。)
	std::vector<size_t> order = makeJacobsthalOrder(pending.size() + 1);
	for (size_t i = 0; i < order.size(); ++i) {
		size_t aIndex = order[i];
		size_t pendingIndex = aIndex - 2;
		Pending& p = pending[pendingIndex];

		//p.valueをmainChainにbinary insertionで入れていく
		binaryInsertVec(mainChain, p);
	}
	return mainChain;
}

std::vector<PmergeMe::Pair> PmergeMe::makeVecPairs(const std::vector<int>& input,
		bool& hasStraggler, int& straggler) {
	//2つずつ比較してペアを作る
	std::vector<Pair> pairs;
	for (size_t i = 0; i + 1 < input.size(); i += 2) {
		Pair p;
		if (input[i] < input[i + 1]) {
			p.low = input[i];
			p.high = input[i + 1];
		} else {
			p.low = input[i + 1];
			p.high = input[i];
		}
		pairs.push_back(p);
	}

	if (input.size() % 2 != 0) {
		hasStraggler = true;
		straggler = input.back();
	}
	return pairs;
}

std::vector<size_t> PmergeMe::makeJacobsthalOrder(size_t maxIndex) {
	std::vector<size_t> order;

	if (maxIndex < 2)
		return order;

	size_t prev = 1;
	size_t curr = 3;
	while (prev < maxIndex) {
		size_t high;
		if (curr < maxIndex)
			high = curr;
		else
			high = maxIndex;

		for (size_t i = high; i > prev; --i) {
			order.push_back(i);
		}
		if (curr >= maxIndex)
			break;

		size_t next = curr + 2 * prev;
		prev = curr;
		curr = next;
	}
	return order;
}

void PmergeMe::binaryInsertVec(std::vector<int>& chain, const Pending& p) {
	//p.valueをmainChainにbinary insertionで入れていく
	if (p.hasPartner) {
		std::vector<int>::iterator partnerIt =
				std::find(chain.begin(), chain.end(), p.partner);

		std::vector<int>::iterator insertPos =
				std::lower_bound(chain.begin(), partnerIt, p.value);

		chain.insert(insertPos, p.value);
	} else {
		std::vector<int>::iterator insertPos =
				std::lower_bound(chain.begin(), chain.end(), p.value);
		
		chain.insert(insertPos, p.value);
	}
}

std::deque<int> PmergeMe::fordJohnsonDeq(const std::deque<int>& input) {
	if (input.size() <= 1)
		return input;

	bool hasStraggler = false;
	int straggler = 0;
	std::deque<Pair> pairs = makeDeqPairs(input, hasStraggler, straggler);

	//highだけ取り出して再帰でソートする
	std::deque<int> highs;
	for (size_t i = 0; i < pairs.size(); ++i)
		highs.push_back(pairs[i].high);
	std::deque<int> sortedHighs = fordJohnsonDeq(highs);

	//sortedHighsの順番にpairを並べ直す
	std::deque<Pair> reorderedPairs;
	for (size_t i = 0; i < sortedHighs.size(); ++i) {
		for (size_t j = 0; j < pairs.size(); ++j) {
			if (pairs[j].high == sortedHighs[i]) {
				reorderedPairs.push_back(pairs[j]);
				break;
			}
		}
	}

	//main chainを作る(sortedHighsにreorderedPairsのhighが一番小さいペアのlowを入れる)
	std::deque<int> mainChain = sortedHighs;
	mainChain.insert(mainChain.begin(), reorderedPairs[0].low);

	//create pending
	std::deque<Pending> pending;
	for (size_t i = 1; i < reorderedPairs.size(); ++i) {
		Pending p;
		p.value = reorderedPairs[i].low;
		p.partner = reorderedPairs[i].high;
		p.hasPartner = true;
		pending.push_back(p);
	}
	if (hasStraggler) {
		Pending p;
		p.value = straggler;
		p.partner = -1;
		p.hasPartner = false;
		pending.push_back(p);
	}

	//insert pending into mainChain using binary insertion
	std::vector<size_t> insertionOrder = makeJacobsthalOrder(pending.size() + 1);
	for (size_t i = 0; i < insertionOrder.size(); ++i) {
		size_t aIndex = insertionOrder[i];
		size_t pIndex = aIndex - 2;
		Pending& p = pending[pIndex];
		binaryInsertDeq(mainChain, p);
	}

	return mainChain;
}

std::deque<PmergeMe::Pair> PmergeMe::makeDeqPairs(const std::deque<int>& input,
		bool& hasStraggler, int& straggler) {
	std::deque<Pair> pairs;
	for (size_t i = 0; i + 1 < input.size(); i += 2) {
		Pair p;
		if (input[i] < input[i + 1]) {
			p.low = input[i];
			p.high = input[i + 1];
		} else {
			p.low = input[i + 1];
			p.high = input[i];
		}
		pairs.push_back(p);
	}

	if (input.size() % 2 != 0) {
		hasStraggler = true;
		straggler = input.back();
	}
	return pairs;
}

void PmergeMe::binaryInsertDeq(std::deque<int>& chain, const Pending& p) {
	if (p.hasPartner) {
		std::deque<int>::iterator partnerIt =
				std::find(chain.begin(), chain.end(), p.partner);
		std::deque<int>::iterator insertPos =
				std::lower_bound(chain.begin(), partnerIt, p.value);
		chain.insert(insertPos, p.value);
	} else {
		std::deque<int>::iterator insertPos =
				std::lower_bound(chain.begin(), chain.end(), p.value);
		chain.insert(insertPos, p.value);
	}
}