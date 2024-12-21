#include "algebra_utils.hh"

std::optional<std::pair<double, double>> solve_equation(
    double a1, double b1, double c1,
    double a2, double b2, double c2) 
{
    // Calculate determinant
    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        // no solution possible
        return std::nullopt;
    }

    // solve using cramer's rule
    double x = (b2 * c1 - b1 * c2) / determinant;
    double y = (a1 * c2 - a2 * c1) / determinant;

    return std::make_pair(x, y);
}