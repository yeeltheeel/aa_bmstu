#include "gtest/gtest.h"
#include "graph.h"

#define Q 16

const std::string file_contents = R"(
digraph G {
	n0 -> n14;
	n0 -> n18;
	n0 -> n19;
	n0 -> n33;
	n0 -> n37;
	n1 -> n21;
	n1 -> n25;
	n2 -> n31;
	n4 -> n29;
	n5 -> n45;
	n6 -> n9;
	n7 -> n24;
	n7 -> n34;
	n12 -> n18;
	n12 -> n37;
	n14 -> n36;
	n15 -> n49;
	n16 -> n26;
	n16 -> n32;
	n17 -> n30;
	n19 -> n38;
	n20 -> n30;
	n27 -> n46;
	n27 -> n48;
	n30 -> n45;
	n31 -> n32;
	n31 -> n44;
	n35 -> n44;
	n38 -> n40;
	n38 -> n44;
	n39 -> n44;
	n39 -> n48;
	n43 -> n46;
}
)";

graphviz_graph mygraph("testgraph.gv");
std::vector<std::unordered_set<std::string>> components = {
	{"n26", "n43", "n0", "n39", "n44", "n18", "n32", "n35", "n31", "n38", "n36", "n40", "n33", "n48", 
	"n27", "n19", "n37", "n2", "n46", "n12", "n14", "n16"},
	{"n5", "n17", "n45", "n30", "n20"}, {"n21", "n1", "n25"}, {"n9", "n6"},
	{"n4", "n29"}, {"n34", "n24", "n7"}, {"n15", "n49"}
};

bool compare_component(std::vector<std::unordered_set<std::string>>& a, std::vector<std::unordered_set<std::string>>& b) {
	bool res = (a.size() == b.size());
	std::sort(a.begin(), a.end(), [](const auto &a1, const auto &a2) { return a1.size() > a2.size(); });
	std::sort(b.begin(), b.end(), [](const auto &a1, const auto &a2) { return a1.size() > a2.size(); });
	res &= (a[0].size() == b[0].size());
	return res;
}

TEST(single, best){
	std::vector<std::unordered_set<std::string>> tmp_components = mygraph.find_components();
	bool res = compare_component(tmp_components, components);
	EXPECT_EQ(res, true);
}

TEST(parallel, best){
	std::vector<std::unordered_set<std::string>> tmp_components = mygraph.find_components_parallel(Q);
	bool res = compare_component(tmp_components, components);
	EXPECT_EQ(res, true);
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}