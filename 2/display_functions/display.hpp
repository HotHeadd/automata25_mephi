#pragma once

#include "myre.hpp"
#include <fstream>
#include <iostream>

using namespace myre;

int generateDot(const std::shared_ptr<SyntaxNode>& node, std::ofstream& dotFile, int& counter);
void visualize(const std::shared_ptr<SyntaxNode>& root, const std::string& filename);
int generateDotAdvanced(const std::shared_ptr<SyntaxNode>& node, std::ofstream& dotFile, int& counter);
void visualize_with_sets(const std::shared_ptr<SyntaxNode>& root, const std::string& filename, Context& context);
void dump_dfa_dot(const DFA& dfa, std::ostream& out = std::cout);
