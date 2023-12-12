#include "string_helper.hh"

std::vector<std::string> split(const std::string &line,
                               const std::string &separators) {

  std::vector<std::string> tokens;

  int pos = 0;
  while (pos < line.size()) {
    auto sepPos = line.find_first_of(separators, pos);
    if (sepPos == std::string::npos) {
      // end of separators
      tokens.push_back(line.substr(pos));
      break;
    }

    if (sepPos - pos > 0) {
      tokens.push_back(line.substr(pos, sepPos - pos));
    }

    pos = sepPos + 1;
  }

  return tokens;
}
