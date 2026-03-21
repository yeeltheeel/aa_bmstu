#include <iostream>
#include <vector>
#include <random>
#include <fstream>

void write_to_file(const std::vector<std::vector<double>>& mat, const std::string& filename){
	std::ofstream out(filename);
	for (const auto& row : mat) {
        for (std::size_t j = 0; j < row.size(); ++j) {
            out << row[j];
            if (j + 1 < row.size())
                out << " "; // разделитель между элементами
        }
        out << std::endl;
    }
}

std::vector<std::vector<double>> generate_dense_graph(int n) {
    std::vector<std::vector<double>> dist(n, std::vector<double>(n, 0.0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1.0, 100.0);
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                dist[i][j] = dis(gen);  // все рёбра существуют
                dist[j][i] = dist[i][j]; // симметричный
            }
        }
    }
    return dist;  // ~100% плотность рёбер
}

std::vector<std::vector<double>> generate_sparse_clustered(int n) {
    std::vector<std::vector<double>> dist(n, std::vector<double>(n, 1e9)); // ∞ по умолчанию
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1.0, 50.0);
    
    // 3 кластера по n/3 вершины
    int cluster_size = n / 3;
    for (int c = 0; c < 3; ++c) {
        int start = c * cluster_size;
        // Внутри кластера - полносвязный
        for (int i = start; i < std::min(start + cluster_size, n); ++i) {
            for (int j = start; j < std::min(start + cluster_size, n); ++j) {
                if (i != j) dist[i][j] = dis(gen);
            }
        }
        // Между кластерами - 10% рёбер
        for (int other = 0; other < 3; ++other) {
            if (other == c) continue;
            int other_start = other * cluster_size;
            std::bernoulli_distribution conn(0.1);
            for (int i = start; i < std::min(start + cluster_size, n); ++i) {
                for (int j = other_start; j < std::min(other_start + cluster_size, n); ++j) {
                    if (conn(gen)) {
                        dist[i][j] = dis(gen) * 2;  // длиннее
                        dist[j][i] = dist[i][j];
                    }
                }
            }
        }
    }
    return dist;  // ~35% плотность
}

struct Point { double x, y; };

std::vector<std::vector<double>> generate_euclidean(int n) {
    std::vector<std::vector<double>> dist(n, std::vector<double>(n, 0.0));
    std::vector<Point> points(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1000.0);
    
    // Генерация координат точек
    for (int i = 0; i < n; ++i) {
        points[i].x = dis(gen);
        points[i].y = dis(gen);
    }
    
    // Евклидово расстояние
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                double dx = points[i].x - points[j].x;
                double dy = points[i].y - points[j].y;
                dist[i][j] = sqrt(dx*dx + dy*dy);
            }
        }
    }
    return dist;  // 100% связность, геометрическая структура
}

int main() {
    int n = 30;
    
    auto graph1 = generate_dense_graph(n);      // Класс 1: Густой
    auto graph2 = generate_sparse_clustered(n); // Класс 2: Разрежённый  
    auto graph3 = generate_euclidean(n);        // Класс 3: Географический

    write_to_file(graph1, "graph1.txt");
    write_to_file(graph2, "graph2.txt");
    write_to_file(graph3, "graph3.txt");
    
    //std::cout << "Размер каждой матрицы: " << n << "x" << n << std::endl;
    //std::cout << "Память: ~" << (n*n*8.0/1e6) << " МБ на матрицу" << std::endl;
    
    return 0;
}
