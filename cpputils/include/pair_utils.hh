#pragma once

#include <string>

struct HashIntPair {
  std::size_t operator()(const std::pair<int, int> &idx) const {
    std::string idxStr =
        std::to_string(idx.first) + "_" + std::to_string(idx.second);
    return std::hash<std::string>()(idxStr);
  }
};
