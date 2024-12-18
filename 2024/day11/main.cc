#include "aocutils.hh"
#include "logger.hh"
#include "numeric_utils.hh"
#include "pair_utils.hh"
#include <cmath>
#include <string>
#include <unordered_map>
#include <utility>

static Logger logger("day11");

using cache_type = std::unordered_map<std::pair<long, int>, long, HashNumericPair<long, int>>;

long blink(long value, int times, cache_type& cache) {
    if (times == 0) {
        return 1;
    }

    auto key = std::make_pair(value, times);
    auto res = cache.find(key);
    if (res != cache.end()) {
        return res->second;
    }

    long result = 0;
    if (value == 0) {
        value = 1;
        result = blink(value, times - 1, cache);
    } else {
        auto num_digits = count_digits<long>(value);
        if (num_digits % 2 == 0) {
            // split the number into two halves
            long divisor = std::pow(10, num_digits / 2);
            auto first_half = value / divisor;
            auto second_half = value % divisor;
            result = blink(first_half, times - 1, cache) + blink(second_half, times - 1, cache);
        } else {
            // multiply by 2024
            value *= 2024;
            result = blink(value, times - 1, cache);
        }
    }

    cache.insert({key, result});
    return result;
}

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(11, 2024, &args).readAllAsVectorOfNumbers<long>();

    long total_stones_after_25_blinks = 0;
    long total_stones_after_75_blinks = 0;
    cache_type cache;
    for (long value : input) {
        total_stones_after_25_blinks += blink(value, 25, cache);
        total_stones_after_75_blinks += blink(value, 75, cache);
    }

    logger.info("Part I  : Stones after 25 blinks : {}", total_stones_after_25_blinks);
    logger.info("Part II : Stones after 75 blinks : {}", total_stones_after_75_blinks);
}
