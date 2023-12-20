#include <aocutils.hh>
#include <iostream>
#include <pair_utils.hh>
#include <unordered_set>

// returns idxs of the pipes the curr pipe connects to
std::vector<std::pair<int, int>>
findConnections(const std::vector<std::string> &sketch,
                std::pair<int, int> &curr) {
  std::pair<int, int> p1 = curr, p2 = curr;
  switch (sketch[curr.first][curr.second]) {
  case '|':
    // north and south
    p1.first--;
    p2.first++;
    break;
  case '-':
    // east and west
    p1.second--;
    p2.second++;
    break;
  case 'L':
    // north and east
    p1.first--;
    p2.second++;
    break;
  case 'J':
    // north and west
    p1.first--;
    p2.second--;
    break;
  case '7':
    // south and west
    p1.first++;
    p2.second--;
    break;
  case 'F':
    // south and east
    p1.first++;
    p2.second++;
    break;
  default:
    // no actions for . and S
    break;
  }

  return std::vector{p1, p2};
}

std::pair<int, int> findNextPipe(const std::vector<std::string> &sketch,
                                 std::pair<int, int> &curr,
                                 std::pair<int, int> &prev) {
  auto connections = findConnections(sketch, curr);
  return (connections[0] == prev) ? connections[1] : connections[0];
}

bool isConnectedToStart(const std::vector<std::string> &sketch,
                        std::pair<int, int> &start,
                        std::pair<int, int> &target) {
  if (target.first < 0 || target.first >= sketch.size() || target.second < 0 ||
      target.second >= sketch[0].size()) {
    return false;
  }

  auto connections = findConnections(sketch, target);
  return (connections[0] == start | connections[1] == start);
}

using PipePathSet = std::unordered_set<std::pair<int, int>, HashIntPair>;

constexpr short NORTH = 1, SOUTH = 1 << 1, EAST = 1 << 2, WEST = 1 << 3;

short pipeValue(std::vector<std::string> &sketch, PipePathSet &path, int i,
                int j) {
  if (path.find({i, j}) == path.end()) {
    return 0;
  }

  short connections = 0;
  switch (sketch[i][j]) {
  case '|':
    // north and south
    connections = NORTH | SOUTH;
    break;
  case '-':
    // east and west
    connections = EAST | WEST;
    break;
  case 'L':
    // north and east
    connections = NORTH | EAST;
    break;
  case 'J':
    // north and west
    connections = NORTH | WEST;
    break;
  case '7':
    // south and west
    connections = SOUTH | WEST;
    break;
  case 'F':
    // south and east
    connections = SOUTH | EAST;
    break;
  default:
    // no actions for . and S
    break;
  }

  return connections;
}

bool isInnerPipe(std::vector<std::string> &sketch, PipePathSet &path, int i,
                 int j) {
  if (path.find({i, j}) != path.end()) {
    return false;
  }

  // check if it is inner pipe
  // it is inner if atleast on one side there exists an odd number of walls
  // between it and the border
  short wallLeft = 0;
  for (int cj = 0; cj < j; cj++) {
    wallLeft ^= pipeValue(sketch, path, i, cj);
  }

  return wallLeft & NORTH && wallLeft & SOUTH;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(10, &args);

  std::vector<std::string> sketch;
  std::pair<int, int> start;
  for (auto it = input.begin(); it != input.end(); ++it) {
    sketch.push_back(*it);
    auto startPos = (*it).find('S');
    if (startPos != std::string::npos) {
      start.first = sketch.size() - 1;
      start.second = startPos;
    }
  }

  // Part I
  auto curr = start;
  // find which way to start
  for (int i = 0; i < 4; i++) {
    switch (i) {
    case 0:
      // try north
      curr.first--;
      break;
    case 1:
      // south
      curr.first += 2;
      break;
    case 2:
      // west
      curr.first--;
      curr.second--;
      break;
    case 3:
      curr.second += 2;
      break;
    }

    if (isConnectedToStart(sketch, start, curr)) {
      // found our start
      break;
    }
  }

  int count = 1;
  std::pair<int, int> prev = start;
  PipePathSet path{start, curr};
  while (curr != start) {
    auto next = findNextPipe(sketch, curr, prev);
    path.insert(next);
    prev = curr;
    curr = next;
    count++;
  }

  std::cout << "Steps to the fartherest (Part I) : " << count / 2 << std::endl;

  int innerPipes = 0;
  for (int i = 0; i < sketch.size(); i++) {
    for (int j = 0; j < sketch[0].size(); j++) {
      if (isInnerPipe(sketch, path, i, j)) {
        innerPipes++;
      }
    }
  }

  std::cout << "Number of inner pipes (Part II) : " << innerPipes << std::endl;
}
