#include "utils.h"

#include <string>

std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t");
    size_t last = s.find_last_not_of(" \t");
    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }
    return s.substr(first, last - first + 1);
}

std::vector<std::string> split(const std::string& s, const std::string& delims) {
    std::vector<std::string> res;
    std::string word = "";
    for (size_t i = 0; i < s.size(); i++) {
        bool isDelimiter = (delims.find(s[i]) != std::string::npos);
        if (isDelimiter && !word.empty()) {
            res.push_back(word);
            word = "";
        } else if (!isDelimiter) {
            word.push_back(s[i]);
        }
    }
    if (!word.empty()) {
        res.push_back(word);
    }
    return res;
}

bool isNumeric(const std::string& s) {
    for (size_t i = 0; i < s.size(); i++) {
        if (!(i == 0 && s[i] == '-') && ((int)s[i] < 0x30 || (int)s[i] > 0x39)) {
            return false;
        }
    }
    return true;
}


