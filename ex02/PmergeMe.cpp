#include "./PmergeMe.hpp"
#include <limits>

void PmergeMe::parse(int ac, char** av) {
    if (ac < 1)
		std::runtime_error("Error");

    int max_int = std::numeric_limits<int>::max();
    int min_int = std::numeric_limits<int>::min();
    
    for (size_t i = 0; i < ac; ++i) {
        if (!isAllDigits(av[i]))
            std::runtime_error("Error");
        if (av[i][0] == '-')
            std::runtime_error("Error");
    }
}

static bool isAllDigits(const std::string& s) {
	for (std::string::size_type i = 0; i < s.size(); ++i) {
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return false;
	}
	return true;
}