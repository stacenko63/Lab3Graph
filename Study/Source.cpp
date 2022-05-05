#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;


class Locality {

private:

	string name; 
	int population; 

public:

	Locality(string name, int population) {
		set_name(name);
		set_population(population); 
	}

	int set_population(int population) {
		if (population < 0) throw " оличество жителей не должно быть отрицательным числом!";
		this->population = population; 
	}

	int set_name(string name) {
		if (name.size() == 0) throw "Ќазвание населенного пункта не должно быть пустым!";
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
	out << locality.get_name() << " " << locality.get_population(); 
	return out; 
}

bool operator==(const Locality& lhs, const Locality& rhs) {
	return lhs.get_name() == rhs.get_name() && lhs.get_population() == rhs.get_population(); 
}

class RoadNetwork {
	
private:
	
	struct Edge {
		int value; 
		double lenght = 1; 
	};

	struct Vertex {
		Vertex(const Locality& Locality): locality(Locality) {}
		Locality locality; 
		list<Edge> edges;  
	};

	vector<Vertex> table;  

public:

	auto check_vertex_existence(const Locality& locality) {
		return find(begin(table), end(table), [locality](const Vertex& vertex) {
			return vertex.locality == locality; 
			});
	}

	void add_vertex(const Locality& locality) {
		if (find(begin(table), end(table), locality) == end(table)) 
			throw "”казанна€ вершина уже присутствует!";
		table.push_back(locality); 
	}

	void add_edge(const Locality& from, const Locality& to, double length) {
		auto it = check_vertex_existence(from); 
		if (it == end(table) || check_vertex_existence(to) == end(table)) {
			throw "ƒанные вершины отсутствуют!";
		}
		it.
	}
};


int main() {
	setlocale(LC_ALL, "RUS"); 
	/*vector<int> v = { 1,2,3,4,5 };
	for (auto el : v) {
		cout << el << " ";

	}
	cout << "\n\n\n";
	auto it = find(begin(v), end(v), 95);
	if (it == end(v)) {
		cout << "!!!!!!";
	}
	else {
		*it = 6;
		for (auto el : v) {
			cout << el << " ";

		}
		cout << "\n\n\n";
	}*/
}