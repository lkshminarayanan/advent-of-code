#include <aocutils.hh>
#include <iostream>

using AocType = int;

AocType solveHorizontal(std::vector<std::string> &pattern, int oldResult) {
  for (int i = 1; i < pattern.size(); i++) {
    int j = 0;
    for (; i + j < pattern.size() && i - j - 1 >= 0; j++) {
      if (pattern[i + j] != pattern[i - j - 1]) {
        break;
      }
    }

    if (i + j == pattern.size() || i - j - 1 == -1) {
      // mirror lies before i
      if (100 * i != oldResult) {
        return 100 * i;
      }
    }
  }

  return 0;
}

AocType solveVertical(std::vector<std::string> &pattern, int oldResult) {
  for (int i = 1; i < pattern[0].size(); i++) {
    int j = 0;
    for (; i + j < pattern[0].size() && i - j - 1 >= 0; j++) {
      int s1_j = i + j, s2_j = i - j - 1, loop_i = 0;
      for (; loop_i < pattern.size(); loop_i++) {
        if (pattern[loop_i][s1_j] != pattern[loop_i][s2_j]) {
          break;
        }
      }
      if (loop_i != pattern.size()) {
        break;
      }
    }

    if (i + j == pattern[0].size() || i - j - 1 == -1) {
      // mirror lies before i
      if (i != oldResult) {
        return i;
      }
    }
  }

  return 0;
}

AocType solvePatternImpl(std::vector<std::string> &pattern,
                         int oldResult = -1) {
  AocType result = solveHorizontal(pattern, oldResult);
  if (result == 0) {
    result = solveVertical(pattern, oldResult);
  }

  return result;
}

std::pair<AocType, AocType> solvePattern(std::vector<std::string> &pattern) {
  // Part I
  AocType originalResult = solvePatternImpl(pattern);

  // Part II
  AocType result = 0;
  for (int i = 0; i < pattern.size(); i++) {
    for (int j = 0; j < pattern[0].size(); j++) {
      pattern[i][j] = (pattern[i][j] == '.') ? '#' : '.';
      result = solvePatternImpl(pattern, originalResult);
      pattern[i][j] = (pattern[i][j] == '.') ? '#' : '.';

      if (result != 0) {
        return {originalResult, result};
      }
    }
  }

  // will not reach this flow
  return {originalResult, originalResult};
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(13, &args);

  AocType result1 = 0;
  AocType result2 = 0;
  std::vector<std::string> pattern;
  for (auto it = input.begin(); it != input.end(); ++it) {
    const std::string &line = *it;
    if (line.length() == 0) {
      // end of pattern
      auto result = solvePattern(pattern);
      result1 += result.first;
      result2 += result.second;
      pattern.clear();
      continue;
    }

    pattern.push_back(*it);
  }

  auto result = solvePattern(pattern);
  result1 += result.first;
  result2 += result.second;
  std::cout << "Summarized value (Part I) : " << result1 << std::endl;
  std::cout << "Summarized value (Part I) : " << result2 << std::endl;
}
