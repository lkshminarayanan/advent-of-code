#pragma once

#include <optional>
#include <utility>

// Solve x and y from the following pair of linear equations:
// a1 * x + b1 * y = c1
// a2 * x + b2 * y = c2
std::optional<std::pair<double, double>> solve_equation(
    double a1, double b1, double c1,
    double a2, double b2, double c2);