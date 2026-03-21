#include "sales.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void min_dist_update(const std::vector<std::vector<int>>& ant_path, const std::vector<double>& ant_dist, 
	std::vector<int>& min_path, double& min_dist){
	int m = ant_path.size();
	for (int i = 0; i < m; i++){
		if (ant_dist[i] >= 0){
			if (min_path.empty() || ant_dist[i] < min_dist){
				min_dist = ant_dist[i];
				min_path = ant_path[i];
			}
		}
	}
}

double calc_path(const int index, const std::vector<std::vector<double>>& dist, const std::vector<std::vector<double>>& phero, 
	std::vector<int>& ant_path, const double a, const double b){
	ant_path.clear();
	int n = dist.size(), cur = index, nxt;
	std::vector<bool> visited(n, false);
	visited[cur] = true;
	double ant_dist = 0;
	for (int i = 1; i < n; i++){
		std::vector<int> allowed;
		for (int j = 0; j < n; j++){
			if (!visited[j] && dist[cur][j] >= 0)
				allowed.push_back(j);
		}
		if (allowed.empty()){
			ant_dist = -1;
			ant_path.clear();
			break;
		}
		else{
			double w_tmp, et;
			std::vector<double> w;
			for (int j: allowed){
				et = ((dist[cur][j] >= 0) ? (1 / dist[cur][j]) : 0);
				w_tmp = std::pow(phero[cur][j], a) * std::pow(et, b);
				if (w_tmp < MIN_CONSTANT)
					w_tmp = MIN_CONSTANT;
				w.push_back(w_tmp);
				w_sum += w_tmp;
			}
			std::uniform_real_distribution<double> dist01(0.0, w_sum);
			double r = dist01(rng), acc = 0.0;
			nxt = allowed.back();
			for (int k = 0; k < allowed.size(); k++) {
				acc += w[k];
				if (r <= acc) {
					nxt = allowed[k];
					break;
				}
			}
			ant_dist += dist[cur][nxt];
			ant_path.push_back(nxt);
			visited[nxt] = true;
			cur = nxt;
		}
	}
	if (dist[cur][index] >= 0){
		ant_dist += dist[cur][index];
		ant_path.insert(ant_path.begin(), index);
		ant_path.push_back(index);
	} 
	else {
		ant_dist = -1;
		ant_path.clear();
	}
	return ant_dist;
}

void phero_update(std::vector<std::vector<double>>& phero, const std::vector<std::vector<int>>& ant_path, 
	const std::vector<double>& ant_dist, const double Q, const double rho){
	int n = phero.size(), m = ant_path.size(); 
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			phero[i][j] *= (1 - rho);
	for (int k = 0; k < m; k++){
		if (ant_dist[k] > 0){
			for (int t = 0; t < ant_path[k].size() - 1; t++)
				phero[ant_path[k][t]][ant_path[k][t + 1]] += Q / ant_dist[k];
		}
	}
}

std::vector<int> ant_algorithm(std::vector<std::vector<double>>& dist, const double a, const double rho, const double tmax){
	int n = dist.size(); // city_num
	std::vector<int> min_path;
	if (n > 1){
		int m = n; // ant_num
		std::vector<std::vector<double>> phero(n, std::vector<double>(n, MIN_CONSTANT)); // pheromones
		double Q = static_cast<double>(n * 1000); // max pheromone per cycle
		std::vector<std::vector<int>> ant_path(m);
		std::vector<double> ant_dist(m);
		double min_dist = MAX_CONSTANT;
		std::uniform_int_distribution<int> city_dist(0, n - 1);
		int i;
		for (int t = 0; t < tmax; t++){
			for (int k = 0; k < m; k++){
				i = city_dist(rng); 
				ant_dist[k] = calc_path(i, dist, phero, ant_path[k], a, 2);
			}
			phero_update(phero, ant_path, ant_dist, Q, rho);
			min_dist_update(ant_path, ant_dist, min_path, min_dist);
		}
	}
	return min_path;
}

std::vector<int> ant_algorithm(std::vector<std::vector<double>>& dist){
	int n = dist.size(); // city_num
	int m = n; // ant_num
	std::vector<std::vector<double>> phero(n, std::vector<double>(n, MIN_CONSTANT)); // pheromones
	double Q = static_cast<double>(n * 100); // max pheromone per cycle
	std::vector<std::vector<int>> ant_path(m);
	std::vector<double> ant_dist(m);
	std::vector<int> min_path;
	double min_dist = MAX_CONSTANT;
	std::uniform_int_distribution<int> city_dist(0, n - 1);
	int i;
	for (int t = 0; t < TMAX; t++){
		for (int k = 0; k < m; k++){
			i = city_dist(rng); 
			ant_dist[k] = calc_path(i, dist, phero, ant_path[k], ALPHA, BETA);
		}
		phero_update(phero, ant_path, ant_dist, Q, RHO);
		min_dist_update(ant_path, ant_dist, min_path, min_dist);
	}
	return min_path;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool my_next_permutation(std::vector<int>& nodes){
	bool res = true;
	int n = nodes.size(), i, j;
	for (i = n - 2; i >= 0 && nodes[i] >= nodes[i + 1]; i--);
	if (i >= 0){
		for (j = n - 1; nodes[j] <= nodes[i]; j--);
		std::swap(nodes[i], nodes[j]);
		std::reverse(nodes.begin() + i + 1, nodes.end());
	}
	else
		res = false;
	return res;
}

std::vector<int> brute_force(const std::vector<std::vector<double>>& dist){
	int n = dist.size(), cur;
	std::vector<int> min_path;
	double min_dist = MAX_CONSTANT, cur_dist;
	bool exists, next = true;
	std::vector<int> allowed;
	for (int i = 1; i < n; i++)
		allowed.push_back(i);
	while (next){
		exists = true;
		cur_dist = 0;		
		cur = 0;
		for (int j: allowed){
			if (dist[cur][j] < 0){
				exists = false;
				break;
			}
			else {
				cur_dist += dist[cur][j];
				cur = j;
			}
		}
		if (exists && dist[cur][0] >= 0){
			cur_dist += dist[cur][0];
			if (cur_dist < min_dist){
				min_dist = cur_dist;
				min_path = {0};
				min_path.insert(min_path.end(), allowed.begin(), allowed.end());
				min_path.push_back(0);
			}
		}
		next = my_next_permutation(allowed);
	}
	return min_path;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool equal_paths(const std::vector<int>& p1, const std::vector<int>& p2){
	bool res = true;
	if (p1.size() == p2.size()){
		for (auto i: p1){
			if (std::find(p2.begin(), p2.end(), i) == p2.end()){
				res = false;
				break;
			}
		}
	}
	else
		res = false;
	return res;
}

int read_matr(const std::string& filename, std::vector<std::vector<double>>& dist){
	int res = 0;
	std::ifstream file(filename);
	if (file.is_open()){
		double value;
		std::string text;
		std::stringstream line;
		int i = 0;
		while (!file.eof()){
			std::getline(file, text);
			line.clear();
			line.str(text);
			dist.emplace_back(); 
			while (!line.eof()){
				line >> value;
				dist[i].push_back(value);
			}
			i++;
		}
	}
	else
		res = 1;
	return res; 
}

std::vector<std::vector<double>> generate_sparse_clustered(const int n){
	std::vector<std::vector<double>> dist(n, std::vector<double>(n, 1e9));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(1.0, 50.0);
	int cluster_size = n / 3;
	for (int c = 0; c < 3; c++) {
		int start = c * cluster_size;
		for (int i = start; i < std::min(start + cluster_size, n); i++) {
			for (int j = start; j < std::min(start + cluster_size, n); j++) {
			if (i != j) 
				dist[i][j] = dis(gen);
			}
		}
		for (int other = 0; other < 3; other++) {
			if (other == c) 
				continue;
			int other_start = other * cluster_size;
			std::bernoulli_distribution conn(0.1);
			for (int i = start; i < std::min(start + cluster_size, n); i++) {
				for (int j = other_start; j < std::min(other_start + cluster_size, n); j++) {
					if (conn(gen)) {
						dist[i][j] = dis(gen) * 2;
						dist[j][i] = dist[i][j];
					}
				}
			}
		}
	}
	return dist;
}

void calc_elapsed(){
	std::vector<std::vector<double>> dist;
	std::ofstream out("timeres.txt");
	int n = 10, m = 15;
	double time_sec;
	for (int i = 2; i < m; i++){
		out << i << " ";
		dist = generate_sparse_clustered(i);
		time_sec = 0;
		for (int j = 0; j < n; j++){
			auto st = std::chrono::high_resolution_clock::now();
			brute_force(dist);
			auto fn = std::chrono::high_resolution_clock::now();
			time_sec += std::chrono::duration_cast<std::chrono::duration<double>>(fn - st).count();
		}
		out << time_sec / n << " ";
		time_sec = 0;
		for (int j = 0; j < n; j++){
			auto st = std::chrono::high_resolution_clock::now();
			ant_algorithm(dist);
			auto fn = std::chrono::high_resolution_clock::now();
			time_sec += std::chrono::duration_cast<std::chrono::duration<double>>(fn - st).count();
		}
		out << time_sec / n << std::endl;
	}
	out.close();
}

double get_len(const std::vector<int>& path, const std::vector<std::vector<double>>& dist){
	double res = 0;
	for (int i = 0; i < path.size() - 1; i++){
		res += dist[path[i]][path[i + 1]];
	}
	return res;
}

double max_dev(double opt, const std::vector<double>& results){
	double max_dev = 0;
	for (double len : results){
		max_dev = std::max(max_dev, (len - opt) / opt * 100);
	}
	return max_dev;
}

double mean_dev(double opt, const std::vector<double>& results){
	double sum = 0;
	for (double len : results){
		sum += (len - opt) / opt * 100;
	}
	return sum / results.size();
}

double med_dev(double opt, const std::vector<double>& results){
	std::vector<double> devs;
	for (double len : results){
		devs.push_back((len - opt) / opt * 100);
	}
	return devs[devs.size() / 2];
}

void param(){
	std::vector<std::vector<double>> dist1, dist2, dist3;
	read_matr("graph1.txt", dist1);
	read_matr("graph2.txt", dist2);
	read_matr("graph3.txt", dist3);
	double a1 = 17.2767, a2 = 55.5927, a3 = 41.2345; // optimal path values
	std::vector<double> t1, t2, t3;
	std::ofstream out("param.txt");
	double rho, alpha;
	std::vector<double> v1 = {0.1, 0.25, 0.5, 0.75, 0.9};
	double max_d, mean_d, med_d;
	double d1, d2, d3;
	std::vector<int> path;
	// for (int tmax = 200; tmax <= 1000; tmax += 200){
	for (int tmax = 800; tmax <= 1000; tmax += 200){
		for (int i = 0; i < 5; i++){
			rho = v1[i];
			for (int j = 0; j < 5; j++){
				alpha = v1[j];
				out << tmax << "&" << rho << "&" << alpha << "&";
				for (int k = 0; k < 10; k++){
					path = ant_algorithm(dist1, alpha, rho, tmax);
					t1.push_back(get_len(path, dist1));
					path = ant_algorithm(dist2, alpha, rho, tmax);
					t2.push_back(get_len(path, dist2));
					path = ant_algorithm(dist3, alpha, rho, tmax);
					t3.push_back(get_len(path, dist3));
				}
				std::sort(t1.begin(), t1.end());
				std::sort(t2.begin(), t2.end());
				std::sort(t3.begin(), t3.end());
				max_d = max_dev(a1, t1);
				mean_d = mean_dev(a1, t1);
				med_d = med_dev(a1, t1);
				d1 = std::max(max_d, std::max(mean_d, med_d));
				out << max_d << "&" << mean_d << "&" << med_d << "&"; 
				max_d = max_dev(a2, t2);
				mean_d = mean_dev(a2, t2);
				med_d = med_dev(a2, t2);
				d2 = std::max(max_d, std::max(mean_d, med_d));
				out << max_d << "&" << mean_d << "&" << med_d << "&"; 
				max_d = max_dev(a3, t3);
				mean_d = mean_dev(a3, t3);
				med_d = med_dev(a3, t3);
				d3 = std::max(max_d, std::max(mean_d, med_d));
				out << max_d << "&" << mean_d << "&" << med_d << "&"; 
				out << d1 << "&" << d2 << "&" << d3 << "\\\\\\hline" << std::endl; 
				t1.clear();
				t2.clear();
				t3.clear();
			}
		}
	}
	out.close();
}
