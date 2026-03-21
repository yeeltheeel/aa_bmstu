#include "matr.h"

int main(int argc, char* argv[]){
	int res = 0;

	if (argc > 1){
		res = user_input();
	}
	else{
		// size, total el in both matr, std, win, win_opt
		for (int i = 3; i <= 50; i++){
			std::cout << i << " ";
			get_data(i, i, i, 1000);
		}
	}

	return res;
}