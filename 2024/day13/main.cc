#include "aocutils.hh"
#include "logger.hh"
#include "string_helper.hh"
#include <cmath>
#include <utility>
#include "algebra_utils.hh"

static Logger logger("day13");

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(13, 2024, &args);

    long a1, b1, a2, b2, c1, c2;

    long tokens_required_part1 = 0;
    long tokens_required_part2 = 0;
    for (auto it = input.begin(); it != input.end(); ++it) {
        auto button_a = extract_numbers<long>(*it);
        a1 = button_a[0];
        a2 = button_a[1];
        ++it;

        auto button_b = extract_numbers<long>(*it);
        b1 = button_b[0];
        b2 = button_b[1];
        ++it;

        auto prize = extract_numbers<long>(*it);
        c1 = prize[0];
        c2 = prize[1];
        ++it;

        auto result = solve_equation(a1, b1, c1, a2, b2, c2).value_or(std::make_pair(0, 0));
        if (result.first >= 0 && result.first <= 100 && std::floor(result.first) == result.first &&
            result.second >= 0 && result.second <= 100 &&
            std::floor(result.second) == result.second) {
            // solution exists
            tokens_required_part1 += 3 * result.first + result.second;
        }

        // part II
        result = solve_equation(a1, b1, c1 + 10000000000000, a2, b2, c2 + 10000000000000).value_or(std::make_pair(0, 0));
        if (result.first >= 0 && std::floor(result.first) == result.first && result.second >= 0 &&
            std::floor(result.second) == result.second) {
            // solution exists
            tokens_required_part2 += 3 * result.first + result.second;
        }
    }

    logger.info("Fewest tokens to win all prizes (Part I)  : {}", tokens_required_part1);
    logger.info("Fewest tokens to win all prizes (Part II) : {}", tokens_required_part2);
}
