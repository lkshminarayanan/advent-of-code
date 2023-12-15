#include <aocutils.hh>
#include <iostream>

using AocType = unsigned long long;

AocType numOfWaysToBeatRecord(const std::string &timeStr,
                              const std::string &recordStr) {
  // find the lowest value of 'n' such that (time-n)*n > record
  // use binary search
  AocType time = std::stoull(timeStr);
  AocType record = std::stoull(recordStr);
  AocType start = 1, end = time / 2;
  AocType n = 0;
  while (start < end) {
    AocType mid = (start + end) / 2;
    AocType dist = (time - mid) * mid;
    if (dist < record) {
      start = mid + 1;
    } else if (dist > record) {
      end = mid - 1;
    } else {
      // dist equal to record => mid + 1 is the value we are looking for
      n = mid + 1;
      break;
    }
  }

  if (n == 0) {
    n = end;
    while ((time - n) * n <= record) {
      n++;
    }
  }

  AocType numOfWays = time - (2 * n) + 1;

  /* std::cout << "time : " << timeStr << "; record : " << recordStr
            << "; n : " << n << "; numOfWays : " << numOfWays << std::endl; */

  return numOfWays;
}

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(6, &args);

  auto it = input.begin();
  auto times = split(*it);
  ++it;
  auto records = split(*it);

  // part 1
  AocType result = 1;
  for (int i = 1; i < times.size(); i++) {
    result *= numOfWaysToBeatRecord(times[i], records[i]);
  }

  std::cout << "Product of ways to beat the record (Part I) : " << result
            << std::endl;

  // part 2
  std::string timeStr, recordStr;
  for (int i = 1; i < times.size(); i++) {
    timeStr += times[i];
    recordStr += records[i];
  }
  result = numOfWaysToBeatRecord(timeStr, recordStr);

  std::cout << "Product of ways to beat the record (Part II) : " << result
            << std::endl;
}
