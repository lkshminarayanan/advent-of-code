#include "aocutils.hh"
#include "pair_utils.hh"
#include <unordered_map>
#include <utility>

enum Direction : short { NONE, UP, DOWN, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT };

static std::unordered_map<Direction, std::pair<int, int>> dir_to_delta = {
    {UP, {-1, 0}},      {DOWN, {1, 0}},     {LEFT, {0, -1}},     {RIGHT, {0, 1}},
    {UPLEFT, {-1, -1}}, {UPRIGHT, {-1, 1}}, {DOWNLEFT, {1, -1}}, {DOWNRIGHT, {1, 1}},
};

static auto update_index = [](Direction dir, int i, int j) -> std::pair<int, int> {
    auto delta = dir_to_delta.at(dir);
    return {i + delta.first, j + delta.second};
};

int search(const std::vector<std::string> &input, int i, int j, const std::string &search_text,
           int si, Direction dir) {
    if (i < 0 || i >= input.size() || j < 0 || j >= input[i].size()) {
        return 0;
    }

    if (input[i][j] != search_text[si]) {
        return 0;
    }

    // curr char matches
    si++;
    if (si == search_text.size()) {
        return 1;
    }

    if (dir == NONE) {
        int count = 0;
        for (auto [dir, delta] : dir_to_delta) {
            auto [ni, nj] = update_index(dir, i, j);
            if (search(input, ni, nj, search_text, si, dir) == 1) {
                count++;
            }
        }
        return count;
    } else {
        auto [ni, nj] = update_index(dir, i, j);
        return search(input, ni, nj, search_text, si, dir);
    }
}

int main(int argc, char *argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(4, 2024, &args).readAllAsVectorOfStrings();

    int count = 0;
    const std::string search_text1 = "XMAS";
    const std::string search_text2 = "MAS";
    std::unordered_map<std::pair<int, int>, int, HashIntPair> mas_midpoints_to_count_map;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            // part I
            count += search(input, i, j, search_text1, 0, NONE);

            // part II - search only diagonally
            for (auto dir : {UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT}) {
                if (search(input, i, j, search_text2, 0, dir) == 1) {
                    mas_midpoints_to_count_map[update_index(dir, i, j)]++;
                }
            }
        }
    }

    // part 2 count calculation
    auto count_x_max = 0;
    // count number of keys in mas_midpoints_to_count_map whose value is two
    for (auto [_, count] : mas_midpoints_to_count_map) {
        if (count == 2) {
            count_x_max++;
        }
    }

    std::cout << "Part I : count(XMAS) : " << count << std::endl;
    std::cout << "Part II : count(X_MAS) : " << count_x_max << std::endl;
}
