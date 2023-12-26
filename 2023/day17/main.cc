#include <aocutils.hh>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_set>

using BlockId = std::pair<int, int>;

struct Block {
  int cumulativeHeatLoss;
  BlockId id;
  short dir, currDistance;

  Block() = default;
  Block(BlockId _id, int _cumulativeHeatLoss, short _dir, short _currDistance)
      : id(_id), cumulativeHeatLoss(_cumulativeHeatLoss), dir(_dir),
        currDistance(_currDistance) {}

  static std::string getKey(std::pair<int, int> _blockId, short _dir,
                            short _currDistance) {
    return std::to_string(_blockId.first) + "_" +
           std::to_string(_blockId.second) + "_" + std::to_string(_dir) + "_" +
           std::to_string(_currDistance);
  }

  std::string getKey() { return getKey(id, dir, currDistance); }
};

struct BlockGreaterThan {
  bool operator()(const Block &b1, const Block &b2) const {
    return b1.cumulativeHeatLoss > b2.cumulativeHeatLoss;
  }
};

using PriorityQueueBlocks =
    std::priority_queue<Block, std::vector<Block>, BlockGreaterThan>;

std::vector<std::pair<int, int>> neighborDiffs{
    {-1, 0},
    {0, +1},
    {+1, 0},
    {0, -1},
};

int solve(std::vector<std::vector<int>> &cityMap, int part) {
  BlockId dest = {cityMap.size() - 1, cityMap[0].size() - 1};

  PriorityQueueBlocks blocks;
  blocks.push(Block({0, 0}, 0, 1, 0));

  std::unordered_set<std::string> visitedCombinations;
  int minHeatLoss = std::numeric_limits<int>::max();

  while (!blocks.empty()) {
    const auto curr = blocks.top();
    blocks.pop();

    if (curr.id == dest && curr.cumulativeHeatLoss < minHeatLoss &&
        (part == 1 || curr.currDistance == 4)) {
      minHeatLoss = curr.cumulativeHeatLoss;
    }

    const auto heatLossUntilCurr = curr.cumulativeHeatLoss;

    for (int i = 0; i < 4; i++) {

      auto neighborId =
          std::make_pair(curr.id.first + neighborDiffs[i].first,
                         curr.id.second + neighborDiffs[i].second);
      int newDistance = (curr.dir == i) ? curr.currDistance + 1 : 1;

      if ( // out of boundary
          neighborId.first < 0 || neighborId.first >= cityMap.size() ||
          neighborId.second < 0 || neighborId.second >= cityMap[0].size() ||
          // don't go back
          i == (curr.dir + 2) % 4 ||
          // part 1 : atmost 3 blocks
          (part == 1 && newDistance == 4) ||
          // part 2 : min 4 blocks before changing direction and atmost 10
          // straight blocks
          (part == 2 &&
           ((curr.currDistance < 4 && curr.dir != i && curr.currDistance > 0) ||
            (newDistance == 11))) ||
          // don't visit same combinations again
          visitedCombinations.find(Block::getKey(neighborId, i, newDistance)) !=
              visitedCombinations.end()) {
        // do nothing
        continue;
      }

      // add neighbor to pq and set
      int cumulativeHeatLossUntilNeighbor =
          curr.cumulativeHeatLoss +
          cityMap[neighborId.first][neighborId.second];
      auto neighbor =
          Block(neighborId, cumulativeHeatLossUntilNeighbor, i, newDistance);
      visitedCombinations.insert(neighbor.getKey());
      blocks.push(std::move(neighbor));
    }
  }

  return minHeatLoss;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(17, &args);

  std::vector<std::vector<int>> cityMap;
  for (auto it = input.begin(); it != input.end(); ++it) {
    std::vector<int> line;
    for (auto c : *it) {
      line.push_back(c - '0');
    }
    cityMap.push_back(std::move(line));
  }

  std::cout << "Minimum heat loss with the crucible (Part I) : "
            << solve(cityMap, 1) << std::endl;
  std::cout << "Minimum heat loss with the ultra crucible (Part II) : "
            << solve(cityMap, 2) << std::endl;
}
