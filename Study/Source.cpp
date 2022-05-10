#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
#include <limits>
#include <iterator>
#include <map>
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

	Locality() {
		set_name("Samara");
		set_population(1000);
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

//конструктор с параметрами и по умолчанию
//перегружен оператор ==
//перегружен оператор !=
//перегружен <
















class Edge {

private:

	Locality value;
	double length; 
	double groundRatio;
	double payRatio;
	bool isPay; 
	bool isGround; 

public:

	Edge(const Locality& locality, bool IsGround, bool IsPay, double Length, double GroundRatio, double PayRatio) : 
		value(locality), isGround(IsGround), isPay(IsPay), 
		length(Length), groundRatio(GroundRatio), payRatio(PayRatio) {}

	operator double() const {
		return get_length_with_ratios();   
	}

	double get_length_with_ratios() const {
		double result = length;
		if (isGround) result *= groundRatio;
		if (isPay) result *= payRatio;
		return result;  
	}

	double get_length_without_ratios() const {
		return length; 
	}

	Locality get_value() const {
		return value; 
	}

	bool get_pay_flag() const {
		return isPay;
	}

	bool get_ground_flag() const {
		return isGround; 
	}
};

struct Vertex {
	Vertex(const Locality& Locality) : locality(Locality) {}
	Locality locality;
	list<Edge> edges;
	bool check = false; 
};

bool operator==(const Vertex& lhs, const Vertex& rhs) {
	return lhs.locality == rhs.locality;
}

bool operator!=(const Vertex& lhs, const Vertex& rhs) {
	return !(lhs == rhs);
}

ostream& operator<<(ostream& out, const Edge& edge) {
	out << "( " << edge.get_value() << " , Length = " << edge.get_length_without_ratios() << " , IsPay = " << 
		!edge.get_pay_flag() << " , IsGround = " << edge.get_ground_flag() << " )"; 
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




template <typename TVertex, typename TEdge>  
class RoadNetwork {
	
private:

	struct E;  

	struct Vert { 
		Vert(const TVertex& Id) : id(Id) {}
		TVertex id; 
		bool check;
		//list<TEdge> edges;
		list<E> edges;  
	};

	struct E {
		E(const TVertex& To, const TEdge& Edge): to(To), edge(Edge) {}
		TVertex to;
		TEdge edge;  
	};

	vector<Vert> table;   

	void change_flags() {
		for (auto el : table) {
			el.check = false;
		}
	}

	auto check_vertex_existence(const TVertex& vert) {
		return find_if(begin(table), end(table), [vert](const Vert& vertex) {
			return vertex.id == vert;
			});
	}

	auto check_vertexes_existence(const TVertex& first, const TVertex& second) {
		auto it = check_vertex_existence(first);
		if (it == end(table) || check_vertex_existence(second) == end(table))
			throw "Данные вершины отсутствуют!";
		return it;  
	}

public:

	void add_vertex(const TVertex& vertex) { 
		if (check_vertex_existence(vertex) != end(table)) 
			throw "Указанная вершина уже присутствует!"; 
		table.push_back(Vert(vertex));  
	}

	void del_vertex(const TVertex& vertex) {
		auto it = check_vertex_existence(vertex); 
		if (it == end(table))
			throw "Указанная вершина уже отсутствует!";
		table.erase(it);
	}

	void add_edge(const TVertex& from, const TVertex& to, const TEdge& edge) {
		auto it = check_vertexes_existence(from, to);
	
		auto tmp = table[it - table.begin()].id.edges;  
		//auto i = find_if(begin(tmp), end(tmp), [to](const E& e) {
		//	return e.to == to; 
		//	});

		//if (i != end(tmp)) throw "Данные ребра уже итак смежны!"; 
		//E e(to, edge);
		//table[it - table.begin()].id.edges.push_back(e); 
	}


	//void del_edge(const TVertex& from, const TVertex& to) {
	//	auto it = check_vertexes_existence(from, to); 
	//	
	//	if (find(begin(table[it - table.begin()].id.edges), end(table[it - table.begin()].id.edges),
	//		edge) != end(table[it - table.begin()].id.edges)) throw "Эти ребра уже итак несмежны!";
	//	table[it - table.begin()].edges.erase(it_edge);
	//}

	void print() const {
		for (auto el : table) {
			cout << el.id << "\n\n\n";  
		}
	}


	void traversing_in_width(const TVertex& start) {/////////////////????????
		queue<Vert> q; 
		auto it = check_vertex_existence(start); 
		if (it == end(table))
			throw "Указанная вершина отсутствует!";

		table[it - table.begin()].check = true;
		q.push(table[it - table.begin()]); 
 		do {
			Vert result = q.front();  
			cout << result.id.locality << "\n"; 
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

	//list<Locality> find_the_shortest_way(const Locality& from_id, const Locality& to_id, double groundRatio = 1, double payRatio = 1) { //алгоритм Беллмана-Форда
	//	map<Locality, Node> m; 
	//	for (int i = 0; i < table.size(); i++) {
	//		m[table[i].id.locality].length = 10'000'000;
	//	}
	//	m[from_id].length = 0;
	//	if (!m.count(from_id) || !m.count(to_id)) throw "Данных верших нет в графе!";
	//	//for (int i = 0; i <= table.size(); i++) {
	//	//	for (int j = 0; j < table.size(); j++) {
	//	//		auto it = begin(table[j].edges);
	//	//		while (it != end(table[j].edges)) {
	//	//			if (it->get_length_with_ratios(groundRatio, payRatio) + m[table[j].locality].length < m[it->get_value()].length) {
	//	//				if (i != table.size()) {
	//	//					m[it->get_value()].length = it->get_length_with_ratios(groundRatio, payRatio) + m[table[j].locality].length;
	//	//					m[it->get_value()].prev = table[j].locality;
	//	//				}
	//	//				else throw "В графе присутствует отрицательный цикл";
	//	//			}
	//	//			it++;
	//	//		} 
	//	//	} 
	//	//}

	//	for (int i = 0; i <= table.size(); i++) {
	//		for (int j = 0; j < table.size(); j++) {
	//			auto it = begin(table[j].edges);
	//			while (it != end(table[j].edges)) {
	//				if (static_cast<double>(*it) + m[table[j].id.locality].length < m[it->get_value()].length) {
	//					if (i != table.size()) {
	//						m[it->get_value()].length = static_cast<double>(*it) + m[table[j].id.locality].length;
	//						m[it->get_value()].prev = table[j].id.locality;
	//					}
	//					else throw "В графе присутствует отрицательный цикл";
	//				}
	//				it++;
	//			}
	//		}
	//	}
	//	if (m[to_id].length == 10'000'000) throw "Путь отсутствует!";
	//	list<Locality> result{ to_id };
	//	Locality value = m[to_id].prev;
	//	while (value != from_id) {
	//		result.push_front(value);
	//		value = m[value].prev;
	//	}
	//	result.push_front(from_id);
	//	return result;
	//}
		
};




int main() {
	setlocale(LC_ALL, "RUS"); 
	try {

		RoadNetwork<Vertex, Edge> rn; 
		Locality samara = Locality("Samara1", 1000000); 
		Locality moscow = Locality("Moscow2", 2000000); 
		Locality saint_peterburg = Locality("Saint_Peterburg3", 10000);  
		Locality sochi = Locality("Sochi4", 100); 
		Locality volgograd = Locality("Volgograd5", 1000); 
		Locality voronesh = Locality("Voronesh6", 20000);

		Edge e(samara, false, false, 4, 1, 1); 
		double a = static_cast<double>(e); 

		rn.add_vertex(samara);
		rn.add_vertex(moscow);
		rn.add_vertex(saint_peterburg);
		rn.add_vertex(sochi);
		rn.add_vertex(volgograd);
		rn.add_vertex(voronesh); 

		rn.add_edge(samara, moscow, Edge(moscow, false, false, 7, 1, 1));
		rn.add_edge(samara, volgograd, Edge(volgograd, false, false, 9, 1, 1));
		rn.add_edge(samara, saint_peterburg, Edge(saint_peterburg, false, false, 5, 1, 1));
		rn.add_edge(moscow, sochi, Edge(sochi, false, false, 4, 1, 1));
		rn.add_edge(moscow, saint_peterburg, Edge(saint_peterburg, false, false, -8, 1, 1));
		rn.add_edge(saint_peterburg, sochi, Edge(sochi, false, false, 3, 1, 1));
		rn.add_edge(saint_peterburg, volgograd, Edge(volgograd, false, false, 6, 1, 1));
		rn.add_edge(sochi, voronesh, Edge(voronesh, false, false, 8, 1, 1));
		rn.add_edge(volgograd, sochi, Edge(sochi, false, false, -4, 1, 1));
		rn.add_edge(volgograd, voronesh, Edge(voronesh, false, false, 6, 1, 1)); 
		
		rn.print(); 

		cout << "\n\n\n";

		//rn.traversing_in_width(samara);  

		//cout << "\n\n\n";
		//auto result = rn.find_the_shortest_way(samara, voronesh);
		//for (auto el : result) {
		//	cout << el << " -> ";
		//}
	}
	catch (const char* ex) {
		cout << ex;  
	}
	
}