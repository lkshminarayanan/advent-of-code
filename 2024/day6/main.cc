#include "aocutils.hh"
#include "logger.hh"
#include <format>
#include <future>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

static Logger logger("day6");

enum Direction { UP, DOWN, LEFT, RIGHT };

const char BLOCKER = '#';
const char VISITED = 'X';

int calculate_visited_blocks(const std::vector<std::string>& input) {
    int count = 0;
    for (const auto& row : input) {
        for (char c : row) {
            if (c == VISITED) {
                count++;
            }
        }
    }
    return count;
}

std::pair<bool, int> move_guard(std::vector<std::string> input, int start_i, int start_j,
                                int block_i = -1, int block_j = -1) {
    int i_end = input.size();
    int j_end = input[0].size();
    int i = start_i, j = start_j;
    Direction dir = UP;
    input[i][j] = VISITED;
    auto num_steps = 0;
    std::unordered_set<std::string> visited_steps_with_direction;
    visited_steps_with_direction.insert(std::format("{}_{}_{}", i, j, short(dir)));
    // update blocker if required
    if (block_i != -1 && block_j != -1) {
        input[block_i][block_j] = BLOCKER;
    }

    static auto next_coordinate = [](Direction dir, int i, int j) {
        switch (dir) {
        case UP:
            return std::make_pair(i - 1, j);
        case DOWN:
            return std::make_pair(i + 1, j);
        case LEFT:
            return std::make_pair(i, j - 1);
        case RIGHT:
            return std::make_pair(i, j + 1);
        }
    };

    while (true) {
        // get next guard location in curr direction
        auto [next_i, next_j] = next_coordinate(dir, i, j);

        if (next_i < 0 || next_i >= i_end || next_j < 0 || next_j >= j_end) {
            // guard has exited the grid
            break;
        }

        // check if there is any blockage
        if (input[next_i][next_j] == BLOCKER) {
            // change direction
            switch (dir) {
            case UP:
                dir = RIGHT;
                break;
            case DOWN:
                dir = LEFT;
                break;
            case LEFT:
                dir = UP;
                break;
            case RIGHT:
                dir = DOWN;
                break;
            }
        } else {
            // move guard
            i = next_i;
            j = next_j;
            input[i][j] = VISITED;
            num_steps++;

            auto visited_step = std::format("{}_{}_{}", i, j, short(dir));
            if (visited_steps_with_direction.contains(visited_step)) {
                return {true, 0};
            }

            visited_steps_with_direction.insert(visited_step);
        }
    }

    for (auto& row : input) {
        logger.debug("{}", row);
    }

    return {false, calculate_visited_blocks(input)};
}

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(6, 2024, &args).readAllAsVectorOfStrings();

    int start_i, start_j;
    for (int i = 0; i < input.size(); i++) {
        auto res = input[i].find("^");
        if (res != std::string::npos) {
            start_i = i;
            start_j = res;
            break;
        }
    }

    logger.debug("start at ({},{})", start_i, start_j);

    logger.info("Part I : Visited blocks : {}", move_guard(input, start_i, start_j).second);

    // Part II
    std::atomic_int obstruction_points = 0;
    auto move_guard_per_thread = [&input, start_i, start_j, &obstruction_points](int i_loop_start,
                                                                                 int i_loop_end) {
        for (int i = i_loop_start; i < i_loop_end; i++) {
            for (int j = 0; j < input[i].size(); j++) {
                if (input[i][j] != BLOCKER) {
                    auto res = std::async(move_guard, input, start_i, start_j, i, j);
                    if (res.get().first) {
                        obstruction_points++;
                    }
                }
            }
        }
    };

    int i_loop_start = 0;
    std::vector<std::thread> threads;
    auto parallel_threads = std::min(std::thread::hardware_concurrency(), unsigned(input.size()));
    for (unsigned int i = 0; i < parallel_threads; ++i) {
        auto batch_size =
            input.size() / parallel_threads + (i < (input.size() % parallel_threads) ? 1 : 0);
        threads.emplace_back(
            std::thread(move_guard_per_thread, i_loop_start, i_loop_start + batch_size));
        i_loop_start += batch_size;
    }

    for (auto& th : threads)
        th.join();

    logger.info("Part II : Obstruction points that can cause loops : {}",
                obstruction_points.load());
}
