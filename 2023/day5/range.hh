#pragma once

#include <iostream>

template <typename T> class Range {
  // range included start to end i.e. [start, end]
  T _start;
  T _end;

  Range(T start, T end) : _start(start), _end(end) {}

  bool rangeOverlaps(const Range<T> &r) {
    return !(_end < r._start || _start > r._end);
  }

  std::vector<Range<T> *> trimRangeToFitSource(const Range<T> &source) {
    std::vector<Range<T> *> splitOutRanges;
    if (_start < source._start) {
      splitOutRanges.push_back(new Range<T>(_start, source._start - 1));
      _start = source._start;
    }

    if (_end > source._end) {
      splitOutRanges.push_back(new Range<T>(source._end + 1, _end));
      _end = source._end;
    }

    return splitOutRanges;
  }

public:
  static Range<T> *newRangeWithStartAndRangeLength(T start, T rangeLength) {
    return new Range<T>(start, start + rangeLength - 1);
  }

  T getStart() { return _start; }

  std::string toString() const {
    return std::to_string(_start) + "-->" + std::to_string(_end);
  }

  std::pair<bool, std::vector<Range<T> *>>
  transformRange(const std::vector<T> &map) {
    // map has 0 -> destStart; 1 -> sourceStart; 2 -> rangeLength
    Range<T> sourceRange(map[1], map[1] + map[2] - 1);

    if (!rangeOverlaps(sourceRange)) {
      // no overlap => nothing to do
      return {false, {}};
    }

    auto splitOutRanges = trimRangeToFitSource(sourceRange);

    // Now apply the map and transform
    _start = map[0] + (_start - map[1]);
    _end = map[0] + (_end - map[1]);

    return {true, splitOutRanges};
  }
};
