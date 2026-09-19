#ifndef RPN_H
#define RPN_H

#include <stack>
#include <string>

class RPN {
private:
	std::stack<int> _stack;

	bool isOperator(const std::string& token) const;
	void applyOperator(char op);

public:
	RPN();
	~RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);

	int calculate(const std::string& expr);
};

#endif