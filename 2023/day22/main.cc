#include "string_helper.hh"
#include <algorithm>
#include <aocutils.hh>
#include <format>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

enum BrickType : short { BTX, BTY, BTZ };

struct Brick {
  short x1, y1, z1, x2, y2, z2;
  BrickType type{BTX};

  bool fallen{false};

  std::unordered_set<Brick *> supportingBricks;
  std::unordered_set<Brick *> dependantBricks;

  void addSupportingBrick(Brick *brick) {
    supportingBricks.insert(brick);
    brick->dependantBricks.insert(this);
  }

  Brick(short _x1, short _y1, short _z1, short _x2, short _y2, short _z2)
      : x1(_x1), y1(_y1), z1(_z1), x2(_x2), y2(_y2), z2(_z2) {}

  void fall(std::vector<std::vector<std::vector<Brick *>>> &graph) {
    switch (type) {
    case BTX: {
      short zNew = z1 - 1;
      while (zNew != 0 && supportingBricks.empty()) {
        for (short i = x1; i <= x2; i++) {
          auto brickAtNewLocation = graph.at(i).at(y1).at(zNew);
          if (brickAtNewLocation != nullptr) {
            // there is already a brick
            addSupportingBrick(brickAtNewLocation);
          }
        }

        if (supportingBricks.empty()) {
          // no obstructing bricks at this height
          // move down one level further
          zNew--;
        }
      }

      zNew++;
      if (z1 != zNew) {
        // brick can fall;
        for (short i = x1; i <= x2; i++) {
          graph[i][y1][z1] = nullptr;
        }

        z1 = z2 = zNew;
        for (short i = x1; i <= x2; i++) {
          graph[i][y1][z1] = this;
        }
      }
    } break;

    case BTY: {
      short zNew = z1 - 1;
      while (zNew != 0 && supportingBricks.empty()) {
        for (short j = y1; j <= y2; j++) {
          auto brickAtNewLocation = graph.at(x1).at(j).at(zNew);
          if (brickAtNewLocation != nullptr) {
            // there is already a brick
            addSupportingBrick(brickAtNewLocation);
          }
        }

        if (supportingBricks.empty()) {
          // no obstructing bricks at this height
          // move down one level further
          zNew--;
        }
      }

      zNew++;
      if (z1 != zNew) {
        // brick can fall;
        for (short j = y1; j <= y2; j++) {
          graph[x1][j][z1] = nullptr;
        }

        z1 = z2 = zNew;
        for (short j = y1; j <= y2; j++) {
          graph[x1][j][z1] = this;
        }
      }
    } break;

    case BTZ: {
      short zNew = z1 - 1;
      for (; zNew > 0; zNew--) {
        auto brickAtNewLocation = graph.at(x1).at(y1).at(zNew);
        if (brickAtNewLocation != nullptr) {
          addSupportingBrick(brickAtNewLocation);
          break;
        }
      }

      zNew++;
      if (z1 != zNew) {
        // brick can fall;
        for (short k = z1; k <= z2; k++) {
          graph[x1][y1][k] = nullptr;
        }

        z2 -= (z1 - zNew);
        z1 = zNew;
        for (short k = z1; k <= z2; k++) {
          graph[x1][y1][k] = this;
        }
      }
    } break;
    }

    fallen = true;
  }

  int getTotalDependants() {

    std::unordered_set<Brick *> disintegratedBricks;
    disintegratedBricks.insert(this);
    std::queue<Brick *> bq;
    for (auto brick : dependantBricks) {
      bq.push(brick);
    }

    while (!bq.empty()) {
      Brick *brick = bq.front();
      bq.pop();

      bool supportersDisintegrated = true;
      for (auto supporterBrick : brick->supportingBricks) {
        if (!disintegratedBricks.contains(supporterBrick)) {
          supportersDisintegrated = false;
          break;
        }
      }

      if (supportersDisintegrated) {
        disintegratedBricks.insert(brick);
        for (auto dependentBrick : brick->dependantBricks) {
          bq.push(dependentBrick);
        }
      }
    }

    return disintegratedBricks.size() - 1;
  }
};

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(22, &args);

  int xMax = 10, yMax = 10, zMax = 316;
  if (args.useExampleInput()) {
    xMax = 3;
    yMax = 3;
    zMax = 10;
  }

  std::vector<std::unique_ptr<Brick>> bricks;
  std::vector<std::vector<std::vector<Brick *>>> graph(
      10, std::vector<std::vector<Brick *>>(
              10, std::vector<Brick *>(316, nullptr)));

  for (auto it = input.begin(); it != input.end(); ++it) {
    auto tokens = splitToIntegers(*it, ",~");
    auto x1 = tokens[0];
    auto y1 = tokens[1];
    auto z1 = tokens[2];
    auto x2 = tokens[3];
    auto y2 = tokens[4];
    auto z2 = tokens[5];

    auto brick = std::make_unique<Brick>(x1, y1, z1, x2, y2, z2);
    auto brickPtr = brick.get();

    while (x1 < x2) {
      graph[x1++][y1][z1] = brickPtr;
      brick->type = BTX;
    }

    while (y1 < y2) {
      graph[x1][y1++][z1] = brickPtr;
      brick->type = BTY;
    }

    while (z1 < z2) {
      graph[x1][y1][z1++] = brickPtr;
      brick->type = BTZ;
    }

    graph[x2][y2][z2] = brickPtr;

    bricks.push_back(std::move(brick));
  }

  for (int k = 0; k < zMax; k++) {
    for (int i = 0; i < xMax; i++) {
      for (int j = 0; j < yMax; j++) {
        auto brick = graph.at(i).at(j).at(k);
        if (brick == nullptr || brick->fallen) {
          continue;
        }

        // brick has to fall down
        brick->fall(graph);
      }
    }
  }

  std::unordered_set<Brick *> requiredBricks;
  for (const auto &brick : bricks) {
    if (brick->supportingBricks.size() == 1) {
      auto brickIt = brick->supportingBricks.begin();
      requiredBricks.insert(*brickIt);
    }
  }

  std::cout << "Number of bricks that can be safely disintegrated (Part I) : "
            << (bricks.size() - requiredBricks.size()) << std::endl;

  // Part II
  int totalDependents = 0;
  for (const auto &brick : bricks) {
    totalDependents += brick->getTotalDependants();
  }

  std::cout << "Sum of all other bricks that would fall (Part II) : "
            << totalDependents << std::endl;
}
