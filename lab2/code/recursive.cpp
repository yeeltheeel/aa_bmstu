#include "recursive.h"

// вывести числа от 1 до n

void print_n(int n){
	for (int i = 1; i <= n; i++)
		std::cout << i << " ";
}

void print_n_rec(int n){
	if (n == 1)
		std::cout << n << " ";
	else{
		print_n(n - 1);
		std::cout << n << " ";
	}
}

double calc_processing_time(void (*func)(int), int n, int num){
	clock_t st, fn;
	double cpu_time_seconds = 0;
	for (int i = 0; i < num; i++){
		st = clock();
		func(n);
		fn = clock();
		cpu_time_seconds += static_cast<double>(fn - st);// / CLOCKS_PER_SEC;
	}
	return cpu_time_seconds / num;
}

