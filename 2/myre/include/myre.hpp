#pragma once

#include "dfa.hpp"
#include "parser.hpp"
#include <functional>
#include <optional>

namespace myre
{

struct Match {
	std::string::const_iterator begin;
	std::string::const_iterator end;

	Match(std::string::const_iterator b, std::string::const_iterator e)
        : begin(b), end(e) {}
	Match() {}

	std::string str() const {
		return std::string(begin, end);
	}

	bool empty() const {
		return begin == end;
	}

	size_t size() const {
		return std::distance(begin, end);
	}
};

DFA compile(const std::string& regex, bool optimize = false);
std::string decompile(const DFA& dfa);

bool search(const std::string& expr, const std::string& regex, bool optimize = false);
bool search(const std::string& expr, DFA& dfa);

bool search_first(const std::string& expr, const std::string& regex, Match& match, bool optimize = false);
bool search_first(const std::string& expr, DFA& dfa, Match& match);

std::function<std::optional<Match>()> make_lazy_search(const std::string& expr, const std::string& regex, bool optimize = false);
std::function<std::optional<Match>()> make_lazy_search(const std::string& expr, DFA& dfa);

bool fullmatch(const std::string& expr, const std::string& regex, bool optimize = false);
bool fullmatch(const std::string& expr, DFA& dfa);
	
} // namespace myre
