#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <codecvt>
#include "graph.h"

using namespace std;
using namespace sf;
/*=========ВЕРШИНЫ И РЕБРА===========*/
const double PI = 3.1415926535;
class DataVertex : public CircleShape
{
public:
	Text text;
	Texture defTexture;
	Texture* defTexture1;
	CircleShape circle;
	Font font;
	vector<int> edgescodes;
	int code;
public:
	DataVertex() //Конструктор по умолчанию
	{
		font.loadFromFile("consolas.ttf");
		this->text.setFillColor(Color::White);
		this->text.setOutlineThickness(1);
		this->text.setOutlineColor(Color::Black);
		this->text.setFont(font);
		this->text.setCharacterSize(15);
		this->text.setStyle(Text::Bold);
		this->circle.setFillColor(Color::White);
		this->circle.setOutlineThickness(0);
		this->circle.setOutlineColor(Color::Black);
		this->code = -1;
		this->defTexture.loadFromFile("5vertex.png");
		defTexture1 = &defTexture;
		this->circle.setTexture(defTexture1);
	};
	void deleteVert()
	{
		setPosition(-200, -200);

	}
	void setCode(int code)
	{
		this->code = code;
	}
	void setTexture(Texture *texture)
	{
		this->circle.setTexture(texture);
	}
	void setPosition(int x, int y)
	{
		this->text.setPosition(x, y);
		this->circle.setPosition(x, y);
	};
	Vector2f getPosition()
	{
		return circle.getPosition();
	}
	void move(int x, int y)
	{
		this->circle.move(x, y);
		this->text.move(x, y);
	};
	void setText(String text)
	{
		this->text.setString(text);
	};
	void setRadius(int r)
	{
		this->circle.setRadius(r);
	}
	int getRadius() { return this->circle.getRadius(); }
	void draw(RenderWindow& window)
	{
		window.draw(this->circle);
		int len_str = this->text.getString().getSize();
		this->text.move(circle.getRadius() - len_str * 4, circle.getRadius() * 0.5);

		window.draw(this->text);
		this->text.move(-circle.getRadius() + len_str * 4, -circle.getRadius() * 0.5);
	};

	void DrawVoyager()
	{
		Color dark_green = { 0,150,0 };
		this->circle.setFillColor(dark_green);
	}

};
vector<DataVertex> vertexes; // Содержит в себе все вершины

class DataEdge				//Класс ребер
{
public:
	Text text;
	RectangleShape line;
	Font font;
	pair<DataVertex, DataVertex> vertexs;
	bool Enabled = true;
	bool hasVertexs = false;
public:
	DataEdge()
	{
		font.loadFromFile("consolas.ttf");
		this->text.setFillColor(Color::Black);
		this->text.setFont(font);
		this->text.setCharacterSize(15);
		this->text.setStyle(Text::Bold);
		this->text.setOutlineThickness(5);
		this->text.setOutlineColor(Color::White);
		this->line.setFillColor(Color::Black);
	}
	void setText(String string)
	{
		this->text.setString(string);
	}

	bool hasVertex()
	{
		return hasVertexs;
	}
	void delVertex(DataVertex& vertex)
	{
		if (vertex.code == vertexs.first.code || vertex.code == vertexs.second.code)
		{
			this->text.setPosition(-200, -200);
			this->line.setPosition(-200, -200);
			this->line.setSize(Vector2f(1, 1));
			this->Enabled = false;
		}
	}
	void setVertex(DataVertex& vertex1, DataVertex& vertex2, string text, int code) //Вызывается при начальном связывании двух вершин ребром
	{
		if (this->Enabled == true)
		{
			hasVertexs = true;
			Vector2f pos1 = vertex1.getPosition();
			Vector2f pos2 = vertex2.getPosition();
			int catetX; int catetY; double lenght; double angle;
			vertexs.first = vertex1; vertexs.second = vertex2;
			vertex1.edgescodes.push_back(code);
			vertex2.edgescodes.push_back(code);
			catetX = abs(pos2.x - pos1.x);
			catetY = abs(pos2.y - pos1.y);
			lenght = sqrt(catetX * catetX + catetY * catetY);
			this->line.setSize(Vector2f(lenght, 5));
			this->line.setFillColor(Color::Black);
			angle = asin(catetY / lenght);
			if (((pos2.x - pos1.x) >= 0) && ((pos2.y - pos1.y) >= 0))
				this->line.rotate(angle * (180 / PI));
			else if (((pos2.x - pos1.x) < 0) && ((pos2.y - pos1.y) >= 0))
			{
				this->line.rotate(180 - (angle * (180 / PI)));
			}
			else if (((pos2.x - pos1.x) < 0) && ((pos2.y - pos1.y) < 0))
			{
				this->line.rotate(180 + (angle * (180 / PI)));
			}
			else if (((pos2.x - pos1.x) >= 0) && ((pos2.y - pos1.y) < 0))
			{
				this->line.rotate(0 - (angle * (180 / PI)));
			}
			Vector2f pos(pos1.x + vertex1.getRadius(), pos1.y + vertex1.getRadius());
			this->line.setPosition(pos);

			this->text.setString(text);
			this->text.setPosition(pos.x + (pos2.x - pos1.x) / 2, pos.y + (pos2.y - pos1.y) / 2 + 10);
		}
	};
	void MoveEdge(DataVertex& vertex1, DataVertex& vertex2) //Используется при перемещении вершин
	{
		if (this->Enabled == true)
		{
			Vector2f pos1 = vertex1.getPosition();
			Vector2f pos2 = vertex2.getPosition();
			int catetX; int catetY; double lenght; double angle;
			catetX = abs(pos2.x - pos1.x);
			catetY = abs(pos2.y - pos1.y);
			lenght = sqrt(catetX * catetX + catetY * catetY);

			this->line.setSize(Vector2f(lenght, 5));
			this->line.setFillColor(Color::Black);
			angle = asin(catetY / lenght);
			if (((pos2.x - pos1.x) >= 0) && ((pos2.y - pos1.y) >= 0))
				this->line.setRotation(angle * (180 / PI));
			else if (((pos2.x - pos1.x) < 0) && ((pos2.y - pos1.y) >= 0))
			{
				this->line.setRotation(180 - (angle * (180 / PI)));
			}
			else if (((pos2.x - pos1.x) < 0) && ((pos2.y - pos1.y) < 0))
			{
				this->line.setRotation(180 + (angle * (180 / PI)));
			}
			else if (((pos2.x - pos1.x) >= 0) && ((pos2.y - pos1.y) < 0))
			{
				this->line.setRotation(0 - (angle * (180 / PI)));
			}
			Vector2f pos(pos1.x + vertex1.getRadius(), pos1.y + vertex1.getRadius());
			this->line.setPosition(pos);

			this->text.setPosition(pos.x + (pos2.x - pos1.x) / 2, pos.y + (pos2.y - pos1.y) / 2 + 10);
		}
	};
	void draw(RenderWindow& window)
	{
		window.draw(this->line);
		window.draw(this->text);
	};
};

class Button : public RectangleShape
{
public:
	Text text;
	RectangleShape button;
	Font font;
	Color dark_blue{0,0,151};
public:
	Button() {
		this->button.setFillColor(Color::Blue);
		this->button.setOutlineColor(dark_blue);
		this->button.setOutlineThickness(2);

		font.loadFromFile("consolas.ttf");
		this->text.setFillColor(Color::White);
		this->text.setFont(font);
		this->text.setStyle(Text::Bold);
		this->text.setOutlineThickness(1);
		this->text.setOutlineColor(Color::Black);
		
	}
	void setText(String text)
	{
		this->text.setString(text);
	}
	void setPosition(int x, int y)
	{
		this->button.setPosition(x, y);
		this->text.setPosition(this->button.getPosition());
		this->text.move(10, this->button.getSize().y / 4);
	}
	void setTextSize(int size)
	{
		this->text.setCharacterSize(size);
		int len_str = this->text.getString().getSize();
		this->button.setSize(Vector2f(178, this->text.getCharacterSize() * 2));
	}
	void draw(RenderWindow& window)
	{
		window.draw(this->button);
		window.draw(this->text);
	};
};

String setWayText(vector<int> way)
{
	String temp;
	if (way.size() == 0)
	{
		temp = L"Решения нет";
	}
	else
	{
		temp = to_string(way[0]);
		for (int i = 1; i < way.size(); i++) {
			temp += " -> " + to_string(way[i]);
		}
	}
	
	return temp;
}

struct pos {
	int x, y;    //Для хранения координат мыши
};

void AddVertex(DataVertex *vertex, int &size, Graph& graph)
{
	wstring name;
	cout << "Введите название вершины: ";
	wcin >> name;
	vertex[size].setRadius(50);
	vertex[size].setText(to_string(size+1) + L"." + name);
	vertex[size].setPosition(100, 100);
	vertex[size].setCode(size);
	vertexes.push_back(vertex[size]);
	size++;
	graph.vertex_list.push_back(size);
}
void AddEdge(DataEdge* edge, DataVertex& vertex1, DataVertex& vertex2, int &size, Graph &graph)
{
	bool f = true;
	for (int i = 0; i < vertex1.edgescodes.size(); i++)
	{
		for (int j = 0; j < vertex2.edgescodes.size(); j++)
			if (vertex1.edgescodes[i] == vertex2.edgescodes[j])
			{
				f = false;
			}
	}
	if (f == false) cout << "Ребро уже существует! " << endl;
	else
	{
	int weight;
	cout << endl << "Введите вес ребра: ";
	cin >> weight;
	graph.EdgeMas[size] = weight;
	graph.insert_edge(vertex1.code+1, vertex2.code+1, weight);
	edge[size].setVertex(vertex1, vertex2, to_string(weight), size);
	size++;
	}
}

int main()
{
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);

	const int MaxSize = 20;
	const int MaxEdgesSize = 100;
	int size = 6;
	int sizeEdges = 9;

	Graph example(size);		//Работа с графом и матрицей в консоли
	bool HasRoadV = true;
	Graph exampleNew(size);
	example.generate(size, sizeEdges);
	cout << endl << example << endl;
	cout << "Расстояние от 1 вершины до других вершин: " << endl;
	int starts = example.vertex_list[0];
	vector<int> way = example.build_route(starts);
	int sum = example.summa(way);  

	String ForText = (setWayText(way)) + "\nCymma: " + to_string(sum) + " KM";
	Text route; Text label1; Font font; font.loadFromFile("consolas.ttf");

	label1.setString(L"Маршрут доставщика Яндекс.еды:");
	label1.setPosition(1250, 750);
	label1.setFont(font);
	label1.setCharacterSize(30);
	label1.setFillColor(Color::White);
	label1.setOutlineThickness(1);
	label1.setOutlineColor(Color::Black);

	route.setString(ForText);
	route.setPosition(1250, 800);
	route.setFont(font);
	route.setCharacterSize(30);
	route.setFillColor(Color::White);
	route.setOutlineThickness(1);
	route.setOutlineColor(Color::Black);

	Texture background; 
	background.loadFromFile("background.png");
	Sprite sprite;
	sprite.setTexture(background);

	Button start;
	start.setText(L"Запустить алгоритм");
	start.setPosition(1600, 100);
	start.setTextSize(15);
	Button release;
	release.setText(L"Сбросить");
	release.setPosition(1600, 150);
	release.setTextSize(17);
	Button add;
	add.setText(L"Добавить вершину");
	add.setPosition(1600, 200);
	add.setTextSize(17);

	RenderWindow MainWindow(VideoMode(1800, 900), L"Коммивояжер");


	//Указатели на элементы
	CircleShape* selectedCircle = nullptr;
	Text* selectedText = nullptr;
	DataEdge* seledge[100] = { nullptr };

	//Вершины
	DataVertex vertex[MaxSize];

	Texture texture[MaxSize];
	Texture *current = nullptr;
	Texture* rand = nullptr;

	texture[0].loadFromFile("0vertex.png");
	texture[1].loadFromFile("1vertex.png");
	texture[2].loadFromFile("2vertex.png");
	texture[3].loadFromFile("3vertex.png");
	texture[4].loadFromFile("4vertex.png");
	texture[5].loadFromFile("5vertex.png");
	texture[6].loadFromFile("5vertex.png");
	texture[7].loadFromFile("5vertex.png");
	texture[8].loadFromFile("5vertex.png");
	texture[9].loadFromFile("5vertex.png");
	texture[10].loadFromFile("4vertex.png");
	texture[11].loadFromFile("5vertex.png");
	texture[12].loadFromFile("5vertex.png");
	texture[13].loadFromFile("5vertex.png");
	texture[14].loadFromFile("5vertex.png");
	texture[15].loadFromFile("5vertex.png");

	Texture visited;
	visited.loadFromFile("0vertexA.png");

	for (int i = 0; i < size; i++)
	{
		vertex[i].setRadius(50);
		vertex[i].setCode(i);
		vertexes.push_back(vertex[i]);
	}
	current = &texture[0];
	vertex[0].setTexture(current);
	vertex[0].setText(L"1.ПНИПУ");
	vertex[0].setPosition(693, 118);

	current = &texture[1];
	vertex[1].setTexture(current);
	vertex[1].setText(L"2.Вокзал");
	vertex[1].setPosition(456, 649);

	current = &texture[2];
	vertex[2].setTexture(current);
	vertex[2].setText(L"3.ЦУМ");
	vertex[2].setPosition(810, 562);

	current = &texture[3];
	vertex[3].setTexture(current);
	vertex[3].setText(L"4.Цирк");
	vertex[3].setPosition(1023, 481);

	current = &texture[4];
	vertex[4].setTexture(current);
	vertex[4].setText(L"5.Закамск");
	vertex[4].setPosition(179, 291);

	current = &texture[5];
	vertex[5].setTexture(current);
	vertex[5].setText(L"6.Садовый");
	vertex[5].setPosition(1274, 657);

	//Ребра
	DataEdge edge[MaxEdgesSize];
	edge[0].setVertex(vertex[0], vertex[1], to_string(example.EdgeMas[0]),0); 
	edge[1].setVertex(vertex[1], vertex[2], to_string(example.EdgeMas[1]),1); 
	edge[2].setVertex(vertex[2], vertex[3], to_string(example.EdgeMas[2]),2); 
	edge[3].setVertex(vertex[3], vertex[4], to_string(example.EdgeMas[3]),3);
	edge[4].setVertex(vertex[4], vertex[5], to_string(example.EdgeMas[4]),4);
	edge[5].setVertex(vertex[5], vertex[1], to_string(example.EdgeMas[5]),5); 
	edge[6].setVertex(vertex[0], vertex[3], to_string(example.EdgeMas[6]),6); 
	edge[7].setVertex(vertex[4], vertex[2], to_string(example.EdgeMas[7]),7);
	edge[8].setVertex(vertex[0], vertex[2], to_string(example.EdgeMas[8]),8);

	pos old_mouse = { 0 };
	int vertexCode;

	int step = 10000000;
	int i = 0;
	int createEdge = 0;
	int pair1;
	int pair2;

	Clock clock;
	float timer = 0;

	//Ввод названий вершин и весов ребер
	bool ClickedVertex = false;
	int ClickCode1 = -1;

	while (MainWindow.isOpen())
	{
		Event event;

		/*=======НАЗВАНИЯ ВЕРШИН========*/
		if (ClickedVertex == true)
		{	
			wstring input;
			ClickedVertex = false;
			cout << endl << "Введите новое название вершины: ";
			wcin >> input;
			vertex[ClickCode1].setText(to_string(ClickCode1+1) + "." + String(input));
		}

		/*=====ТАЙМЕР========*/
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		timer += time;
		if (timer > step + 1000)
		{
			current = &visited; //visited и current - имена объектов текстур
			vertex[vertexes[way[i]-1].code].circle.setTexture(current);
			i++;
			step = timer;
		}
		if (i == size) { step = 10000000; timer = 0; };


		/*========СОБЫТИЯ========*/
		while (MainWindow.pollEvent(event)) {
			switch (event.type)
			{
			case Event::Closed:
			{
				// "закрыть окно"
				MainWindow.close();
				break;
			}
			case Event::KeyPressed:
			{
				if (event.key.code == Keyboard::LShift)
				{
					auto mb = Mouse::getPosition(MainWindow);
					cout << endl;
					
					for (int i = 0; i < size; i++)
					{
						if (vertex[i].circle.getGlobalBounds().contains(mb.x, mb.y))
						{
							cout << "Нажата вершина под номером " << i+1;
							switch (createEdge)
							{
							case 0: pair1 = i; createEdge++; break;
							case 1:
								pair2 = i;
								AddEdge(edge, vertex[pair1], vertex[pair2], sizeEdges, example);
								pair1 = 0; pair2 = 0;
								createEdge = 0;
								way = example.build_route(starts);
								sum = example.summa(way);
								ForText = (setWayText(way)) + "\nCymma: " + to_string(sum) + " KM";
								route.setString(ForText);
								break;
							}
						}
					}

				}
				if (event.key.code == Keyboard::Delete)
				{
					auto mb = Mouse::getPosition(MainWindow);
					for (int i = 0; i < size; i++)
					{
						if (vertex[i].circle.getGlobalBounds().contains(mb.x, mb.y))
						{
							for (int k = 0; k < vertex[i].edgescodes.size(); k++)
								for (int j = 0; j < sizeEdges; j++)
									if (vertex[i].edgescodes[k] == j)
										edge[j].delVertex(vertex[i]);
							vertex[i].deleteVert();
							int it = vertex[i].code;
							for (int k = 0; k < vertexes.size(); k++)
							{
								if (vertexes[k].code == it)
									vertexes.erase(vertexes.begin() + k);
							}
							for (int k = 0; k < vertexes.size(); k++)		//ВЫВОД МАССИВА ВЕРШИН
							{
								cout << vertexes[k].code;
							}
							example.delete_vertex(example.get_index(i+1), example.sizee - 1);
							cout << endl << example;
							if (HasRoadV == false)
							{
								ForText = L"Задача не имеет решения";
								route.setString(ForText);
							}
							else
							{
								way = example.build_route(starts);
								sum = example.summa(way);
								ForText = (setWayText(way)) + "\nCymma: " + to_string(sum) + " KM";
								route.setString(ForText);
							}
						}
					}
				}
			}
			case Event::MouseButtonPressed: 
			{
				if (event.mouseButton.button == Mouse::Right)
				{
					auto mb = event.mouseButton;

					for (int k = 0; k < size; k++)
					{
						if (vertex[k].circle.getGlobalBounds().contains(mb.x, mb.y))
						{
							ClickCode1 = k;
							ClickedVertex = true;
						}
					}
				}
				if (event.mouseButton.button == Mouse::Left)
				{
					auto mb = event.mouseButton;
					if (add.button.getGlobalBounds().contains(mb.x, mb.y))
					{
						AddVertex(vertex, size, example);
						way = example.build_route(starts);
						sum = example.summa(way);
						ForText = (setWayText(way)) + "\nCymma: " + to_string(sum) + " KM";
						route.setString(ForText);
						for (int k = 0; k < vertexes.size(); k++)		//ВЫВОД МАССИВА ВЕРШИН
						{
							cout << vertexes[k].code;
						}
					}
					if (start.button.getGlobalBounds().contains(mb.x, mb.y))
					{
						timer = 0;
						step = 0;
					}
					if (release.button.getGlobalBounds().contains(mb.x, mb.y))
					{
						timer = 0;
						step = 10000000;
						i = 0;
						for (int i = 0; i < size; i++)
						{
							current = &texture[i];
							vertex[i].circle.setTexture(current);
						}
					}

					
					// если объект не выбран
					if (selectedCircle == nullptr) //Вершины не будут "цепляться" при перемещении - если выбрали один объект, второй не выберется
					{
						// Смотрим где сейчас мышь
						// и проверяем, не попали ли мы в какой объект
						for (int i = 0; i < size; i++)
						{
							if (vertex[i].circle.getGlobalBounds().contains(mb.x, mb.y))
							{
								// да попали
								// запоминаем текущие координаты мыши
								old_mouse = { mb.x, mb.y };
								// запоминаем выбранный объект
								selectedCircle = &vertex[i].circle;
								selectedText = &vertex[i].text;
								for (int k = 0; k < sizeEdges; k++)
								{
									for (int j = 0; j < vertex[i].edgescodes.size(); j++)
										if (k == vertex[i].edgescodes[j])
												seledge[k] = &edge[k];
								}
								vertexCode = i;
							}
						}
					}
				}  
				break;
			}
			case sf::Event::MouseMoved:
			{
				// мышка сдвинулась
				auto mm = event.mouseMove;
				auto mb = event.mouseButton;
				// есть кого перемещать?
				if (selectedCircle!=nullptr)
				{
					// да, есть

					// рассчитаем смещение мыши
					pos diff = {
						old_mouse.x - mm.x,
						old_mouse.y - mm.y
					};
					// запомним текущие координаты
					old_mouse = { mm.x, mm.y };
					// переместим выбранный объект
					selectedCircle->move(-diff.x, -diff.y);
					selectedText->move(-diff.x, -diff.y);

					//Перемещаем связанные с вершиной ребра
					for (int k = 0; k < sizeEdges; k++)
					{
						if (seledge[k] != nullptr)
								seledge[k]->MoveEdge(vertex[seledge[k]->vertexs.first.code], vertex[seledge[k]->vertexs.second.code]);
					}
				}
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				// Отпущена левая кнопка мыши
				if (event.mouseButton.button == Mouse::Left)
				{
					// "забываем" выбранный объект
					if (selectedCircle)
					{
						selectedCircle = nullptr;
						selectedText = nullptr;
						for (int k = 0; k < sizeEdges; k++)
							seledge[k] = nullptr;
					}
				}
				break;
			}
			}

		};
		/*================= ОТРИСОВКА ================*/
		MainWindow.clear(Color::White);
		MainWindow.draw(sprite);
		for (int i = 0; i < sizeEdges; i++)
			edge[i].draw(MainWindow);

		for (int i = 0; i < size; i++)
		{
			vertex[i].draw(MainWindow);
		}
		start.draw(MainWindow);
		release.draw(MainWindow);
		add.draw(MainWindow);
		MainWindow.draw(route);
		MainWindow.draw(label1);
		MainWindow.display(); //Достаем все из буфера

	};
	return 0;

}