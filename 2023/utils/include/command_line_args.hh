#pragma once

#include <string>
#include <unordered_set>

class AocCmdLineArgs {
  std::unordered_set<std::string> _args;

public:
  AocCmdLineArgs(int argc, char *argv[]);

  template <typename... CommandLineArgs>
  bool hasBoolFlag(CommandLineArgs... args) const {
    for (const std::string &flag : {args...}) {
      if (_args.find(flag) != _args.end()) {
        return true;
      }
    }

    return false;
  }

  bool useExampleInput() const;
};
