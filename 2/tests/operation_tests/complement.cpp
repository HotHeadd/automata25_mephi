#include <gtest/gtest.h>
#include "myre.hpp"
#include "../fullmatch_tests.hpp"

using namespace myre;

TEST(complement, tests_for_fullmatch_reversed){
    for (auto& tc : fullmatch_test_cases){
        std::string test = tc.regex;
        DFA dfa = compile(test);
        DFA min_dfa = compile(test, true);
        
        std::string back_regex = decompile(dfa);
        std::string back_regex_min = decompile(min_dfa);

        DFA back_back = compile(back_regex);
        DFA back_back_min = compile(back_regex, true);
        DFA back_back_min_min = compile(back_regex_min);
        DFA back_back_min_min_min = compile(back_regex_min, true);


        DFABuilder builder;
        DFA complement = builder.build_complement(dfa);
        DFA complement_min = builder.build_complement(min_dfa);
        DFA complement_back = builder.build_complement(back_back);
        DFA complement_back_min = builder.build_complement(back_back_min);
        DFA complement_back_min_min = builder.build_complement(back_back_min_min);
        DFA complement_back_min_min_min = builder.build_complement(back_back_min_min_min);
        

        std::array<std::reference_wrapper<DFA>, 6> dfas = {
            std::ref(complement),
            std::ref(complement_min),
            std::ref(complement_back),
            std::ref(complement_back_min),
            std::ref(complement_back_min_min),
            std::ref(complement_back_min_min_min),};

        for (auto& dfa : dfas){
            for (auto& good : tc.good){
                EXPECT_FALSE(fullmatch(good, dfa)) << " COMPLEMENT Good test: " << good  << ", for regex: " << test; 
            }
            for (auto& bad : tc.bad){
                EXPECT_TRUE(fullmatch(bad, dfa)) << "COMPLEMENT Bad test: " << bad << ", for regex: " << test; 
            }
        }
    }
}
