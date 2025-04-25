#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

TEST(operations, complement_basic){
	std::string test = "v+";
	DFA dfa = compile(test);
	DFA min_dfa = compile(test, true);

	DFABuilder builder;
	DFA complement = builder.build_complement(dfa);
	DFA complement_min = builder.build_complement(min_dfa);
	std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};

	for (auto& dfa : dfas){
		ASSERT_FALSE(fullmatch("v", dfa));
        ASSERT_FALSE(fullmatch("vvv", dfa));
        ASSERT_FALSE(fullmatch("vvvvvvvvvvvvv", dfa));
    
        ASSERT_TRUE(fullmatch("vvvvDD", dfa));
        ASSERT_TRUE(fullmatch("dDvv", dfa));
        ASSERT_TRUE(fullmatch("d", dfa));
        ASSERT_TRUE(fullmatch("VV", dfa));
        ASSERT_TRUE(fullmatch("", dfa));
	}
}

TEST(operations, complement_basic_empty){
    std::string test = "";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("", dfa));

        ASSERT_TRUE(fullmatch("v", dfa));
        ASSERT_TRUE(fullmatch("vvv", dfa));
        ASSERT_TRUE(fullmatch("vvv   vDD", dfa));
        ASSERT_TRUE(fullmatch("	dv", dfa));
    }
}

TEST(operations, complement_basic_or_empty){
    std::string test = "aadf|";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("aadf", dfa));
        ASSERT_FALSE(fullmatch("", dfa));

        ASSERT_TRUE(fullmatch("bce", dfa));
        ASSERT_TRUE(fullmatch(" ", dfa));
    }
}

TEST(operations, complement_basic_or){
    std::string test = "a|b";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("a", dfa));
        ASSERT_FALSE(fullmatch("b", dfa));

        ASSERT_TRUE(fullmatch("iiia", dfa));
        ASSERT_TRUE(fullmatch("hhjkb", dfa));
        ASSERT_TRUE(fullmatch("", dfa));
    }
}

TEST(operations, complement_basic_concat){
    std::string test = "abc";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("abc", dfa));

        ASSERT_TRUE(fullmatch("", dfa));
        ASSERT_TRUE(fullmatch("ab", dfa));
        ASSERT_TRUE(fullmatch("ab c", dfa));
        ASSERT_TRUE(fullmatch("aabc c", dfa));
        ASSERT_TRUE(fullmatch("bc", dfa));
        ASSERT_TRUE(fullmatch("abcc", dfa));
        ASSERT_TRUE(fullmatch("ac", dfa));
    }
}

TEST(operations, complement_basic_range){
    std::string test = "ba{2,4}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("baa", dfa));
        ASSERT_FALSE(fullmatch("baaa", dfa));
        ASSERT_FALSE(fullmatch("baaaa", dfa));

        ASSERT_TRUE(fullmatch("qwerbaaaaaaa", dfa));
        ASSERT_TRUE(fullmatch("ba", dfa));
        ASSERT_TRUE(fullmatch("b", dfa));
        ASSERT_TRUE(fullmatch("baAa", dfa));
        ASSERT_TRUE(fullmatch("", dfa));
    }
}

TEST(operations, complement_basic_parenthesis){
    std::string test = "(ab)";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("ab", dfa));

        ASSERT_TRUE(fullmatch("abc", dfa));
        ASSERT_TRUE(fullmatch("dab", dfa));
        ASSERT_TRUE(fullmatch("(ab)", dfa));
        ASSERT_TRUE(fullmatch("ba", dfa));
        ASSERT_TRUE(fullmatch("b", dfa));
        ASSERT_TRUE(fullmatch("a b Aa", dfa));
        ASSERT_TRUE(fullmatch("", dfa));
    }
}

TEST(operations, complement_pars_range){
    std::string test = "hll(abcd){2,3}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("hllabcdabcd", dfa));
        ASSERT_FALSE(fullmatch("hllabcdabcdabcd", dfa));

        ASSERT_TRUE(fullmatch("man hllabcdabcdabcd dude", dfa));
        ASSERT_TRUE(fullmatch("(hllabcdabcda)", dfa));
        ASSERT_TRUE(fullmatch("hllabcd man abcd", dfa));
        ASSERT_TRUE(fullmatch("hll abcdabcd", dfa));
        ASSERT_TRUE(fullmatch("hllabcd", dfa));
        ASSERT_TRUE(fullmatch("", dfa));
    }
}

TEST(operations, complement_pars_or){
    std::string test = "(abcd|edfh){2}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("abcdedfh", dfa));
        ASSERT_FALSE(fullmatch("abcdabcd", dfa));
        ASSERT_FALSE(fullmatch("edfhedfh", dfa));

        ASSERT_TRUE(fullmatch("asd abcdedfh adfs", dfa));
        ASSERT_TRUE(fullmatch("edfh edfh", dfa));
        ASSERT_TRUE(fullmatch("edfhdsfabcd ", dfa));
        ASSERT_TRUE(fullmatch("", dfa));
    }
}

TEST(operations, complement_pars_empty){
    std::string test = "()";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("", dfa));

        ASSERT_TRUE(fullmatch("asd abcdedfh adfs", dfa));
        ASSERT_TRUE(fullmatch("edfh edfh", dfa));
        ASSERT_TRUE(fullmatch("edfhdsfabcd ", dfa));
    }
}

TEST(operations, complement_pars_empty_range){
    std::string test = "(){2,3}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);

    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};
    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("", dfa));

        ASSERT_TRUE(fullmatch("asd abcdedfh adfs", dfa));
        ASSERT_TRUE(fullmatch("edfh edfh", dfa));
        ASSERT_TRUE(fullmatch("edfhdsfabcd ", dfa));
    }
}

TEST(operations, complement_double_range_nozero){
    std::string test = "a{2,3}{1,2}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};

    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("aa", dfa));
        ASSERT_FALSE(fullmatch("aaa", dfa));
        ASSERT_FALSE(fullmatch("aaaa", dfa));
        ASSERT_FALSE(fullmatch("aaaaa", dfa));
        ASSERT_FALSE(fullmatch("aaaaaa", dfa));

        ASSERT_TRUE(fullmatch("aaaaaaa", dfa));
        ASSERT_TRUE(fullmatch("a", dfa));
        ASSERT_TRUE(fullmatch("aaabc", dfa));
        ASSERT_TRUE(fullmatch("", dfa));
    }
}

TEST(operations, complement_sigle_lost_brackets){
    std::string test = "(((((((a))))))){2,3}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};

    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("aa", dfa));
        ASSERT_FALSE(fullmatch("aaa", dfa));

        ASSERT_TRUE(fullmatch("a", dfa));
        ASSERT_TRUE(fullmatch("aaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaaaaa", dfa));
        ASSERT_TRUE(fullmatch("", dfa));
    }
}

TEST(operations, complement_triple_no_zero){
    std::string test = "a{2,3}{1,2}{3,4}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};

    for (auto& dfa : dfas){
        for (int i = 6; i < 24; ++i){
            std::string str(i, 'a');
            ASSERT_FALSE(fullmatch(str, dfa));
        }

        ASSERT_TRUE(fullmatch("", dfa));
        ASSERT_TRUE(fullmatch("a", dfa));
        ASSERT_TRUE(fullmatch("aa", dfa));
        ASSERT_TRUE(fullmatch("aaa", dfa));
        ASSERT_TRUE(fullmatch("aaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));
    }
}

TEST(operations, complement_triple_middle_zero){
    std::string test = "a{2,3}{0,2}{3,4}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};

    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("", dfa));
        for (int i = 6; i < 24; ++i){
            std::string str(i, 'a');
            ASSERT_FALSE(fullmatch(str, dfa));
        }

        ASSERT_TRUE(fullmatch("a", dfa));
        ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", dfa));
    }
}

TEST(operations, complement_triple_end_zero_4to4){
    std::string test = "a{2,3}{4,4}{0,4}";
    DFA dfa = compile(test);
    DFA min_dfa = compile(test, true);

    DFABuilder builder;
    DFA complement = builder.build_complement(dfa);
    DFA complement_min = builder.build_complement(min_dfa);
    std::array<std::reference_wrapper<DFA>, 2> dfas = {std::ref(complement), std::ref(complement_min)};

    for (auto& dfa : dfas){
        ASSERT_FALSE(fullmatch("", dfa));
        for (int i = 8; i < 13; ++i){
            std::string str(i, 'a');
            ASSERT_FALSE(fullmatch(str, dfa));
        }
        for (int i = 16; i < 49; ++i){
            std::string str(i, 'a');
            ASSERT_FALSE(fullmatch(str, dfa));
        }

        ASSERT_TRUE(fullmatch("a", dfa));
        ASSERT_TRUE(fullmatch("aa", dfa));
        ASSERT_TRUE(fullmatch("aaa", dfa));
        ASSERT_TRUE(fullmatch("aaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaaaaaaaaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaa", dfa));
        ASSERT_TRUE(fullmatch("aaaaaaaaaaaaaaa", dfa));
    }
}
