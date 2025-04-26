#pragma once

#include "myre.hpp"
#include <fstream>
#include <iostream>

using namespace myre;

void visualize(Context& ctx, const std::string& filename);
void generateDotAdvanced(const Context& ctx, std::ofstream& dotFile);
void visualize_with_sets(const Context& ctx, const std::string& filename);
void dump_dfa_dot(const DFA& dfa, std::ostream& out = std::cout);
