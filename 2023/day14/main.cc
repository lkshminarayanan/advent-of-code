#include <algorithm>
#include <aocutils.hh>
#include <iostream>

using AocType = int;

constexpr char RoundRock = 'O';
constexpr char CubeRock = '#';
constexpr char EmptySpace = '.';

AocType totalLoad(std::vector<std::string> &rockMap) {
  AocType load = 0;
  for (int i = 0; i < rockMap.size(); i++) {
    AocType weight = rockMap.size() - i;
    for (int j = 0; j < rockMap[0].size(); j++) {
      if (rockMap[i][j] == RoundRock) {
        load += weight;
      }
    }
  }

  return load;
}

enum Direction : short { NORTH, SOUTH, WEST, EAST };

void move(std::vector<std::string> &rockMap, Direction d, int i, int j) {
  short di = 0, dj = 0;
  switch (d) {
  case NORTH:
    di = -1;
    break;
  case SOUTH:
    di = 1;
    break;
  case WEST:
    dj = -1;
    break;
  case EAST:
    dj = +1;
    break;

  default:
    break;
  }

  int newi = i + di, newj = j + dj;
  while (newi >= 0 && newi < rockMap.size() && newj >= 0 &&
         newj < rockMap[0].size() && rockMap[newi][newj] == EmptySpace) {
    newi = newi + di;
    newj = newj + dj;
  }
  rockMap[i][j] = EmptySpace;
  rockMap[newi - di][newj - dj] = RoundRock;
}

void tilt(std::vector<std::string> &rockMap, Direction d) {
  if (d == NORTH || d == WEST) {
    for (int i = 0; i < rockMap.size(); i++) {
      for (int j = 0; j < rockMap[0].size(); j++) {
        if (rockMap[i][j] == RoundRock) {
          // Move it
          move(rockMap, d, i, j);
        }
      }
    }
  } else {
    for (int i = rockMap.size() - 1; i >= 0; i--) {
      for (int j = rockMap[0].size() - 1; j >= 0; j--) {
        if (rockMap[i][j] == RoundRock) {
          // Move it
          move(rockMap, d, i, j);
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(14, &args);

  std::vector<std::string> rockMap;
  for (auto it = input.begin(); it != input.end(); ++it) {
    rockMap.push_back(*it);
  }

  tilt(rockMap, NORTH);
  std::cout << "Total load on the north support beams (Part I) : "
            << totalLoad(rockMap) << std::endl;

  // complete first cycle
  tilt(rockMap, WEST);
  tilt(rockMap, SOUTH);
  tilt(rockMap, EAST);

  // start caching the total load to detect pattern
  std::vector<AocType> totalLoads{totalLoad(rockMap)};

  // run rest of the cycles
  int i = 1;
  int patternStartingAt = 0;
  int totalCycles = 1000000000;
  AocType result = 0;
  for (; i < totalCycles; i++) {
    tilt(rockMap, NORTH);
    tilt(rockMap, WEST);
    tilt(rockMap, SOUTH);
    tilt(rockMap, EAST);
    AocType tl = totalLoad(rockMap);

    auto totalLoadsIt = std::find(totalLoads.rbegin(), totalLoads.rend(), tl);
    if (totalLoadsIt != totalLoads.rend()) {
      // there might be a pattern
      patternStartingAt = std::distance(totalLoadsIt, totalLoads.rend()) - 1;
      int seq1Idx = totalLoads.size() - 1;
      int seq2Idx = patternStartingAt - 1;
      while (totalLoads[seq1Idx] == totalLoads[seq2Idx] &&
             seq1Idx > patternStartingAt) {
        seq1Idx--;
        seq2Idx--;
      }

      if (seq1Idx == patternStartingAt &&
          totalLoads.size() - patternStartingAt > 1) {
        // found a complete pattern with more than an element
        patternStartingAt++;
        totalLoads.push_back(tl);

        auto patternLength = totalLoads.size() - patternStartingAt;

        auto remainingCycles = totalCycles - i - 1;
        auto remainder = remainingCycles % patternLength;
        result = totalLoads[patternStartingAt + remainder - 1];
        break;
      }
    }

    totalLoads.push_back(tl);
  }

  std::cout << "Total load on the north support beams (Part II) : " << result
            << std::endl;
}
