#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <functional> 
#include <chrono>
#include <random>

class graphviz_graph{
private:
	std::string name;
	bool type;
	std::unordered_map<std::string, std::vector<std::string>> graph;
public:
	graphviz_graph();
	graphviz_graph(const std::string& filename);
	~graphviz_graph();
	void new_node(const std::string& n1);
	void del_node(const std::string& n1);
	void new_link(const std::string& n1, const std::string& n2);
	void del_link(const std::string& n1, const std::string& n2);
	void set_type(const bool t);
	bool get_type() const;
	void print_name();
	void print_type();
	void print_nodes();
	void print_nodes_total();
	void print_data();
	std::unordered_map<std::string, std::vector<std::string>> undirect();
	std::vector<std::unordered_set<std::string>> find_components();
	std::vector<std::unordered_set<std::string>> find_components_parallel(int thread_cnt);
	void task(const int st, const int fn, const std::vector<std::string>& nodes, std::vector<std::unordered_set<std::string>>& components, std::mutex& cm, std::unordered_set<std::string>& visited, std::mutex& vm, const std::unordered_map<std::string, std::vector<std::string>>& graph);
};

std::unordered_set<std::string> bfs(const std::string& node, const std::unordered_map<std::string, std::vector<std::string>>& graph);

void print_arr(const std::vector<std::string>& a);
void print_arr(const std::vector<std::unordered_set<std::string>>& a);
void print_set(const std::unordered_set<std::string>& a);
graphviz_graph gen_graph(const int n);
void elapsed_time();
