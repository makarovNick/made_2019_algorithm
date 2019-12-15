#include "DSU.h"
#include "graph.h"

void Graph::addEdge(int from_, int to_, double w)
{
    edges.emplace_back(Edge(from_,to_,w));
}

size_t Graph::getPointsCount() const
{
    return points_count;
}

double Graph::getWeight(int from_, int to_)
{
    for (auto& edge : edges)
    {
        if ((from_ == edge.from && to_ == edge.to)
            || (from_ == edge.to && to_ == edge.from))
        {
            return edge.weight;
        }
    }

    return -1;
}


double Graph::exactSolution()
{
    std::vector<double> weights;
    std::vector<int> vertexContainer(points_count);
    std::iota(vertexContainer.begin(), vertexContainer.end(), 0);
    //bruteforce
    do
    {
        int edge_in_tree = 0;
        double weight = 0;
        for (size_t i = 0; i < vertexContainer.size(); i++)
        {
            double res = getWeight(vertexContainer[i], vertexContainer[i + 1]);
            if (res == -1)
            {
                break;
            }
            else
            {
                weight += res;
                edge_in_tree++;
            }
        }

        if (edge_in_tree == points_count - 1)
        {
            weights.push_back(weight);
        }

    } while (std::next_permutation(vertexContainer.begin(), vertexContainer.end()));

    return *std::min_element(weights.begin(), weights.end());
}

double Graph::MSTSolution()
{
    double weight = 0;
    std::sort(edges.begin(), edges.end());
    DSU dsu(points_count);

    for (auto& edge : edges)
    {
        int from_ = edge.from;
        int to_ = edge.to;

        int setU = dsu.find(from_);
        int setV = dsu.find(to_);

        if (setU != setV)
        {
            weight += edge.weight;
            dsu.merge(setU, setV);
        }
    }

    return weight;
}