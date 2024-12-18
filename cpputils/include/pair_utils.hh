#pragma once

#include <concepts>
#include <string>

struct HashIntPair {
  std::size_t operator()(const std::pair<int, int> &idx) const {
    std::string idxStr =
        std::to_string(idx.first) + "_" + std::to_string(idx.second);
    return std::hash<std::string>()(idxStr);
  }
};

template <std::integral T1, std::integral T2>
struct HashNumericPair {
  std::size_t operator()(const std::pair<T1, T2> &idx) const {
    std::string idxStr =
        std::to_string(idx.first) + "_" + std::to_string(idx.second);
    return std::hash<std::string>()(idxStr);
  }
};
