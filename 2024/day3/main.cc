#include "aocutils.hh"
#include <regex>

std::regex mul_pattern(R"(mul\((\d+),(\d+)\)([\s\S]*))");
std::regex dont_pattern(R"(don't\(\)([\s\S]*))");
std::regex do_pattern(R"(do\(\)([\s\S]*))");

enum do_dont : short { DO = 0, DONT = 1, NONE = 2 };

long maybe_multiply(const std::string &program_start) {
    std::smatch matches;
    if (std::regex_match(program_start, matches, mul_pattern)) {
        return std::stol(matches[1]) * std::stol(matches[2]);
    }
    return 0;
}

do_dont is_do_dont(const std::string &program_start) {
    if (std::regex_match(program_start, do_pattern)) {
        return DO;
    } else if (std::regex_match(program_start, dont_pattern)) {
        return DONT;
    }
    return NONE;
}

int main(int argc, char *argv[]) {
    AocCmdLineArgs args(argc, argv);
    InputReader input(3, 2024, &args);
    auto program = input.readAll();

    long unconditional_sum = 0;
    long conditional_sum = 0;
    bool enabled = true;
    for (int i = 0; i < program.size(); i++) {
        long product = 0;

        switch (program[i]) {
        case 'm':
            product += maybe_multiply(program.substr(i));
            break;
        case 'd':
            switch (is_do_dont(program.substr(i))) {
            case DO:
                enabled = true;
                break;
            case DONT:
                enabled = false;
                break;
            case NONE:
                break;
            }
            break;
        }

        unconditional_sum += product;
        if (enabled) {
            conditional_sum += product;
        }
    }

    std::cout << "Part I : sum of all multiplications : " << unconditional_sum
              << std::endl;
    std::cout << "Part II : sum of all multiplications with conditional : "
              << conditional_sum << std::endl;
}
