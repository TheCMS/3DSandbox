#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

long long getCurrentTimeMillis();
std::vector<std::string> getLinesFromChar(const char* str);
std::string trimmLead(std::string& aStr, const std::string& whiteSpace=" \t\n\r");
std::string trimmEnd(std::string& aStr, const std::string& whiteSpace=" \t\n\r");
std::string trimm(std::string& aStr, const std::string& whitespace=" \t\n\r");

#endif // UTILS_H
