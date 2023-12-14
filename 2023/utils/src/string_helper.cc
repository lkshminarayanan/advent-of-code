#include "string_helper.hh"

void splitToCustomContainer(
    const std::string &line,
    std::function<void(const std::string &)> tokenStorer,
    const std::string &separators) {
  int pos = 0;
  while (pos < line.size()) {
    auto sepPos = line.find_first_of(separators, pos);
    if (sepPos == std::string::npos) {
      // end of separators
      tokenStorer(line.substr(pos));
      break;
    }

    if (sepPos - pos > 0) {
      tokenStorer(line.substr(pos, sepPos - pos));
    }

    pos = sepPos + 1;
  }
}

std::vector<std::string> split(const std::string &line,
                               const std::string &separators) {

  std::vector<std::string> tokens;
  splitToCustomContainer(
      line, [&tokens](const std::string &token) { tokens.push_back(token); },
      separators);
  return tokens;
}

std::vector<int> splitToIntegers(const std::string &line,
                                 const std::string &separators) {
  std::vector<int> numbers;
  splitToCustomContainer(
      line,
      [&numbers](const std::string &token) {
        numbers.push_back(std::stoi(token));
      },
      separators);
  return numbers;
}

std::vector<long> splitToLong(const std::string &line,
                              const std::string &separators) {
  std::vector<long> numbers;
  splitToCustomContainer(
      line,
      [&numbers](const std::string &token) {
        numbers.push_back(std::stol(token));
      },
      separators);
  return numbers;
}
