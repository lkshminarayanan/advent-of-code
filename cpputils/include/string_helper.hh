#pragma once

#include <concepts>
#include <functional>
#include <string>
#include <vector>

// split the given string based on the separators and calls the tokenStorer
void splitToCustomContainer(
    const std::string &line,
    std::function<void(const std::string &)> tokenStorer,
    const std::string &separators = " ");

// split the given string into tokens based on the separators
std::vector<std::string> split(const std::string &line,
                               const std::string &separators = " ");

// split the given string into intergers based on the separators
std::vector<int> splitToIntegers(const std::string &line,
                                 const std::string &separators = " ");

// split the given string into long values based on the separators
std::vector<long> splitToLong(const std::string &line,
                              const std::string &separators = " ");

template <std::integral T> std::vector<T> extract_numbers(const std::string& line) {
    std::vector<T> numbers;
    std::string number;
    for (char c : line) {
        if (std::isdigit(c)) {
            number += c;
        } else {
            if (!number.empty()) {
                numbers.push_back(std::stoi(number));
                number.clear();
            }
        }
    }
    if (!number.empty()) {
        numbers.push_back(std::stoll(number));
    }
    return numbers;
}
