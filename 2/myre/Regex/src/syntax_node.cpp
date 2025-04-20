#include "syntax_node.hpp"
#include "context.hpp"


namespace myre
{
SyntaxNode::SyntaxNode(
	NodeType type_i,
	Context& context,
	std::shared_ptr<SyntaxNode> left_kid, 
	std::shared_ptr<SyntaxNode> right_kid
)
: type(type_i), left(left_kid), right(right_kid) {
	context.deduce_sets(this);
}

SyntaxNode::SyntaxNode(
	NodeType type_i,
	Context& context, 
	char value_i)
	: type(type_i), value(value_i) {
		context.deduce_sets(this);
}
} // namespace myre

