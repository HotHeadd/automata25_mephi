#include <gtest/gtest.h>
#include "myre.hpp"
#include "../fullmatch_tests.hpp"

using namespace myre;

TEST(fullmatch, all_basic){
    for (auto& tc : fullmatch_test_cases){
        std::string test = tc.regex;
        DFA dfa_old = compile(test);
        DFA dfa_min = compile(test, true);
        
        std::string back_regex = decompile(dfa_old);
        std::string back_regex_min = decompile(dfa_min);
        DFA back_back = compile(back_regex);
        DFA back_back_min = compile(back_regex, true);
        DFA back_back_min_min = compile(back_regex_min);
        DFA back_back_min_min_min = compile(back_regex_min, true);

        std::array<std::reference_wrapper<DFA>, 6> dfas = {
            std::ref(dfa_old), 
            std::ref(dfa_min), 
            std::ref(back_back),
            std::ref(back_back_min),
            std::ref(back_back_min_min),
            std::ref(back_back_min_min_min),
        };
        for (auto& dfa : dfas){
            for (auto& good : tc.good){
                EXPECT_TRUE(fullmatch(good, dfa)) << "DEFAULT FULLMATCH Good test: \"" << good  << "\", for regex: \"" << test << "\""; 
            }
            for (auto& bad : tc.bad){
                EXPECT_FALSE(fullmatch(bad, dfa)) << "DEFAULT FULLMATCH Bad test: \"" << bad << "\", for regex: \"" << test << "\""; 
            }
        }
    }
}

