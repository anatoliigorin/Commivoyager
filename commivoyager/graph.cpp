#include "graph.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <windows.h>



Graph::Graph()
{
	max_size = 0;
}
Graph::Graph(const Graph& another_one)
{
	vertex_list = another_one.vertex_list;
	matrix = another_one.matrix;
	max_size = another_one.max_size;
}
Graph::Graph(const int& size)
{
	sizee = size;
	matrix = vector<vector<int>>(max_size, vector<int>(max_size));

	for (int i = 0; i < sizee; ++i)
	{
		for (int j = 0; j < sizee; ++j)
		{
			matrix[i][j] = 0;
		}
	}
}
void Graph::setGraph(const int& size)
{
	sizee = size;
	matrix = vector<vector<int>>(sizee, vector<int>(sizee));

	for (int i = 0; i < sizee; ++i)
	{
		for (int j = 0; j < sizee; ++j)
		{
			matrix[i][j] = 0;
		}
	}
}
void Graph::clear()
{
	vertex_list.clear();
	matrix.clear();
	copyMatrix.clear();
	grafMatrix.clear();
	max_size = 10;
	sizee = 0;
}
int Graph::size()
{
	return vertex_list.size();
}
bool Graph::is_full()
{
	return (size() == max_size);
}
bool Graph::is_empty()
{
	return (size() == 0);
}
void Graph::insert_vertex(const int& vertex)
{
	if (is_full())
		cout << "Невозможно добавить еще одну вершину" << endl;
	else
	{
		vertex_list.push_back(vertex);
	}
}
void Graph::delete_vertex(const int& vertex, const int& new_size)
{
	vertex_list.erase(vertex_list.begin() + vertex);
	matrix.erase(matrix.begin() + vertex);
	for (auto& line : matrix)
	{
		line.erase(line.begin() + vertex);
	}
	copyMatrix.clear();
	grafMatrix.clear();
	list_for_paths.clear();
	sizee = new_size;
}
int Graph::get_index(const int& vertex)
{
	for (int i = 0; i < vertex_list.size(); ++i)
	{
		if (vertex_list[i] == vertex)
			return i;
	}
	return -1;
}
int Graph::get_weight(const int& vertex1, const int& vertex2)
{
	if (is_empty())
		return 0;

	int pos1 = get_index(vertex1);
	int pos2 = get_index(vertex2);

	if (pos1 == -1 or pos2 == -1)
	{
		cout << "Как минимум одна из вершин отсутствует в графе" << endl;
		return 0;
	}

	return matrix[pos1][pos2];
}
vector<int> Graph::get_neighbour_list(const int& vertex)
{
	vector<int> neighbour_list;
	int pos = get_index(vertex);

	if (pos != -1)
	{
		for (int i = 0; i < sizee; ++i)
		{
			if (matrix[pos][i] != 0)
				neighbour_list.push_back(i + 1);
		}
	}

	return neighbour_list;
}
void Graph::insert_edge(const int& vrtx1, const int& vrtx2, const int& weight = 1)
{
	int pos1 = get_index(vrtx1);
	int pos2 = get_index(vrtx2);

	if (pos1 == -1 or pos2 == -1)
		cout << "Как минимум одна из вершин отсутствует в графе" << endl;
	else
		if (matrix[pos1][pos2] == 0 and matrix[pos2][pos1] == 0)
		{
			matrix[pos1][pos2] = weight;
			matrix[pos2][pos1] = weight;
		}
		else
			cout << "Ребро уже существует" << endl;
}

void Graph::generate(int vrtx_count, int edge_count)
{
	int vrtx;
	int vrtx1;
	int vrtx2;
	int weight;

	for (int i = 0; i < vrtx_count; ++i)
	{
		vrtx = i + 1;
		insert_vertex(vrtx);
		cout << endl;
	}
	insert_edge(1, 2, EdgeMas[0]);
	insert_edge(2, 3, EdgeMas[1]);
	insert_edge(3, 4, EdgeMas[2]);
	insert_edge(4, 5, EdgeMas[3]);
	insert_edge(5, 6, EdgeMas[4]);
	insert_edge(6, 2, EdgeMas[5]);
	insert_edge(1, 3, EdgeMas[6]);
	insert_edge(1, 4, EdgeMas[7]);
	insert_edge(5, 3, EdgeMas[8]);

}

int Graph::summa(vector<int> way) {
	int sum = 0;
	for (int i = 0; i < way.size(); i++)
		sum = get_route_distance(way);
	return sum;
}

vector<int> Graph::recursive_sailsman(vector<int> visited, int finish)
{
	//Все возможные пути
	vector<vector<int>> options;

	//Кратчайший путь и его длина
	vector<int> final_route;
	int shortest = -1;

	//Промежуточный путь для рекурсивых повторений функции
	vector<int> da_way;

	//Отмечаем посещенную вершину
	visited.push_back(finish);

	//Выполняется, если посещены еще не все вершины
	if (visited.size() < vertex_list.size())
	{
		for (int i = 0; i < vertex_list.size(); ++i)
		{
			//Если вершины нет среди посещенных и к ней можно пройти, то дальше посещаем ее
			if (check_insertion(visited, vertex_list[i]) == false && get_weight(vertex_list[i], finish) > 0)
			{
				options.push_back(recursive_sailsman(visited, vertex_list[i]));
			}
		}

		//Если есть несколько путей, то выбираем кратчайший
		if (options.size() > 0)
		{
			for (int i = 0; i < options.size(); ++i)
			{
				if (options[i].size() > 0)
				{
					if (((get_route_distance(options[i]) + get_weight(options[i][0], finish)) < shortest) || shortest == -1)
					{
						shortest = get_route_distance(options[i]) + get_weight(options[i][0], finish);
						final_route = options[i];
					}
				}
			}

			//Если нашли какой-то путь, то добавим его к возвращаемому
			if (final_route.size() > 0)
			{
				da_way.push_back(finish);

				for (int i = 0; i < final_route.size(); ++i)
				{
					da_way.push_back(final_route[i]);
				}
			}
		}

		return da_way;
	}
	//Если посещены все вершины, то возвращаем итоговый маршрут
	else
	{
		//Если из конечной точки нет прямого пути в точку начала, то вернем пустой вектор
		if (get_weight(vertex_list[0], finish) > 0)
		{
			final_route.push_back(finish);
			final_route.push_back(vertex_list[0]);
		}

		return final_route;
	}
}
void Graph::set_start(vector<int>& route, const int& start)
{
	if (route.size() > 0)
	{
		if (count(route.begin(), route.end(), start) > 0)
		{
			if (route[0] != start)
			{
				vector<int> ans;
				int start_i = 0;
				bool flag_break = false;
				for (int i = 0; i < route.size() - 1 && flag_break == false; ++i)
				{
					if (route[i] == start)
					{
						flag_break = true;
						start_i = i;
					}
				}
				for (int i = start_i; i < route.size() - 1; ++i)
				{
					ans.push_back(route[i]);
				}
				for (int i = 0; i < start_i; ++i)
				{
					ans.push_back(route[i]);
				}
				ans.push_back(start);
				route = ans;
			}
		}
	}
}
void print(const vector<vector<int>>& matrix, const vector<int> names)
{
	int size = names.size();

	for (int i = 0; i < size; ++i)
	{
		cout << i << ": " << names[i] << endl;
	}

	cout << endl << "-  ";

	for (int i = 0; i < size; ++i)
	{
		cout << i+1 << "   ";
	}

	cout << endl;

	for (int i = 0; i < size; ++i)
	{
		cout << i+1 << " ";

		for (int j = 0; j < size; ++j)
		{
			if (matrix[i][j] >= 0 && matrix[i][j] <= 9)
			{
				cout << " " << matrix[i][j] << "  ";
			}
			else
			{
				cout << " " << matrix[i][j] << " ";
			}
		}

		cout << endl;
	}
}
vector<int> Graph::build_route(const int& start)
{
	vector<int> visited;
	vector<int> ans = recursive_sailsman(visited, vertex_list[0]);
	set_start(ans, start);
	cout << "---------------------------------------------------------" << endl;
	print(matrix, vertex_list);
	cout << endl;
	cout << "Длина маршрута: " << get_route_distance(ans) << endl;
	return ans;
}
int Graph::get_route_distance(const vector<int>& route)
{
	int dist = 0;
	for (int i = route.size() - 1; i > 0; --i)
	{
		dist += get_weight(route[i - 1], route[i]);
	}
	return dist;
}
Graph::~Graph() {};

ostream& operator<<(ostream& stream, Graph& to_print)
{
	if (to_print.is_empty())
		stream << "Граф пуст" << endl;
	else
	{
		stream << "-  ";

		for (int i = 0; i < to_print.size(); ++i)
			stream << to_print.vertex_list[i] << "  ";

		stream << endl;

		for (int i = 0; i < to_print.size(); ++i)
		{
			stream << to_print.vertex_list[i] << " ";

			for (int j = 0; j < to_print.size(); ++j)
				stream << " " << to_print.matrix[i][j] << " ";

			stream << endl;
		}
		stream << endl;

		return stream;
	}
}
bool check_insertion(vector<int> vctr, const int& element)
{
	for (int i = 0; i < vctr.size(); ++i)
		if (vctr[i] == element)
			return true;
	return false;
}