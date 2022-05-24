#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace sf;



class Graph
{
public:
	vector<int> vertex_list;
	vector<vector<int>> matrix;
	vector<vector<int>> copyMatrix;
	vector<vector<int>> grafMatrix;
	vector<pair<int, int>> list_for_paths;
	int max_size=15;
	int sizee;
	int EdgeMas[100] = { 11, 6, 5, 14, 20, 18, 9, 8, 13 };
	Graph();

	Graph(const Graph& another_one);

	Graph(const int& size);

	void setGraph(const int& size);

	int size();

	bool is_full();

	bool is_empty();

	int summa(vector<int> way);

	void insert_vertex(const int& vertex);

	int get_index(const int& vertex);

	int get_weight(const int& vertex1, const int& vertex2);

	vector<int> get_neighbour_list(const int& vertex);

	void insert_edge(const int& vrtx1, const int& vrtx2, const int& weight);

	void generate(int vrtx_count, int edge_count);

	void delete_vertex(const int &vertex, const int &new_size);
	

	void set_start(vector<int>& route, const int& start);
	vector<int> build_route(const int& start);

	friend ostream& operator<<(ostream&, const Graph& to_print);
	void clear();
	vector<int> recursive_sailsman(vector<int> visited, int finish);
	int get_route_distance(const vector<int>& route);
	~Graph();
};
bool check_insertion(vector<int> vctr, const int& element);
ostream& operator<<(ostream& stream, Graph& to_print);
void print(const vector<vector<int>>& matrix, const vector<int> names);
#endif