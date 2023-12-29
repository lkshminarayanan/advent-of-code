#include <aocutils.hh>
#include <iostream>
#include <sstream>

using AocType = long long;

void addNextCoords(std::vector<std::pair<AocType, AocType>> &coords, char dir,
                   AocType dist) {
  std::pair<AocType, AocType> next = coords.back();
  switch (dir) {
  case 'R':
  case '0':
    next.second += dist;
    break;
  case 'D':
  case '1':
    next.first += dist;
    break;
  case 'L':
  case '2':
    next.second -= dist;
    break;
  case 'U':
  case '3':
    next.first -= dist;
    break;
  }

  coords.push_back(next);
}

AocType calculateArea(const std::vector<std::pair<AocType, AocType>> &coords,
                      AocType perimeter) {
  AocType areaUsingShoeLace = 0;
  for (int i = 0; i < coords.size(); i++) {
    const std::pair<AocType, AocType> &p1 = coords[i];
    const std::pair<AocType, AocType> &p2 =
        (i == coords.size() - 1) ? coords[0] : coords[i + 1];
    areaUsingShoeLace += (p1.first * p2.second) - (p1.second * p2.first);
  }

  areaUsingShoeLace = std::abs(areaUsingShoeLace) * 0.5;

  // Shoelace formula doesn't give the correct area here as the borders
  // themselves are cubes. So, calculate the "inner area" using pick's theorem
  // and the area calculated by the shoelace formula and then add border to get
  // actual area that includes the borders.
  // picks theorem : Area = innerArea + (perimeter/2) - 1
  //    => innerArea = Area - (perimeter/2) + 1
  //    and, actualArea = innerArea + perimeter
  //    => actualArea = Area - (perimeter/2) + 1 + perimeter
  return areaUsingShoeLace + (perimeter / 2) + 1;
}

void extractAndAddNextCoords(std::vector<std::pair<AocType, AocType>> &coords,
                             const std::string &code, AocType &perimeter) {
  AocType dist;
  std::stringstream ss;
  ss << std::hex << code.substr(0, 5);
  ss >> dist;

  perimeter += dist;

  addNextCoords(coords, code[5], dist);
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(18, &args);

  std::vector<std::pair<AocType, AocType>> coordsPart1;
  std::vector<std::pair<AocType, AocType>> coordsPart2;
  coordsPart1.push_back({0, 0});
  coordsPart2.push_back({0, 0});

  AocType perimeterPart1 = 0;
  AocType perimeterPart2 = 0;

  for (auto it = input.begin(); it != input.end(); ++it) {
    auto tokens = split(*it, " ()#");
    // extract part 1 coords
    int dist = std::stoi(tokens[1]);
    addNextCoords(coordsPart1, tokens[0][0], dist);
    perimeterPart1 += dist;

    // extract part 2 coords
    extractAndAddNextCoords(coordsPart2, tokens[2], perimeterPart2);
  }

  std::cout << "Lagoon volume (Part I) : "
            << calculateArea(coordsPart1, perimeterPart1) << std::endl;

  std::cout << "Lagoon volume (Part II) : "
            << calculateArea(coordsPart2, perimeterPart2) << std::endl;
}
