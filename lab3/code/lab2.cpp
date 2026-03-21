#include "recursive.h"
#include <fstream>

#define N 1000

int main(int argc, char* argv[]){
	if (argc == 1){
		std::ofstream file("proctime.txt");
		for (int i = 1; i <= 100; i++){
			file << i << " ";
			file << calc_processing_time(print_n_rec, i, N) << " ";
			file << calc_processing_time(print_n, i, N) << std::endl;
		}
		file.close();
	}
	else{
		int n;
		std::cout << "input n: ";
		std::cin >> n;
		if (n < 1){
			std::cout << "error: n is not positive integer" << std::endl;
		}
		else{
			std::cout << "recursive    : ";
			print_n_rec(n);
			std::cout << std::endl;
			std::cout << "non-recursive: ";
			print_n(n);
			std::cout << std::endl;
		}
	}

	return 0;
}
