#include "sales.h"
#include "gtest/gtest.h"

std::vector<std::vector<double>> dist = {
	{0,  2,  9, 10,  7},
	{1,  0,  6,  4,  3},
	{9,  7,  0,  8,  5},
	{6,  3,  2,  0,  4},
	{3,  8,  5,  6,  0}
};
std::vector<int> res_path = {0, 1, 3, 2, 4, 0};

TEST(ant_algorithm, best){
	std::vector<int> path = ant_algorithm(dist);
	bool res = equal_paths(path, res_path);
	ASSERT_EQ(res, true);
}

TEST(brute_force, best){
	std::vector<int> path = brute_force(dist);
	bool res = equal_paths(path, res_path);
	ASSERT_EQ(res, true);
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
