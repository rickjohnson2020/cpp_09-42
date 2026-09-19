#include "./BitcoinExchange.hpp"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

static std::string trim(const std::string& s);
static bool isAllDigits(const std::string& s);

static bool isLeapYear(int year);
static int daysInMonth(int year, int month);
static bool isValidDate(const std::string& s);
static bool isValidDateFormat(const std::string& s);
static bool isValidCalendarDate(int y, int m, int d);

enum ValueError { VALUE_OK, VALUE_BAD_FORMAT, VALUE_NEGATIVE, VALUE_TOO_LARGE };
static ValueError parseValue(const std::string& s, double& out);

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::~BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _db(other._db) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
	if (this == &other)
		return *this;
	_db = other._db;
	return *this;
}

void BitcoinExchange::loadDatabase(const std::string& path) {
	std::ifstream ifs(path.c_str());
	if (!ifs)
		throw FileOpenException();
	std::string line;
	while (std::getline(ifs, line)) {
		std::string trimmed = trim(line);
		if (trimmed.empty() || trimmed == "date,exchange_rate")
			continue;
		size_t pos = trimmed.find(',');
		if (pos == std::string::npos)
			continue;
		std::string date = trimmed.substr(0, pos);
		std::string rateStr = trimmed.substr(pos + 1);

		if (!isValidDateFormat(date))
			continue;

		char* end;
		double rate = std::strtod(rateStr.c_str(), &end);
		if (end == rateStr.c_str() || *end != '\0')
			continue;

		_db[date] = rate;
	}
	if (_db.empty())
		throw EmptyDatabaseException();
}

void BitcoinExchange::processInput(const std::string& path) const {
	std::ifstream ifs(path.c_str());
	if (!ifs)
		throw FileOpenException();

	std::string line;
	while (std::getline(ifs, line)) {
		processLine(line);
	}
}

void BitcoinExchange::processLine(const std::string& line) const {
	std::string trimmed = trim(line);
	if (trimmed.empty() || trimmed == "date | value")
		return;

	size_t pos = trimmed.find('|');
	if (pos == std::string::npos) {
		std::cout << "Error: bad input => " << trimmed << std::endl;
		return;
	}
	
	std::string date = trim(trimmed.substr(0, pos));
	std::string valueStr = trim(trimmed.substr(pos + 1));
	if (date.empty() || valueStr.empty()) {
		std::cout << "Error: bad input => " << trimmed << std::endl;
		return;
	}

	//日付の検証
	if (!isValidDate(date)) {
		std::cout << "Error: bad input => " << date << std::endl;
		return;
	}

	//値の検証
	double value;
	ValueError err = parseValue(valueStr, value);
	if (err != VALUE_OK) {
		switch (err) {
			case VALUE_BAD_FORMAT:
				std::cout << "Error: bad input => " << valueStr << std::endl;
				break;
			case VALUE_NEGATIVE:
				std::cout << "Error: not a positive number." << std::endl;
				break;
			case VALUE_TOO_LARGE:
				std::cout << "Error: too large a number." << std::endl;
				break;
			default:
				break;
		}
		return;
	}
	
	try {
		double rate = getRate(date);
		std::cout << date << " => " << value << " = " << value * rate << std::endl;
	} catch (const DateOutOfRangeException& e) {
		std::cout << e.what() << std::endl;
	}
}

static bool isValidDate(const std::string& s) {
	if (!isValidDateFormat(s))
		return false;
	int y = std::atoi(s.substr(0, 4).c_str());
	int m = std::atoi(s.substr(5, 2).c_str());
	int d = std::atoi(s.substr(8, 2).c_str());
	if (!isValidCalendarDate(y, m, d))
		return false;
	return true;
}

static bool isValidDateFormat(const std::string& s) {
	if (!(s.size() == 10 && s[4] == '-' && s[7] == '-'
			&& isAllDigits(s.substr(0, 4)) && isAllDigits(s.substr(5, 2))
			&& isAllDigits(s.substr(8, 2)))) {
		return false;
	}
	return true;
}

static bool isValidCalendarDate(int y, int m, int d) {
	if (m < 1 || m > 12)
		return false;
	if (d < 1 || d > daysInMonth(y, m))
		return false;
	return true;
}

static int daysInMonth(int year, int month) {
	static const int days[12] = {31, 28, 31, 30, 31, 30,
			31, 31, 30, 31, 30, 31};

	if (month == 2 && isLeapYear(year))
		return 29;
	return days[month - 1];
}

//4で割り切れる年は、原則としてうるう年
//ただし、100で割り切れて400で割り切れない年は、うるう年ではなく平年（365日）（例：2100年など）。
//400で割り切れる年は、100で割り切れても、うるう年（例：2000年など）。
static bool isLeapYear(int year) {
	if (year % 400 == 0)
		return true;
	if (year % 100 == 0)
		return false;
	if (year % 4 == 0)
		return true;
	return false;
}

static ValueError parseValue(const std::string& s, double& out) {
	if (s.empty())
		return VALUE_BAD_FORMAT;
	size_t i = 0;
	if (s[i] == '-') {
		++i;
		if (i >= s.size())
			return VALUE_BAD_FORMAT;
	}
	size_t start_pos = i;
	size_t dot_pos = std::string::npos;
	while (i < s.size()) {
		if (!(s[i] == '.' || std::isdigit(static_cast<unsigned char>(s[i]))))
			return VALUE_BAD_FORMAT;
		if (s[i] == '.') {
			if (dot_pos != std::string::npos)
				return VALUE_BAD_FORMAT;
			dot_pos = i;
		}
		++i;
	}
	if (dot_pos != std::string::npos) {
		if (!(dot_pos > start_pos && dot_pos + 1 < s.size()))
			return VALUE_BAD_FORMAT;
	}

	double d = std::strtod(s.c_str(), NULL);
	if (d < 0)
		return VALUE_NEGATIVE;
	if (d > 1000)
		return VALUE_TOO_LARGE;
	out = d;
	return VALUE_OK;
}

//dateで調べてrateを取ってくる。
//ない場合はその日付以下で最も近い日付のレートを返す
double BitcoinExchange::getRate(const std::string& date) const {
	//指定キーより大きい最初の要素を取って1つ戻す
	std::map<std::string, double>::const_iterator it = _db.upper_bound(date);
	if (it == _db.begin())
		throw DateOutOfRangeException();
	--it;
	return it->second;
}

static std::string trim(const std::string& s) {
	size_t first = s.find_first_not_of(" \t\n\r");
	if (first == std::string::npos)
		return "";
	size_t last = s.find_last_not_of(" \t\n\r");
	return s.substr(first, last - first + 1);
}

static bool isAllDigits(const std::string& s) {
	for (size_t i = 0; i < s.size(); ++i) {
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return false;
	}
	return true;
}

const char* BitcoinExchange::FileOpenException::what() const throw() {
	return "Error: could not open file.";
}

const char* BitcoinExchange::EmptyDatabaseException::what() const throw() {
	return "Error: database is empty.";
}

const char* BitcoinExchange::DateOutOfRangeException::what() const throw() {
	return "Error: date is before the database range.";
}