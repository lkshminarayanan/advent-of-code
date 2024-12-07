#pragma once

#include "file_iterator.hh"
#include <vector>

class AocCmdLineArgs;

class InputReader {
private:
  std::string _inputFilePath;

  void downloadInput(const std::string &url);

public:
  InputReader(int day, int year, bool useExample = false);
  // constructor to read args from cmd line
  InputReader(int day, int year, AocCmdLineArgs *args);

  // backward compatibility constructors for 2023
  InputReader(int day, bool useExample = false) : InputReader(day, 2023, useExample) {}
  InputReader(int day, AocCmdLineArgs *args) : InputReader(day, 2023, args) {}

  // File iterators for the input
  auto begin() const { return FileIterator(_inputFilePath); }
  auto end() const { return FileIteratorEnd(); }

  std::string readAll();
  std::vector<std::string> readAllAsVectorOfStrings();
};
