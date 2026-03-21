#include "matr.h"


int std_matrmul(std::vector<std::vector<int>>& matr1, std::vector<std::vector<int>>& matr2, std::vector<std::vector<int>>& matr3, 
	int n, int m, int r, int q){
	int res = 0;
	if (m == r)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < q; j++){
				matr3[i][j] = 0;
				for (int k = 0; k < m; k++)
					matr3[i][j] += matr1[i][k] * matr2[k][j];
			}
	else
		res = 1;
	return res;
}

int winograd(std::vector<std::vector<int>>& matr1, std::vector<std::vector<int>>& matr2, std::vector<std::vector<int>>& matr3, 
	int n, int m, int r, int q){
	int res = 0;

	if (m == r){
		int d = m / 2;
		
		std::vector<int> row_coef(m, 0);
		for (int i = 0; i < n; i++){
			row_coef[i] = matr1[i][0] * matr1[i][1];
			for (int k = 1; k < d; k++)
				row_coef[i] += matr1[i][2 * k] * matr1[i][2 * k + 1];
		}

		std::vector<int> col_coef(q, 0);
		for (int i = 0; i < q; i++){
			col_coef[i] = matr2[0][i] * matr2[1][i];
			for (int k = 1; k < d; k++)
				col_coef[i] += matr2[2 * k][i] * matr2[2 * k + 1][i];
		}

		for (int i = 0; i < n; i++)
			for (int j = 0; j < q; j++){
				matr3[i][j] = -row_coef[i] - col_coef[j];
				for (int k = 0; k < d; k++)
					matr3[i][j] += (matr1[i][2 * k + 1] + matr2[2 * k][j]) * (matr1[i][2 * k] + matr2[2 * k + 1][j]);
			}

		if (m % 2 == 1){
			for (int i = 0; i < n; i++)
				for (int j = 0; j < q; j++)
					matr3[i][j] += matr1[i][m - 1] * matr2[r - 1][j]; 
		}
	}
	else
		res = 1;

	return res;
}

int winograd_optimized(std::vector<std::vector<int>>& matr1, std::vector<std::vector<int>>& matr2, std::vector<std::vector<int>>& matr3, 
	int n, int m, int r, int q){
	int res = 0;

	if (m == r){
		int d = m / 2;

		std::vector<int> col_coef(q, 0);
		for (int i = 0; i < q; i++){
			col_coef[i] = matr2[0][i] * matr2[1][i];
			for (int k = 2; k < d; k += 2)
				col_coef[i] += matr2[k][i] * matr2[k + 1][i];
		}

		for (int i = 0; i < n; i++)
			for (int j = 0; j < q; j++){
				matr3[i][j] = -col_coef[j];
				for (int k = 0; k < d; k += 2){
					matr3[i][j] += -matr1[i][k] * matr1[i][k + 1] + (matr1[i][k + 1] + matr2[k][j]) * (matr1[i][k] + matr2[k + 1][j]);
				}
			}

		if (m % 2 == 1){
			for (int i = 0; i < n; i++)
				for (int j = 0; j < q; j++)
					matr3[i][j] += matr1[i][m - 1] * matr2[r - 1][j]; 
		}
	}
	else
		res = 1;

	return res;
}


int read_matr(std::vector<std::vector<int>>& matr, int n, int m){
	std::cout << "input matr " << n << "x" << m << ":" << std::endl;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++)
			std::cin >> matr[i][j];
		std::cout << std::endl;
	}
	return 0;
}

int print_matr(std::vector<std::vector<int>>& matr, int n, int m){
	std::cout << "matr " << n << "x" << m << ":" << std::endl;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++)
			std::cout << matr[i][j] << " ";
		std::cout << std::endl;
	}
	return 0;
}

int fill_matr(std::vector<std::vector<int>>& matr, int n, int m){
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			matr[i][j] = rand();
	return 0;
}

double calc_processing_time(int (*func)(std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, 
	int, int, int, int), std::vector<std::vector<int>>& matr1, std::vector<std::vector<int>>& matr2, std::vector<std::vector<int>>& matr3, 
	int n, int m, int r, int q, int num){
	clock_t st, fn;
	double cpu_time_seconds = 0;
	for (int i = 0; i < num; i++){
		st = clock();
		func(matr1, matr2, matr3, n, m, r, q);
		fn = clock();
		cpu_time_seconds += static_cast<double>(fn - st);// / CLOCKS_PER_SEC;
	}
	return cpu_time_seconds / num;
}

int get_data(int n, int m, int q, int num){
	std::vector<std::vector<int>> matr1(n, std::vector<int>(m, 0));
	std::vector<std::vector<int>> matr2(m, std::vector<int>(q, 0));
	std::vector<std::vector<int>> matr3(n, std::vector<int>(q, 0));
	fill_matr(matr1, n, m);
	fill_matr(matr2, m, q);
	std::cout << n * m + m * q << " ";
	double tmp;
	tmp = calc_processing_time(std_matrmul, matr1, matr2, matr3, n, m, m, q, num);
	std::cout << tmp << " ";
	tmp = calc_processing_time(winograd, matr1, matr2, matr3, n, m, m, q, num);
	std::cout << tmp << " ";
	tmp = calc_processing_time(winograd_optimized, matr1, matr2, matr3, n, m, m, q, num);
	std::cout << tmp << std::endl;
	return 0;
}

int user_input(){
	int res = 0;

	int n, m, r, q;
	std::cout << "input n: ";
	std::cin >> n;
	std::cout << "input m: ";
	std::cin >> m;
	std::cout << "input r: ";
	std::cin >> r;
	std::cout << "input q: ";
	std::cin >> q;
	std::cout << std::endl;

	if (m == r){
		std::vector<std::vector<int>> matr1(n, std::vector<int>(m, 0));
		std::vector<std::vector<int>> matr2(r, std::vector<int>(q, 0));
		std::vector<std::vector<int>> matr3(n, std::vector<int>(q, 0));

		read_matr(matr1, n, m);
		read_matr(matr2, r, q);

		print_matr(matr1, n, m);
		print_matr(matr2, r, q);

		std::cout << "standard:" << std::endl;
		std_matrmul(matr1, matr2, matr3, n, m, r, q);
		print_matr(matr3, n, q);

		for (auto& row: matr3)
			std::fill(row.begin(), row.end(), 0);

		std::cout << "winograd:" << std::endl;
		winograd(matr1, matr2, matr3, n, m, r, q);
		print_matr(matr3, n, q);

		for (auto& row: matr3)
			std::fill(row.begin(), row.end(), 0);

		std::cout << "winograd_optimized:" << std::endl;
		winograd_optimized(matr1, matr2, matr3, n, m, r, q);
		print_matr(matr3, n, q);
	}
	else
		res = 1;

	return res;
}
