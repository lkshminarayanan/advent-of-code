#pragma once

#include <string>
#include <vector>

// split the given string into tokens based on the separators
std::vector<std::string> split(const std::string &line,
                               const std::string &separators);

// split the given string into intergers based on the separators
std::vector<int> splitToIntegers(const std::string &line,
                                 const std::string &separators = " ");
