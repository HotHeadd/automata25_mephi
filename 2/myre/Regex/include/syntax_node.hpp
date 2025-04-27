#pragma once

#include <set>

namespace myre
{

using ContextIndex = size_t;

enum class NodeType{
	KLEENE,
	OR,
	CONCAT,
	CHAR,
	EOS,
	EPSYLON
};

class Context;

class SyntaxNode {
public:
	NodeType type;
	char value='\0';
	ContextIndex left=-1, right=-1;

	unsigned number;
	bool is_nullable;
	std::set<unsigned> first_pos;
	std::set<unsigned> last_pos;

	SyntaxNode(
		NodeType type_i,
		Context& context,
		ContextIndex left_kid = -1, 
		ContextIndex right_kid = -1
	);
	SyntaxNode(
		NodeType type_i, 
		Context& context,
		char value_i);
};

} // namespace myre


