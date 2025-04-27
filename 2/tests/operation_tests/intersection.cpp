#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

struct InterTestCase {
    std::string regex1;
    std::string regex2;
    std::vector<std::string> good;
    std::vector<std::string> bad;
};

// regex1, regex2
// strings that should match their intersection
// strings that should not match their intersection

inline std::vector<InterTestCase> intersection_test_cases = {
    {
        "v{2,5}",
        "v{3,6}",
        {"vvv", "vvvv", "vvvvv"},
        {"v", "vv", "vvvvvv", "vvvvvvv", "", "vvvabc"}
    },
    {
        "x{3,4}",
        "x{3,4}",
        {"xxx", "xxxx"},
        {"x", "xx", "xxxxx", "xxxabc"}
    },
    {
        "z{1,3}",
        "z{3,5}",
        {"zzz"},
        {"z", "zz", "zzzz", "zzzzz"}
    },
    {
        "a{0,2}",
        "a{2,4}",
        {"aa"},
        {"", "a", "aaa", "aaaa"}
    },
    {
        "b{1,2}",
        "b{3,4}",
        {},
        {"", "b", "bb", "bbb", "bbbb"}
    }
};

TEST(operations, intersection_basic){
    for (auto& tc : intersection_test_cases){
        std::string regex_1 = tc.regex1;
        std::string regex_2 = tc.regex2;

        DFA dfa1 = compile(regex_1);
        DFA dfa2 = compile(regex_2);
        DFA dfa1_min = compile(regex_1, true);
        DFA dfa2_min = compile(regex_2, true);

        DFABuilder builder;
        DFA intersection = builder.build_intersection(dfa1, dfa2);
        DFA intersection_min = builder.build_intersection(dfa1_min, dfa2_min);

        std::vector<std::reference_wrapper<DFA>> dfas = {std::ref(intersection), std::ref(intersection_min)};

        for (auto& dfa : dfas){
            for (auto& good : tc.good){
                EXPECT_TRUE(fullmatch(good, dfa)) << "INTERSECTION Good test: \"" << good  << "\", for intersection: \"" << regex_1 << "\"" << " and \"" << regex_1 << "\""; 
            }
            for (auto& bad : tc.bad){
                EXPECT_FALSE(fullmatch(bad, dfa)) << "INTERSECTION Bad test: \"" << bad << "\", for intersection \"" << regex_1 << "\"" << " and \"" << regex_2 << "\""; 
            }
        }
    }
}