/*******************************************************/
// 11_2. Количество различных путей
// Ограничение времени	0.2 секунды
// Ограничение памяти	10Mb
// Ввод	стандартный ввод или input.txt
// Вывод	стандартный вывод или output.txt
// Дан невзвешенный неориентированный граф. 
// В графе может быть несколько кратчайших путей между какими-то вершинами. 
// Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).

// Формат ввода
// v: кол-во вершин (макс. 50000),
// n: кол-во ребер(макс. 200000),
// n пар реберных вершин,
// пара вершин (u, w) для запроса.

// Формат вывода
// Количество кратчайших путей от v к w.
/*******************************************************/
#include <iostream>
#include <vector>
#include <queue>

const int MAX_VERTICES = 50000;

int numberOfPath(const std::vector<std::vector<int>>& graph, int from, int to) 
{
	std::vector<int> dist(graph.size(), MAX_VERTICES);
	std::vector<int> path(graph.size(), 0);
	std::vector<bool> visited(graph.size(), false);

	std::queue<int> vert_queue;

	path[from] = 1;
	for (auto i : graph[from]) 
	{
		path[i] = 1;
	}

	vert_queue.push(from);
	dist[from] = 0;

	while (!vert_queue.empty()) 
	{
		int v = vert_queue.front();
		visited[v] = true;

		vert_queue.pop();

		for (auto i : graph[v]) 
		{
			if (dist[i] > dist[v] + 1) 
			{
				dist[i] = dist[v] + 1;
				path[i] = path[v];
			}
			else if (dist[i] == dist[v] + 1 )
			{
				path[i] += path[v];
			}
			if (!visited[i]) 
			{
				vert_queue.push(i);
				visited[i] = true;
			}
		}
	}

	return path[to];
}

int main() 
{
	int n, from, to;
	std::cin >> n;
	std::vector<std::vector<int>> graph(n);

	std::cin >> n;
	for (int i = 0; i < n; ++i)
	{
		std::cin >> from >> to;

		graph[from].push_back(to);
		graph[to].push_back(from);
	}

	std::cin >> from >> to;
	std::cout << numberOfPath(graph, from, to);

	return 0;
}