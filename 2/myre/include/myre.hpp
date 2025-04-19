#pragma once

#include "dfa.hpp"
#include "nodes.hpp"
#include "parser.hpp"

namespace myre
{

DFA compile(const std::string& regex); // parse + toDFA

bool search(const std::string& expr, const std::string& regex);
bool search(const std::string& expr, DFA& dfa);

bool fullmatch(const std::string& expr, const std::string& regex);
bool fullmatch(const std::string& expr, DFA& dfa);
	
} // namespace myre
