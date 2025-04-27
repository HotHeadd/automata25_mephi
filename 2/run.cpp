#include "display.hpp"
#include "myre.hpp"
#include <iostream>

using namespace myre;

int main(){

	std::string test = "((ab|ac|ad)(ba|ca|da)(ab|ba|cb))*";

	RegexParser parser;
	Context context;

	ContextIndex node = parser.parse(test, context);
	visualize(context, "visuals/ast.dot");
	visualize_with_sets(context, "visuals/ast_plus.dot");

	DFABuilder builder;
	DFA dfa = builder.buildDFA(node, context);
	std::ofstream dotFile("visuals/automaton.dot");
	dump_dfa_dot(dfa, dotFile);
	
	DFA min_dfa = builder.minimize_dfa(dfa);
	std::ofstream minfile("visuals/minimal.dot");
	dump_dfa_dot(min_dfa, minfile);

	DFA complement = builder.build_complement(min_dfa);
	std::ofstream c_file("visuals/complement.dot");
	dump_dfa_dot(complement, c_file);

	DFA intersection = builder.build_intersection(dfa, dfa);

	std::ofstream inter_file("visuals/intersection.dot");
	dump_dfa_dot(intersection, inter_file);

	std::string back = decompile(dfa);
	std::cout << "\n" <<  back << std::endl;

	DFA back_dfa = compile(back);
	DFA back_min_dfa = compile(back, true);

	std::ofstream back_file("visuals/back_dfa.dot");
	std::ofstream back_file_min("visuals/back_min_dfa.dot");

	dump_dfa_dot(back_dfa, back_file);
	dump_dfa_dot(back_min_dfa, back_file_min);
	
	std::string expr = "";

	if (fullmatch(expr, min_dfa)){
		std::cout << "\nTRUE\n";
	}
	else{
		std::cout << "\nFALSE\n";
	}

	return 0;
}