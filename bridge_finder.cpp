/*******************************************************/
// 12. Мосты
// Ограничение времени	2 секунды
// Ограничение памяти	256Mb
// Ввод	bridges.in
// Вывод	bridges.out
// Ребро неориентированного графа называется мостом, если удаление этого ребра из графа увеличивает число компонент связности.
// Дан неориентированный граф, требуется найти в нем все мосты.
// Формат ввода
// Первая строка входного файла содержит два целых числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 1 ≤ m ≤ 200000).
// Следующие m строк содержат описание ребер по одному на строке.
// Ребро номер i описывается двумя натуральными числами bi, ei — номерами концов ребра (1 ≤ bi, ei ≤ n).
// Формат вывода
// Первая строка выходного файла должна содержать одно натуральное число b — количество мостов в заданном графе. 
// На следующей строке выведите b целых чисел — номера ребер, которые являются мостами, в возрастающем порядке. 
// Ребра нумеруются с единицы в том порядке, в котором он+и заданы во входном файле.
/*******************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
//	Pick an arbitrary vertex of the graph rootand run depth first search from it.
//	Note the following fact(which is easy to prove) :
//
//	Let's say we are in the DFS, looking through the edges starting from vertex v. 
//	The current edge (v,to) is a bridge if and only if none of the vertices to and 
//	its descendants in the DFS traversal tree has a back-edge to vertex v or any of its ancestors. 
//	Indeed, this condition means that there is no other way from v to to except for edge (v,to).
//	Now we have to learn to check this fact for each vertex efficiently.
//	We'll use "time of entry into node" computed by the depth first search.
//
//	So, let tin[v] denote entry time for node v.
//	We introduce an array low which will let us check the fact for each vertex v.low[v] is the minimum of tin[v], 
//	the entry times tin[p] for each node p that is connected to node v via a back - edge(v, p) 
//	and the values of low[to] for each vertex to which is a direct descendant of v in the DFS tree :
//
//				       ⎧ tin[v]    , otherwise
//	low[v] = min ⎨ tin[p]    , for all p for which(v, p) is a back edge
//				       ⎩ low[to]   , for all to for which(v, to) is a tree edge
//
//  
//	Now, there is a back edge from vertex v or one of its descendants to one of its ancestors 
//	if and only if vertex v has a child to for which low[to]≤tin[v].
//	If low[to] = tin[v], the back edge comes directly to v, otherwise it comes to one of the ancestors of v.
//
//	Thus, the current edge(v, to) in the DFS tree is a bridge if and only if low[to] > tin[v].

class BridgeFinder
{
public:

	BridgeFinder(const std::vector<std::vector<std::pair<int, int>>>& _graph_with_indexes);
	BridgeFinder(BridgeFinder&&) = delete;
	BridgeFinder(const BridgeFinder&) = delete;
	BridgeFinder& operator=(BridgeFinder&&) = delete;
	BridgeFinder& operator=(const BridgeFinder&) = delete;

	std::vector<int> findBridge();

private:

	void DFS(int v, int p = -1);

	const std::vector<std::vector<std::pair<int, int>>>* graph;

	std::vector<bool> visited;
	std::vector<int> tin;
	std::vector<int> low;
	std::vector<int> answer;

	int timer;
};


BridgeFinder::BridgeFinder(const std::vector<std::vector<std::pair<int, int>>>& _graph_with_indexes)
	: graph(&_graph_with_indexes)
	, visited(graph->size(), false)
	, tin(graph->size(), -1)
	, low(graph->size(), -1)
	, timer(0)
{
}

std::vector<int> BridgeFinder::findBridge()
{
	for (size_t i = 0; i < graph->size(); ++i)
	{
		if (!visited[i])
		{
			DFS(i);
		}
	}
  //mb set?
  std::sort(answer.begin(), answer.end());

	return answer;
}

void BridgeFinder::DFS(int v, int p)
{
	visited[v] = true;

	tin[v] = timer;
	low[v] = timer;
	timer++;

	for (auto i : (*graph)[v])
	{
		if (i.first != p)
		{
      if (visited[i.first])
      {
        low[v] = std::min(low[v], tin[i.first]);
      }
      else
      {
        DFS(i.first, v);

        low[v] = std::min(low[v], low[i.first]);
        if (low[i.first] > tin[v])
        {
          if(std::count_if( (*graph)[v].begin()
                          , (*graph)[v].end()
                          , [&](auto& p){ return p.first == i.first;}) == 1)
          {
            answer.push_back(i.second);
          }
        }
    }
		}
	}
}

int main()
{
  std::ifstream in("bridges.in");
  std::ofstream out("bridges.out");

	int V, E;
	in >> V >> E;
  //pair.second containts index of current edge {graph[i], graph[i].first} 
	std::vector<std::vector<std::pair<int, int>>> graph(V);

	int V1, V2;
	for (int i = 0; i < E; i++)
	{
	  in >> V1 >> V2;
		graph[V1 - 1].push_back(std::make_pair(V2 - 1, i + 1));
		graph[V2 - 1].push_back(std::make_pair(V1 - 1, i + 1));
	}

	BridgeFinder finder(graph);
	auto answer = finder.findBridge();

	out << answer.size() << std::endl;
	for (auto i : answer)
	{
		out << i << " ";
	}

  in.close();
  out.close();
	return 0;
}