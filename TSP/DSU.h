#pragma once

#include <vector>

class DSU 
{
public:
	explicit DSU(int n);
	int find(int u);
	void merge(int u, int v);

private:
	std::vector<int> parent;
	std::vector<int> rank;
};