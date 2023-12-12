#include "aocutils.hh"

#include <iostream>
#include <string>
#include <vector>

bool isDigit(char c) {
  int digitVal = c - '0';
  return (digitVal >= 0 && digitVal <= 9);
}

bool isSymbol(char c) { return !isDigit(c) && c != '.'; }

const char *digits = "0123456789";

void extractNumber(std::vector<std::string> &schematic,
                   std::vector<std::vector<bool>> &visited,
                   std::vector<int> &partNumbers, int i, int j) {
  if (i < 0 || i == schematic.size() || j < 0 || j == schematic[0].length() ||
      !isDigit(schematic[i][j]) || visited[i][j]) {
    return;
  }

  auto endPos = schematic[i].find_first_not_of(digits, j);
  if (endPos == std::string::npos) {
    endPos = schematic[i].size();
  }

  // no stl for rfind_first_not_of
  int rj;
  for (rj = j; rj > -1; rj--) {
    if (!isDigit(schematic[i][rj])) {
      break;
    }
  }

  auto startPos = rj + 1;

  for (int vj = startPos; vj < endPos; vj++) {
    visited[i][vj] = true;
  }

  partNumbers.push_back(
      std::stoi(schematic[i].substr(startPos, endPos - startPos)));
  // std::cout << "Extracted : " << partNumbers.back() << std::endl;
}

void lookForNumbers(std::vector<std::string> &schematic,
                    std::vector<std::vector<bool>> &visited,
                    std::vector<int> &partNumbers, long &gearRatios, int i,
                    int j) {
  if (i < 0 || i == schematic.size() || j < 0 || j == schematic[0].length() ||
      visited[i][j] || !isSymbol(schematic[i][j])) {
    // nothing to do
    return;
  }

  int prevNumOfPartNumbers = partNumbers.size();

  extractNumber(schematic, visited, partNumbers, i - 1, j - 1);
  extractNumber(schematic, visited, partNumbers, i - 1, j);
  extractNumber(schematic, visited, partNumbers, i - 1, j + 1);
  extractNumber(schematic, visited, partNumbers, i, j - 1);
  extractNumber(schematic, visited, partNumbers, i, j);
  extractNumber(schematic, visited, partNumbers, i, j + 1);
  extractNumber(schematic, visited, partNumbers, i + 1, j - 1);
  extractNumber(schematic, visited, partNumbers, i + 1, j);
  extractNumber(schematic, visited, partNumbers, i + 1, j + 1);

  if (schematic[i][j] == '*' &&
      partNumbers.size() - prevNumOfPartNumbers == 2) {
    // Gear; Calculate ratio
    gearRatios += partNumbers[prevNumOfPartNumbers] *
                  partNumbers[prevNumOfPartNumbers + 1];
  }
}

void solve(std::vector<std::string> &schematic) {
  std::vector<std::vector<bool>> visited(
      schematic.size(), std::vector<bool>(schematic[0].length(), false));

  std::vector<int> partNumbers;
  long gearRatios = 0;

  for (int i = 0; i < schematic.size(); i++) {
    for (int j = 0; j < schematic[0].length(); j++) {
      lookForNumbers(schematic, visited, partNumbers, gearRatios, i, j);
    }
  }

  int sum = 0;
  for (auto partNumber : partNumbers) {
    sum += partNumber;
  }

  std::cout << "Sum of Part Numbers : " << sum << std::endl;
  std::cout << "Sum of Gear Ratio : " << gearRatios << std::endl;
}

int main() {
  std::vector<std::string> schematic;
  InputReader input(3);

  for (auto it = input.begin(); it != input.end(); ++it) {
    schematic.push_back(*it);
  }

  solve(schematic);
}
