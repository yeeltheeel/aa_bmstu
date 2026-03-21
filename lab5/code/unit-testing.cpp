#include "gtest/gtest.h"
#include "conv.h"

TEST(conveyer_single, best){
	int n = conveyer_single("requests_test.txt");
	bool res = (n == 0) && std::filesystem::exists("log.txt");
	ASSERT_EQ(res, true);
}

TEST(conveyer_multi, best){
	int n = conveyer("requests_test.txt");
	bool res = (n == 0) && std::filesystem::exists("log.txt");
	ASSERT_EQ(res, true);
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
