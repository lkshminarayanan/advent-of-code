#include "aocutils.hh"

#include <cmath>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int getMatchingNumbers(std::vector<int> &winningNumbers,
                       std::vector<int> &scratchCardNumbers) {
  std::unordered_set<int> winningSet(winningNumbers.begin(),
                                     winningNumbers.end());

  int matchingNums = 0;

  for (auto num : scratchCardNumbers) {
    if (winningSet.find(num) != winningSet.end()) {
      matchingNums++;
    }
  }

  return matchingNums;
}

int main() {
  int resultOne = 0;
  int cardId = 0;
  std::vector<int> numberOfCards;
  InputReader input(4);
  for (auto it = input.begin(); it != input.end(); ++it, ++cardId) {
    auto tokens = split(*it, ":|");
    auto winningNumbers = splitToIntegers(tokens[1]);
    auto scratchCardNumbers = splitToIntegers(tokens[2]);

    auto matchingNums = getMatchingNumbers(winningNumbers, scratchCardNumbers);
    // part 1
    resultOne += (matchingNums == 0) ? 0 : (std::pow(2, (matchingNums - 1)));

    // part 2 - add more scratchcards
    if (numberOfCards.size() == cardId) {
      // card has only the original
      numberOfCards.push_back(1);
    }
    auto numOfCopies = numberOfCards[cardId];

    for (int i = cardId + 1; i <= cardId + matchingNums; i++) {
      if (numberOfCards.size() == i) {
        // not inited yet - add original + copies
        numberOfCards.push_back(numOfCopies + 1);
      } else {
        // add copies
        numberOfCards[i] += numOfCopies;
      }
    }
  }

  int totalNumberOfCards = 0;
  for (int i = 0; i < cardId; i++) {
    totalNumberOfCards += numberOfCards[i];
  }

  std::cout << "Total points from scratchcards (Part I) : " << resultOne
            << std::endl;
  std::cout << "Total number of scratchcards (Part II) : " << totalNumberOfCards
            << std::endl;
}
