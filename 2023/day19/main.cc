#include <aocutils.hh>
#include <iostream>
#include <memory>
#include <unordered_map>

struct Rule {
  uint8_t category{0};
  int value{0};

  bool isAccepted{false};

  std::unique_ptr<Rule> left{nullptr};
  std::unique_ptr<Rule> right{nullptr};

  Rule(char finalResult) {
    // constructor for leaf node
    isAccepted = finalResult == 'A';
  }

  Rule(char _category, const std::string &_value) : value(std::stoi(_value)) {
    switch (_category) {
    case 'x':
      category = 0;
      break;
    case 'm':
      category = 1;
      break;
    case 'a':
      category = 2;
      break;
    case 's':
      category = 3;
      break;
    }
  }

  bool validate(const std::vector<int> &part) {
    if (left) {
      // std::cout << (int)category << " : " << value << std::endl;
      if (part[category] < value) {
        return left->validate(part);
      } else {
        return right->validate(part);
      }
    } else {
      // leaf node
      return isAccepted;
    }
  }
};

std::unique_ptr<Rule>
buildTree(std::unordered_map<std::string, std::vector<std::string>> &defn,
          const std::string &name, int id) {
  const std::vector<std::string> &rules = defn[name];
  if (id == rules.size() - 1) {
    // last rule in the workflow
    const std::string &wfName = rules[id];
    if (wfName == "A" || wfName == "R") {
      return std::make_unique<Rule>(wfName[0]);
    } else {
      // needs a new workflow
      return buildTree(defn, wfName, 1);
    }
  }

  if (name == "A" || name == "R") {
    return std::make_unique<Rule>(name[0]);
  }

  const std::string &test = rules[id];
  std::unique_ptr<Rule> rule = std::make_unique<Rule>(test[0], test.substr(2));

  std::unique_ptr<Rule> nextRule = buildTree(defn, name, id + 2);
  std::unique_ptr<Rule> successRule = buildTree(defn, rules[id + 1], 1);

  switch (test[1]) {
  case '<':
    rule->left = std::move(successRule);
    rule->right = std::move(nextRule);
    break;
  case '>':
    // inc value as ours is a tree where all left elements are < root
    // and the right ones are >= root.
    rule->value++;
    rule->left = std::move(nextRule);
    rule->right = std::move(successRule);
    break;
  }

  return std::move(rule);
}

using AocType = unsigned long long;
AocType getAcceptedCombinations(const std::unique_ptr<Rule> &rule,
                                std::vector<std::pair<int, int>> &ranges) {
  if (!rule->left) {
    // leaf node
    if (rule->isAccepted) {
      AocType combinations = 1;
      for (const auto range : ranges) {
        combinations *= (range.second - range.first + 1);
      }
      return combinations;
    } else {
      return 0;
    }
  }

  std::pair<int, int> originalRange = ranges[rule->category];

  // call left
  ranges[rule->category].second = rule->value - 1;
  auto leftCombinations = getAcceptedCombinations(rule->left, ranges);

  // call right
  ranges[rule->category].first = rule->value;
  ranges[rule->category].second = originalRange.second;
  auto rightCombinations = getAcceptedCombinations(rule->right, ranges);

  // reset ranges and return the sum
  ranges[rule->category] = originalRange;
  return leftCombinations + rightCombinations;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(19, &args);

  std::unordered_map<std::string, std::vector<std::string>> defn;

  auto it = input.begin();
  for (; it->length() != 0; ++it) {
    auto tokens = split(*it, "{}:,");
    defn[tokens[0]] = std::move(tokens);
  }

  auto in = buildTree(defn, "in", 1);

  int totalRatings = 0;

  for (++it; it != input.end(); ++it) {
    const std::vector<int> part = splitToIntegers(*it, "{x=,m=,a=,s=}");
    if (in->validate(part)) {
      for (const auto rating : part) {
        totalRatings += rating;
      }
    }
  }

  std::cout << "Total Ratings (Part I) : " << totalRatings << std::endl;

  // Part II
  std::vector<std::pair<int, int>> ranges(4, {1, 4000});
  std::cout << "Total possible combinations (Part II) : "
            << getAcceptedCombinations(in, ranges) << std::endl;
}
