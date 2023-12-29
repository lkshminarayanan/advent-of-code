#pragma once

#include <fstream>
#include <string_view>

// Iterators for the file
class FileIteratorEnd {};

class FileIterator {
public:
  FileIterator(const std::string &filepath);
  ~FileIterator() { _fs.close(); }

  const std::string &operator*() const { return _item; }
  const std::string *operator->() const { return &_item; }

  FileIterator &operator++();

  bool operator!=(FileIteratorEnd) const { return _has_more; }

private:
  void readline();

  std::fstream _fs;
  std::string _item;
  bool _has_more = {false};
};
