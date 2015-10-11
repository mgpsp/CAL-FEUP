#include <iostream>
#include <limits>
#include <algorithm>

#include "Graph.h"
#include "GraphUtilities.h"
#include "graphviewer.h"
#include "FloydWarshallAlgorithm.h"
#include "KruskalAlgorithm.h"
#include "Utilities.h"

void mainMenu();

using namespace std;

void settingsMenu(vector<int> houses, const Graph<NodeInfo> &graph)
{
	cout << endl;
	cout << "\t1. View graph" << endl;
	cout << "\t2. Get network layout - fixed single station" << endl;
	cout << "\t3. Get network layout - variable number of stations" << endl;
	cout << "\t4. Back" << endl;
	cout << endl;
	cout << "\t> ";

	int op;
	cin >> op;

	while (op < 1 || op > 4)
	{
		cin.clear();
		cin.ignore(100, '\n');
		cout << "\t> ";
		cin >> op;
	}

	vector<int> path;
	vector<int> stations;
	int numVertex = graph.getNumVertex();
	vector<vector<double> > dist (numVertex, vector<double>(numVertex, numeric_limits<double>::infinity()));
	vector<vector<int> > nextVertex (numVertex, vector<int>(numVertex, NULL));
	vector<Edge<NodeInfo> > edges;
	vector<Vertex<NodeInfo> *> vertexes = graph.getVertexSet();
	int maxStations;
	double stationCost;
	int location, flag;

	switch(op)
	{
	case 1:
		printGraph(graph, path, stations);
		settingsMenu(houses, graph);
		break;
	case 2:
		flag = 0;
		cout << endl;
		cout << "\tStation's location: ";
		cin >> location;
		floydWarshallAlgorithm(graph, dist, nextVertex);

		if (!exists(houses, location))
		{
			flag = 1;
			houses.push_back(location);
		}
		stations.push_back(location);

		for (size_t i = 0; i < houses.size(); i++)
		{
			for (size_t j = i + 1; j < houses.size(); j++)
			{
				Edge<NodeInfo> edge(vertexes[houses[i]], vertexes[houses[j]], dist[houses[i]][houses[j]]);
				edges.push_back(edge);
			}
		}

		sort(edges.begin(), edges.end());

		path = kruskalAlgorithm(edges, houses, nextVertex, dist, 0, 1, stations);
		if (stations.size() != 0)
			printGraph(graph, path, stations);
		else
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "\n\tPress enter to continue...";
			cin.get();
		}
		if (flag)
			houses.pop_back();
		settingsMenu(houses, graph);
		break;
	case 3:
		cout << endl;
		cout << "\tMaximum number of stations: ";
		cin >> maxStations;
		cout <<"\tStation cost: ";
		cin >> stationCost;

		floydWarshallAlgorithm(graph, dist, nextVertex);

		for (size_t i = 0; i < houses.size(); i++)
		{
			for (size_t j = i + 1; j < houses.size(); j++)
			{
				Edge<NodeInfo> edge(vertexes[houses[i]], vertexes[houses[j]], dist[houses[i]][houses[j]]);
				edges.push_back(edge);
			}
		}

		sort(edges.begin(), edges.end());

		path = kruskalAlgorithm(edges, houses, nextVertex, dist, stationCost, maxStations, stations);
		if (stations.size() != 0)
			printGraph(graph, path, stations);
		else
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "\n\tPress enter to continue...";
			cin.get();
		}
		settingsMenu(houses, graph);
		break;
	case 4:
		mainMenu();
		break;
	default:
		settingsMenu(houses, graph);
		break;
	}
}

void mainMenu()
{
	cout << endl;
	cout << "\t1. Load graph from file" << endl;
	cout << "\t2. Get random graph" << endl;
	cout << "\t3. Exit" << endl;
	cout << endl;
	cout << "\t> ";

	int op;
	cin >> op;

	while (op < 1 || op > 3)
	{
		cin.clear();
		cin.ignore(100, '\n');
		cout << "\t> ";
		cin >> op;
	}

	int vertexes;
	vector<int> houses;
	Graph<NodeInfo> graph;
	switch(op)
	{
	case 1:
		graph = loadGraph(houses, "vertexes1.txt", "edges1.txt");
		settingsMenu(houses, graph);
		break;
	case 2:
		cout << endl;
		cout << "\tNumber of vertexes: ";
		cin >> vertexes;
		graph = getRandomGraph(houses, vertexes);
		settingsMenu(houses, graph);
		break;
	case 3:
		return;
		break;
	default:
		mainMenu();
		break;
	}
}

int main()
{
	mainMenu();
	return 0;
}
