#include "KruskalAlgorithm.h"

#include <iostream>
#include <algorithm>
#include <limits>
#include <float.h>

#include "Utilities.h"

using namespace std;

vector<int> kruskalAlgorithm(vector<Edge<NodeInfo> > &edges, const vector<int> &vertexSet, const vector<vector<int> > &nextVertex,const vector<vector<double> > &dist, double stationCost, int maxStations, vector<int> &stationsLocation)
{
	int size;
	if (vertexSet[vertexSet.size() - 2] > vertexSet[vertexSet.size() - 1])
		size = vertexSet[vertexSet.size() - 2] + 1;
	else
		size = vertexSet[vertexSet.size() - 1] + 1;

	vector<int> parent(size);
	vector<int> path;
	int stations = vertexSet.size();
	double totalCost = stations * stationCost;
	bool stop = false;
	int flag = 0;

	for (size_t i = 0; i < vertexSet.size(); i++)
		parent[vertexSet[i]] = vertexSet[i];

	size_t i = 0;
	while ((stations > maxStations || !stop) && i < edges.size())
	{
		flag = 0;

		// Check for cycles
		int srcParent = findParent(parent, edges[i].getSource()->getInfo().id);
		int dstParent = findParent(parent, edges[i].getDestination()->getInfo().id);

		if (srcParent != dstParent)
		{
			if(edges[i].getWeight() == numeric_limits<double>::infinity())
				break;

			double tempCost = totalCost - stationCost + edges[i].getWeight();
			if (tempCost < totalCost || stop)
			{
				parent[dstParent] = srcParent;

				totalCost = tempCost;
				stations--;
				vector<int> tempPath = getPath(path, nextVertex, parent, edges[i].getSource()->getInfo().id, edges[i].getDestination()->getInfo().id);

				path.insert(path.end(), tempPath.begin(), tempPath.end());
				updateEdges(edges, path, nextVertex, dist, i++);

				flag = 1;
			}
			else
			{
				stop = true;
				flag = 1;
			}
		}

		if (!flag)
			i++;

	}

	if (stations > maxStations)
	{
		cout << endl;
		cout << "\tYou need at least " << stations - maxStations << " more station(s)." << endl;
		stationsLocation.clear();
		return path;
	}


	if (stationsLocation.size() == 0) // no need to do this if the station's location is fixed
	{
		stationsLocation = placeStations(path, parent);
		for (size_t i = 0; i < vertexSet.size(); i++)
		{
			if (!exists(path, vertexSet[i]))
				stationsLocation.push_back(vertexSet[i]);
		}
	}

	cout << endl;
	cout << "\tNumber of placed stations: " << stations << endl;
	cout << "\tOptical fiber's length: " << totalCost - (stations * stationCost) << endl;
	cout << "\tTotal cost: " << totalCost << endl;

	return path;
}

int findParent(const vector<int> &parent, int index)
{
	if (parent[index] != index)
		index = findParent(parent, parent[index]);

	return index;
}

void updateEdges(vector<Edge<NodeInfo> > &edges, const vector<int> &path, const vector<vector<int> > &nextVertex, const vector<vector<double> > &dist, int i)
{
	double cost;
	int x;

	for (size_t j = i; j < edges.size(); j++)
	{
		int d = edges[j].getDestination()->getInfo().id;
		int s = edges[j].getSource()->getInfo().id;
		cost = 0;

		if(edges[j].getWeight() == numeric_limits<double>::infinity())
			return;

		while(s != d)
		{
			x = s;
			s = nextVertex[s][d];
			if (!edgeExists(path, x, s))
				cost += dist[x][s];
		}

		edges[j].setWeight(cost);
	}

	sort(edges.begin() + i, edges.end());
}

vector<int> getPath(const vector<int> &path, const vector<vector<int> > &nextVertex, vector<int> &parent, int i, int j)
{
	vector<int> newPath;
	int x, y = i;

	while (y != j)
	{
		x = y;
		y = nextVertex[y][j];
		if (!edgeExists(path, x, y))
		{
			if (parent[y] == 0)
				parent[y] = x;
			newPath.push_back(x);
			newPath.push_back(y);
		}
	}

	return newPath;
}

vector<int> placeStations(const vector<int> &path, const vector<int> &parent)
{
	vector<int> stations;
	vector<int> treesWithStation;

	for (size_t i = 0; i < path.size(); i++)
	{
		int p = findParent(parent, path[i]);
		if (!exists(treesWithStation, p))
		{
			stations.push_back(path[i]);
			treesWithStation.push_back(p);
		}
	}

	return stations;
}
