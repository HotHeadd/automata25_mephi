#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(search, basic_plus){
    std::string test = "v+";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
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
}

TEST(search, basic_empty){
    std::string test = "";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
        ASSERT_TRUE(search("v", dfa));
        ASSERT_TRUE(search("vvv", dfa));
        ASSERT_TRUE(search("", dfa));
        ASSERT_TRUE(search("vvv   vDD", dfa));
        ASSERT_TRUE(search("	dv", dfa));
    }
}

TEST(search, basic_or_empty){
    std::string test = "aadf|";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
        ASSERT_TRUE(search("aadf", dfa));
        ASSERT_TRUE(search("", dfa));
        ASSERT_TRUE(search("bce", dfa));
        ASSERT_TRUE(search(" ", dfa));
        ASSERT_TRUE(search("", dfa));
    }
}

TEST(search, basic_or){
    std::string test = "a|b";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
        ASSERT_TRUE(search("a", dfa));
        ASSERT_TRUE(search("b", dfa));
        ASSERT_TRUE(search("iiia", dfa));
        ASSERT_TRUE(search("hhjkb", dfa));
        ASSERT_TRUE(search("poiaujq", dfa));
    }
}

TEST(search, basic_concat){
    std::string test = "abc";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
        ASSERT_TRUE(search("abc", dfa));
        ASSERT_TRUE(search("abcde", dfa));
        ASSERT_TRUE(search("asdfabcde", dfa));

        ASSERT_FALSE(search("", dfa));
        ASSERT_FALSE(search("ab", dfa));
        ASSERT_FALSE(search("ab c", dfa));
        ASSERT_FALSE(search("bc", dfa));
        ASSERT_FALSE(search("ac", dfa));
    }
}

TEST(search, basic_range){
    std::string test = "ba{2,4}";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
        ASSERT_TRUE(search("baa", dfa));
        ASSERT_TRUE(search("baaa", dfa));
        ASSERT_TRUE(search("baaaa", dfa));
        ASSERT_TRUE(search("qwerbaaaaaaa", dfa));

        ASSERT_FALSE(search("ba", dfa));
        ASSERT_FALSE(search("b", dfa));
        ASSERT_FALSE(search("baAa", dfa));
        ASSERT_FALSE(search("", dfa));
    }
}

TEST(search, basic_parenthesis){
    std::string test = "(ab)";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
        ASSERT_TRUE(search("ab", dfa));
        ASSERT_TRUE(search("abc", dfa));
        ASSERT_TRUE(search("dab", dfa));
        ASSERT_TRUE(search("(ab)", dfa));

        ASSERT_FALSE(search("ba", dfa));
        ASSERT_FALSE(search("b", dfa));
        ASSERT_FALSE(search("a b Aa", dfa));
        ASSERT_FALSE(search("", dfa));
    }
}

TEST(search, pars_range){
    std::string test = "hll(abcd){2,3}";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
        ASSERT_TRUE(search("hllabcdabcd", dfa));
        ASSERT_TRUE(search("man hllabcdabcdabcd dude", dfa));
        ASSERT_TRUE(search("hllabcdabcdabcd", dfa));
        ASSERT_TRUE(search("(hllabcdabcda)", dfa));

        ASSERT_FALSE(search("hllabcd man abcd", dfa));
        ASSERT_FALSE(search("hll abcdabcd", dfa));
        ASSERT_FALSE(search("hllabcd", dfa));
        ASSERT_FALSE(search("", dfa));
    }
}

TEST(search, pars_or){
    std::string test = "(abcd|edfh){2}";
    DFA dfa_old = compile(test);
    DFA dfa_min = compile(test, true);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
    for (auto& dfa : dfas){
        ASSERT_TRUE(search("abcdedfh", dfa));
        ASSERT_TRUE(search("abcdabcd", dfa));
        ASSERT_TRUE(search("edfhedfh", dfa));
        ASSERT_TRUE(search("asd abcdedfh adfs", dfa));

        ASSERT_FALSE(search("edfh edfh", dfa));
        ASSERT_FALSE(search("edfhdsfabcd ", dfa));
        ASSERT_FALSE(search("", dfa));
    }
}
