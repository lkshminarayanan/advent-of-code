#include "file_iterator.hh"

#include <string>

FileIterator::FileIterator(const std::string &filepath) {
  _fs.open(filepath, std::ios::in);
  if (!_fs.is_open()) {
    // file is not open
    throw std::runtime_error("failed to open the input file");
  }

  readline();
}

FileIterator &FileIterator::operator++() {
  readline();
  return *this;
}

void FileIterator::readline() {
  if (std::getline(_fs, _item)) {
    _has_more = true;
  } else {
    _has_more = false;
  }
}
