#ifndef BITCOIN_EXCHANGE_H
#define BITCOIN_EXCHANGE_H

#include <iostream>
#include <map>
#include <string>

class BitcoinExchange
{
private:
	std::map<std::string, double> _db;

public:
	BitcoinExchange();
	~BitcoinExchange();

	void loadDatabase();
	void processInput(const std::string& filename);

	bool isValidDate(const std::string& data);
	bool isValidValue(const std::string& value);

	double getRate(const std::string& data);
};

#endif