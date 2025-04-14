#include "regex.hpp"
#include <stack>

namespace myre
{

DFA Regex::compile(){
	RegexParser parser;
	DFABuilder builder;

	std::shared_ptr<SyntaxNode> tree = parser.parse(expr_);
	return builder.buildDFA(tree);
}

} // namespace myre
