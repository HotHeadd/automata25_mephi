#include "display.hpp"
#include "myre.hpp"
#include <iostream>

using namespace myre;

int main(){

	std::string test = "a|";

	RegexParser parser;
	Context context;

	std::cout << "Parsing\n" << std::endl;
	ContextIndex node = parser.parse(test, context);
	std::cout << "vizualising\n" << std::endl;
	visualize(context, "visuals/ast.dot");
	std::cout << "new_vizualining\n" << std::endl;
	visualize_with_sets(context, "visuals/ast_plus.dot");

	std::cout << "Building\n" << std::endl;
	DFABuilder builder;
	DFA dfa = builder.buildDFA(node, context);
	std::cout << "vizualising\n" << std::endl;
	std::ofstream dotFile("visuals/automaton.dot");
	dump_dfa_dot(dfa, dotFile);
	
	std::cout << "Minimizing\n" << std::endl;
	DFA min_dfa = builder.minimize_dfa(dfa);
	std::cout << "vizualising\n" << std::endl;
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

	if (fullmatch(expr, dfa)){
		std::cout << "\nTRUE\n";
	}
	else{
		std::cout << "\nFALSE\n";
	}

	//TODO: dfa transition vector of maps
	//TODO: unite test cases in single table, check compile time
	//TODO: parse group number (super easy)
	//TODO: K-PATH, ADD TO TESTS
	//TODO: fix shared ptrs in AST
	return 0;
}