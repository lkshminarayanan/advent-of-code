#include "aocutils.hh"
#include <algorithm>
#include <sstream>
#include <unordered_map>

int find_distance(const std::vector<int> &v1, const std::vector<int> &v2) {
  int distance = 0;
  for (int i = 0; i < v1.size(); ++i) {
    distance += std::abs(v1[i] - v2[i]);
  }
  return distance;
}

int find_similarity(const std::vector<int> &v1, const std::vector<int> &v2) {
  std::unordered_map<int, int> v2_count_map;
  for (int i = 0; i < v2.size(); ++i) {
    v2_count_map[v2[i]]++;
  }

  int similarity = 0;
  for (int i = 0; i < v1.size(); ++i) {
    if (v2_count_map[v1[i]] > 0) {
      similarity += v1[i] * v2_count_map[v1[i]];
    }
  }

  return similarity;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(1, 2024, &args);

  std::vector<int> v1, v2;

  int num1, num2;
  for (auto it = input.begin(); it != input.end(); ++it) {
    std::stringstream ss(*it);
    ss >> num1 >> num2;
    v1.push_back(num1);
    v2.push_back(num2);
  }

  // sort both the vectors
  std::ranges::sort(v1);
  std::ranges::sort(v2);

  std::cout << "Part I : Distance : " << find_distance(v1, v2) << std::endl;
  std::cout << "Part II : Similarity : " << find_similarity(v1, v2) << std::endl;
}
