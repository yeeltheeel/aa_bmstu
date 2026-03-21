#include "gtest/gtest.h"
#include <sstream>
#include <string>

#include "recursive.h"

// non-rec

TEST(print_n, best) {
	std::stringstream s;
	std::streambuf* oldcout = std::cout.rdbuf();
	std::cout.rdbuf(s.rdbuf());
	print_n(1);
	std::cout.rdbuf(oldcout);
	ASSERT_EQ("1 ", s.str());
	EXPECT_TRUE(true);
}

TEST(print_n, worst) {
	std::stringstream s;
	std::streambuf* oldcout = std::cout.rdbuf();
	std::cout.rdbuf(s.rdbuf());
	print_n(10);
	std::cout.rdbuf(oldcout);
	ASSERT_EQ("1 2 3 4 5 6 7 8 9 10 ", s.str());
	EXPECT_TRUE(true);
}

// rec

TEST(print_n_rec, best) {
	std::stringstream s;
	std::streambuf* oldcout = std::cout.rdbuf();
	std::cout.rdbuf(s.rdbuf());
	print_n_rec(1);
	std::cout.rdbuf(oldcout);
	ASSERT_EQ("1 ", s.str());
	EXPECT_TRUE(true);
}

TEST(print_n_rec, worst) {
	std::stringstream s;
	std::streambuf* oldcout = std::cout.rdbuf();
	std::cout.rdbuf(s.rdbuf());
	print_n_rec(10);
	std::cout.rdbuf(oldcout);
	ASSERT_EQ("1 2 3 4 5 6 7 8 9 10 ", s.str());
	EXPECT_TRUE(true);
}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
