#include "display.hpp"
#include "myre.hpp"

using namespace myre;

int main(){
	std::string test = "(qwertyuiopasdfghjklzxcvbnm)*";

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


	std::string expr = "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm";

	auto gen = make_lazy_search(expr, dfa);

	std::optional<Match> match = gen();
	std::cout << match->str() << std::endl;
	match = gen();
	std::cout << match->str() << std::endl;


	if (fullmatch(expr, min_dfa)){
		std::cout << "\nTRUE\n";
	}
	else{
		std::cout << "\nFALSE\n";
	}
	return 0;
}