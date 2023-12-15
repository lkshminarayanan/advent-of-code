#pragma once

#include <string>
#include <unordered_set>

class AocCmdLineArgs {
  std::unordered_set<std::string> _args;

  template <typename... CommandLineArgs>
  bool _argsContains(CommandLineArgs... args) const {
    for (const std::string &flag : {args...}) {
      if (_args.find(flag) != _args.end()) {
        return true;
      }
    }

    return false;
  }

public:
  AocCmdLineArgs(int argc, char *argv[]);

  bool useExampleInput() const;
};
