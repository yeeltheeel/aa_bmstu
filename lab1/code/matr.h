#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

int std_matrmul(std::vector<std::vector<int>>& matr1, std::vector<std::vector<int>>& matr2, std::vector<std::vector<int>>& matr3, 
	int n, int m, int r, int q);
int winograd(std::vector<std::vector<int>>& matr1, std::vector<std::vector<int>>& matr2, std::vector<std::vector<int>>& matr3, 
	int n, int m, int r, int q);
int winograd_optimized(std::vector<std::vector<int>>& matr1, std::vector<std::vector<int>>& matr2, std::vector<std::vector<int>>& matr3, 
	int n, int m, int r, int q);

int read_matr(std::vector<std::vector<int>>& matr, int n, int m);
int print_matr(std::vector<std::vector<int>>& matr, int n, int m);
double calc_processing_time(int (*func)(std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, std::vector<std::vector<int>>&, 
	int, int, int, int), std::vector<std::vector<int>>& matr1, std::vector<std::vector<int>>& matr2, std::vector<std::vector<int>>& matr3, 
	int n, int m, int r, int q, int num);
int get_data(int n, int m, int q, int num);
int user_input();
