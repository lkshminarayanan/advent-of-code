#pragma once

#include "file_iterator.hh"
#include <fstream>

class InputReader {
private:
  std::string _inputFilePath;
  std::fstream _inputFS;

  void downloadInput(const std::string &url);

public:
  InputReader(int day, bool useExample = false);

  // File iterators for the input
  auto begin() const { return FileIterator(_inputFilePath); }
  auto end() const { return FileIteratorEnd(); }
};
