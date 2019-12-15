#include "box_muller_generator.h"

#include <cmath>
#include <random>

std::vector<std::pair<double, double>> BoxMullerGenerator::generate_points(size_t n, size_t seed) 
{
    std::vector<std::pair<double, double>> points;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    gen.seed(seed);

    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for(size_t i = 0; i < n; ++i) 
    {
        double radius = dis(gen);
        double phi = dis(gen);

        double temp_1 = std::cos(2 * M_PI * phi);
        double temp_2 = sqrt(-2 * std::log(radius));

        double x = temp_1 * temp_2;
        double y = sqrt(1 - temp_1 * temp_1) * temp_2;
        
        points.emplace_back(std::make_pair(x, y));
    }

    return points;
}