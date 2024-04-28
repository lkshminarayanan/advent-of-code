#include <cstdlib>
#include <iostream>
#include <format>
#include <queue>
#include <utility>
#include <fstream>

using vertex = std::pair<int, int>;
constexpr vertex invalid_vertex = {-1,-1};

struct pq_item {
  int distance;
  vertex v;

  pq_item(int _distance, const vertex &_v) : distance(_distance), v(_v) {}
  pq_item(const pq_item &item) : distance(item.distance), v(item.v) {}

  bool operator> (const pq_item &other) const {
    return distance > other.distance;
  }
};

int find_shortest_path(const std::vector<std::string> &graph,
                       const vertex &start, const vertex end = invalid_vertex) {
  std::priority_queue<pq_item, std::vector<pq_item>, std::greater<pq_item>> pq;
  pq.emplace(0, start);
  std::vector<std::vector<bool>> visited;
  for (int i = 0; i < graph.size(); i++) {
    visited.push_back(std::vector<bool>(graph[i].size(), false));
  }

  while (!pq.empty()) {
    // choose the curr vertex with smallest path
    const auto curr = pq.top();
    pq.pop();

    const auto &v_pt = curr.v;
    const auto &v = graph[v_pt.first][v_pt.second];
    if (visited[v_pt.first][v_pt.second]) {
      continue;
    }

    // for part 2, traverse and reach the first 'a' from end.
    if ((end == invalid_vertex && v == 'a') || v_pt == end) {
      return curr.distance;
    }

    // update neighbor distances
    for (auto neigh : std::vector<vertex>{
             {v_pt.first, v_pt.second - 1},
             {v_pt.first, v_pt.second + 1},
             {v_pt.first - 1, v_pt.second},
             {v_pt.first + 1, v_pt.second},
         }) {
      if (neigh.first < 0 || neigh.second < 0 || neigh.first >= graph.size() ||
          neigh.second >= graph[0].size() || visited[v_pt.first][v_pt.second]) {
        continue;
      }

      auto neigh_val = graph[neigh.first][neigh.second];
      if (end == invalid_vertex) {
        // part 2 => going from reverse
        if (v - neigh_val > 1) {
          continue;
        }
      } else if (neigh_val - v > 1) {
        // part 1
        continue;
      }

      // update their distance => just add the new distance and let pq take care
      // of giving us the shortest
      pq.emplace(curr.distance + 1, neigh);
    }

    visited[v_pt.first][v_pt.second] = true;

  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Please pass input file name" << std::endl;
    std::exit(-1);
  }

  std::fstream input_fs;
  input_fs.open(argv[1], std::ios::in);
  if (!input_fs.is_open()) {
    std::cerr << "Failed to open input" << std::endl;
    std::exit(-1);
  }

  // read the file
  std::string line;
  std::vector<std::string> input;
  vertex start{-1, -1}, end{-1, -1}, invalid{-1, -1};
  std::string::size_type char_it;
  while (std::getline(input_fs, line)) {
    input.push_back(line);

    if (start == invalid && (char_it = line.find_first_of('S')) != std::string::npos) {
      start = std::make_pair(input.size() - 1, char_it);
      input[input.size() - 1][char_it] = 'a';
    }

    if (end == invalid && (char_it = line.find_first_of('E')) != std::string::npos) {
      end = std::make_pair(input.size() - 1, char_it);
      input[input.size() - 1][char_it] = 'z';
    }
  }
  input_fs.close();

  std::cout << std::format("Fewest required steps (Part I) : {}\n",
                           find_shortest_path(input, start, end));
  std::cout << std::format("Fewest required steps (Part II) : {}\n",
                           find_shortest_path(input, end));

}

