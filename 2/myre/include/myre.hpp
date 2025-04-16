#pragma once

#include "regex.hpp"

namespace myre
{

bool search(const std::string& expr, Regex& regex);
bool search(const std::string& expr, DFA& dfa);
	
} // namespace myre
