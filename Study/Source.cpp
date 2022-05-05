#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
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
	out << locality.get_name() << " " << locality.get_population(); 
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
	out << "( " << edge.value << ", " << edge.length << " )";
	return out;
}

ostream& operator<<(ostream& out, const Vertex& vertex) {
	out << vertex.locality << " : ";
	for (auto el : vertex.edges) {
		cout << el << " , ";
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
		Vertex& result = *it; 
		if (check_edge_existence(result, to) != end(result.edges))
			throw "��� ����� ��� ���� ������!";
		result.edges.push_back(Edge(to, length)); 
	}

	void del_vertex(const Locality& locality) {
		auto it = check_vertex_existence(locality);
		if (it == end(table))
			throw "��������� ������� ��� �����������!";
		table.erase(it);
	}

	void del_edge(const Locality& from, const Locality& to) {
		auto it = check_vertexes_existence(from, to); 
		Vertex& result = *it; 
		auto it_edge = check_edge_existence(result, to);
		if (it_edge != end(result.edges))
			throw "��� ����� ��� ���� ������!";
		result.edges.erase(it_edge); 
	}

	void print() const {
		for (auto el : table) {
			cout << el << ":  ";
			for (auto element : el.edges) {
				cout << element << " ";
			}
			cout << "\n";
		}
	}

	void traversing_in_width(const Locality& locality) {
		queue<Vertex&> q;
		auto it = check_vertex_existence(locality);
		if (it == end(table)) 
			throw "��������� ������� �����������!"; 
		Vertex& result = *it; 
		q.push(result);  
		do {
			result = q.back(); 
			q.pop();
			result.check = true; 
			for (auto el : result.edges) {
				auto element = check_vertex_existence(el.value);
				Vertex& tmp = *element;
				if (!tmp.check) q.push(tmp); 
			} 
		} while (!q.empty()); 
		change_flags();  
	}

	list<Vertex> get_latest_way(const Locality& from, const Locality& to) {
		auto it = check_vertexes_existence(from, to);
		list<Vertex> result;
		 

		return result;
	}
};


int main() {
	setlocale(LC_ALL, "RUS"); 
	RoadNetwork rn; 
}