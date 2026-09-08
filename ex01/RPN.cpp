#include "./RPN.hpp"
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>

RPN::RPN() {}
RPN::~RPN() {}
RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other) {
	if (this == &other)
		return *this;
	_stack = other._stack;
	return *this;
}

int RPN::calculate(const std::string& expr) {
	std::istringstream iss(expr);
	std::string token;

	while (iss >> token) {
		if (isOperator(token)) {
			if (_stack.size() < 2)
				throw std::runtime_error("Error");
			applyOperator(token[0]);
		} else if (token.length() == 1 && std::isdigit(token[0])) {
			_stack.push(token[0] - '0');
		} else {
			throw std::runtime_error("Error");
		}
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error");
	return _stack.top();
}

bool RPN::isOperator(const std::string& token) const {
	if (token == "+" || token == "-" || token == "*" || token == "/")
		return true;
	return false;
}

void RPN::applyOperator(char op) {
	int right = _stack.top();
	_stack.pop();
	int left = _stack.top();
	_stack.pop();

	if (op == '+')
		_stack.push(left + right);
	else if (op == '-')
		_stack.push(left - right);
	else if (op == '*')
		_stack.push(left * right);
	else {
		if (right == 0)
			throw std::runtime_error("Error");
		_stack.push(left / right);
	}
}