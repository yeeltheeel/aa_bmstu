#include <iostream>
#include <fstream>
#include <random>

void generate_test_graph(const int n) {
	std::string filename = "testgraph.gv";
	std::ofstream out(filename);
	if (!out.is_open()) {
		std::cerr << "Cannot create " << filename << std::endl;
		return;
	}
	out << "digraph G {" << std::endl;
	std::mt19937 rng(42);  // фиксированный seed для воспроизводимости
	std::uniform_real_distribution<double> prob_edge(0.0, 1.0);

	// 3000 узлов: n0 -> n2999
	// for (int i = 0; i < n; ++i) {
	// 	std::string node_id = "n" + std::to_string(i);
	// 	out << "  " << node_id << ";" << std::endl;
	// }
	// out << std::endl;

	// Генерируем разреженные рёбра (~15k рёбер, 0.5% плотность)
	int edge_count = 0;
	for (int i = 0; i < n && edge_count < n * n * 5; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (prob_edge(rng) < 0.03) {  
				std::string n1 = "n" + std::to_string(i);
				std::string n2 = "n" + std::to_string(j);
				out << "\t" << n1 << " -> " << n2 << ";\n";
				edge_count++;
			}
		}
	}
	out << "}" << std::endl;
	out.close();
}

int main(){
	generate_test_graph(50);
	return 0;
}
