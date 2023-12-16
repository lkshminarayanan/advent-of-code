#include <aocutils.hh>
#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_map>

struct Node {
  std::string name;
  std::shared_ptr<Node> left, right;

  Node(std::string name) : name(name) {}
};

using AocStepType = long;
AocStepType solveSteps(std::shared_ptr<Node> &node, const std::string &path) {
  AocStepType steps = 1;
  for (int i = 0;; i = (i + 1) % path.size(), steps++) {
    if (path[i] == 'L') {
      node = node->left;
    } else {
      node = node->right;
    }

    // part one ends with node named ZZZ but just checking for last Z works too
    if (node->name[2] == 'Z') {
      break;
    }
  }

  return steps;
}

std::unordered_map<AocStepType, int> factorize(AocStepType num) {
  std::unordered_map<AocStepType, int> factors;
  AocStepType i = 2;
  while (i < num / 2) {
    if (num % i == 0) {
      factors[i]++;
      num = num / i;
      // try again with same i
      continue;
    }

    i++;
  }

  if (num != 1) {
    factors[num]++;
  }

  return factors;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(8, &args);

  auto it = input.begin();
  std::string path = *it;
  ++it;

  // skip empty line
  ++it;

  std::unordered_map<std::string, std::shared_ptr<Node>> nodes;

  for (; it != input.end(); ++it) {
    auto tokens = split(*it, " =(,)");

    const std::string &rootName = tokens[0];
    std::shared_ptr<Node> root = nodes[rootName];
    if (root == nullptr) {
      root.reset(new Node(rootName));
      nodes[rootName] = root;
    }

    const std::string &leftName = tokens[1];
    std::shared_ptr<Node> left = nodes[leftName];
    if (left == nullptr) {
      left.reset(new Node(leftName));
      nodes[leftName] = left;
    }
    root->left = left;

    const std::string &rightName = tokens[2];
    std::shared_ptr<Node> right = nodes[rightName];
    if (right == nullptr) {
      right.reset(new Node(rightName));
      nodes[rightName] = right;
    }
    root->right = right;
  }

  std::cout << "Steps (Part I) : " << solveSteps(nodes["AAA"], path)
            << std::endl;

  // Part II
  std::vector<AocStepType> steps;
  for (auto &entry : nodes) {
    if (entry.first[2] == 'A') {
      steps.push_back(solveSteps(entry.second, path));
    }
  }

  // LCM of all the steps is the answer
  std::unordered_map<AocStepType, int> lcmFactors;
  for (auto step : steps) {
    auto factors = factorize(step);
    for (auto factor : factors) {
      if (lcmFactors[factor.first] < factor.second) {
        lcmFactors[factor.first] = factor.second;
      }
    }
  }

  AocStepType lcm = 1;
  for (auto factor : lcmFactors) {
    for (int i = 0; i < factor.second; i++) {
      lcm *= factor.first;
    }
  }

  std::cout << "Steps (Part II) : " << lcm << std::endl;
}
