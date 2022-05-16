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
	out << "Name = " << locality.get_name() << ", Population = " << locality.get_population();
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

ostream& operator<<(ostream& out, const Edge& edge) {
	out << "Length: " << edge.get_length_without_ratios() << "; IsPay: " <<
		!edge.get_pay_flag() << "; IsGround: " << edge.get_ground_flag();
	return out;
}

struct Vertex {
	Vertex(const Locality& Locality) : locality(Locality) {}
	Vertex(): locality(Locality("Samara", 1000)) {}
	Locality locality;
	list<Edge> edges;
	bool check = false; 


};






//bool operator<(const Vertex& lhs, const Vertex& rhs) {
//	return lhs.locality < rhs.locality;
//}

ostream& operator<<(ostream& out, const Vertex& vertex) {
	out << vertex.locality;
	return out; 
}


//
//template<typename TVertex>
//bool operator==(const TVertex& lhs, const TVertex& rhs) {
//	return false;
//}

bool operator==(const Vertex& lhs, const Vertex& rhs) {
	return lhs.locality == rhs.locality;
}







template <typename TVertex, typename TEdge, typename TCompare = equal_to<TVertex>>
class RoadNetwork {
	
private:

	struct E;  

	struct Vert { 
		Vert(const TVertex& Id) : id(Id) {}
		TVertex id; 
		bool check = false;
		list<E> edges;  
	};

	struct E {
		E(const TVertex& To, const TEdge& Edge): to(To), edge(Edge) {}
		TVertex to;
		TEdge edge;  
	};

	struct Node {
		Node(double Length) : length(Length) {}
		Node() : length(10'000'000) {}
		double length;
		TVertex prev;
	};

	vector<Vert> table;   

	void change_flags() {
		for (auto el : table) {
			el.check = false;
		}
	}

	template<typename TWeightSelector>
	void Method(double length, TWeightSelector weightSelector) {
		TEdge edge = length; 
		double weigth = weightSelector(edge); 
	}

	auto check_vertex_existence(const TVertex& vert) {
		return find_if(begin(table), end(table), [vert](const Vert& vertex) {
			return TCompare()(vertex.id, vert);   
			});
	}

	auto check_vertexes_existence(const TVertex& first, const TVertex& second) {
		auto it = check_vertex_existence(first);
		if (it == end(table) || check_vertex_existence(second) == end(table))
			throw "Данные вершины отсутствуют!";
		return it;  
	}

	auto find(const TVertex& vert) {
		return find_if(begin(table), end(table), [vert](const Vert& element) {
			return vert == element.id;
			});
	}

public:

	void add_vertex(const TVertex& vertex) { 
		if (check_vertex_existence(vertex) != end(table)) 
			throw "Указанная вершина уже присутствует!"; 
		table.push_back(Vert(vertex));  
	}

	void del_vertex(const TVertex& vertex) {
		auto it = check_vertex_existence(vertex); 
		if (it == end(table)) {
			throw "Указанная вершина уже отсутствует!";
		}
		if (it != end(table)) {
			table.erase(it);
		}
	}

	void add_edge(const TVertex& from, const TVertex& to, const TEdge& edge) {
		auto it = check_vertexes_existence(from, to);
		if (it != end(table)) {
			auto tmp = table[it - table.begin()].edges;
			auto i = find_if(begin(tmp), end(tmp), [to](const E& e) {
				return TCompare()(e.to, to);
				});

			if (i != end(tmp)) throw "Данные ребра уже итак смежны!";
			table[it - table.begin()].edges.push_back(E(to, edge));
		}
	}


	void del_edge(const TVertex& from, const TVertex& to) {
		auto it = check_vertexes_existence(from, to); 
		if (it != end(table)) {
			auto it_edge = find_if(begin(table[it - table.begin()].edges), end(table[it - table.begin()].edges),
				[to](const E& edge) {
					return TCompare()(edge.to, to);
				});
			if (it_edge == end(table[it - table.begin()].edges)) throw "Эти ребра уже итак несмежны!";
			table[it - table.begin()].edges.erase(it_edge);
		}
	}

	void print() const {
		for (auto el : table) {
			cout << el.id << " : ";
			for (auto it = begin(el.edges); it != end(el.edges); ++it) {
				cout << "(To: (" << it->to << "); " << it->edge << ")";
				if (next(it) != end(el.edges)) cout << " -> ";
			}
			cout << "\n\n\n";
		}
	}


	void traversing_in_width(const TVertex& start) {///несвязный граф??
		queue<Vert> q; 
		auto it = check_vertex_existence(start); 
		if (it == end(table)) {
			throw "Указанная вершина отсутствует!";
		}
		if (it != end(table)) {
			table[it - table.begin()].check = true;
			q.push(table[it - table.begin()]);
			do {
				Vert result = q.front();
				cout << result.id << "\n";
				q.pop();
				for (auto el : result.edges) {
					auto it = check_vertex_existence(el.to);
					if (it != end(table)) {
						if (!table[it - table.begin()].check) {
							table[it - table.begin()].check = true;
							q.push(table[it - table.begin()]);
						}
					}
				}
			} while (!q.empty());
			change_flags();
		}
	}

	list<TVertex> find_the_shortest_way(const TVertex& from_id, const TVertex& to_id) { //алгоритм Беллмана-Форда
		//map<TVertex, Node> m; 
		//for (int i = 0; i < table.size(); i++) {
		//	m[table[i].id].length = 10'000'000;
		//}
		//m[from_id].length = 0;
		//if (!m.count(from_id) || !m.count(to_id)) throw "Данных верших нет в графе!";
		//for (int i = 0; i <= table.size(); i++) {
		//	for (int j = 0; j < table.size(); j++) {
		//		auto it = begin(table[j].edges);
		//		while (it != end(table[j].edges)) {
		//			if (static_cast<double>(it->edge) + m[table[j].id].length < m[it->to].length) {
		//				if (i != table.size()) {
		//					m[it->to].length = static_cast<double>(it->edge) + m[table[j].id].length;
		//					m[it->to].prev = table[j].id;
		//				}
		//				else throw "В графе присутствует отрицательный цикл";
		//			}
		//			it++;
		//		}
		//	}
		//}
		//if (m[to_id].length == 10'000'000) throw "Путь отсутствует!";
		//list<TVertex> result{ to_id };
		//TVertex value = m[to_id].prev;
		//while (!TCompare()(value, from_id)) {
		//	result.push_front(value);
		//	value = m[value].prev;
		//}
		//result.push_front(from_id);
		//return result;


		vector<pair<TVertex, Node>> v(table.size());
		for (int i = 0; i < table.size(); i++) {
			v[i] = pair<TVertex, Node>(table[i].id, Node(10'000'000));  
		}

		v[find(from_id) - table.begin()].second.length = 0; 

		if (find(from_id) == end(table) || find(to_id) == end(table)) throw "Данных верших нет в графе!";
		for (int i = 0; i <= table.size(); i++) {
			for (int j = 0; j < table.size(); j++) {
				auto it = begin(table[j].edges);
				while (it != end(table[j].edges)) {
					if (static_cast<double>(it->edge) + v[find(table[j].id)-table.begin()].second.length < v[find(it->to)-table.begin()].second.length) {
						if (i != table.size()) {
							v[find(it->to)-table.begin()].second.length = static_cast<double>(it->edge) + v[find(table[j].id) - table.begin()].second.length;
							v[find(it->to) - table.begin()].second.prev = table[j].id;
						}
						else throw "В графе присутствует отрицательный цикл";
					}
					it++;
				}
			}
		}
		if (v[find(to_id)-table.begin()].second.length == 10'000'000) throw "Путь отсутствует!";
		list<TVertex> result{ to_id };
		TVertex value = v[find(to_id) - table.begin()].second.prev;
		while (!TCompare()(value, from_id)) {
			result.push_front(value);
			value = v[find(value) - table.begin()].second.prev;
		}
		result.push_front(from_id); 
		return result;
	}
		
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
		a = e;  

		rn.add_vertex(Vertex(samara));
		rn.add_vertex(Vertex(moscow));
		rn.add_vertex(Vertex(saint_peterburg));
		rn.add_vertex(Vertex(sochi));
		rn.add_vertex(Vertex(volgograd));
		rn.add_vertex(Vertex(voronesh)); 



		rn.add_edge(Vertex(samara), Vertex(moscow), Edge(moscow, false, false, 7, 1, 1));
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

		rn.traversing_in_width(samara);  

		cout << "\n\n\n";

		auto result = rn.find_the_shortest_way(samara, voronesh);
		for (auto el : result) {
			cout << el << " "; 
		}
		
	}
	catch (const char* ex) {
		cout << ex;  
	}
	
}