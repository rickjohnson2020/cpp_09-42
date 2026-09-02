#ifndef BITCOIN_EXCHANGE_H
#define BITCOIN_EXCHANGE_H

#include <iostream>
#include <map>
#include <string>

class BitcoinExchange
{
private:
	std::map<std::string, double> _db;

	void processLine(const std::string& line) const;
	double getRate(const std::string& date) const;

public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange& operator=(const BitcoinExchange& other);
	~BitcoinExchange();

	bool loadDatabase(const std::string& path);
	void processFile(const std::string& path) const;

	// bool isValidDate(const std::string& data);
	// bool isValidValue(const std::string& value);

	// double getRate(const std::string& data);
};

#endif