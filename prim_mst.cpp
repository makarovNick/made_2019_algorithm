/*******************************************************/
// 14. Минимальное остовное дерево
// Ограничение времени	0.2 секунды
// Ограничение памяти	10Mb
// Ввод	стандартный ввод или kruskal.in
// Вывод	стандартный вывод или kruskal.out
// Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе. 
// Вариант 1. С помощью алгоритма Прима.
// Вариант 2. С помощью алгоритма Крускала.
// Вариант 3. С помощью алгоритма Борувки.
// Ваш номер варианта прописан в ведомости.
// Формат ввода
// Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000). 
// Следующие m строк содержат описание ребер по одному на строке. 
// Ребро номер i описывается тремя натуральными числами bi, ei и wi 
//— номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).
// Формат вывода
// Выведите единственное целое число - вес минимального остовного дерева.
/*******************************************************/
#include <iostream>
#include <vector>
#include <numeric>
#include <set>

const int MAX_EDGES = 100000;

struct Graph
{
    Graph(int verts)
    {
        edges.resize(verts);
    }
    Graph(const Graph& other)
		: edges(other.edges)
	{
    }
	~Graph() = default;
	Graph(Graph&&) = delete;
	Graph& operator=(Graph&&) = delete;
	Graph& operator=(const Graph&) = delete;

    void addEdge(int from, int to, int weight)
    {
        edges[from - 1].push_back(std::make_pair(to - 1, weight));
        edges[to - 1].push_back(std::make_pair(from - 1, weight));
    }

    std::vector<std::pair<int,int>> getEdgesFrom(int vert) const
    {
        return edges[vert];
    }

    size_t size() const
    {
        return edges.size();
    }

private:
    std::vector<std::vector<std::pair<int, int>>> edges;
};  

int PrimMST(const Graph &graph) 
{
    std::vector<int> min_weights(graph.size(), MAX_EDGES);
    std::vector<bool> visited(graph.size(), false);

    std::set<std::pair<int,int>> vert_queue;
    
    vert_queue.insert(std::make_pair(0, 0));
    min_weights[0] = 0;

    int weight, to, next_vert;
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

    return std::accumulate(min_weights.begin(), min_weights.end(), 0);
}

int main() 
{
    int V, E;
    std::cin >> V >> E;

    Graph graph(V);

    int V1, V2, W;
    for (int i = 0; i < E; i++)
    {
        std::cin >> V1 >> V2 >> W;
        graph.addEdge(V1, V2, W);
    }

    std::cout << PrimMST(graph);

    return 0;
}
