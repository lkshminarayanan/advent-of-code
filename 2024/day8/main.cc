#include "aocutils.hh"
#include <format>
#include <unordered_set>
#include <utility>
#include <vector>
#include "logger.hh"
#include "pair_utils.hh"

static Logger logger("day8");

using point = std::pair<int, int>;

static constexpr char DOT = '.';

point operator-(const point& p1, const point& p2) {
    return {p1.first - p2.first, p1.second - p2.second};
}

point operator+(const point& p1, const point& p2) {
    return {p1.first + p2.first, p1.second + p2.second};
}

std::pair<int, int> count_antinodes(const std::unordered_map<char, std::vector<point>>& antennas_map, int i_max, int j_max) {
    std::unordered_set<point, HashIntPair> antinodes_part1;
    std::unordered_set<point, HashIntPair> antinodes_part2;

    auto insert_antinode = [i_max, j_max](const point& p,
                                          std::unordered_set<point, HashIntPair>& antinodes_set) {
        if (p.first < 0 || p.first >= i_max || p.second < 0 || p.second >= j_max) {
            return false;
        }
        antinodes_set.insert(p);
        return true;
    };

    for (auto [antenna, points] : antennas_map) {
        std::string output;
        for (int i = 0; i < points.size(); i++) {
            for (int j = i + 1; j < points.size(); j++) {
                point diff = points[j] - points[i];
                // Part I
                for (const auto& point : {points[i] - diff, points[j] + diff}) {
                    insert_antinode(point, antinodes_part1);
                }
                // Part II
                auto p = points[i];
                while (insert_antinode(p, antinodes_part2)) {
                    p = p - diff;
                }
                p = points[j];
                while (insert_antinode(p, antinodes_part2)) {
                    p = p + diff;
                }
            }
            output += std::format("({}, {}), ", points[i].first, points[i].second);
        }
        logger.debug("Antenna {} : {}", antenna, output);
    }

    return {antinodes_part1.size(), antinodes_part2.size()};
}

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(8, 2024, &args).readAllAsVectorOfStrings();

    // build a hashmap of antennas
    std::unordered_map<char, std::vector<point>> antennas_map;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] != DOT) {
                antennas_map[input[i][j]].push_back({i, j});
            }
        }
    }

    auto result = count_antinodes(antennas_map, input.size(), input[0].size());
    logger.info("Part I  : Unique antinode locations : {}", result.first);
    logger.info("Part II : Unique antinode locations : {}", result.second);
}
