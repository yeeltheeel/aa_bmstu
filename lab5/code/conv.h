#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional> 
#include <chrono>
#include <random>
#include <filesystem>

#define K 16

class request {
private:
	int id;
	std::string filename;
	std::vector<std::vector<double>> matr;
	std::vector<int> res;
	std::vector<std::chrono::system_clock::time_point> time_log;
public:
	request();
	request(const int i, const std::string& f);
	~request();
	int get_id() const;
	std::string get_filename() const;
	friend std::ostream& operator<<(std::ostream& os, const request& r);
	void set_matr(std::vector<std::vector<double>>& new_matr);
	std::vector<std::vector<double>> get_matr() const;
	void set_res(std::vector<int>& new_res);
	std::vector<int> get_res() const;
	void add_timestamp();
	std::vector<std::chrono::system_clock::time_point> get_timestamp() const;
};

int fill_requests(const std::string& filename, std::queue<request>& queue);
void read_to_matr(request& r);
std::vector<int> bfs(const int node, const std::vector<std::vector<double>>& matr);
void find_component_task(const int st, const int fn, const std::vector<std::vector<double>>& matr, std::vector<int>& res, std::mutex& mr);
void find_components(request& r);
void dump_to_file(const request& r);
void get_elapsed(const int n, const int a);
void gen_req_files(const int n);
void gen_matr(const int n, const double edge_prob);

void task_1(std::queue<request>& queue_1, std::queue<request>& queue_2, std::mutex& m2, std::condition_variable& cv12, bool& d12);
void task_2(std::queue<request>& queue_2, std::queue<request>& queue_3, std::mutex& m2, std::mutex& m3, std::condition_variable& cv12, std::condition_variable& cv23, bool& d12, bool& d23);
void task_3(std::queue<request>& queue_3, std::mutex& m3, std::condition_variable& cv23, bool& d23, std::map<std::chrono::system_clock::time_point, std::vector<int>>& log);
void create_log(request& r, std::map<std::chrono::system_clock::time_point, std::vector<int>>& log);
void log_to_file(std::map<std::chrono::system_clock::time_point, std::vector<int>>& log);
int conveyer(const std::string& filename);
int conveyer_single(const std::string& filename);
