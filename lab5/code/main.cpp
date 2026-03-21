#include "conv.h"

int main(int argc, char* argv[]) {
	if (argc == 1){
		conveyer("requests_test.txt");
		// conveyer_single("requests_test.txt");
	}
	else
		get_elapsed(200, 10);
	return 0;
}