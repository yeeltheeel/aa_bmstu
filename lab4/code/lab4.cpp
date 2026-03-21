#include "graph.h"

int main(int argc, char* argv[]){
	if (argc < 2)
		elapsed_time();
	else {
		graphviz_graph mygraph("testgraph.gv"); 
		// mygraph.print_name();
		// mygraph.print_type();
		// mygraph.print_nodes();
		mygraph.print_nodes_total();
		// mygraph.print_data();
		std::vector<std::unordered_set<std::string>> res;
		std::cout << "single threaded: " std::endl;
		res = mygraph.find_components();
		print_arr(res);
		res = mygraph.find_components_parallel(5); 
		std::cout << "multi threaded: " std::endl;
		print_arr(res);
	}
	return 0;
}
