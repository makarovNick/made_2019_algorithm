#include "DSU.h"

DSU::DSU(int n)
    : parent(n)
    , rank(n, 1)
{
    for (int i = 0; i < n; ++i)
    {
        parent[i] = i;
    }
}

int DSU::find(int u)
{
    if (parent[u] == u)
    {
        return u;
    }

    return parent[u] = find(parent[u]);
}

void DSU::merge(int u, int v)
{
    const int rootU = find(u);
    const int rootV = find(v);

    if (rank[rootU] < rank[rootV])
    {
        parent[rootU] = rootV;
    }
    else
    {
        if (rank[rootU] == rank[rootV])
        {
            ++rank[rootU];
        }

        parent[rootV] = rootU;
    }
}