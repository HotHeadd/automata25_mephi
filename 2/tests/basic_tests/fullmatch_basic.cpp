#include <gtest/gtest.h>
#include "myre.hpp"
#include "../fullmatch_tests.hpp"

using namespace myre;

TEST(fullmatch, all_basic){
    for (auto& tc : test_cases){
        std::string test = tc.regex;
        DFA dfa_old = compile(test);
        DFA dfa_min = compile(test, true);

        std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(dfa_old), std::ref(dfa_min)};
        for (auto& dfa : dfas){
            for (auto& good : tc.good){
                EXPECT_TRUE(fullmatch(good, dfa)) << "DEFAULT FULLMATCH Good test: " << good  << ", for regex: " << test; 
            }
            for (auto& bad : tc.bad){
                EXPECT_FALSE(fullmatch(bad, dfa)) << "DEFAULT FULLMATCH Bad test: " << bad << ", for regex: " << test; 
            }
        }
    }
}
