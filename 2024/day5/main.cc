#include "aocutils.hh"
#include "string_helper.hh"
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

std::vector<int> topological_sort(const std::unordered_map<int, std::set<int>>& main_adj_list,
                                  const std::vector<int>& pages) {

    // create a subset graph with only the pages
    std::unordered_map<int, std::set<int>> adj_list;
    for (int i = 0; i < pages.size(); i++) {
        for (int j = 0; j < pages.size(); j++) {
            if (i == j) {
                continue;
            }

            if (main_adj_list.contains(pages[i]) && main_adj_list.at(pages[i]).contains(pages[j])) {
                adj_list[pages[i]].insert(pages[j]);
            } else if (main_adj_list.contains(pages[j]) &&
                       main_adj_list.at(pages[j]).contains(pages[i])) {
                adj_list[pages[j]].insert(pages[i]);
            }

            if (!adj_list.contains(pages[i])) {
                adj_list[pages[i]].insert({});
            }

            if (!adj_list.contains(pages[j])) {
                adj_list[pages[j]].insert({});
            }
        }
    }

    // calculate in-degree of each vertex
    std::unordered_map<int, int> in_degree;
    for (const auto& [vertex, adj_vertices] : adj_list) {
        for (const auto& adj_vertex : adj_vertices) {
            in_degree[adj_vertex]++;
        }

        if (!in_degree.contains(vertex)) {
            in_degree[vertex] = 0;
        }
    }

    // queues to store vertices with in-degree 0
    std::queue<int> vertex_queue;
    for (const auto& [vertex, degree] : in_degree) {
        if (degree == 0) {
            vertex_queue.push(vertex);
        }
    }

    std::vector<int> sorted_vertices;
    while (!vertex_queue.empty()) {
        auto vertex = vertex_queue.front();
        vertex_queue.pop();

        // add vertex to the sorted list
        sorted_vertices.push_back(vertex);

        // reduce in-degree of all adjacent vertices
        for (const auto& adj_vertex : adj_list.at(vertex)) {
            in_degree[adj_vertex]--;
            if (in_degree[adj_vertex] == 0) {
                vertex_queue.push(adj_vertex);
            }
        }
    }

    // if there are any vertices left, then there is a cycle in the graph
    for (const auto& [vertex, degree] : in_degree) {
        if (degree != 0) {
            std::cout << vertex << " : " << degree << std::endl;
            throw std::runtime_error("Cycle detected in the graph");
        }
    }

    return sorted_vertices;
}

bool is_valid_update(const std::vector<int>& pages,
                     std::unordered_map<int, std::set<int>> adj_list) {
    // check if the pages are in the correct order
    for (int i = 1; i < pages.size(); i++) {
        if (adj_list[pages[i - 1]].find(pages[i]) == adj_list[pages[i - 1]].end()) {
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    AocCmdLineArgs args(argc, argv);
    auto input = InputReader(5, 2024, &args);

    std::unordered_map<int, std::set<int>> adj_list;

    std::function<void(std::string&)> parse_line = [&adj_list](std::string& line) {
        auto vertices = splitToIntegers(line, "|");
        adj_list[vertices[0]].insert(vertices[1]);
        if (adj_list.find(vertices[1]) == adj_list.end()) {
            adj_list[vertices[1]] = {};
        }
    };

    int sum_of_valid_middle_pages = 0;
    int sum_of_invalid_middle_pages = 0;
    for (auto it = input.begin(); it != input.end(); ++it) {
        auto line = *it;
        if (it->empty()) {
            parse_line = [&adj_list, &sum_of_valid_middle_pages,
                          &sum_of_invalid_middle_pages](std::string& line) {
                auto pages = splitToIntegers(line, ",");
                if (is_valid_update(pages, adj_list)) {
                    sum_of_valid_middle_pages += pages[pages.size() / 2];
                } else {
                    auto ordered_pages = topological_sort(adj_list, pages);
                    sum_of_invalid_middle_pages += ordered_pages[ordered_pages.size() / 2];
                }
            };

            continue;
        }

        parse_line(line);
    }

    std::cout << "Part I : sum of middle pages : " << sum_of_valid_middle_pages << std::endl;
    std::cout << "Part II : sum of middle pages, which had to be ordered : "
              << sum_of_invalid_middle_pages << std::endl;
}
