#include "display.hpp"
#include "myre.hpp"

using namespace myre;

int main(){
	std::string test = "a{0,2}";

	// TODO: test new searches
	RegexParser parser;
	Context context;

	std::shared_ptr<SyntaxNode> node = parser.parse(test, context);
	visualize(node, "visuals/ast.dot");
	visualize_with_sets(node, "visuals/ast_plus.dot", context);

	DFABuilder builder;
	DFA dfa = builder.buildDFA(node, context);
	std::ofstream dotFile("visuals/automaton.dot");
	dump_dfa_dot(dfa, dotFile);
	
	DFA min_dfa = builder.minimize_dfa(dfa, context);
	std::ofstream minfile("visuals/minimal.dot");\
	dump_dfa_dot(min_dfa, minfile);

	DFA complement = builder.build_complement(min_dfa);
	std::ofstream c_file("visuals/complement.dot");\
	dump_dfa_dot(complement, c_file);


	std::string expr = "aaa";
	if (fullmatch(expr, min_dfa)){
		std::cout << "\nTRUE\n";
	}
	else{
		std::cout << "\nFALSE\n";
	}
	return 0;
}