#include <cctype>
#include <iostream>
#include <vector>

#include "aocutils.hh"

const char *numbers = "0123456789";
static std::vector<std::string> numbersWord = {"zero",  "one",  "two", "three",
                                               "four",  "five", "six", "seven",
                                               "eight", "nine"};

std::pair<int, int> extractNumber(const std::string &line) {
  auto firstNumPos = line.find_first_of(numbers);
  auto lastNumPos = line.find_last_of(numbers);
  int firstNum = line[firstNumPos] - '0';
  int lastNum = line[lastNumPos] - '0';
  int numPartOne = (firstNum * 10) + lastNum;
  std::cout << line << "\n" << numPartOne << " ";

  for (int i = 0; i < 10; i++) {
    auto firstNumWordPos = line.find(numbersWord[i]);
    if (firstNumWordPos != std::string::npos && firstNumWordPos < firstNumPos) {
      firstNumPos = firstNumWordPos;
      firstNum = i;
    }

    auto lastNumWordPos = line.rfind(numbersWord[i]);
    if (lastNumWordPos != std::string::npos && lastNumWordPos > lastNumPos) {
      lastNumPos = lastNumWordPos;
      lastNum = i;
    }
  }

  int numPartTwo = (firstNum * 10) + lastNum;
  std::cout << numPartTwo << std::endl;
  return std::make_pair(numPartOne, numPartTwo);
}

int main() {
  int resultOne = 0;
  int resultTwo = 0;
  InputReader input(1);
  for (auto input_it = input.begin(); input_it != input.end(); ++input_it) {
    std::string line = *input_it;
    auto res = extractNumber(*input_it);
    resultOne += res.first;
    resultTwo += res.second;
  }

  std::cout << "Part 1 : Result : " << resultOne << std::endl;
  std::cout << "Part 2 : Result : " << resultTwo << std::endl;
}
