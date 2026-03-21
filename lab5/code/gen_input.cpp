#include <iostream>
#include <fstream>
#include <vector>
#include <random>

std::mt19937 gen(std::random_device{}());

void gen_matr(const int n, const double edgeProb){
	std::string filename = "graph/graph_" + std::to_string(n) + ".txt";
	
	std::ofstream req("requests.txt", std::ios::out | std::ios::app);
	req << filename << std::endl;
	req.close();

	std::ofstream out(filename);
	if (!out.is_open()) {
		std::cerr << "cannot open file\n";
		return;
	}
	out << n << std::endl;
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	std::uniform_real_distribution<double> wdist(1.0, 10.0);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			double w = 0.0;
			if (i != j && dist(gen) < edgeProb)
				w = wdist(gen);
			out << w;
			if (j + 1 < n) 
				out << " ";
		}
		out << std::endl;
	}
	out.close();
}

void gen_n_matr(const int n){
	std::uniform_real_distribution<double> rng(0.0, 1.0);
	double prob;
	for (int i = 25; i <= n; i += 25){
		prob = rng(gen);
		gen_matr(i, prob);
	}
}

void gen_matr_range(const int n, const int m){
	std::uniform_real_distribution<double> rng(0.0, 1.0);
	double prob;
	for (int i = n; i <= m; i += 25){
		prob = rng(gen);
		gen_matr(i, prob);
	}
}

void gen_req_files(const int n){
	std::uniform_real_distribution<double> rng(0.0, 1.0);
	int a = 5, b = 15, m;
	std::uniform_int_distribution<> distrib(a, b);
	double prob;
	std::ofstream req("requests.txt");
	req.close();
	// matr + full request file
	for (int i = 0; i <= n; i++){
		m = distrib(gen);
		prob = rng(gen);
		gen_matr(m, prob);
	}
	// iter request files
	std::string filename, line;
	for (int i = 10; i <= n; i += 5){
		std::ifstream req("requests.txt");
		filename = "req/req_" + std::to_string(i) + ".txt";
		std::ofstream out(filename);
		for (int k = 0; k < i; k++){
			getline(req, line);
			out << line << std::endl;;
		}
		out.close();
		req.close();
	}
}

int main() {
	// gen_matr_range(3025, 4000);
	gen_req_files(100);
	return 0;
}