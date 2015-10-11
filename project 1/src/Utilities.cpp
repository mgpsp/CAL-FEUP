#include "Utilities.h"

using namespace std;

bool exists(const vector<int> &v, int value)
{
	size_t i = 0;
	while (i < v.size() && v[i] != value)
		i++;

	if (i == v.size())
		return false;
	else
		return true;
}

bool edgeExists(const vector<int> &edges, int x, int y)
{
	size_t i = 0;
	while (i < edges.size() && !((edges[i] == x && edges[i + 1] == y) || (edges[i] == y && edges[i + 1] == x)))
		i+=2;

	if (i == edges.size())
		return false;
	else
		return true;
}
