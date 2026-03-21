#pragma once
#include <iostream>
#include <ctime>

void print_n(int n);
void print_n_rec(int n);
double calc_processing_time(void (*func)(int), int n, int num);
