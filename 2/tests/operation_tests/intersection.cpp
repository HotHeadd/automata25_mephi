#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(operations, intersection_basic){
	std::string regex_1 = "v{2,5}";
	std::string regex_2 = "v{3,6}";

	DFA dfa1 = compile(regex_1);
	DFA dfa2 = compile(regex_2);
	DFA dfa1_min = compile(regex_1, true);
	DFA dfa2_min = compile(regex_2, true);

	DFABuilder builder;
	DFA intersection = builder.build_intersection(dfa1, dfa2);
	DFA intersection_min = builder.build_intersection(dfa1_min, dfa2_min);

	std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(intersection), std::ref(intersection_min)};

	for (auto& dfa : dfas){
		ASSERT_FALSE(fullmatch("v", dfa));
        ASSERT_FALSE(fullmatch("vv", dfa));
        ASSERT_FALSE(fullmatch("vvvvvv", dfa));
        ASSERT_FALSE(fullmatch("vvvvvvv", dfa));
        ASSERT_FALSE(fullmatch("", dfa));
        ASSERT_FALSE(fullmatch("vvvabc", dfa));
    
        ASSERT_TRUE(fullmatch("vvv", dfa));
        ASSERT_TRUE(fullmatch("vvvv", dfa));
        ASSERT_TRUE(fullmatch("vvvvv", dfa));
	}
}

TEST(operations, intersection_same_range){
    std::string regex_1 = "x{3,4}";
    std::string regex_2 = "x{3,4}";

    DFA dfa1 = compile(regex_1);
    DFA dfa2 = compile(regex_2);
    DFA dfa1_min = compile(regex_1, true);
    DFA dfa2_min = compile(regex_2, true);

    DFABuilder builder;
    DFA intersection = builder.build_intersection(dfa1, dfa2);
    DFA intersection_min = builder.build_intersection(dfa1_min, dfa2_min);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(intersection), std::ref(intersection_min)};

    for (auto& dfa : dfas){
        ASSERT_TRUE(fullmatch("xxx", dfa));
        ASSERT_TRUE(fullmatch("xxxx", dfa));
        ASSERT_FALSE(fullmatch("x", dfa));
        ASSERT_FALSE(fullmatch("xx", dfa));
        ASSERT_FALSE(fullmatch("xxxxx", dfa));
        ASSERT_FALSE(fullmatch("xxxabc", dfa));
    }
}

TEST(operations, intersection_single_common_word){
    std::string regex_1 = "z{1,3}";
    std::string regex_2 = "z{3,5}";

    DFA dfa1 = compile(regex_1);
    DFA dfa2 = compile(regex_2);
    DFA dfa1_min = compile(regex_1, true);
    DFA dfa2_min = compile(regex_2, true);

    DFABuilder builder;
    DFA intersection = builder.build_intersection(dfa1, dfa2);
    DFA intersection_min = builder.build_intersection(dfa1_min, dfa2_min);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(intersection), std::ref(intersection_min)};

    for (auto& dfa : dfas){
        ASSERT_TRUE(fullmatch("zzz", dfa));
        ASSERT_FALSE(fullmatch("z", dfa));
        ASSERT_FALSE(fullmatch("zz", dfa));
        ASSERT_FALSE(fullmatch("zzzz", dfa));
        ASSERT_FALSE(fullmatch("zzzzz", dfa));
    }
}

TEST(operations, intersection_with_zero){
    std::string regex_1 = "a{0,2}";
    std::string regex_2 = "a{2,4}";

    DFA dfa1 = compile(regex_1);
    DFA dfa2 = compile(regex_2);
    DFA dfa1_min = compile(regex_1, true);
    DFA dfa2_min = compile(regex_2, true);

    DFABuilder builder;
    DFA intersection = builder.build_intersection(dfa1, dfa2);
    DFA intersection_min = builder.build_intersection(dfa1_min, dfa2_min);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(intersection), std::ref(intersection_min)};

    for (auto& dfa : dfas){
        ASSERT_TRUE(fullmatch("aa", dfa));
        ASSERT_FALSE(fullmatch("", dfa));
        ASSERT_FALSE(fullmatch("a", dfa));
        ASSERT_FALSE(fullmatch("aaa", dfa));
        ASSERT_FALSE(fullmatch("aaaa", dfa));
    }
}

TEST(operations, intersection_disjoint){
    std::string regex_1 = "b{1,2}";
    std::string regex_2 = "b{3,4}";

    DFA dfa1 = compile(regex_1);
    DFA dfa2 = compile(regex_2);
    DFA dfa1_min = compile(regex_1, true);
    DFA dfa2_min = compile(regex_2, true);

    DFABuilder builder;
    DFA intersection = builder.build_intersection(dfa1, dfa2);
    DFA intersection_min = builder.build_intersection(dfa1_min, dfa2_min);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(intersection), std::ref(intersection_min)};

    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("", dfa));
        ASSERT_FALSE(fullmatch("b", dfa));
        ASSERT_FALSE(fullmatch("bb", dfa));
        ASSERT_FALSE(fullmatch("bbb", dfa));
        ASSERT_FALSE(fullmatch("bbbb", dfa));
    }
}