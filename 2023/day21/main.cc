#include "pair_utils.hh"
#include <aocutils.hh>
#include <iostream>
#include <queue>
#include <unordered_set>

constexpr char GardenPlot = '.';
constexpr char Rock = '#';

int findReachablePlots(std::vector<std::string> &gardenMap, int maxSteps) {
  std::unordered_set<std::pair<int, int>, HashIntPair> visitedPlots;
  std::priority_queue<std::vector<int>, std::vector<std::vector<int>>,
                      std::greater<std::vector<int>>>
      pq;
  pq.push(
      std::vector{0, (int)gardenMap.size() / 2, (int)gardenMap[0].size() / 2});

  short targetPlotRemainder = maxSteps % 2;
  while (!pq.empty()) {
    std::vector<int> top = pq.top();
    pq.pop();

    int elapsedSteps = top[0], i = top[1], j = top[2];
    if (i < 0 || j < 0 || i == gardenMap.size() || j == gardenMap[0].size() ||
        elapsedSteps > maxSteps || gardenMap[i][j] == Rock ||
        visitedPlots.contains({i, j})) {
      continue;
    }

    if (elapsedSteps % 2 == targetPlotRemainder) {
      visitedPlots.insert({i, j});
    }

    // gardenMap[i][j] = (elapsedSteps % 2 == 0) ? EvenWalked : OddWalked;
    elapsedSteps++;

    pq.push(std::vector{elapsedSteps, i + 1, j});
    pq.push(std::vector{elapsedSteps, i - 1, j});
    pq.push(std::vector{elapsedSteps, i, j + 1});
    pq.push(std::vector{elapsedSteps, i, j - 1});
  }

  /*
  for (int i = 0; i < gardenMap.size(); i++) {
    for (int j = 0; j < gardenMap[0].size(); j++) {
      std::cout << (visitedPlots.contains({i,j})?'V':gardenMap[i][j]);
    }
    std::cout << std::endl;
  }
  */

  return visitedPlots.size();
}

using AocValueType = long long;

// part II based on the brilliant discovery from
// https://www.reddit.com/r/adventofcode/comments/18orn0s/2023_day_21_part_2_links_between_days/
AocValueType
extrapolateUsingLagrange(AocValueType targetSteps,
                         std::vector<AocValueType> steps,
                         std::vector<AocValueType> reachablePlots) {
  // order of parameter : X, x array, y array
  // using lagrange
  AocValueType result = 0;
  for (int i = 0; i < steps.size(); i++) {
    double top = reachablePlots[i], bottom = 1, stepsi = steps[i];
    for (int j = 0; j < steps.size(); j++) {
      if (i == j) {
        continue;
      }
      top *= (targetSteps - steps[j]);
      bottom *= (stepsi - steps[j]);
    }
    result += (top / bottom);
  }

  return result;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(21, &args);

  auto gardenMap = input.readAllAsVectorOfStrings();

  int maxSteps = (args.useExampleInput()) ? 6 : 64;

  std::cout << "Reachable plots with 64 steps (Part I) : "
            << findReachablePlots(gardenMap, maxSteps) << std::endl;

  // Part II - expand map to find first three values
  int factor = 5;
  std::vector<std::string> largerGardenMap;
  for (const auto &s : gardenMap) {
    std::string newLine = "";
    for (int i = 0; i < factor; i++) {
      newLine += s;
    }
    largerGardenMap.push_back(newLine);
  }
  int singleHeight = largerGardenMap.size();
  for (int i = 0; i < factor - 1; i++) {
    for (int j = 0; j < singleHeight; j++) {
      largerGardenMap.push_back(largerGardenMap[j]);
    }
  }

  std::vector<AocValueType> steps;
  std::vector<AocValueType> reachablePlots;
  for (int i = 0; i < 3; i++) {
    steps.push_back(65 + 131 * i);
    reachablePlots.push_back(findReachablePlots(largerGardenMap, steps.back()));
  }

  AocValueType targetSteps = 26501365;
  std::cout << "Reachable plots with " << targetSteps
            << " steps on an infinite map (Part II) : "
            << extrapolateUsingLagrange(targetSteps, steps, reachablePlots)
            << std::endl;
}
