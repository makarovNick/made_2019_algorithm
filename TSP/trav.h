#pragma once

#include <vector>
#include <utility>

class MSTTravellingSalesmanProblem {
public:
    explicit MSTTravellingSalesmanProblem(std::vector<std::pair<double, double>>& points);
    double get_distance() const;
private:
    double distance;
};