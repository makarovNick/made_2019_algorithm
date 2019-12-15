#include "graph.h"

#include <numeric>
#include <set>
#include <algorithm>

const size_t MAX_EDGES = 1000.0;

double PrimMST(const Graph &graph) 
{
    std::vector<double> min_weights(graph.size(), MAX_EDGES);
    std::vector<bool> visited(graph.size(), false);

    std::set<std::pair<int,int>> vert_queue;
    
    vert_queue.insert(std::make_pair(0, 0));
    min_weights[0] = 0.0;

    double weight;
    int to, next_vert;
    for (size_t i = 0; i < graph.size(); i++) 
    {
        next_vert = vert_queue.begin()->second;
        vert_queue.erase(vert_queue.begin());
        visited[next_vert] = true;

        auto edges = graph.getEdgesFrom(next_vert);
        for (size_t j = 0; j < edges.size(); j++) 
        {
            to = edges[j].first;
            weight = edges[j].second;

            if (weight < min_weights[to] && !visited[to]) 
            {
                vert_queue.erase(std::make_pair(min_weights[to], to));
                min_weights[to] = weight;
                vert_queue.insert(std::make_pair(min_weights[to], to));
            }
        }
    }

    return std::accumulate(min_weights.begin(), min_weights.end(), 0.0);
}