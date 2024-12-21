#include "aocutils.hh"
#include "logger.hh"
#include <functional>
#include <initializer_list>
#include <set>

static Logger logger("day12");

using point = std::tuple<int, int, int>;
using border_type = std::multiset<point>;
using visited_type = std::vector<std::vector<bool>>;

// returns area
long calculate_fence_requirements(std::vector<std::string>& input, int i, int j, const char plant,
                                  visited_type& visited, border_type& border) {
    if (i < 0 || i == input.size() || j < 0 || j == input[i].size()) {
        return -1;
    }

    if (input[i][j] != plant) {
        return -1;
    }

    if (visited[i][j]) {
        // same plant but already visited
        return 0;
    }

    // mark it as visited
    visited[i][j] = true;

    // visit neighbors
    long area = 1;
    int dir = 0;
    for (auto [next_i, next_j] :
         {std::pair<int, int>{i, j - 1}, {i - 1, j}, {i, j + 1}, {i + 1, j}}) {
        if (next_i == 8 && next_j == 92) {
            logger.debug("Next point : {}, {}; dir : {}; source : {}, {}", next_i, next_j, dir, i,
                         j);
        }
        auto neighbor_area =
            calculate_fence_requirements(input, next_i, next_j, plant, visited, border);
        if (neighbor_area == -1) {
            // we crossed a border
            // track direction, to use it later to deduce sides
            border.insert({next_i, next_j, dir});
        } else {
            area += neighbor_area;
        }
        dir++;
    }

    return area;
}

// count lines formed by the border points
int count_sides(border_type& border) {
    int sides = 0;
    while (!border.empty()) {
        auto border_point = border.extract(border.begin()).value();
        sides++;
        logger.debug("New Side : {}, {}; dir : {}", std::get<0>(border_point),
                     std::get<1>(border_point), std::get<2>(border_point));

        // remove a line of points
        for (auto next_point_func : std::initializer_list<std::function<point(point)>>{
                 [](point p) -> point {
                     return {std::get<0>(p) + 1, std::get<1>(p), std::get<2>(p)};
                 },
                 [](point p) -> point {
                     return {std::get<0>(p), std::get<1>(p) + 1, std::get<2>(p)};
                 }}) {
            if (!border.contains(next_point_func(border_point))) {
                continue;
            }
            while (true) {
                auto next_point = next_point_func(border_point);
                auto extracted = border.extract(next_point);
                if (extracted.empty()) {
                    break;
                }
                logger.debug("{}, {}, {} ==> {}, {}, {}", std::get<0>(border_point),
                             std::get<1>(border_point), std::get<2>(border_point),
                             std::get<0>(next_point), std::get<1>(next_point),
                             std::get<2>(next_point));
                border_point = extracted.value();
            }
            break;
        }
    }

    return sides;
}

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(12, 2024, &args).readAllAsVectorOfStrings();

    visited_type visited;
    for (int i = 0; i < input.size(); i++) {
        visited.push_back(std::vector<bool>(input[i].size(), false));
    }

    long fencing_cost_part1 = 0;
    long fencing_cost_part2 = 0;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (!visited[i][j]) {
                const char plant = input[i][j];
                border_type border;
                auto area = calculate_fence_requirements(input, i, j, input[i][j], visited, border);
                fencing_cost_part1 += area * border.size();
                auto sides = count_sides(border);
                fencing_cost_part2 += area * sides;
            }
        }
    }

    logger.info("Part I  : Total Price of fencing all regions : {}", fencing_cost_part1);
    logger.info("Part II : Total Price of fencing all regions with bulk discount : {}",
                fencing_cost_part2);
}
