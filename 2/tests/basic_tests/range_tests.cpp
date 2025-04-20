#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(mutiple_ranges, double_range_nozero){
	std::string test = "a{2,3}{1,2}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("aa", dfa));
	ASSERT_TRUE(fullmatch("aaa", dfa));
	ASSERT_TRUE(fullmatch("aaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaa", dfa));

	ASSERT_FALSE(fullmatch("aaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aaabc", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(mutiple_ranges, sigle_lost_brackets){
	std::string test = "(((((((a))))))){2,3}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("aa", dfa));
	ASSERT_TRUE(fullmatch("aaa", dfa));

	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(mutiple_ranges, triple_no_zero){
	std::string test = "a{2,3}{1,2}{3,4}";
	DFA dfa = compile(test);

	for (int i=6; i<24; ++i){
		std::string str;
		for (int j=0; j<i; ++j){
			str += 'a';
		}
		ASSERT_TRUE(fullmatch(str, dfa));
	}

	ASSERT_FALSE(fullmatch("", dfa));
	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aa", dfa));
	ASSERT_FALSE(fullmatch("aaa", dfa));
	ASSERT_FALSE(fullmatch("aaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));
}

TEST(mutiple_ranges, triple_middle_zero){
	std::string test = "a{2,3}{0,2}{3,4}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));
	for (int i=6; i<24; ++i){
		std::string str;
		for (int j=0; j<i; ++j){
			str += 'a';
		}
		ASSERT_TRUE(fullmatch(str, dfa));
	}

	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));
}

TEST(mutiple_ranges, triple_end_zero_4to4){
	std::string test = "a{2,3}{4,4}{0,4}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));
	for (int i=8; i<13; ++i){
		std::string str;
		for (int j=0; j<i; ++j){
			str += 'a';
		}
		ASSERT_TRUE(fullmatch(str, dfa));
	}
	for (int i=16; i<49; ++i){
		std::string str;
		for (int j=0; j<i; ++j){
			str += 'a';
		}
		ASSERT_TRUE(fullmatch(str, dfa));
	}

	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aa", dfa));
	ASSERT_FALSE(fullmatch("aaa", dfa));
	ASSERT_FALSE(fullmatch("aaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaaaaaa", dfa));
}

TEST(mutiple_ranges, triple_end_zero_3to4){
	std::string test = "a{2,3}{3,4}{0,4}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));
	for (int i=6; i<24; ++i){
		std::string str;
		for (int j=0; j<i; ++j){
			str += 'a';
		}
		ASSERT_TRUE(fullmatch(str, dfa));
	}

	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aa", dfa));
	ASSERT_FALSE(fullmatch("aaa", dfa));
	ASSERT_FALSE(fullmatch("aaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));
}

TEST(mutiple_ranges, sigle_1){
	std::string test = "a{1,1}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("a", dfa));

	ASSERT_FALSE(fullmatch("aaa", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(mutiple_ranges, quadruple_1){
	std::string test = "a{1,1}{1,1}{1,1}{1,1}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("a", dfa));

	ASSERT_FALSE(fullmatch("aa", dfa));
	ASSERT_FALSE(fullmatch("aaa", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(mutiple_ranges, seven_to_inf){
	std::string test = "a{7,}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("aaaaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));

	ASSERT_FALSE(fullmatch("", dfa));
	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aa", dfa));
	ASSERT_FALSE(fullmatch("aaa", dfa));
	ASSERT_FALSE(fullmatch("aaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaa", dfa));
}

TEST(mutiple_ranges, sevet_to_inf_kleene){
	std::string test = "a{7,}*";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));

	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aa", dfa));
	ASSERT_FALSE(fullmatch("aaa", dfa));
	ASSERT_FALSE(fullmatch("aaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaa", dfa));
}

TEST(mutiple_ranges, seven_to_eight_kleene){
	std::string test = "a{7,8}*";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaa", dfa));
	ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaaa", dfa));

	ASSERT_FALSE(fullmatch("a", dfa));
	ASSERT_FALSE(fullmatch("aa", dfa));
	ASSERT_FALSE(fullmatch("aaa", dfa));
	ASSERT_FALSE(fullmatch("aaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("aaaaaaaaaaaaa", dfa));
}