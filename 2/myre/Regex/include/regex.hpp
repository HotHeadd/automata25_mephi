#pragma once

#include "parser.hpp"
#include "dfa.hpp"

namespace myre
{

class Regex {
public:
	Regex(std::string expr) : expr_(expr) {}
	DFA compile(); // parse + toDFA
private:
	std::string expr_;
};


} // namespace myre
