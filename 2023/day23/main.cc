#include "pair_utils.hh"
#include <algorithm>
#include <aocutils.hh>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

constexpr char FOREST = '#';
constexpr char PATH = '.';

class Graph {
  std::vector<std::string> _input;
  std::pair<int, int> _start, _end;

  std::unordered_map<std::pair<int, int>,
                     std::unordered_map<std::pair<int, int>, int, HashIntPair>,
                     HashIntPair>
      _adj_list;

  std::function<std::vector<std::pair<int, int>>(int i, int j, const char curr)>
      _get_next_paths_impl;

  void print_input() {
    std::cout << std::string(_input.size(), '-') << std::endl;
    for (const auto &line : _input) {
      std::cout << line << std::endl;
    }
    std::cout << std::string(_input.size(), '-') << std::endl;
  }

  bool valid_vertex(int i, int j) const {
    return i >= 0 && i < _input.size() && j >= 0 && j < _input[0].size() &&
           _input.at(i).at(j) != FOREST;
  }

  std::vector<std::pair<int, int>> get_next_paths_part1(int i, int j,
                                                        const char curr) {
    switch (curr) {
    case '>':
      return std::vector<std::pair<int, int>>{{i, j + 1}};
    case '<':
      return std::vector<std::pair<int, int>>{{i, j - 1}};
    case '^':
      return std::vector<std::pair<int, int>>{{i - 1, j}};
    case 'v':
      return std::vector<std::pair<int, int>>{{i + 1, j}};
    default:
      return std::vector<std::pair<int, int>>{
          {i, j + 1}, {i, j - 1}, {i + 1, j}, {i - 1, j}};
    }
  }

  std::vector<std::pair<int, int>> get_next_paths_part2(int i, int j,
                                                        const char curr) {
    return std::vector<std::pair<int, int>>{
        {i, j + 1}, {i, j - 1}, {i + 1, j}, {i - 1, j}};
  }

  std::vector<std::pair<int, int>> get_next_paths(int i, int j,
                                                  const char curr) {
    std::vector<std::pair<int, int>> paths;
    for (auto &path : _get_next_paths_impl(i, j, curr)) {
      if (valid_vertex(path.first, path.second)) {
        paths.push_back(path);
      }
    }

    return paths;
  }

  void set_part(int part) {
    switch (part) {
    case 1:
      _get_next_paths_impl =
          std::bind(&Graph::get_next_paths_part1, this, std::placeholders::_1,
                    std::placeholders::_2, std::placeholders::_3);
      break;
    case 2:
      _get_next_paths_impl =
          std::bind(&Graph::get_next_paths_part2, this, std::placeholders::_1,
                    std::placeholders::_2, std::placeholders::_3);
      break;
    }
  }

  using visited_vertices = std::unordered_set<std::pair<int, int>, HashIntPair>;

  void find_neighbors(std::pair<int, int> &curr, int i, int j, int distance,
                      std::queue<std::pair<int, int>> &q,
                      visited_vertices &visited) {
    auto candidate = std::make_pair(i, j);
    if (_end == candidate) {
      // reached end
      _adj_list[curr][candidate] = distance;
      return;
    }

    char &curr_char = _input.at(i).at(j);
    char actual = curr_char;
    curr_char = FOREST;

    auto paths = get_next_paths(i, j, actual);

    if (paths.size() > 1 && !(curr.first == i && curr.second == j)) {
      // neighbor
      _adj_list[curr][candidate] = distance;
      if (!visited.contains(candidate)) {
        // neighbor unvisited - add to queue for later exploration
        q.push(candidate);
      }
      curr_char = actual;
      return;
    }

    // explore paths in other cases
    for (auto &path : paths) {
      find_neighbors(curr, path.first, path.second, distance + 1, q, visited);
    }

    curr_char = actual;
  }

  int dfs_adj_list(std::pair<int, int> curr, visited_vertices &visited) {
    if (visited.contains(curr)) {
      return -1;
    }

    if (curr == _end) {
      return 0;
    }

    visited.insert(curr);

    // extend path through all neigbors and find max_path
    int max_distance = -1;
    for (const auto &neighbor : _adj_list.at(curr)) {
      auto distance = dfs_adj_list(neighbor.first, visited);
      if (distance != -1) {
        max_distance = std::max(distance + neighbor.second, max_distance);
      }
    }

    visited.erase(curr);

    return max_distance;
  }

public:
  Graph(std::vector<std::string> &input) : _input(std::move(input)) {
    // locate start and end points
    _start = std::make_pair(0, _input[0].find_first_not_of(FOREST));
    _end = std::make_pair(_input.size() - 1,
                          _input[_input.size() - 1].find_first_not_of(FOREST));
  }

  void print_adjacency_list() {
    for (const auto &entry : _adj_list) {
      for (const auto &sub_entry : entry.second) {
        std::cout << entry.first.first << ", " << entry.first.second << " --> "
                  << sub_entry.first.first << ", " << sub_entry.first.second
                  << " : " << sub_entry.second << std::endl;
      }
    }
  }

  void reset_adjacency_list() { _adj_list.clear(); }

  // Convert the given input into a graph that has only the co-ordinates where multiple paths join as vertices
  void generate_adjacency_list(short part) {
    set_part(part);

    visited_vertices visited;
    std::queue<std::pair<int, int>> q;
    q.push(_start);

    while (!q.empty()) {
      auto curr = q.front();
      q.pop();

      if (visited.contains(curr)) {
        continue;
      }

      visited.insert(curr);
      find_neighbors(curr, curr.first, curr.second, 0, q, visited);
    }
  }

  int get_longest_possible_hike_distance() {
    visited_vertices visited;
    return dfs_adj_list(_start, visited);
  }
};

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input_reader(23, &args);

  std::vector<std::string> input;

  for (auto it = input_reader.begin(); it != input_reader.end(); ++it) {
    input.push_back(*it);
  }

  Graph graph(input);
  graph.generate_adjacency_list(1);
  std::cout << "Number of steps in the longest hike (Part I) : "
            << graph.get_longest_possible_hike_distance() << std::endl;

  graph.reset_adjacency_list();
  graph.generate_adjacency_list(2);
  std::cout << "Number of steps in the longest hike (Part II) : "
            << graph.get_longest_possible_hike_distance() << std::endl;
}
