#include <aocutils.hh>
#include <bitset>
#include <iostream>
#include <pair_utils.hh>
#include <unordered_map>

constexpr char EmptySpace = '.';
constexpr char Mirror1 = '\\';
constexpr char Mirror2 = '/';
constexpr char VerticalSplitter = '|';
constexpr char HorizontalSplitter = '-';

enum Direction { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3 };

Direction reflectBeam(char mirror, Direction dir) {
  switch (dir) {
  case NORTH:
    return (mirror == Mirror1) ? WEST : EAST;
    break;
  case SOUTH:
    return (mirror == Mirror1) ? EAST : WEST;
    break;
  case WEST:
    return (mirror == Mirror1) ? NORTH : SOUTH;
    break;
  case EAST:
    return (mirror == Mirror1) ? SOUTH : NORTH;
    break;
  }
}

void solve(std::vector<std::string> &contraption, int i, int j, Direction dir,
           std::unordered_map<std::pair<int, int>, std::bitset<4>, HashIntPair>
               &energizedTiles);

void move(std::vector<std::string> &contraption, int i, int j, Direction dir,
          std::unordered_map<std::pair<int, int>, std::bitset<4>, HashIntPair>
              &energizedTiles) {
  switch (dir) {
  case NORTH:
    i--;
    break;
  case SOUTH:
    i++;
    break;
  case WEST:
    j--;
    break;
  case EAST:
    j++;
    break;
  }

  /*std::cout << i << " " << j << " "
            << (dir == NORTH ? "NORTH"
                             : (dir == SOUTH ? "SOUTH"
                                             : (dir == EAST ? "EAST" : "WEST")))
            << std::endl;*/
  solve(contraption, i, j, dir, energizedTiles);
}

void solve(std::vector<std::string> &contraption, int i, int j, Direction dir,
           std::unordered_map<std::pair<int, int>, std::bitset<4>, HashIntPair>
               &energizedTiles) {
  if (i < 0 || i == contraption.size() || j < 0 || j == contraption[0].size() ||
      energizedTiles[{i, j}][dir] == true) {
    return;
  }

  energizedTiles[{i, j}][dir] = true;

  // calculate the next tile
  switch (contraption[i][j]) {
  case EmptySpace:
    // do nothing and move
    move(contraption, i, j, dir, energizedTiles);
    break;
  case Mirror1:
  case Mirror2:
    dir = reflectBeam(contraption[i][j], dir);
    move(contraption, i, j, dir, energizedTiles);
    break;
  case VerticalSplitter:
    if (dir == NORTH || dir == SOUTH) {
      move(contraption, i, j, dir, energizedTiles);
    } else {
      // split the beam to NORTH and SOUTH
      move(contraption, i, j, NORTH, energizedTiles);
      move(contraption, i, j, SOUTH, energizedTiles);
    }
    break;
  case HorizontalSplitter:
    if (dir == WEST || dir == EAST) {
      move(contraption, i, j, dir, energizedTiles);
    } else {
      // split the beam to NORTH and SOUTH
      move(contraption, i, j, WEST, energizedTiles);
      move(contraption, i, j, EAST, energizedTiles);
    }
    break;
  }
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(16, &args);

  std::vector<std::string> contraption;
  for (auto it = input.begin(); it != input.end(); ++it) {
    contraption.push_back(*it);
  }

  // part I
  std::unordered_map<std::pair<int, int>, std::bitset<4>, HashIntPair>
      energizedTiles;
  solve(contraption, 0, 0, EAST, energizedTiles);
  std::cout << "Number of tiles energized (Part I) : " << energizedTiles.size()
            << std::endl;

  // Part II
  int maxEnergizedTiles = 0;
  for (int j = 0; j < contraption[0].size(); j++) {
    energizedTiles.clear();
    solve(contraption, 0, j, SOUTH, energizedTiles);
    if (energizedTiles.size() > maxEnergizedTiles) {
      maxEnergizedTiles = energizedTiles.size();
    }

    energizedTiles.clear();
    solve(contraption, contraption.size() - 1, j, NORTH, energizedTiles);
    if (energizedTiles.size() > maxEnergizedTiles) {
      maxEnergizedTiles = energizedTiles.size();
    }
  }

  for (int i = 0; i < contraption.size(); i++) {
    energizedTiles.clear();
    solve(contraption, i, 0, EAST, energizedTiles);
    if (energizedTiles.size() > maxEnergizedTiles) {
      maxEnergizedTiles = energizedTiles.size();
    }

    energizedTiles.clear();
    solve(contraption, i, contraption[0].size() - 1, WEST, energizedTiles);
    if (energizedTiles.size() > maxEnergizedTiles) {
      maxEnergizedTiles = energizedTiles.size();
    }
  }

  std::cout << "Maximum possible number of tiles energized (Part II) : "
            << maxEnergizedTiles << std::endl;
}
