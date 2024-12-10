#include "aocutils.hh"
#include "logger.hh"
#include "string_helper.hh"

static Logger logger("day7");

// returns a pair of bools,
// the first one is true if the expression is true,
// the second one is true if the concatenation was used
std::pair<bool, bool> evaluate(const std::vector<long> input, long value_so_far, int i) {
    if (i == input.size()) {
        return {value_so_far == input[0], false};
    }

    // prune branches that have already exceeded the target
    if (value_so_far > input[0]) {
        return {false, false};
    }

    static auto concatenate = [](long a, long b) {
        // contatenate a and b
        long b_copy = b;
        while (b_copy > 0) {
            a *= 10;
            b_copy /= 10;
        }
        return a + b;
    };

    auto eval_add = evaluate(input, value_so_far + input[i], i + 1);
    if (eval_add.first) {
        return eval_add;
    }

    auto eval_mul = evaluate(input, value_so_far * input[i], i + 1);
    if (eval_mul.first) {
        return eval_mul;
    }

    if (evaluate(input, concatenate(value_so_far, input[i]), i + 1).first) {
        return {true, true};
    }

    return {false, false};
}

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(7, 2024, &args);

    long sum_without_concatenation = 0, sum_with_concatenation = 0;
    for (auto& line : input) {
        auto input = splitToLong(line, " :");
        auto res = evaluate(input, input[1], 2);
        if (res.first) {
            sum_with_concatenation += input[0];
            if (!res.second) {
                sum_without_concatenation += input[0];
            }
        }
    }

    logger.info("Part I  : Sum without concatenation operator : {}", sum_without_concatenation);
    logger.info("Part II : Sum with concatenation operator    : {}", sum_with_concatenation);
}
