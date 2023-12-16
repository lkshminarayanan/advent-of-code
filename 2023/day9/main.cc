#include <aocutils.hh>
#include <iostream>

using AocValueType = long;

std::pair<AocValueType, AocValueType>
extrapolate(std::vector<AocValueType> &values) {
  std::vector<AocValueType> diffs;
  bool allDiffsAreSame = true;

  // calculate all the diffs
  for (int i = 1; i < values.size(); i++) {
    diffs.push_back(values[i] - values[i - 1]);
    if (i > 1 && allDiffsAreSame) {
      allDiffsAreSame &= (diffs[i - 1] == diffs[i - 2]);
    }
  }

  if (allDiffsAreSame) {
    // found the last level - extrapolated num is same as other diffs
    AocValueType diff = diffs.front();
    return {values.front() - diff, values.back() + diff};
  }

  // need to extrapolate the diffs to get the next value
  auto extrapolatedDiffs = extrapolate(diffs);
  return {values.front() - extrapolatedDiffs.first,
          values.back() + extrapolatedDiffs.second};
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(9, &args);

  AocValueType resultPart1 = 0;
  AocValueType resultPart2 = 0;
  for (auto it = input.begin(); it != input.end(); ++it) {
    std::vector<AocValueType> values = splitToLong(*it);
    auto extrapolatedValues = extrapolate(values);
    resultPart1 += extrapolatedValues.second;
    resultPart2 += extrapolatedValues.first;
  }

  std::cout << "Sum of extrapolated values at back (Part I) : " << resultPart1
            << std::endl;
  std::cout << "Sum of extrapolated values at front (Part II) : " << resultPart2
            << std::endl;
}
