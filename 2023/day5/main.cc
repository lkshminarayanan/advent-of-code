#include "aocutils.hh"

#include <algorithm>
#include <iostream>

#include "range.hh"

using AocValue = unsigned long long;
using AocRange = Range<AocValue>;

AocValue solve(InputReader &input, int part) {
  auto it = input.begin();
  std::vector<AocValue> seeds;
  // parse all the seeds
  splitToCustomContainer((*it).substr(7), [&seeds](const std::string &token) {
    seeds.push_back(std::stoull(token));
  });
  ++it;

  std::vector<std::pair<AocRange *, bool>> ranges;
  switch (part) {
  case 1:
    // to treat seeds as individual values, we use a range with length 1
    for (int i = 0; i < seeds.size(); i++) {
      ranges.push_back(
          {AocRange::newRangeWithStartAndRangeLength(seeds[i], 1), false});
    }
    break;
  case 2:
    // treat seeds as range
    for (int i = 0; i < seeds.size(); i += 2) {
      ranges.push_back(
          {AocRange::newRangeWithStartAndRangeLength(seeds[i], seeds[i + 1]),
           false});
    }
    break;
  default:
    throw std::runtime_error("unknown part");
  }

  // handle the maps one by one
  for (; it != input.end(); ++it) {
    auto line = *it;
    if (line.length() == 0) {
      // end of a map;

      // reset visited column
      for (int i = 0; i < ranges.size(); i++) {
        ranges[i].second = false;
      }

      // skip next line as its just a header
      ++it;
      continue;
    }

    // parse the current map
    std::vector<AocValue> map;
    splitToCustomContainer(line, [&map](const std::string &token) {
      map.push_back(std::stoull(token));
    });

    for (int i = 0; i < ranges.size(); i++) {
      if (ranges[i].second) {
        // already visited
        continue;
      }

      auto result = ranges[i].first->transformRange(map);
      if (!result.first) {
        // no overlap
        continue;
      }

      ranges[i].second = true;

      // there is a chance ranges could have been split add them to the back
      for (auto &splitRange : result.second) {
        ranges.push_back({splitRange, false});
      }
    }
  }

  // find lowest location
  auto rangesIt = ranges.begin();
  AocValue lowestLocation = rangesIt->first->getStart();
  for (; rangesIt != ranges.end(); rangesIt++) {
    if (lowestLocation > rangesIt->first->getStart()) {
      lowestLocation = rangesIt->first->getStart();
    }
  }

  for (auto &range : ranges) {
    delete range.first;
  }

  return lowestLocation;
}

int main() {
  InputReader input(5, false);
  // part 1
  std::cout << "Lowest Location Number(Part II) : " << solve(input, 1)
            << std::endl;
  // part 2
  std::cout << "Lowest Location Number(Part II) : " << solve(input, 2)
            << std::endl;
}
