#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
#include <limits>
#include <iterator>
#include <map>
using namespace std;

//������� 1: 0�2^1 + 1�2^0 = 01    (����� � ������ � �������� ��������-�����) 

class Locality {

private:

	string name; 
	int population; 

public:

	Locality(const string& name, int population) {
		set_name(name);
		set_population(population); 
	}

	Locality() {
		set_name("Samara");
		set_population(1000);
	}

	int set_population(int population) {
		if (population < 0) throw "���������� ������� �� ������ ���� ������������� ������!";
		this->population = population; 
	}

	int set_name(const string& name) {
		if (name.size() == 0) throw "�������� ����������� ������ �� ������ ���� ������!";
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

bool operator!=(const Locality& lhs, const Locality& rhs) {
	return !(lhs == rhs);
}

bool operator<(const Locality& lhs, const Locality& rhs) {
	return lhs.get_name() < rhs.get_name();  
}


struct Node {
	double length;
	Locality prev;
};

//����������� � ����������� � �� ���������
//���������� �������� ������
//���������� �������� ==
//���������� �������� !=
//���������� <
















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
			throw "������ ������� �����������!";
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
			throw "��������� ������� ��� ������������!";
		table.push_back(Vertex(locality)); 
	}

	void add_edge(const Locality& from, const Locality& to, double length) {
		auto it = check_vertexes_existence(from, to); 
		if (check_edge_existence(table[it-table.begin()], to) != end(table[it - table.begin()].edges))
			throw "��� ����� ��� ���� ������!";
		table[it - table.begin()].edges.push_back(Edge(to, length));
	}

	void del_vertex(const Locality& locality) {
		auto it = check_vertex_existence(locality);
		if (it == end(table))
			throw "��������� ������� ��� �����������!";
		table.erase(it);
	}

	void del_edge(const Locality& from, const Locality& to) {
		auto it = check_vertexes_existence(from, to); 
		auto it_edge = check_edge_existence(table[it-table.begin()], to);
		if (it_edge == end(table[it - table.begin()].edges))
			throw "��� ����� ��� ���� ��������!";
		table[it - table.begin()].edges.erase(it_edge);
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
			throw "��������� ������� �����������!";
		table[it - table.begin()].check = true;
		q.push(table[it - table.begin()]);
 		do {
			Vertex result = q.front(); 
			cout << result.locality << "\n";
			q.pop();
			for (auto el : result.edges) {
				auto it = check_vertex_existence(el.value);
				if (!table[it - table.begin()].check) {
					table[it - table.begin()].check = true;
					q.push(table[it - table.begin()]);
				}
			}
		} while (!q.empty());
		change_flags();
	}

	list<Locality> find_the_shortest_way(const Locality& from_id, const Locality& to_id) { //�������� ��������-�����
		map<Locality, Node> m; 
		for (int i = 0; i < table.size(); i++) {
			m[table[i].locality].length = 10'000'000;
		}
		m[from_id].length = 0;
		if (!m.count(from_id) || !m.count(to_id)) throw "������ ������ ��� � �����!";
		for (int i = 0; i <= table.size(); i++) {
			for (int j = 0; j < table.size(); j++) {
				auto it = begin(table[j].edges);
				while (it != end(table[j].edges)) {
					if (it->length + m[table[j].locality].length < m[it->value].length) {
						if (i != table.size()) {
							m[it->value].length = it->length + m[table[j].locality].length;
							m[it->value].prev = table[j].locality;
						}
						else throw "� ����� ������������ ������������� ����";
					}
					it++;
				} 
			} 
		}
		if (m[to_id].length == 10'000'000) throw "���� �����������!";
		list<Locality> result{ to_id };
		Locality value = m[to_id].prev;
		while (value != from_id) {
			result.push_front(value);
			value = m[value].prev;
		}
		result.push_front(from_id);
		return result;
	}
		
};


int main() {
	setlocale(LC_ALL, "RUS"); 
	try {
		RoadNetwork rn;
		Locality samara = Locality("Samara1", 1000000); 
		Locality moscow = Locality("Moscow2", 2000000); 
		Locality saint_peterburg = Locality("Saint_Peterburg3", 10000);  
		Locality sochi = Locality("Sochi4", 100); 
		Locality volgograd = Locality("Volgograd5", 1000); 
		Locality voronesh = Locality("Voronesh6", 20000);

		rn.add_vertex(samara);
		rn.add_vertex(moscow);
		rn.add_vertex(saint_peterburg);
		rn.add_vertex(sochi);
		rn.add_vertex(volgograd);
		rn.add_vertex(voronesh); 

		rn.add_edge(samara, moscow, 7);
		rn.add_edge(samara, volgograd, 9);
		rn.add_edge(samara, saint_peterburg, 5);
		rn.add_edge(moscow, sochi, 4);
		rn.add_edge(moscow, saint_peterburg, -8);
		rn.add_edge(saint_peterburg, sochi, 3);
		rn.add_edge(saint_peterburg, volgograd, 6);
		rn.add_edge(sochi, voronesh, 8);
		rn.add_edge(volgograd, sochi, -4);
		rn.add_edge(volgograd, voronesh, 6);
		
		//rn.traversing_in_width(samara);



		cout << "\n\n\n";
		auto result = rn.find_the_shortest_way(samara, voronesh);
		for (auto el : result) {
			cout << el << " -> ";
		}
	}
	catch (const char* ex) {
		cout << ex;  
	}
	
}