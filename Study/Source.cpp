#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
#include <limits>
#include <iterator>
using namespace std;

//Вариант 1: 0·2^1 + 1·2^0 = 01    (обход в ширину и алгоритм Беллмана-Форда) 

class Locality {

private:

	string name; 
	int population; 

public:

	Locality(const string& name, int population) {
		set_name(name);
		set_population(population); 
	}

	int set_population(int population) {
		if (population < 0) throw "Количество жителей не должно быть отрицательным числом!";
		this->population = population; 
	}

	int set_name(const string& name) {
		if (name.size() == 0) throw "Название населенного пункта не должно быть пустым!";
		this->name = name;
	}
	
	int get_population() const {
		return population;
	}

	string get_name() const {
		return name;
	}

};

ostream& operator<<(ostream& out, const Locality& locality) {
	out << "Name = " << locality.get_name() << " Population = " << locality.get_population();
	return out; 
}

bool operator==(const Locality& lhs, const Locality& rhs) {
	return lhs.get_name() == rhs.get_name() && lhs.get_population() == rhs.get_population(); 
}



struct Edge {
	Edge(const Locality& locality, double Length = 1) : value(locality), length(Length) {}
	Locality value;
	double length;
};

struct Vertex {
	Vertex(const Locality& Locality) : locality(Locality) {}
	Locality locality;
	list<Edge> edges;
	bool check = false; 
};

ostream& operator<<(ostream& out, const Edge& edge) {
	out << "( " << edge.value << ", Length = " << edge.length << " )";
	return out;
}

ostream& operator<<(ostream& out, const Vertex& vertex) {
	out << vertex.locality << " : ";
	for (auto it = begin(vertex.edges); it != end(vertex.edges); ++it) {
		cout << *it;
		if (next(it) != end(vertex.edges)) cout << " -> ";
	}
	return out; 
}





class RoadNetwork {
	
private:

	vector<Vertex> table;  

	vector<Vertex>::iterator check_vertex_existence(const Locality& locality) {
		return find_if(begin(table), end(table), [locality](const Vertex& vertex) {
			return vertex.locality == locality;
			});
	}

	vector<Vertex>::iterator check_vertexes_existence(const Locality& first, const Locality& second) {
		auto it = check_vertex_existence(first);
		if (it == end(table) || check_vertex_existence(second) == end(table)) {
			throw "Данные вершины отсутствуют!";
		}
		return it;
	}

	list<Edge>::const_iterator check_edge_existence(const Vertex& vertex, const Locality& locality) {
		return find_if(begin(vertex.edges), end(vertex.edges), [locality](const Edge& edge) {
			return locality == edge.value;
			});
	}

	void change_flags() {
		for (auto el : table) {
			el.check = false;
		}
	}

public:



	void add_vertex(const Locality& locality) {
		if (check_vertex_existence(locality) != end(table))
			throw "Указанная вершина уже присутствует!";
		table.push_back(Vertex(locality)); 
	}

	void add_edge(const Locality& from, const Locality& to, double length) {
		auto it = check_vertexes_existence(from, to);
		Vertex& result = *it; 
		if (check_edge_existence(result, to) != end(result.edges))
			throw "Эти ребра уже итак смежны!";
		result.edges.push_back(Edge(to, length)); 
	}

	void del_vertex(const Locality& locality) {
		auto it = check_vertex_existence(locality);
		if (it == end(table))
			throw "Указанная вершина уже отсутствует!";
		table.erase(it);
	}

	void del_edge(const Locality& from, const Locality& to) {
		auto it = check_vertexes_existence(from, to); 
		Vertex& result = *it; 
		auto it_edge = check_edge_existence(result, to);
		if (it_edge == end(result.edges))
			throw "Эти ребра уже итак несмежны!";
		result.edges.erase(it_edge); 
	}

	void print() const {
		for (auto el : table) {
			cout << el << "\n\n\n";
		}
	}


	void traversing_in_width(const Locality& locality) {
		queue<Vertex> q;
		auto it = check_vertex_existence(locality);
		if (it == end(table))
			throw "Указанная вершина отсутствует!";
		Vertex& begin = *it;
		begin.check = true;   
		q.push(begin); 
		do {
			Vertex result = q.back(); 
			cout << result.locality << " , ";
			q.pop();
			for (auto el : result.edges) {
				auto element = check_vertex_existence(el.value);
				Vertex& tmp = *element;  
				if (!tmp.check) {
					tmp.check = true;  
					q.push(tmp);
				}
			}
		} while (!q.empty());
		change_flags();
	}


	//struct str {
	//	double v;
	//	str* prev;  
	//};

	//list<Vertex> get_latest_way(const Locality& from, const Locality& to) { //алгоритм Беллмана-Форда
	//	auto it = check_vertexes_existence(from, to);
	//	list<Vertex> result {*it};
	//	vector<vector<double>> v;
	//	v.resize(table.size());
	//	for (int i = 0; i < table.size(); i++) {
	//		v[i].resize(table.size());
	//	}
	//	for (int i = 0; i < table.size(); i++) {
	//		v[0][i] = numeric_limits<int>::infinity();
	//	}
	//	v[0][it-begin(table)] = 0; 
	//	for (int i = 1; i < table.size(); i++) {
	//		for (int j = 0; j < table.size(); j++) {
	//			v[i][j] = table[i].edges[];
	//		}
	//	}
	//	reverse(begin(result), end(result)); 
	//	return result;
	//}
};


int main() {
	setlocale(LC_ALL, "RUS"); 
	try {
		RoadNetwork rn;
		rn.add_vertex(Locality("Samara", 1000000));
		rn.add_vertex(Locality("Moscow", 2000000));
		rn.add_vertex(Locality("Saint-Peterburg", 10000));
		rn.add_vertex(Locality("Sochi", 100));

		rn.add_edge(Locality("Samara", 1000000), Locality("Moscow", 2000000), 350);
		rn.add_edge(Locality("Moscow", 2000000), Locality("Saint-Peterburg", 10000), 200);
		rn.add_edge(Locality("Saint-Peterburg", 10000), Locality("Sochi", 100), 200);
		rn.add_edge(Locality("Sochi", 100), Locality("Samara", 1000000), 200);

		rn.print();  

		rn.traversing_in_width(Locality("Samara", 1000000));
	}
	catch (const char* ex) {
		cout << ex;  
	}
	
}