#include <iostream>
#include <vector>


int main() {
	std::vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);

	std::vector<int>::iterator it = v.begin();

	std::cout << *it << std::endl;

	return 0;
}
