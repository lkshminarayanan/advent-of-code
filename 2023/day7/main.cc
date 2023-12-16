#include <algorithm>
#include <aocutils.hh>
#include <iostream>
#include <memory>
#include <unordered_map>

// First zero reserved for Joker
constexpr std::string LabelsDict = "023456789TJQKA";
constexpr int idxOfJ = LabelsDict.find('J');
constexpr int idxOfJoker = 0;

enum CardTypes {
  HighCard = 1,
  OnePair,
  TwoPair,
  ThreeOfAKind,
  FullHouse,
  FourOfAKind,
  FiveOfAKind
};

class Card {
  // numeric representation of hand
  std::vector<int> _hand;
  int _bid;
  CardTypes _type;

  void setHand(const std::string &handStr) {
    for (const char &c : handStr) {
      _hand.push_back(LabelsDict.find(c));
    }
  }

  void updateType() {
    std::unordered_map<int, int> labels;
    for (const int &label : _hand) {
      labels[label]++;
    }

    switch (labels.size()) {
    case 1:
      // five of a kind
      _type = FiveOfAKind;
      break;
    case 2:
      if (labels.find(idxOfJoker) != labels.end()) {
        // there are two distinct labels and one of them is a Joker!
        // this is now a five of a kind
        _type = FiveOfAKind;
      } else {
        // four of a kind (4 + 1), (or) full house (3 + 2),
        _type = (labels[_hand[0]] == 4 || labels[_hand[0]] == 1) ? FourOfAKind
                                                                 : FullHouse;
      }
      break;
    case 3:
      for (const auto &entry : labels) {
        if (entry.second == 3) {
          // three of a kind (3 + 1 + 1)
          _type = ThreeOfAKind;
          if (labels.find(idxOfJoker) != labels.end()) {
            // Joker exists => promote to four of a kind
            _type = FourOfAKind;
          }
          break;
        } else if (entry.second == 2) {
          // two pair (2 + 2 + 1)
          _type = TwoPair;
          if (labels[idxOfJoker] > 0) {
            // Joker exists
            _type = (labels[idxOfJoker] == 2) ? FourOfAKind : FullHouse;
          }
          break;
        }
      }
      break;
    case 4:
      // one pair (2 + 1 + 1 + 1) and with joker, three of a kind
      _type =
          (labels.find(idxOfJoker) != labels.end()) ? ThreeOfAKind : OnePair;
      break;
    case 5:
      // high card and with joker, one pair
      _type = (labels.find(idxOfJoker) != labels.end()) ? OnePair : HighCard;
      break;
    }
  }

public:
  Card(const std::string &hand, int bid) : _bid(bid) {
    setHand(hand);
    updateType();
  }

  int getBid() const { return _bid; }

  std::string handToString() {
    std::string hand;
    for (auto idx : _hand) {
      hand += (idx = idxOfJoker) ? 'J' : LabelsDict[idx];
    }
    return hand;
  }

  std::string toString() {
    return handToString() + " bid : " + std::to_string(_bid) +
           " type :" + std::to_string(_type);
  }

  static bool compare(const std::unique_ptr<Card> &c1,
                      const std::unique_ptr<Card> &c2) {
    if (c1->_type != c2->_type) {
      return c1->_type < c2->_type;
    }

    // same type => compare the hands
    for (int i = 0; i < 5; i++) {
      if (c1->_hand[i] == c2->_hand[i]) {
        continue;
      }

      return c1->_hand[i] < c2->_hand[i];
    }

    return true;
  }

  void promote_J_ToJoker() {
    for (int i = 0; i < 5; i++) {
      if (_hand[i] == idxOfJ) {
        // demote to the lowest idx
        _hand[i] = 0;
      }
    }

    updateType();
  }
};

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(7, &args);

  std::vector<std::unique_ptr<Card>> cards;

  for (auto it = input.begin(); it != input.end(); ++it) {
    auto tokens = split(*it);
    cards.push_back(std::make_unique<Card>(tokens[0], std::stoi(tokens[1])));
  }

  // sort them
  std::sort(cards.begin(), cards.end(), Card::compare);

  // Part I
  long totalWinnings = 0;
  int rank = 1;
  for (auto &card : cards) {
    totalWinnings += (rank++ * card->getBid());
  }

  std::cout << "Total Winnings (Part I) : " << totalWinnings << std::endl;

  // Part II - prepare the input
  for (auto &card : cards) {
    card->promote_J_ToJoker();
  }

  // now with joker in play, sort them again
  std::sort(cards.begin(), cards.end(), Card::compare);

  totalWinnings = 0;
  rank = 1;
  for (auto &card : cards) {
    totalWinnings += (rank++ * card->getBid());
  }

  std::cout << "Total Winnings (Part II) : " << totalWinnings << std::endl;
}
