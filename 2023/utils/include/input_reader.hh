#pragma once

#include "file_iterator.hh"
#include <fstream>

class AocCmdLineArgs;

class InputReader {
private:
  std::string _inputFilePath;

  void downloadInput(const std::string &url);

public:
  InputReader(int day, bool useExample = false);
  // constructor to read args from cmd line
  InputReader(int day, AocCmdLineArgs *args);

  // File iterators for the input
  auto begin() const { return FileIterator(_inputFilePath); }
  auto end() const { return FileIteratorEnd(); }
};
