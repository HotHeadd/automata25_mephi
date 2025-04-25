#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(search_first, basic_plus){
	std::string test = "v+";
	DFA dfa = compile(test);
	
	std::vector<std::pair<std::string, size_t>> positives = {
        {"v", 0},
		{"vvv", 0},
		{"abcvv", 3},
		{"vvagbc", 0},
		{"abcvvagbc", 3},
		{"abcvvagbcvvv", 3},
    };

	for (const auto& [input, offset] : positives) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str(), "v");
        ASSERT_EQ(match.begin, input.begin() + offset);
    }
}

TEST(search_first, basic_empty){
    std::string test = "";
    DFA dfa = compile(test);

    std::vector<std::string> inputs = {
        "v", "vvv", "", "vvv   vDD", "\tdv"
    };

    for (const auto& input : inputs) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str(), "");
        ASSERT_EQ(match.begin, input.begin());
    }
}

TEST(search_first, basic_or_empty){
    std::string test = "aadf|";
    DFA dfa = compile(test);

    std::vector<std::string> inputs = {
        "aadf", "", "bce", " ", ""
    };

    for (const auto& input : inputs) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str(), "");
        ASSERT_EQ(match.begin, input.begin());
    }
}

TEST(search_first, basic_or){
    std::string test = "a|b";
    DFA dfa = compile(test);

    std::vector<std::pair<std::string, size_t>> cases = {
        {"a", 0}, {"b", 0}, {"iiia", 3}, {"hhjkb", 4}, {"poiaujq", 3}
    };

    for (const auto& [input, offset] : cases) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str().size(), 1);
        ASSERT_EQ(match.begin, input.begin() + offset);
    }
}

TEST(search_first, basic_concat){
    std::string test = "abc";
    DFA dfa = compile(test);

    std::vector<std::pair<std::string, size_t>> positives = {
        {"abc", 0}, {"abcde", 0}, {"asdfabcde", 4}
    };

    for (const auto& [input, offset] : positives) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str(), "abc");
        ASSERT_EQ(match.begin, input.begin() + offset);
    }

    std::vector<std::string> negatives = {"", "ab", "ab c", "bc", "ac"};
    for (const auto& input : negatives) {
        Match match;
        ASSERT_FALSE(search_first(input, dfa, match));
    }
}

TEST(search_first, basic_range){
    std::string test = "ba{2,4}";
    DFA dfa = compile(test);

    std::vector<std::pair<std::string, std::string>> positives = {
        {"baa", "baa"}, {"baaa", "baa"}, {"baaaa", "baa"}, {"qwerbaaaaaaa", "baa"}
    };

    for (const auto& [input, expected] : positives) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str(), expected);
    }

    std::vector<std::string> negatives = {"ba", "b", "baAa", ""};
    for (const auto& input : negatives) {
        Match match;
        ASSERT_FALSE(search_first(input, dfa, match));
    }
}

TEST(search_first, basic_parenthesis){
    std::string test = "(ab)";
    DFA dfa = compile(test);

    std::vector<std::pair<std::string, size_t>> positives = {
        {"ab", 0}, {"abc", 0}, {"dab", 1}, {"(ab)", 1}
    };

    for (const auto& [input, offset] : positives) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str(), "ab");
        ASSERT_EQ(match.begin, input.begin() + offset);
    }

    std::vector<std::string> negatives = {"ba", "b", "a b Aa", ""};
    for (const auto& input : negatives) {
        Match match;
        ASSERT_FALSE(search_first(input, dfa, match));
    }
}

TEST(search_first, pars_range){
    std::string test = "hll(abcd){2,3}";
    DFA dfa = compile(test);

    std::vector<std::pair<std::string, std::string>> positives = {
        {"hllabcdabcd", "hllabcdabcd"},
        {"man hllabcdabcdabcd dude", "hllabcdabcd"},
        {"hllabcdabcdabcd", "hllabcdabcd"},
        {"(hllabcdabcda)", "hllabcdabcd"}
    };

    for (const auto& [input, expected] : positives) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str(), expected);
    }

    std::vector<std::string> negatives = {"hllabcd man abcd", "hll abcdabcd", "hllabcd", ""};
    for (const auto& input : negatives) {
        Match match;
        ASSERT_FALSE(search_first(input, dfa, match));
    }
}

TEST(search_first, pars_or){
    std::string test = "(abcd|edfh){2}";
    DFA dfa = compile(test);

    std::vector<std::pair<std::string, std::string>> positives = {
        {"abcdedfh", "abcdedfh"},
        {"abcdabcd", "abcdabcd"},
        {"edfhedfh", "edfhedfh"},
        {"asd abcdedfh adfs", "abcdedfh"}
    };

    for (const auto& [input, expected] : positives) {
        Match match;
        ASSERT_TRUE(search_first(input, dfa, match));
        ASSERT_EQ(match.str(), expected);
    }

    std::vector<std::string> negatives = {
        "edfh edfh", "edfhdsfabcd ", ""
    };

    for (const auto& input : negatives) {
        Match match;
        ASSERT_FALSE(search_first(input, dfa, match));
    }
}
