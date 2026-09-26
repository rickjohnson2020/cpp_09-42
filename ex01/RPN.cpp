#include "./RPN.hpp"
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>

RPN::RPN() {}
RPN::~RPN() {}
RPN::RPN(const RPN& other) {
	(void)other;
}

RPN& RPN::operator=(const RPN& other) {
	(void)other;
	return *this;
}

int RPN::calculate(const std::string& expr) {
	std::stack<int> stack;
	std::istringstream iss(expr);
	std::string token;

	while (iss >> token) {
		if (isOperator(token)) {
			applyOperator(stack, token[0]);
		} else if (token.length() == 1 && std::isdigit(static_cast<unsigned char>(token[0]))) {
			stack.push(token[0] - '0');
		} else {
			throw std::runtime_error("Error");
		}
	}
	if (stack.size() != 1)
		throw std::runtime_error("Error");
	return stack.top();
}

bool RPN::isOperator(const std::string& token) {
	if (token == "+" || token == "-" || token == "*" || token == "/")
		return true;
	return false;
}

void RPN::applyOperator(std::stack<int>& stack, char op) {
	if (stack.size() < 2)
		throw std::runtime_error("Error");

	int right = stack.top();
	if (op == '/' && right == 0)
		throw std::runtime_error("Error");

	stack.pop();
	int left = stack.top();
	stack.pop();

	if (op == '+')
		stack.push(left + right);
	else if (op == '-')
		stack.push(left - right);
	else if (op == '*')
		stack.push(left * right);
	else if (op == '/')
		stack.push(left / right);
}