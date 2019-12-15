#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include <cmath> 
#include <numeric>
#include <iostream>

class Graph 
{
    struct Edge
    {
        int from;
        int to;
        double weight;

        Edge(int f, int t, double w)
            : from(f)
            , to(t)
            , weight(w)
        {
        }
        bool operator<(const Edge& other)
        {
            return weight < other.weight;
        }
    };

public:
    Graph(size_t n)
        : points_count(n)
    {
    }

    void addEdge(int from, int to, double w);

    double getWeight(int from, int to);
    size_t getPointsCount() const;
    
    double MSTSolution();
    double exactSolution();

private:

    int points_count;
    std::vector<Edge> edges;
};