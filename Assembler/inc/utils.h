#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <functional>

std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, const std::string& delims = " \t");
bool isNumeric(const std::string& s);

#endif // _UTILS_H_
