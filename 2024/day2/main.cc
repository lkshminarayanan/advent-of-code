#include "aocutils.hh"
#include "string_helper.hh"
#include <algorithm>
#include <cassert>
#include <cstdlib>

enum safety_report : short {
    SAFE = 0,
    SAFE_WITH_LEVEL_TOLERANCE = 1,
    NOT_SAFE = 2
};

safety_report is_safe(const std::vector<int> &report) {
    const bool sign_trend = (report[1] - report[0]) < 0;
    int prev = report[0];
    bool level_tolerated = false;

    for (int i = 1; i < report.size(); i++) {
        auto diff = report[i] - prev;
        auto abs_diff = std::abs(diff);
        if ((diff < 0 != sign_trend) || (abs_diff < 1) || (abs_diff > 3)) {
            if (level_tolerated) {
                return NOT_SAFE;
            }
            level_tolerated = true;
            // skip this report
            continue;
        }

        // update prev
        prev = report[i];
    }

    return level_tolerated ? SAFE_WITH_LEVEL_TOLERANCE : safety_report::SAFE;
}

int main(int argc, char *argv[]) {
    AocCmdLineArgs args(argc, argv);
    InputReader input(2, 2024, &args);

    int safe_reports = 0;
    int safe_reports_with_level_tolerance = 0;
    for (auto it = input.begin(); it != input.end(); ++it) {
        auto report = splitToIntegers(*it, " ");
        switch (is_safe(report)) {
        case SAFE:
            safe_reports++;
        case SAFE_WITH_LEVEL_TOLERANCE:
            safe_reports_with_level_tolerance++;
            break;
        case NOT_SAFE:
            // reverse the report and try again as not_safe doesn't handle bad
            // reports at index 0 and 1
            std::reverse(report.begin(), report.end());
            if (is_safe(report) != NOT_SAFE) {
                safe_reports_with_level_tolerance++;
            }
            break;
        }
    }

    std::cout << "Part I : number of safe reports : " << safe_reports
              << std::endl;
    std::cout << "Part II : number of safe reports with problem dampener : "
              << safe_reports_with_level_tolerance << std::endl;
}
