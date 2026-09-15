#ifndef BITCOIN_EXCHANGE_H
#define BITCOIN_EXCHANGE_H

#include <exception>
#include <iostream>
#include <map>
#include <string>

class BitcoinExchange {
private:
	std::map<std::string, double> _db;

	void processLine(const std::string& line) const;
	double getRate(const std::string& date) const;

public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange& operator=(const BitcoinExchange& other);
	~BitcoinExchange();

	void loadDatabase(const std::string& path);
	void processInput(const std::string& path) const;

	class FileOpenException : public std::exception {
	public:
		virtual const char* what() const throw();
	};

	class EmptyDatabaseException : public std::exception {
	public:
		virtual const char* what() const throw();
	};

	class DateOutOfRangeException : public std::exception {
	public:
		virtual const char* what() const throw();
	};
};

#endif