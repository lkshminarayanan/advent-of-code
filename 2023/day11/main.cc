#include <aocutils.hh>
#include <iostream>
#include <set>

constexpr char GALAXY = '#';

long solve(InputReader &input, int expansionFactor) {

  std::vector<std::string> image;
  std::set<int> columnsWithGalaxies;

  int i = 0;
  for (auto it = input.begin(); it != input.end(); ++it) {
    const std::string &line = *it;
    image.push_back(line);

    int j = 0;
    bool hasGalaxy = false;
    while (j < line.size()) {
      j = line.find(GALAXY, j);

      if (j == line.npos) {
        break;
      }

      hasGalaxy = true;
      columnsWithGalaxies.insert(j);
      j++;
    }

    if (!hasGalaxy) {
      // no galaxy - expand this row
      for (int ef_i = 0; ef_i < expansionFactor - 1; ef_i++) {
        image.push_back(line);
      }
    }

    i++;
  }

  int prev = -1, columnsAdded = 0;
  for (auto col : columnsWithGalaxies) {
    while (prev != col - 1) {
      // missing a value - add columns at idx prev + 1
      for (int i = 0; i < image.size(); i++) {
        // expand column
        image[i].insert(prev + 1 + columnsAdded,
                        std::string(expansionFactor - 1, '.'));
      }
      prev++;
      columnsAdded += expansionFactor - 1;
    }

    prev = col;
  }

  // Deduce the location of all galaxies
  std::vector<std::pair<int, int>> galaxies;
  for (int i = 0; i < image.size(); i++) {
    for (int j = 0; j < image[0].size(); j++) {
      if (image[i][j] == GALAXY) {
        galaxies.push_back({i, j});
      }
    }
  }

  long sum = 0;
  for (int i = 0; i < galaxies.size() - 1; i++) {
    for (int j = i + 1; j < galaxies.size(); j++) {
      sum += std::abs(galaxies[j].first - galaxies[i].first) +
             std::abs(galaxies[j].second - galaxies[i].second);
    }
  }

  return sum;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(11, &args);

  long distWithoutExpansion = solve(input, 1);
  long distWithDoubleExpansion = solve(input, 2);
  long distIncrease = distWithDoubleExpansion - distWithoutExpansion;

  auto distance = [](long distWithoutExpansion, long distIncrease,
                     long expansionFactor) {
    std::cout << "Sum of all the shortest paths between galaxies, with "
                 "expansion factor `"
              << expansionFactor << "` : "
              << distWithoutExpansion + (expansionFactor - 1) * distIncrease
              << std::endl;
  };

  std::cout << "Part I : ";
  distance(distWithoutExpansion, distIncrease, 2);
  std::cout << "Part II : ";
  distance(distWithoutExpansion, distIncrease, 1000000);
}
