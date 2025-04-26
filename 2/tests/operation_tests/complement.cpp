#include <gtest/gtest.h>
#include "myre.hpp"
#include "../fullmatch_tests.hpp"

using namespace myre;

TEST(complement, tests_for_fullmatch_reversed){
    for (auto& tc : test_cases){
        std::string test = tc.regex;
        DFA dfa = compile(test);
        DFA min_dfa = compile(test, true);

        DFABuilder builder;
        DFA complement = builder.build_complement(dfa);
        DFA complement_min = builder.build_complement(min_dfa);
        std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};

        for (auto& dfa : dfas){
            for (auto& good : tc.good){
                EXPECT_FALSE(fullmatch(good, dfa)) << "DEFAULT FULLMATCH Good test: " << good  << ", for regex: " << test; 
            }
            for (auto& bad : tc.bad){
                EXPECT_TRUE(fullmatch(bad, dfa)) << "DEFAULT FULLMATCH Bad test: " << bad << ", for regex: " << test; 
            }
        }
    }
}
