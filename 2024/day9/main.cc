#include "aocutils.hh"
#include "logger.hh"
#include <cassert>
#include <format>
#include <memory>
#include <sstream>
#include <string>

static Logger logger("day9");

struct disk_content {
    long file_id = -1;
    long block_start, length;

    std::shared_ptr<disk_content> next = nullptr, prev = nullptr;

    disk_content(long block_start, long length) : block_start(block_start), length(length) {}

    disk_content(long file_id, long block_start, long length)
        : block_start(block_start), length(length), file_id(file_id) {}

    std::string to_string() const {
        return is_free_space()
                   ? std::format("Free Space : Block start : {}, Length : {}", block_start, length)
                   : std::format("File id : {}, Block start : {}, Length : {}", file_id,
                                 block_start, length);
    }

    // returns already allocated space
    void allocate_file(std::shared_ptr<disk_content> file_ptr) {
        if (!this->is_free_space() || file_ptr->is_free_space()) {
            // file cannot be free space and this cannot be a file
            assert(false);
        }

        if (file_ptr->length >= length) {
            // consume this free space entirely
            this->file_id = file_ptr->file_id;
            file_ptr->length -= length;

            // insert an empty space next to file to denote the free space
            auto new_empty_space = std::make_shared<disk_content>(
                -1, file_ptr->block_start + file_ptr->length, length);
            if (file_ptr->length > 0) {
                new_empty_space->next = file_ptr->next;
                new_empty_space->prev = file_ptr;
                file_ptr->next = new_empty_space;
                if (new_empty_space->next != nullptr) {
                    new_empty_space->next->prev = new_empty_space;
                }
            } else {
                new_empty_space->next = file_ptr->next;
                new_empty_space->prev = file_ptr->prev;
                file_ptr->prev->next = new_empty_space;
                if (file_ptr->next != nullptr) {
                    file_ptr->next->prev = new_empty_space;
                }
                file_ptr->file_id = -1;
            }
        } else {
            // file can be fully allocated insert new file before this free space
            auto new_file =
                std::make_shared<disk_content>(file_ptr->file_id, block_start, file_ptr->length);
            new_file->next = prev->next;
            new_file->prev = prev;
            prev->next = new_file;
            prev = new_file;
            // reduce free space in current instance
            this->block_start += file_ptr->length;
            this->length -= file_ptr->length;
            // file is fully moved
            file_ptr->file_id = -1;
        }
    }

    bool is_free_space() const { return file_id == -1; }

    long checksum() {
        if (is_free_space()) {
            return 0;
        }

        return file_id * (length * ((2 * block_start) + length - 1)) / 2;
    }
};

std::pair<std::shared_ptr<disk_content>, std::shared_ptr<disk_content>>
init_blocks(const std::string& disk_map) {
    // init head
    long next_block_start = disk_map.at(0) - '0';
    std::shared_ptr<disk_content> head = std::make_shared<disk_content>(0, 0, next_block_start);

    // next file id
    long file_id = 1;
    bool is_file = false;

    auto curr = head;
    for (int i = 1; i < disk_map.size(); i++, is_file = !is_file) {
        int length = disk_map.at(i) - '0';
        if (length == 0) {
            continue;
        }
        std::shared_ptr<disk_content> next_item =
            std::make_shared<disk_content>(is_file ? file_id++ : -1, next_block_start, length);
        curr->next = next_item;
        next_item->prev = curr;
        curr = next_item;
        next_block_start += length;
    }

    // return head, tail
    return {head, curr};
}

void merge_blocks(std::shared_ptr<disk_content> head) {
    long prev_file_id = -2;
    long cumulative_length = 0;
    std::shared_ptr<disk_content> content_start = nullptr;
    while (head != nullptr) {
        if (head->file_id == prev_file_id) {
            cumulative_length += head->length;
        } else {
            if (content_start != nullptr && head->prev != content_start) {
                // merge the contents
                content_start->length = cumulative_length;
                content_start->next = head;
                head->prev = content_start;
            }
            // reset trackers
            prev_file_id = head->file_id;
            content_start = head;
            cumulative_length = head->length;
        }
        head = head->next;
    }

    // merge last contents
    if (content_start->next != nullptr) {
        content_start->length = cumulative_length;
        content_start->next = nullptr;
    }
}

void compact_blocks_part1(std::shared_ptr<disk_content> head, std::shared_ptr<disk_content> tail) {
    auto curr = head;
    while (curr != nullptr && tail != nullptr) {
        // go to the next file to be moved
        while (tail->is_free_space()) {
            tail = tail->prev;
            if (tail == curr) {
                return;
            }
        }

        // go to the next free space from beginning
        while (!curr->is_free_space()) {
            curr = curr->next;
            if (curr == tail) {
                return;
            }
        }

        // found a file and free space to be moved
        curr->allocate_file(tail);
    }

    // merge nearby blocks
    merge_blocks(head);
}

std::string print_disk(std::shared_ptr<disk_content> head) {
    std::stringstream output;
    while (head != nullptr) {
        output << std::string(head->length, head->is_free_space() ? '.' : (head->file_id + '0'));
        head = head->next;
    }
    return output.str();
}

long calculate_filesystem_checksum(std::shared_ptr<disk_content> head) {
    long sum = 0;
    while (head != nullptr) {
        sum += head->checksum();
        logger.debug("cs : {} : {}", head->to_string(), head->checksum());
        head = head->next;
    }

    return sum;
}

void compact_blocks_part2(std::shared_ptr<disk_content> head, std::shared_ptr<disk_content> tail) {

    while (tail->is_free_space()) {
        tail = tail->prev;
    }

    long file_id = tail->file_id;

    for (long file_id = tail->file_id; file_id >= 0; file_id--) {
        // pickup next file to move
        while (tail->file_id != file_id) {
            tail = tail->prev;
        }

        // find the free space to move the file
        auto curr = head;
        while (curr != tail && (!curr->is_free_space() || curr->length < tail->length)) {
            curr = curr->next;
        }

        if (curr == tail) {
            // cannot move this block
            continue;
        }

        // move the block
        curr->allocate_file(tail);
    }

    // merge nearby blocks
    merge_blocks(head);
}

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(9, 2024, &args).readAll();
    {
        auto [head, tail] = init_blocks(input);
        compact_blocks_part1(head, tail);
        // logger.info("{}", print_disk(head));
        logger.info("Part I : Filesystem Checksum : {}", calculate_filesystem_checksum(head));
    }

    {
        auto [head, tail] = init_blocks(input);
        compact_blocks_part2(head, tail);
        // logger.info("{}", print_disk(head));
        logger.info("Part II : Filesystem Checksum : {}", calculate_filesystem_checksum(head));
    }
}
