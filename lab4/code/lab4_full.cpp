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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

graphviz_graph::graphviz_graph(){
	name = "none";
}

graphviz_graph::graphviz_graph(const std::string& filename){
	std::ifstream file(filename);
	if (file.is_open()){
		int a, b;
		std::string line, n1, n2;
		std::stringstream sline;
		while (!file.eof()){
			getline(file, line);
			a = line.find("[");
			if (a != -1){
				b = line.find("]");
				line.erase(a - 1, b);
			}
			if (line[0] == '\t'){
				sline << line;
				n1 = "*"; n2 = "*";
				sline >> n1;
				sline >> n2;
				sline >> n2;
				if (n2.find("{") == -1){
					if (n2.find(";") != -1)
						n2.pop_back();
					graph[n2];
					new_link(n1, n2);
					if (type) 
						new_link(n2, n1);
					sline.clear();
				}
				else{
					n2.erase(0, 1);
					if (n2.length() > 0){
						graph[n2];
						new_link(n1, n2);
						if (type) 
							new_link(n2, n1);
					}
					if (n2.find(";") != -1){
						n2.pop_back();
						n2.pop_back();
						graph[n2];
						new_link(n1, n2);
						new_link(n2, n1);
					}
					else{
						while (sline >> n2){
							if (n2.find(";") != -1)
								n2.pop_back();
							if (n2.find("}") != -1)
								n2.pop_back();
							if (n2.length() > 0){
								graph[n2];
								new_link(n1, n2);
								if (type) 
									new_link(n2, n1);
							}
							sline.clear();
						}
					}
					sline.clear();
				}
			}
			else{
				if (line[0] != '}'){
					if (line[0] == 's')
						line.erase(0, 7);
					if (line[0] == 'd' || line[0] == 'g'){
						if (line[0] == 'd')
							type = false;
						else
							type = true;
						sline << line;
						sline >> n1;
						sline >> n1;
						if (n1.find("{") != -1)
							n1.pop_back();
						name = n1;
						if (n1.find("{") == -1)
							sline >> n1;
						sline.clear();
						if (name.length() == 0)
							name = "none";
					}
				}
			}
		}
	}
	file.close();
}

graphviz_graph::~graphviz_graph(){
	for (auto kv: graph)
	 	graph[kv.first].clear();
}

void graphviz_graph::new_node(const std::string& n1){
	graph[n1];
}

void graphviz_graph::del_node(const std::string& n1){
	if (graph.find(n1) == graph.end())
		graph.erase(n1);
}

void graphviz_graph::new_link(const std::string& n1, const std::string& n2){
	if (graph[n1].end() - find(graph[n1].begin(), graph[n1].end(), n2) == 0)
		graph[n1].push_back(n2);
}

void graphviz_graph::del_link(const std::string& n1, const std::string& n2){
	graph[n1].erase(find(graph[n1].begin(), graph[n1].end(), n2));
}

void graphviz_graph::print_name(){
	std::cout << "name: " << name << std::endl;
}

void graphviz_graph::print_type(){
	std::cout << "type: ";
	if (type)
		std::cout << "graph" << std::endl;
	else
		std::cout << "diagraph" << std::endl;
}

void graphviz_graph::set_type(const bool t){
	type = t;
}

bool graphviz_graph::get_type() const {
	return type;
}

void graphviz_graph::print_nodes(){
	std::cout << "nodes: ";
	for (auto kv: graph)
		std::cout << kv.first << ", ";
	std::cout << std::endl;
}

void graphviz_graph::print_nodes_total(){
	std::cout << "total_nodes: " << graph.size() << std::endl;
}

void graphviz_graph::print_data(){
	for (auto kv: graph){
		std::cout << kv.first << ": ";
		for (auto v: graph[kv.first])
			std::cout << v << " ";
		std::cout << std::endl;
	}
}

std::unordered_map<std::string, std::vector<std::string>> graphviz_graph::undirect(){
	std::unordered_map<std::string, std::vector<std::string>> graph1 = graph;
	if (!type){
		std::string n1, n2;
		for (auto& kv: graph1){
			n1 = kv.first;
			for (auto& n2: graph1[n1])
				graph1[n2].push_back(n1);
		}
	}
	return graph1;
}

std::vector<std::unordered_set<std::string>> graphviz_graph::find_components(){
	std::unordered_map<std::string, std::vector<std::string>> graph1 = undirect();
	std::vector<std::unordered_set<std::string>> components;
	std::unordered_set<std::string> component;
	std::string node;
	std::unordered_set<std::string> visited;
	for (const auto& kv: graph){
		node = kv.first;
		if (visited.find(node) == visited.end()){
			component = bfs(node, graph1);
			visited.insert(component.begin(), component.end());
			components.push_back(component);
		}
	}
	return components;
}

std::vector<std::unordered_set<std::string>> graphviz_graph::find_components_parallel(int thread_cnt){
	std::unordered_map<std::string, std::vector<std::string>> graph1 = undirect();
	std::vector<std::unordered_set<std::string>> components;
	std::vector<std::string> nodes;
	for (const auto& kv: graph)
		nodes.push_back(kv.first);
	std::unordered_set<std::string> visited;
	std::mutex c_mutex, v_mutex;
	std::vector<std::thread> threads(thread_cnt);
	int nodes_cnt = nodes.size(), n, step, k = 0, kf;
	if (nodes_cnt <= thread_cnt) {
		thread_cnt = nodes_cnt;
		n = 1;
		step = 1;
	}
	else {
		n = nodes_cnt / thread_cnt + ((nodes_cnt % thread_cnt > 0) ? 1 : 0);
		step = std::min(n, nodes_cnt);
	}
	for (int i = 0; i < thread_cnt; i++) {
		kf = std::min(k + step, nodes_cnt);
		//threads[i] = std::thread(task, this, k, kf, std::ref(nodes), std::ref(components), std::ref(c_mutex), std::ref(visited), std::ref(v_mutex), std::ref(graph1));
		threads[i] = std::thread([this, k, kf, &nodes, &components, &c_mutex, &visited, &v_mutex, &graph1]() {
			task(k, kf, nodes, components, c_mutex, visited, v_mutex, graph1);
		});
		k += step;
	}
	for (int i = 0; i < thread_cnt; i++){
		if (threads[i].joinable())
			threads[i].join();
	}
	return components;
}

void graphviz_graph::task(const int st, const int fn, const std::vector<std::string>& nodes, std::vector<std::unordered_set<std::string>>& components, std::mutex& cm, std::unordered_set<std::string>& visited, std::mutex& vm, const std::unordered_map<std::string, std::vector<std::string>>& graph){
	std::unordered_set<std::string> component;
	std::unordered_set<std::string> local_visited;
	bool v_node;
	for (int i = st; i < fn; i++){
		if (!local_visited.insert(nodes[i]).second)
			continue;
		{
			std::lock_guard<std::mutex> lk(vm);
			if (!visited.insert(nodes[i]).second)
				continue;
		}
		component = bfs(nodes[i], graph);
		local_visited.insert(component.begin(), component.end());
		{
			std::lock_guard<std::mutex> lk(vm);
			visited.insert(component.begin(), component.end());
		}
		{
			std::lock_guard<std::mutex> lk(cm);
			components.push_back(component);
		}
	}
}

std::unordered_set<std::string> bfs(const std::string& node, const std::unordered_map<std::string, std::vector<std::string>>& graph){
	std::unordered_set<std::string> res, visited_nodes;
	std::queue<std::string> queue;
	visited_nodes.insert(node);
	queue.push(node);
	std::string tmp;
	while (!queue.empty()){
		tmp = queue.front();
		queue.pop();
		res.insert(tmp);
		for (const auto& neighbour: graph.find(tmp)->second){
			if (visited_nodes.insert(neighbour).second){
				queue.push(neighbour);
			}
		}
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void print_arr(const std::vector<std::string>& a){
	for (auto i: a)
		std::cout << i << " ";
	std::cout << std::endl;
}

void print_arr(const std::vector<std::unordered_set<std::string>>& a){
	int n = a.size();
	for (int i = 0; i < n; i++){
		print_set(a[i]);
		if (i + 1 < n)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

void print_set(const std::unordered_set<std::string>& a){
	std::cout << "{ ";
	for (auto i: a)
		std::cout << i << " ";
	std::cout << "}";
}

graphviz_graph gen_graph(const int n){
	graphviz_graph g;
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<double> prob(0.0, 1.0);
	std::string node_name;
	for (int i = 0; i < n; i++){
		node_name = "n" + std::to_string(i);
		g.new_node(node_name);
	}
	g.set_type(true);
	std::string n1, n2;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (i != j && prob(rng) < 0.3){
				n1 = "n" + std::to_string(i);
				n2 = "n" + std::to_string(j);
				g.new_link(n1, n2);
				if (g.get_type())
					g.new_link(n2, n1);
			}
		}
    }
	return g;
}

void elapsed_time(){
	graphviz_graph graph;
	std::ofstream out("timeres.txt");
	std::ofstream out1("single.txt");
	int n = 90, m = 7; 
	double time_seconds;
	if (out.is_open()){
		for (int i = 3; i <= n; i += 3){
			out << i << " ";
			out1 << i << " ";
			graph = gen_graph(i);
			time_seconds = 0;
			for (int k = 0; k < m; k++){
				auto st = std::chrono::high_resolution_clock::now();
				graph.find_components();
				auto fn = std::chrono::high_resolution_clock::now();
				time_seconds += std::chrono::duration_cast<std::chrono::duration<double>>(fn - st).count(); // seconds
			}
			out1 << time_seconds / m << " ";
			for (int q = 1; q <= 8 * 16; q *= 2){
				time_seconds = 0;
				for (int k = 0; k < m; k++){
					auto st = std::chrono::high_resolution_clock::now();
					graph.find_components_parallel(q);
					auto fn = std::chrono::high_resolution_clock::now();
					time_seconds += std::chrono::duration_cast<std::chrono::duration<double>>(fn - st).count(); // seconds
				}
				out << time_seconds / m << " ";
				if (q == 1)
					out1 << time_seconds / m << std::endl;
			}
			out << std::endl;
		}
	}
	out.close();
	out1.close();
}

bool sort_func(const std::unordered_set<std::string> &a, const std::unordered_set<std::string>& b) {
	return a.size() > b.size();
}

bool compare_component(std::vector<std::unordered_set<std::string>>& a, std::vector<std::unordered_set<std::string>>& b) {
	bool res = (a.size() == b.size());
	std::sort(a.begin(), a.end(), sort_func);
	for (auto k: a){
		std::cout << k.size() << " ";
		print_set(k);
		std::cout << std::endl;
	}
	for (auto k: b){
		std::cout << k.size() << " ";
		print_set(k);
		std::cout << std::endl;
	}
	res = (a[0].size() == b[0].size());
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string file_contents = R"(
digraph G {
	n0 -> n14;
	n0 -> n18;
	n0 -> n19;
	n0 -> n33;
	n0 -> n37;
	n1 -> n21;
	n1 -> n25;
	n2 -> n31;
	n4 -> n29;
	n5 -> n45;
	n6 -> n9;
	n7 -> n24;
	n7 -> n34;
	n12 -> n18;
	n12 -> n37;
	n14 -> n36;
	n15 -> n49;
	n16 -> n26;
	n16 -> n32;
	n17 -> n30;
	n19 -> n38;
	n20 -> n30;
	n27 -> n46;
	n27 -> n48;
	n30 -> n45;
	n31 -> n32;
	n31 -> n44;
	n35 -> n44;
	n38 -> n40;
	n38 -> n44;
	n39 -> n44;
	n39 -> n48;
	n43 -> n46;
}
)";

// graphviz_graph mygraph("testgraph.gv"); 
graphviz_graph mygraph(file_contents);
std::vector<std::unordered_set<std::string>> components = {
		{"n26", "n43", "n0", "n39", "n44", "n18", "n32", "n35", "n31", "n38", "n36", "n40", "n33", "n48", 
		"n27", "n19", "n37", "n2", "n46", "n12", "n14", "n16"},
		{"n5", "n17", "n45", "n30", "n20"}, {"n21", "n1", "n25"}, {"n9", "n6"},
		{"n4", "n29"}, {"n34", "n24", "n7"}, {"n15", "n49"}
	};

int main(int argc, char* argv[]){
	// elapsed_time();
	
	// mygraph.print_name();
	// mygraph.print_type();
	// mygraph.print_nodes();
	mygraph.print_nodes_total();
	// mygraph.print_data();

	std::vector<std::unordered_set<std::string>> res;
	res = mygraph.find_components();
	std::cout << compare_component(res, components) << std::endl;
	// print_arr(res);
	res = mygraph.find_components_parallel(5); 
	std::cout << compare_component(res, components) << std::endl;
	// print_arr(res);

	return 0;
}

// gswin64c -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dNOPAUSE -dQUIET -dBATCH -dSubsetFonts=true -sOutputFile=report1.pdf report.pdf
