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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

request::request() { id = -1; filename = ""; }
request::request(const int i, const std::string& f) { id = i; filename = f; }
request::~request() {}
int request::get_id() const { return id; }
std::string request::get_filename() const { return filename; }
std::ostream& operator<<(std::ostream& os, const request& r) { return os << "{" << r.id << ", " << r.filename << "}"; }
void request::set_matr(std::vector<std::vector<double>>& new_matr) { matr = new_matr; }
void request::set_res(std::vector<int>& new_res) { res = new_res; }
std::vector<int> request::get_res() const { return res; }
std::vector<std::vector<double>> request::get_matr() const { return matr; }
void request::add_timestamp(){ time_log.push_back(std::chrono::system_clock::now()); }
std::vector<std::chrono::system_clock::time_point> request::get_timestamp() const { return time_log; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void task_1(std::queue<request>& queue_1, std::queue<request>& queue_2, std::mutex& m2, std::condition_variable& cv12, bool& d12) {
	request rtmp;
	while (!queue_1.empty()) {
		rtmp = queue_1.front();
		queue_1.pop();
		rtmp.add_timestamp();
		read_to_matr(rtmp);
		{
			std::lock_guard<std::mutex> lk(m2);
			queue_2.push(rtmp);
			cv12.notify_one();
		}
	}
	{
		std::lock_guard<std::mutex> lk(m2);
		d12 = true;
	}
	cv12.notify_all();
}

void task_2(std::queue<request>& queue_2, std::queue<request>& queue_3, std::mutex& m2, std::mutex& m3, 
	std::condition_variable& cv12, std::condition_variable& cv23, bool& d12, bool& d23) {
	request rtmp;
	while (true) {
		{
			std::unique_lock<std::mutex> lk(m2);
			cv12.wait(lk, [&]{ return !queue_3.empty() || d12; });
			if (queue_2.empty() && d12)
				break;
			else {
				rtmp = queue_2.front();
				queue_2.pop();
			}
		}
		rtmp.add_timestamp();
		find_components(rtmp);
		{
			std::lock_guard<std::mutex> lk(m3);
			queue_3.push(rtmp);
			cv23.notify_one();
		}
	}
	{
		std::lock_guard<std::mutex> lk(m3);
		d23 = true;
	}
	cv23.notify_all();
}

void task_3(std::queue<request>& queue_3, std::mutex& m3, std::condition_variable& cv23, bool& d23, 
	std::map<std::chrono::system_clock::time_point, std::vector<int>>& log) {
	request rtmp;
	while (true) {
		{
			std::unique_lock<std::mutex> lk(m3);
			cv23.wait(lk, [&] { return !queue_3.empty() || d23; });
			if (queue_3.empty() && d23)
				break;
			else {
				rtmp = queue_3.front();
				queue_3.pop();
			}
		}
		rtmp.add_timestamp();
		dump_to_file(rtmp);
		create_log(rtmp, log);
	}
}

int conveyer(const std::string& filename) {
	std::queue<request> queue_1, queue_2, queue_3;
	int res = fill_requests(filename, queue_1);
	if (res == 0) {
		std::condition_variable cv12, cv23;
		bool d12 = false, d23 = false;
		std::mutex m2, m3;
		std::map<std::chrono::system_clock::time_point, std::vector<int>> log; // timestamp : [id, i]
		std::vector<std::thread> threads(3);
		threads[0] = std::thread(task_1, std::ref(queue_1), std::ref(queue_2), std::ref(m2), std::ref(cv12), std::ref(d12));
		threads[1] = std::thread(task_2, std::ref(queue_2), std::ref(queue_3), std::ref(m2), std::ref(m3), std::ref(cv12), std::ref(cv23), std::ref(d12), std::ref(d23));
		threads[2] = std::thread(task_3, std::ref(queue_3), std::ref(m3), std::ref(cv23), std::ref(d23), std::ref(log));
		for (int i = 0; i < 3; i++) {
			if (threads[i].joinable())
				threads[i].join();
		}
		log_to_file(log);
	}
	return res;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int conveyer_single(const std::string& filename){
	std::queue<request> queue_1, queue_2, queue_3;
	int res = fill_requests(filename, queue_1);
	if (res == 0) {
		std::map<std::chrono::system_clock::time_point, std::vector<int>> log; // timestamp : [id, i]
		request rtmp;
		// task_1
		while (!queue_1.empty()) {
			rtmp = queue_1.front();
			queue_1.pop();
			rtmp.add_timestamp();
			read_to_matr(rtmp);
			queue_2.push(rtmp);
		}
		// task_2
		while (!queue_2.empty()) {
			rtmp = queue_2.front();
			queue_2.pop();
			rtmp.add_timestamp();
			find_components(rtmp);
			queue_3.push(rtmp);
		}
		// task_3
		while (!queue_3.empty()) {
			rtmp = queue_3.front();
			queue_3.pop();
			rtmp.add_timestamp();
			dump_to_file(rtmp);
			create_log(rtmp, log);
		}
		// log
		log_to_file(log);
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fill_requests(const std::string& filename, std::queue<request>& queue) {
	int res = 0;
	std::ifstream file(filename);
	if (file.is_open()) {
		int i = 0;
		std::string line;
		while (!file.eof()) {
			getline(file, line);
			if (line.length() > 0)
				queue.push(request(i, line));
			i++;
		}
	}
	else
		res = 1;
	return res;
}

void read_to_matr(request& r) {
	std::ifstream file(r.get_filename());
	if (file.is_open()) {
		int n;
		std::string line;
		file >> n;
		std::vector<std::vector<double>> matr(n, std::vector<double>(n));
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				file >> matr[i][j];
		r.set_matr(matr);
	}
}

std::vector<int> bfs(const int node, const std::vector<std::vector<double>>& matr) {
	std::vector<int> comp;
	int n = matr.size();
	if (n > 0 && node >= 0 && node < n) {
		std::vector<bool> visited(n, false);
		std::queue<int> q;
		visited[node] = true;
		q.push(node);
		int v;
		while (!q.empty()) {
			v = q.front();
			q.pop();
			comp.push_back(v);
			for (int u = 0; u < n; u++) {
				if (matr[v][u] != 0.0 && !visited[u]) {
					visited[u] = true;
					q.push(u);
				}
			}
		}
	}
	return comp;
}

void find_component_task(const int st, const int fn, const std::vector<std::vector<double>>& matr, std::vector<int>& res, std::mutex& mr) {
	std::vector<int> tmp;
	for (int i = st; i < fn; i++) {
		tmp = bfs(i, matr);
		{
			std::lock_guard<std::mutex> lk(mr);
			if (tmp.size() > res.size())
				res = tmp;
		}
	}
}

void find_components(request& r) {
	std::vector<std::vector<double>> matr = r.get_matr();
	std::vector<int> res;
	std::mutex mr;
	if (matr.size() > 0) {
		int m = matr.size(), t_cnt = K - 3, n, step, k = 0, kf;
		if (m <= t_cnt) {
			t_cnt = m;
			n = 1;
			step = 1;
		}
		else {
			n = m / t_cnt + ((m % t_cnt > 0) ? 1 : 0);
			step = std::min(n, m);
		}
		std::vector<std::thread> threads(t_cnt);
		for (int i = 0; i < t_cnt; i++) {
			kf = std::min(k + step, m);
			threads[i] = std::thread(find_component_task, k, kf, std::ref(matr), std::ref(res), std::ref(mr));
			k += step;
		}
		for (int i = 0; i < t_cnt; i++) {
			if (threads[i].joinable()) {
				threads[i].join();
			}
		}
		r.set_res(res);
	}
}

void dump_to_file(const request& r) {
	std::string filename = r.get_filename();
	int d = filename.find_last_of("/\\");
	if (d > 0) {
		std::string dir = filename.substr(0, d + 1), base = filename.substr(d + 1);
		filename = dir + "res_" + base;
	}
	else
		filename = "res_" + filename;
	std::ofstream file(filename);
	if (file.is_open()) {
		std::vector<int> res = r.get_res();
		if (res.size() > 0) {
			file << 1 << std::endl;
			for (auto i : res)
				file << i << " ";
		}
		else
			file << 0;
	}
}

void create_log(request& r, std::map<std::chrono::system_clock::time_point, std::vector<int>>& log) {
	std::vector<std::chrono::system_clock::time_point> tmp = r.get_timestamp();
	int i = 0;
	for (auto t: tmp){
		log[t] = {r.get_id(), i};
		i++;
	}
}

void log_to_file(std::map<std::chrono::system_clock::time_point, std::vector<int>>& log){
	std::ofstream out("log.txt");
	std::chrono::system_clock::time_point t;
	std::time_t tmp;
	char buf[26];
	if (out.is_open()) {
		for (const auto& pair: log){
			t = pair.first;
			out << log[t][0] << " " << log[t][1] << " ";
			tmp = std::chrono::system_clock::to_time_t(t);
			if (std::strftime(buf, sizeof(buf), "%Y-%m-%d,%H:%M:%S", std::localtime(&tmp)))
				out << buf;
			else
				out << tmp;
			out << std::endl;
		}
	}
	out.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::mt19937 gen(std::random_device{}());

void gen_matr(const int n, const double edge_prob){
	std::string filename = "graph/graph_" + std::to_string(n) + ".txt";
	if (std::filesystem::exists(filename))
		filename.insert(filename.length() - 4, "_copy");
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
			if (i != j && dist(gen) < edge_prob)
				w = wdist(gen);
			out << w;
			if (j + 1 < n) 
				out << " ";
		}
		out << std::endl;
	}
	out.close();
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

void get_elapsed(const int n, const int a){
	std::ofstream res("restm.txt");
	std::string filename;
	gen_req_files(n);
	double t;
	for (int i = 10; i <= n; i += 5){
		filename = "req/req_" + std::to_string(i) + ".txt";
		res << i << " ";
		t = 0;
		for (int k = 0; k < a; k++){
			auto st = std::chrono::high_resolution_clock::now();
			conveyer(filename);
			auto fn = std::chrono::high_resolution_clock::now();
			t += static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(fn - st).count());
		}
		res << t / n  << " ";
		t = 0;
		for (int k = 0; k < a; k++){
			auto st = std::chrono::high_resolution_clock::now();
			conveyer_single(filename);
			auto fn = std::chrono::high_resolution_clock::now();
			t += static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(fn - st).count());
		}
		res << t / n  << std::endl;
	}
	res.close();
}

int main(int argc, char* argv[]) {
	// conveyer("requests_test.txt");
	// conveyer_single("requests_test.txt");
	// get_elapsed(200, 10);
	conveyer("requests_test.txt");
	return 0;
}