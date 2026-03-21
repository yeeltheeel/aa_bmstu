#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <thread>
#include <random>
#include <algorithm> 
#include <fstream>
#include <sstream>
#include <chrono>

#define MAX_CONSTANT 1e10
#define TIME_LIM 6912000 // 80 * 24 * 60 * 60
#define MIN_CONSTANT 1e-6

#define RHO 0.25
#define ALPHA 0.9
#define TMAX 200
#define BETA 2

static thread_local std::mt19937 rng{std::random_device{}()};

void min_dist_update(const std::vector<std::vector<int>>& ant_path, const std::vector<double>& ant_dist, 
	std::vector<int>& min_path, double& min_dist);
double calc_path(const int index, const std::vector<std::vector<double>>& dist, const std::vector<std::vector<double>>& phero, 
	std::vector<int>& ant_path, const double a, const double b);
void phero_update(std::vector<std::vector<double>>& phero, const std::vector<std::vector<int>>& ant_path, 
	const std::vector<double>& ant_dist, const double Q);
std::vector<int> ant_algorithm(std::vector<std::vector<double>>& dist);
std::vector<int> ant_algorithm(std::vector<std::vector<double>>& dist, const double a, const double b, const double tmax);

std::vector<int> brute_force(const std::vector<std::vector<double>>& dist);

bool equal_paths(const std::vector<int>& p1, const std::vector<int>& p2);
int read_matr(const std::string& filename, std::vector<std::vector<double>>& dist);
std::vector<std::vector<double>> generate_sparse_clustered(const int n);
void calc_elapsed();
double get_len(const std::vector<int>& path, const std::vector<std::vector<double>>& dist);
double max_dev(double opt, const std::vector<double>& results);
double mean_dev(double opt, const std::vector<double>& results);
double med_dev(double opt, const std::vector<double>& results);
void param();
