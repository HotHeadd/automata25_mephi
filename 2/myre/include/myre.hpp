#pragma once

#include "regex.hpp"
#include "smatch.hpp"

namespace myre
{

void compile_regex(Regex& regex);
bool search(const std::string& expr, Regex& regex);
bool search(const std::string& expr, Regex regex, Smatch matches);
	
} // namespace myre
