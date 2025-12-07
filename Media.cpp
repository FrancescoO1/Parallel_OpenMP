#include "Media.h"
#include <numeric>
#include <cmath>

std::pair<double, double> calculateStats(const std::vector<double>& values) {
    if (values.empty()) {
        return {0.0, 0.0};
    }

    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    double mean = sum / values.size();

    double sq_sum = std::inner_product(values.begin(), values.end(), values.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / values.size() - mean * mean);

    return {mean, stdev};
}