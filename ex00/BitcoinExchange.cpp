#include "./BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {

}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {

}

bool BitcoinExchange::loadDatabase(const std::string& path) {

}

    for (std::string::size_type i = 0; i < s.size()
void BitcoinExchange::processFile(const std::string& path) const {

}

void BitcoinExchange::processLine(const std::string& line) const {
    std::string trimmed = trim(line);
    if (trimmed.empty() || trimmed == "date | value")
        return;

    size_t pos = trimmed.find('|');
    if (pos == std::string::npos) {
        std::cerr << "Error: bad input => " << trimmed << std::endl;
        return;
    }
    
    std::string date = trim(trimmed.substr(0, pos));
    std::string value = trim(trimmed.substr(pos + 1));

    //日付の検証
    if (date.size() == 10 && date[4] == '-' && date[7] == '-'
            && isAllDigits(date.substr(0, 4)) && isAllDigits(date.substr(5, 2))
            && isAllDigits(date.substr(8, 2))) {
        std::cerr << "Error: bad input => " << date << std::endl;
        return;
    }

    //値の検証
}

double BitcoinExchange::getRate(const std::string& date) const {

}

static std::string trim(const std::string& s) {
    std::string::size_type first = s.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    std::string::size_type last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, last - first + 1);
}

static bool isAllDigits(const std::string& s) {
    for (std::string::size_type i = 0; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
    }
    return true;
}

static bool isLeapYear(int year);
static int daysInMonth(int year, int month);
static bool isValidDate(const std::string& s);
static ValueError parseValue(const std::string& s, double& out);