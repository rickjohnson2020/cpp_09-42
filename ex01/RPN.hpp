#ifndef RPN_H
#define RPN_H

#include <stack>
#include <string>

class RPN {
private:
	static bool isOperator(const std::string& token);
	static void applyOperator(std::stack<int>& stack, char op);

public:
	RPN();
	~RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);

	static int calculate(const std::string& expr);
};

#endif