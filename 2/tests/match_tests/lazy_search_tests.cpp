#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(lazy_search, single_match){
    std::string input = "abc";
    DFA dfa = compile("abc");
    auto gen = make_lazy_search(input, dfa);

    auto m1 = gen();
    ASSERT_TRUE(m1.has_value());
    ASSERT_EQ(m1->str(), "abc");

    auto m2 = gen();
    ASSERT_FALSE(m2.has_value());
}

TEST(lazy_search, multiple_matches){
    std::string input = "abc abc";
    DFA dfa = compile("abc");
    auto gen = make_lazy_search(input, dfa);

    auto m1 = gen();
    ASSERT_TRUE(m1.has_value());
    ASSERT_EQ(m1->str(), "abc");

    auto m2 = gen();
    ASSERT_TRUE(m2.has_value());
    ASSERT_EQ(m2->str(), "abc");

    auto m3 = gen();
    ASSERT_FALSE(m3.has_value());
}

TEST(lazy_search, match_in_noise){
    std::string input = "xxxabcxxabcxx";
    DFA dfa = compile("abc");
    auto gen = make_lazy_search(input, dfa);

    std::vector<size_t> starts = {3, 8};
    for (size_t i = 0; i < starts.size(); ++i) {
        auto m = gen();
        ASSERT_TRUE(m.has_value());
        ASSERT_EQ(m->str(), "abc");
        ASSERT_EQ(m->begin - input.begin(), starts[i]);
    }

    ASSERT_FALSE(gen().has_value());
}

TEST(lazy_search, accepts_empty){
    std::string input = "abc";
    DFA dfa = compile("");
    auto gen = make_lazy_search(input, dfa);

    for (size_t i = 0; i <= input.size(); ++i){
        auto m = gen();
        ASSERT_TRUE(m.has_value());
        ASSERT_EQ(m->str(), "");
        ASSERT_EQ(m->begin - input.begin(), i);
    }

    ASSERT_FALSE(gen().has_value());
}

TEST(lazy_search, no_matches){
    std::string input = "abc";
    DFA dfa = compile("xyz");
    auto gen = make_lazy_search(input, dfa);

    ASSERT_FALSE(gen().has_value());
}

TEST(lazy_search, overlapping_matches_not_found){
    std::string input = "aaaa";
    DFA dfa = compile("aa");
    auto gen = make_lazy_search(input, dfa);

    auto m1 = gen();
    ASSERT_TRUE(m1.has_value());
    ASSERT_EQ(m1->str(), "aa");

    auto m2 = gen();
    ASSERT_TRUE(m2.has_value());
    ASSERT_EQ(m2->str(), "aa");

    auto m3 = gen();
    ASSERT_TRUE(m3.has_value());
    ASSERT_EQ(m3->str(), "aa");

	auto m4 = gen();
	ASSERT_FALSE(m4.has_value());
}

TEST(lazy_search, lazy_vs_greedy){
    std::string input = "aaaaa";
    DFA dfa = compile("a+");
    auto gen = make_lazy_search(input, dfa);

    auto m = gen();
    ASSERT_TRUE(m.has_value());
    ASSERT_EQ(m->str(), "a");
}

