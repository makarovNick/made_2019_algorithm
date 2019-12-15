#include "trav.h"
#include "graph.h"

#include <cmath>

MSTTravellingSalesmanProblem::MSTTravellingSalesmanProblem(std::vector<std::pair<double, double>>& points) {
    distance = 0;
    Graph graph(points.size());

    for(int i = 0; i < points.size(); ++i) 
    {
        for(int j = 0; j < points.size(); ++j) 
        {
            if(i == j) 
            {
                continue;
            }
            graph.addEdge(i, j, std::hypot(points[i].first - points[j].first, points[i].second - points[j].second));
        }
    }
    MSTBoruvka MST(graph);
    CListWeightedGraph tree(MST.get_tree());
    // Выполняем DFS и он возвращает нам последовательность обхода точек
    std::vector<int> path = tree.dfs(0); // Тщательно проверить код у этой штуки
    // Выполняем подсчет длины всего пути
    path.push_back(path[0]);
    for(int i = 0; i < path.size() - 1; ++i) 
    {
        distance += hypot(points[path[i]].first - points[path[i + 1]].first, points[path[i]].second - points[path[i + 1]].second);
    }
}
double MSTTravellingSalesmanProblem::get_distance() const {
    return distance;
}