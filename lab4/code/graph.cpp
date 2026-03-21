#include "graph.h"

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
		threads[i] = std::thread(task, this, k, kf, std::ref(nodes), std::ref(components), std::ref(c_mutex), std::ref(visited), std::ref(v_mutex), std::ref(graph1));
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
	std::ofstream out("timeres1.txt");
	std::ofstream out1("single1.txt");
	int n = 90, m = 3; 
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
