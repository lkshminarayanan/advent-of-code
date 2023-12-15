#include "command_line_args.hh"

AocCmdLineArgs::AocCmdLineArgs(int argc, char *argv[]) {
  if (argc == 1) {
    return;
  }

  _args.insert(argv + 1, argv + argc);
}

bool AocCmdLineArgs::useExampleInput() const {
  return _argsContains("--useExample");
}
