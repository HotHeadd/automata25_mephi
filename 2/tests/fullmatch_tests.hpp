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
std::vector<TestCase> fullmatch_test_cases = {
    {
        "a{2,3}{1,2}",
        {"aa", "aaa", "aaaa", "aaaaa", "aaaaaa"},
        {"aaaaaaa", "a", "aaabc", ""}
    },
    {
        "(((((((a))))))){2,3}",
        {"aa", "aaa"},
        {"a", "aaaa", "aaaaaaaa", ""}
    },
    {
        "a{2,3}{1,2}{3,4}",
        [](){
            std::vector<std::string> v;
            for (int i = 6; i < 24; ++i){
                v.emplace_back(std::string(i, 'a'));
            }
            return v;
        }(),
        {"", "a", "aa", "aaa", "aaaa", "aaaaa", std::string(47, 'a')}
    },
    {
        "a{2,3}{0,2}{3,4}",
        [](){
            std::vector<std::string> v{""};
            for (int i = 6; i < 24; ++i){
                v.emplace_back(std::string(i, 'a'));
            }
            return v;
        }(),
        {"a", std::string(47, 'a')}
    },
    {
        "a{2,3}{4,4}{0,4}",
        [](){
            std::vector<std::string> v{""};
            for (int i = 8; i < 13; ++i){
                v.emplace_back(std::string(i, 'a'));
            }
            for (int i = 16; i < 49; ++i){
                v.emplace_back(std::string(i, 'a'));
            }
            return v;
        }(),
        {"a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa", "aaaaaaa", "aaaaaaaaaaaaa", "aaaaaaaaaaaaaa", "aaaaaaaaaaaaaaa"}
    },
    {
        "a{2,3}{3,4}{0,4}",
        [](){
            std::vector<std::string> v{""};
            for (int i = 6; i < 24; ++i){
                v.emplace_back(std::string(i, 'a'));
            }
            return v;
        }(),
        {"a", "aa", "aaa", "aaaa", "aaaaa", std::string(66, 'a')}
    },
    {
        "a{1,1}",
        {"a"},
        {"aaa", ""}
    },
    {
        "a{1,1}{1,1}{1,1}{1,1}",
        {"a"},
        {"aa", "aaa", ""}
    },
    {
        "a{7,}",
        {"aaaaaaa", "aaaaaaaaaaaaaaaaaaaaaaaaaaa", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"},
        {"", "a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa"}
    },
    {
        "a{7,}*",
        {"", "aaaaaaa", "aaaaaaaaaaaaaaaaaaaaaaaaaaa", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"},
        {"a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa"}
    },
    {
        "a{7,8}*",
        {"", "aaaaaaa", "aaaaaaaa", "aaaaaaaaaaaaaa", "aaaaaaaaaaaaaaa", "aaaaaaaaaaaaaaaa"},
        {"a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa", "aaaaaaaaa", "aaaaaaaaaa", "aaaaaaaaaaa", "aaaaaaaaaaaa", "aaaaaaaaaaaaa"}
    },
	{
		"v+", 
		{"v", "vvv", "vvvvvvvvvvvvv"}, 
		{"vvvvDD", "dDvv", "d", "VV", ""}
	},
    {
		"", 
		{""}, 
		{"v", "vvv", "vvv   vDD", "	dv"}
	},
    {
		"aadf|", 
		{"aadf", ""}, 
		{"bce", " "}
	},
    {
		"a|b", 
		{"a", "b"}, 
		{"iiia", "hhjkb", ""}
	},
    {
		"abc", 
		{"abc"}, 
		{"", "ab", "ab c", "aabc c", "bc", "abcc", "ac"}
	},
	{
		"ba{2,4}", 
		{"baa", "baaa", "baaaa"}, 
		{"qwerbaaaaaaa", "ba", "b", "baAa", ""}
	},
	{
		"(ab)", 
		{"ab"}, 
		{"abc", "dab", "(ab)", "ba", "b", "a b Aa", ""}
	},
	{
		"hll(abcd){2,3}", 
		{"hllabcdabcd", "hllabcdabcdabcd"}, 
		{"man hllabcdabcdabcd dude", "(hllabcdabcda)", "hllabcd man abcd", "hll abcdabcd", "hllabcd", ""}
	},
	{
		"(abcd|edfh){2}", 
		{"abcdedfh", "abcdabcd", "edfhedfh"}, 
		{"asd abcdedfh adfs", "edfh edfh", "edfhdsfabcd ", ""}
	},
	{
		"()", 
		{""}, 
		{"asd abcdedfh adfs", "edfh edfh", "edfhdsfabcd "}
	},
	{
		"(){2,3}", 
		{""}, 
		{"asd abcdedfh adfs", "edfh edfh", "edfhdsfabcd "}
	},
	{
		"(abc){2,}", 
		{"abcabc", "abcabcabc", "abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc"}, 
		{"abc", "abcab", "bcabc ", ""}
	},
	{
		"(abc){,3}", 
		{"", "abc", "abcabc", "abcabcabc"}, 
		{"abcabcabcabc", "abcab", "bcabc "}
	},
	{
		"(abc){,}", 
		{"", "abc", "abcabc", "abcabcabc", "abcabcabcabcabcabcabcabcabcabcabcabcabcabcabc"}, 
		{"abcabcab", "abcab", "bcabc "}
	},
	{
		"(a#)#(){2,3}", 
		{"a)(a)(", "a)(a)(a)("}, 
		{"a)(", "a)(a)", "a)(a(", "a)(a)(a)(a)(a)("}
	},
	{
		"(a##){2,3}", 
		{"a#a#", "a#a#a#"}, 
		{"a#", "", "a#a#a#a#", "a#a#a#a#a#a#a#a#a#a#a#a#a#a#a#", "a#a#a", "a#a#a#a#a#a#a#aa#a#a#a#a#a#a#"}
	},
	{
		"(1324:a##){2,3}", 
		{"a#a#", "a#a#a#"}, 
		{"a#", "", "a#a#a#a#", "a#a#a#a#a#a#a#a#a#a#a#a#a#a#a#", "a#a#a", "a#a#a#a#a#a#a#aa#a#a#a#a#a#a#"}
	},
    {
        "(a|b|c|d|e)*",
        {"abebcbde", "", "a", "b", "c", "d", "e", "ab", "ed", "abb"},
        {"f", "abcfde", "00ab"}
    },
    {
        "(a|b|c|)+",
        {"", "a", "b", "c", "aaabc", "bc"},
        {"f", "a b", "dc"}
    },
    {
        "(1234:ab){2,3}",
        {"abab", "ababab"},
        {"", "ab", "abababab", "aab", "1234:ab1234:ab", "1234ab1234ab", "12341234", ":ab:ab:ab", ":ab:ab"}
    },
    {
        "(ab)*|(cd)",
        {"ab", "abab", "cd", ""},
        {"cdcd", "aba"}
    },
    {
        "(123)?",
        {"123", ""},
        {"1234", "123123", "12"}
    },
    {
        "(:abc)?",
        {"", "abc"},
        {":abc", "abcab", "abcabc", "ab", ":ab"}
    },
    {
        "(abc:123)?",
        {"", "abc:123"},
        {"abc", "123", "abc123", "abcabc", "123123"}
    },
    {
        "((ab|ac|ad)(ba|ca|da)(ab|ba|cb))*",
        {"abcaab", "", "abbaababbaab", "acbacb"},
        {"abba", "bacb", "addac", "add", "abbaabba", "abbaabaddaa"}
    }
};
