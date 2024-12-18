#include "aocutils.hh"
#include "logger.hh"
#include "pair_utils.hh"
#include <cassert>
#include <string>
#include <unordered_set>

static Logger logger("day10");

// returns ratings of the trail starting from i, j
int build_trail(const std::vector<std::string>& input, int i, int j, char prev,
                std::unordered_set<std::pair<int, int>, HashIntPair>& visited_peaks) {
    if (i < 0 || i == input.size() || j < 0 || j == input[i].size() || input[i][j] != prev + 1) {
        return 0;
    }

    if (input[i][j] == '9') {
        visited_peaks.insert({i, j});
        logger.debug("{}, {} '9' : {}", i, j, 1);
        return 1;
    }

    // traverse up, down, left and right
    int ratings = 0;
    for (auto [next_i, next_j] :
         {std::pair<int, int>{i, j - 1}, {i, j + 1}, {i - 1, j}, {i + 1, j}}) {
        ratings += build_trail(input, next_i, next_j, input[i][j], visited_peaks);
    }

    return ratings;
}

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(10, 2024, &args).readAllAsVectorOfStrings();

    int total_trails = 0;
    int ratings = 0;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == '0') {
                std::unordered_set<std::pair<int, int>, HashIntPair> visited_peaks;
                ratings += build_trail(input, i, j, '0' - 1, visited_peaks);
                total_trails += visited_peaks.size();
            }
        }
    }

    logger.info("Part I  : Sum of scores of all trailheads  : {}", total_trails);
    logger.info("Part II : Sum of ratings of all trailheads : {}", ratings);
}
