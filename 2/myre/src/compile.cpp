#include "myre.hpp"

namespace myre
{
	
DFA compile(const std::string& regex, bool optimize){
	RegexParser parser;
	DFABuilder builder;
	Context context;
	ContextIndex tree = parser.parse(regex, context);
	DFA dfa = builder.buildDFA(tree, context);
	if (optimize){
		dfa = builder.minimize_dfa(dfa);
	}
	return dfa;
}
}