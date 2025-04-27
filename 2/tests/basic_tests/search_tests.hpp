#pragma once

#include <unordered_map>
#include <vector>
#include <string>


struct TestCase {
    std::string regex;
    std::vector<std::string> good;
    std::vector<std::string> bad;
};

// regex
// correct expressions
// incorrect expressions

inline std::vector<TestCase> search_test_cases = {
    {
        "v+",
        {"v", "vvv", "dDvv", "vvvvDD", "dv", "[Vv]"},
        {"d", "VV", ""}
    },
    {
        "",
        {"v", "vvv", "", "vvv   vDD", "\tdv"},
        {}
    },
    {
        "aadf|",
        {"aadf", "", "bce", " ", ""},
        {}
    },
    {
        "a|b",
        {"a", "b", "iiia", "hhjkb", "poiaujq"},
        {}
    },
    {
        "abc",
        {"abc", "abcde", "asdfabcde"},
        {"", "ab", "ab c", "bc", "ac"}
    },
    {
        "ba{2,4}",
        {"baa", "baaa", "baaaa", "qwerbaaaaaaa"},
        {"ba", "b", "baAa", ""}
    },
    {
        "(ab)",
        {"ab", "abc", "dab", "(ab)"},
        {"ba", "b", "a b Aa", ""}
    },
    {
        "hll(abcd){2,3}",
        {"hllabcdabcd", "man hllabcdabcdabcd dude", "hllabcdabcdabcd", "(hllabcdabcda)"},
        {"hllabcd man abcd", "hll abcdabcd", "hllabcd", ""}
    },
    {
        "(abcd|edfh){2}",
        {"abcdedfh", "abcdabcd", "edfhedfh", "asd abcdedfh adfs"},
        {"edfh edfh", "edfhdsfabcd ", ""}
    }
};
