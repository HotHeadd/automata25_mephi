#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(search, basic_1){
    Regex regex("v+");
	DFA dfa = regex.compile();

	ASSERT_TRUE(search("v", dfa));
	ASSERT_TRUE(search("vvv", dfa));
	ASSERT_TRUE(search("dDvv", dfa));
	ASSERT_TRUE(search("vvvvDD", dfa));
	ASSERT_TRUE(search("dv", dfa));
	ASSERT_TRUE(search("[Vv]", dfa));

	ASSERT_FALSE(search("d", dfa));
	ASSERT_FALSE(search("VV", dfa));
	ASSERT_FALSE(search("", dfa));
}

TEST(search, basic_empty){
    Regex regex("");
	DFA dfa = regex.compile();

	ASSERT_TRUE(search("v", dfa));
	ASSERT_TRUE(search("vvv", dfa));
	ASSERT_TRUE(search("", dfa));
	ASSERT_TRUE(search("vvv   vDD", dfa));
	ASSERT_TRUE(search("	dv", dfa));
}

TEST(search, basic_or_empty){
    Regex regex("aadf|");
	DFA dfa = regex.compile();

	ASSERT_TRUE(search("aadf", dfa));
	ASSERT_TRUE(search("", dfa));
	ASSERT_TRUE(search("bce", dfa));
	ASSERT_TRUE(search(" ", dfa));
	ASSERT_TRUE(search("", dfa));
}

TEST(search, basic_or){
    Regex regex("a|b");
	DFA dfa = regex.compile();

	ASSERT_TRUE(search("a", dfa));
	ASSERT_TRUE(search("b", dfa));
	ASSERT_TRUE(search("iiia", dfa));
	ASSERT_TRUE(search("hhjkb", dfa));
	ASSERT_TRUE(search("poiaujq", dfa));
}

TEST(search, basic_concat){
    Regex regex("abc");
	DFA dfa = regex.compile();

	ASSERT_TRUE(search("abc", dfa));
	ASSERT_TRUE(search("abcde", dfa));
	ASSERT_TRUE(search("asdfabcde", dfa));

	ASSERT_FALSE(search("", dfa));
	ASSERT_FALSE(search("ab", dfa));
	ASSERT_FALSE(search("bc", dfa));
	ASSERT_FALSE(search("ac", dfa));
}

TEST(search, basic_range){
    Regex regex("ba{2,4}");
	DFA dfa = regex.compile();

	ASSERT_TRUE(search("baa", dfa));
	ASSERT_TRUE(search("baaa", dfa));
	ASSERT_TRUE(search("baaaa", dfa));
	ASSERT_TRUE(search("qwerbaaaaaaa", dfa));

	ASSERT_FALSE(search("ba", dfa));
	ASSERT_FALSE(search("b", dfa));
	ASSERT_FALSE(search("baAa", dfa));
	ASSERT_FALSE(search("", dfa));
}