#include "display.hpp"
#include "myre.hpp"
#include <iostream>

using namespace myre;

int main(){

	std::string test = "v+";

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
	std::ofstream minfile("visuals/minimal.dot");\
	dump_dfa_dot(min_dfa, minfile);

	DFA complement = builder.build_complement(min_dfa);
	std::ofstream c_file("visuals/complement.dot");\
	dump_dfa_dot(complement, c_file);

	DFA intersection = builder.build_intersection(dfa, dfa);

	std::ofstream inter_file("visuals/intersection.dot");\
	dump_dfa_dot(intersection, inter_file);

	// auto gen = make_lazy_search(expr, dfa);

	// std::optional<Match> match = gen();
	// std::cout << match->str() << std::endl;
	// match = gen();
	// std::cout << match->str() << std::endl;
	
	std::string expr = "vvv";

	if (fullmatch(expr, min_dfa)){
		std::cout << "\nTRUE\n";
	}
	else{
		std::cout << "\nFALSE\n";
	}

	//TODO: unite test cases in single table
	//TODO: K-PATH, ADD TO TESTS
	return 0;
}