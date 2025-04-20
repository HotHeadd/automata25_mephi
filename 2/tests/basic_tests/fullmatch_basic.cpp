#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(fullmatch, basic_plus){
	std::string test = "v+";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("v", dfa));
	ASSERT_TRUE(fullmatch("vvv", dfa));
	ASSERT_TRUE(fullmatch("vvvvvvvvvvvvv", dfa));

	ASSERT_FALSE(fullmatch("vvvvDD", dfa));
	ASSERT_FALSE(fullmatch("dDvv", dfa));
	ASSERT_FALSE(fullmatch("d", dfa));
	ASSERT_FALSE(fullmatch("VV", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(fullmatch, basic_empty){
    std::string test = "";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));

	ASSERT_FALSE(fullmatch("v", dfa));
	ASSERT_FALSE(fullmatch("vvv", dfa));
	ASSERT_FALSE(fullmatch("vvv   vDD", dfa));
	ASSERT_FALSE(fullmatch("	dv", dfa));
}

TEST(fullmatch, basic_or_empty){
    std::string test = "aadf|";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("aadf", dfa));
	ASSERT_TRUE(fullmatch("", dfa));
	ASSERT_FALSE(fullmatch("bce", dfa));
	ASSERT_FALSE(fullmatch(" ", dfa));
}

TEST(fullmatch, basic_or){
    std::string test = "a|b";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("a", dfa));
	ASSERT_TRUE(fullmatch("b", dfa));
	ASSERT_FALSE(fullmatch("iiia", dfa));
	ASSERT_FALSE(fullmatch("hhjkb", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(fullmatch, basic_concat){
    std::string test = 	"abc";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("abc", dfa));

	ASSERT_FALSE(fullmatch("", dfa));
	ASSERT_FALSE(fullmatch("ab", dfa));
	ASSERT_FALSE(fullmatch("ab c", dfa));
	ASSERT_FALSE(fullmatch("aabc c", dfa));
	ASSERT_FALSE(fullmatch("bc", dfa));
	ASSERT_FALSE(fullmatch("abcc", dfa));
	ASSERT_FALSE(fullmatch("ac", dfa));
}

TEST(fullmatch, basic_range){
    std::string test = "ba{2,4}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("baa", dfa));
	ASSERT_TRUE(fullmatch("baaa", dfa));
	ASSERT_TRUE(fullmatch("baaaa", dfa));

	ASSERT_FALSE(fullmatch("qwerbaaaaaaa", dfa));
	ASSERT_FALSE(fullmatch("ba", dfa));
	ASSERT_FALSE(fullmatch("b", dfa));
	ASSERT_FALSE(fullmatch("baAa", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(fullmatch, basic_parenthesis){
    std::string test = "(ab)";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("ab", dfa));
	ASSERT_FALSE(fullmatch("abc", dfa));
	ASSERT_FALSE(fullmatch("dab", dfa));
	ASSERT_FALSE(fullmatch("(ab)", dfa));

	ASSERT_FALSE(fullmatch("ba", dfa));
	ASSERT_FALSE(fullmatch("b", dfa));
	ASSERT_FALSE(fullmatch("a b Aa", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(fullmatch, pars_range){
    std::string test = "hll(abcd){2,3}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("hllabcdabcd", dfa));
	ASSERT_TRUE(fullmatch("hllabcdabcdabcd", dfa));

	ASSERT_FALSE(fullmatch("man hllabcdabcdabcd dude", dfa));
	ASSERT_FALSE(fullmatch("(hllabcdabcda)", dfa));
	ASSERT_FALSE(fullmatch("hllabcd man abcd", dfa));
	ASSERT_FALSE(fullmatch("hll abcdabcd", dfa));
	ASSERT_FALSE(fullmatch("hllabcd", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(fullmatch, pars_or){
    std::string test = "(abcd|edfh){2}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("abcdedfh", dfa));
	ASSERT_TRUE(fullmatch("abcdabcd", dfa));
	ASSERT_TRUE(fullmatch("edfhedfh", dfa));

	ASSERT_FALSE(fullmatch("asd abcdedfh adfs", dfa));
	ASSERT_FALSE(fullmatch("edfh edfh", dfa));
	ASSERT_FALSE(fullmatch("edfhdsfabcd ", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(fullmatch, pars_empty){
    std::string test = "()";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));

	ASSERT_FALSE(fullmatch("asd abcdedfh adfs", dfa));
	ASSERT_FALSE(fullmatch("edfh edfh", dfa));
	ASSERT_FALSE(fullmatch("edfhdsfabcd ", dfa));
}

TEST(fullmatch, pars_empty_range){
    std::string test = "(){2,3}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));

	ASSERT_FALSE(fullmatch("asd abcdedfh adfs", dfa));
	ASSERT_FALSE(fullmatch("edfh edfh", dfa));
	ASSERT_FALSE(fullmatch("edfhdsfabcd ", dfa));
}


TEST(fullmatch, ragne_empty_border1){
    std::string test = "(abc){2,}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("abcabc", dfa));
	ASSERT_TRUE(fullmatch("abcabcabc", dfa));
	ASSERT_TRUE(fullmatch("abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc", dfa));

	ASSERT_FALSE(fullmatch("abc", dfa));
	ASSERT_FALSE(fullmatch("abcab", dfa));
	ASSERT_FALSE(fullmatch("bcabc ", dfa));
	ASSERT_FALSE(fullmatch("", dfa));
}

TEST(fullmatch, ragne_empty_border2){
    std::string test = "(abc){,3}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));
	ASSERT_TRUE(fullmatch("abc", dfa));
	ASSERT_TRUE(fullmatch("abcabc", dfa));
	ASSERT_TRUE(fullmatch("abcabcabc", dfa));

	ASSERT_FALSE(fullmatch("abcabcabcabc", dfa));
	ASSERT_FALSE(fullmatch("abcab", dfa));
	ASSERT_FALSE(fullmatch("bcabc ", dfa));
}

TEST(fullmatch, ragne_empty_border3){
    std::string test = "(abc){,}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("", dfa));
	ASSERT_TRUE(fullmatch("abc", dfa));
	ASSERT_TRUE(fullmatch("abcabc", dfa));
	ASSERT_TRUE(fullmatch("abcabcabc", dfa));
	ASSERT_TRUE(fullmatch("abcabcabcabcabcabcabcabcabcabcabcabcabcabcabc", dfa));

	ASSERT_FALSE(fullmatch("abcabcab", dfa));
	ASSERT_FALSE(fullmatch("abcab", dfa));
	ASSERT_FALSE(fullmatch("bcabc ", dfa));
}

TEST(fullmatch, shielding){
	std::string test = "(a#)#(){2,3}";
	DFA dfa = compile(test);

	ASSERT_TRUE(fullmatch("a)(a)(", dfa));
	ASSERT_TRUE(fullmatch("a)(a)(a)(", dfa));

	ASSERT_FALSE(fullmatch("a)(", dfa));
	ASSERT_FALSE(fullmatch("a)(a)", dfa));
	ASSERT_FALSE(fullmatch("a)(a(", dfa));
	ASSERT_FALSE(fullmatch("a)(a)(a)(a)(a)(", dfa));
	
}