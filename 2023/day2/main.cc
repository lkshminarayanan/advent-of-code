#include "aocutils.hh"
#include <iostream>
#include <unordered_map>

std::unordered_map<std::string, int> target = {
    {"red", 12}, {"green", 13}, {"blue", 14}};

bool isPossibleGame(const std::string &game) {
  auto tokens = tokenize(game, ";, ");
  for (int i = 1; i < tokens.size(); i += 2) {
    if (std::stoi(tokens[i - 1]) > target[tokens[i]]) {
      return false;
    }
  }

  return true;
}

int power(const std::string &game) {

  auto tokens = tokenize(game, ";, ");
  std::unordered_map<std::string, int> maxCubes = {
      {"red", 0}, {"green", 0}, {"blue", 0}};
  for (int i = 1; i < tokens.size(); i += 2) {
    auto numOfCubes = std::stoi(tokens[i - 1]);
    if (numOfCubes > maxCubes[tokens[i]]) {
      maxCubes[tokens[i]] = numOfCubes;
    }
  }

  return maxCubes["red"] * maxCubes["blue"] * maxCubes["green"];
}

int main() {
  InputReader input(2);
  int gameId = 1;
  int resultPartOne = 0;
  int resultPartTwo = 0;
  for (auto input_it = input.begin(); input_it != input.end();
       ++input_it, gameId++) {
    auto tokens = tokenize(*input_it, ":");

    // Part one
    if (isPossibleGame(tokens[1])) {
      resultPartOne += gameId;
    }

    // Part two
    resultPartTwo += power(tokens[1]);
  }

  std::cout << "Part 1 : " << resultPartOne << std::endl;
  std::cout << "Part 2 : " << resultPartTwo << std::endl;
}
