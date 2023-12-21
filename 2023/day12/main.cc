#include <algorithm>
#include <aocutils.hh>
#include <iostream>
#include <memory>
#include <thread>
#include <unordered_map>

constexpr char UNKNOWN = '?';
constexpr char DAMAGED = '#';
constexpr char OPERATIONAL = '.';

using AocType = long;

class HotSpring {
  void updateMetadata() {
    int totalDamaged = 0;
    for (auto damaged : damagedCount) {
      totalDamaged += damaged;
    }
    unknownDamagedCount =
        totalDamaged - std::count(condition.begin(), condition.end(), DAMAGED);

    int i = 0;
    unknownIndexes.clear();
    while (i < condition.size()) {
      i = condition.find(UNKNOWN, i);
      if (i == condition.npos) {
        break;
      }

      unknownIndexes.push_back(i);
      i++;
    }
  }

public:
  std::string condition;
  std::vector<int> unknownIndexes;
  std::vector<int> damagedCount;
  int unknownDamagedCount;

  HotSpring(const std::string &line) {
    auto tokens = split(line, " ");
    condition = tokens[0];
    damagedCount = splitToIntegers(tokens[1], ",");
    updateMetadata();
  }

  void unfoldRecords() {
    std::string newCondition;
    std::vector<int> newdamagedCount;

    for (int i = 0; i < 5; i++) {
      newCondition += condition;
      if (i < 4) {
        newCondition += '?';
      }
      newdamagedCount.insert(newdamagedCount.end(), damagedCount.begin(),
                             damagedCount.end());
    }

    condition = newCondition;
    damagedCount = std::move(newdamagedCount);
    updateMetadata();
  }

  std::string toString() {
    std::string result;
    result += "condition : " + condition + "\ndamagedCounts : ";
    for (auto dc : damagedCount) {
      result += std::to_string(dc) + ", ";
    }
    result += "\nunknownDamagedCount = " + std::to_string(unknownDamagedCount) +
              "\nunknownIndexes : ";
    for (auto ui : unknownIndexes) {
      result += std::to_string(ui) + ", ";
    }
    result += "\n";

    return result;
  }
};

// returns -1 = invalid, otherwise the damagedCount idx until which there is a
// match
// Note: can be improved by maintaining cond_i until which validation is done
int validCondition(const std::string &condition, std::vector<int> damagedCount,
                   int checkUntilIdx = -1) {
  bool allowPartial = false;
  if (checkUntilIdx == -1) {
    checkUntilIdx = condition.size() - 1;
  } else {
    allowPartial = true;
  }

  int i = 0, dc_i = 0, start, end;
  while (i <= checkUntilIdx && dc_i < damagedCount.size()) {
    start = condition.find_first_of(DAMAGED, i);
    if ((start == condition.npos || start > checkUntilIdx) && allowPartial) {
      // partially valid; as start already exceeds checkUntilIdx
      /*std::cout << condition << " is partially valid until(a) " <<
         checkUntilIdx
                << std::endl;*/
      return dc_i;
    }

    end = condition.find_first_of(OPERATIONAL, start + 1);
    if (end == condition.npos || end > checkUntilIdx) {
      if (allowPartial) {
        // partially valid; as end exceeds checkUntilIdx
        /*std::cout << condition << " is partially valid until(b) "
                  << checkUntilIdx << std::endl;*/
        return dc_i;
      } else {
        end = checkUntilIdx + 1;
      }
    }

    auto chunkSize = end - start;
    if (chunkSize != damagedCount[dc_i]) {
      // invalid
      /*std::cout << condition << " is invalid until " << checkUntilIdx
                << " :: " << start << " " << end << " " << damagedCount[dc_i]
                << std::endl;*/
      return -1;
    }

    dc_i++;
    i = end + 1;
  }

  /*std::cout << condition << " is "
            << ((dc_i == damagedCount.size()) ? "valid" : "partially valid(c)")
            << " until " << checkUntilIdx << std::endl;*/

  return dc_i;
}

std::string mapKey(int ui_i, int dc_i, int unknownDamagedCount) {
  return std::to_string(ui_i) + "_" + std::to_string(dc_i) + "_" +
         std::to_string(unknownDamagedCount);
}

AocType findArrangementCount(std::string &condition,
                             std::unordered_map<std::string, AocType> &cache,
                             std::vector<int> &unknownIndexes, int ui_i,
                             std::vector<int> &damagedCount, int dc_i,
                             int unknownDamagedCount) {
  auto cacheKey = mapKey(ui_i, dc_i, unknownDamagedCount);
  auto entry = cache.find(cacheKey);
  if (entry != cache.end()) {
    return entry->second;
  }

  // std::cout << "Processing " << condition << std::endl;
  if (unknownDamagedCount == 0 ||
      unknownDamagedCount == unknownIndexes.size() - ui_i) {
    // either we have used up all unknownDamagedCounts in previous ? (or) the
    // remaining ? count is equal to remaining unknownDamagedCount; fill rest of
    // the condition as operational or damaged and check.
    auto restOfEntires = (unknownDamagedCount == 0) ? OPERATIONAL : DAMAGED;
    while (ui_i < unknownIndexes.size()) {
      condition[unknownIndexes[ui_i++]] = restOfEntires;
    }
    return cache[cacheKey] =
               ((validCondition(condition, damagedCount) != -1 ? 1 : 0));
  }

  // set spring at ui_i as either operational or damaged
  auto unknownIdx = unknownIndexes[ui_i];
  condition[unknownIdx] = OPERATIONAL;
  long updated_dc_i = validCondition(condition, damagedCount, unknownIdx);
  AocType withOperational =
      (updated_dc_i != -1)
          ? findArrangementCount(condition, cache, unknownIndexes, ui_i + 1,
                                 damagedCount, updated_dc_i,
                                 unknownDamagedCount)
          : 0;

  condition[unknownIdx] = DAMAGED;
  updated_dc_i = validCondition(condition, damagedCount, unknownIdx);
  AocType withDamaged =
      (updated_dc_i != -1)
          ? findArrangementCount(condition, cache, unknownIndexes, ui_i + 1,
                                 damagedCount, updated_dc_i,
                                 unknownDamagedCount - 1)
          : 0;

  return cache[cacheKey] = withOperational + withDamaged;
}

void processHotSprings(
    int threadId, const std::vector<std::unique_ptr<HotSpring>> &hotSprings,
    std::vector<AocType> &results, int start, int end) {
  long result = 0;
  std::unordered_map<std::string, AocType> cache;
  for (int i = start; i < end; i++) {
    auto &hotSpring = hotSprings[i];
    auto condition = hotSpring->condition;
    result += findArrangementCount(condition, cache, hotSpring->unknownIndexes,
                                   0, hotSpring->damagedCount, 0,
                                   hotSpring->unknownDamagedCount);
    cache.clear();
  }
  results[threadId] = result;
}

long solve(int numThreads,
           std::vector<std::unique_ptr<HotSpring>> &hotSprings) {
  int start = 0;
  int batchSize = hotSprings.size() / numThreads;
  int batchSizeRem = hotSprings.size() % numThreads;
  std::vector<std::thread> threads;
  std::vector<AocType> results(numThreads);

  for (int i = 0; i < numThreads; i++) {
    int end = start + batchSize + (batchSizeRem > i ? 1 : 0);
    threads.push_back(std::thread(processHotSprings, i, std::ref(hotSprings),
                                  std::ref(results), start, end));
    start = end;
  }

  for (auto &th : threads) {
    th.join();
  }

  AocType arrangements = 0;
  for (auto result : results) {
    arrangements += result;
  }

  return arrangements;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(12, &args);

  bool disableThreads = args.hasBoolFlag("--disableThreads", "-dt");

  std::vector<std::unique_ptr<HotSpring>> hotSprings;
  for (auto it = input.begin(); it != input.end(); ++it) {
    hotSprings.push_back(std::move(std::make_unique<HotSpring>(*it)));
  }

  unsigned int numThreads = (args.useExampleInput() || disableThreads)
                                ? 1
                                : std::thread::hardware_concurrency();

  std::cout << "Total number of arrangements (Part I) : "
            << solve(numThreads, hotSprings) << std::endl;

  for (std::unique_ptr<HotSpring> &hotSpring : hotSprings) {
    hotSpring->unfoldRecords();
  }

  std::cout << "Total number of arrangements (Part II) : "
            << solve(numThreads, hotSprings) << std::endl;
}
