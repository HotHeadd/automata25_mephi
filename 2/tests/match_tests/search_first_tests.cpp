#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(search_first, basic_plus){
	std::string test = "v+";
	DFA dfa = compile(test);
	
	std::string t1 = "v";
	std::string t2 = "vvv";
	std::string t3 = "abcvv";
	std::string t4 = "vvagbc";
	std::string t5 = "abcvvagbc";
	std::string t6 = "abcvvagbcvvv";
	Match match1;
	Match match2;
	Match match3;
	Match match4;
	Match match5;
	Match match6;
	
	ASSERT_TRUE(search_first(t1, dfa, match1));
	ASSERT_TRUE(search_first(t2, dfa, match2));
	ASSERT_TRUE(search_first(t3, dfa, match3));
	ASSERT_TRUE(search_first(t4, dfa, match4));
	ASSERT_TRUE(search_first(t5, dfa, match5));
	ASSERT_TRUE(search_first(t6, dfa, match6));

	ASSERT_EQ(match1.str(), "v");
	ASSERT_EQ(match2.str(), "v");
	ASSERT_EQ(match3.str(), "v");
	ASSERT_EQ(match4.str(), "v");
	ASSERT_EQ(match5.str(), "v");
	ASSERT_EQ(match6.str(), "v");

	ASSERT_EQ(match1.begin, t1.begin());
	ASSERT_EQ(match2.begin, t2.begin());
	ASSERT_EQ(match3.begin, t3.begin()+3);
	ASSERT_EQ(match4.begin, t4.begin());
	ASSERT_EQ(match5.begin, t5.begin()+3);
	ASSERT_EQ(match6.begin, t6.begin()+3);
}