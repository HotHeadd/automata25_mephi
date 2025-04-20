#pragma once

#include <memory>
#include <set>

namespace myre
{
	
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
	std::shared_ptr<SyntaxNode> left=nullptr, right=nullptr;

	unsigned number;
	bool is_nullable;
	std::set<unsigned> first_pos;
	std::set<unsigned> last_pos;

	SyntaxNode(
		NodeType type_i,
		Context& context,
		std::shared_ptr<SyntaxNode> left_kid = nullptr, 
		std::shared_ptr<SyntaxNode> right_kid = nullptr
	);
	SyntaxNode(
		NodeType type_i, 
		Context& context,
		char value_i);
};

} // namespace myre


