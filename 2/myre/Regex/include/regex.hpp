#pragma once

#include <memory>

enum class Type {
	KLEENE,
	OR,
	CONCAT,
	CHAR
};

struct SyntaxNode {
	Type type;
	char value;
	std::shared_ptr<SyntaxNode> left, right;

	SyntaxNode(Type type_i, char value_i='\0') : type(type_i), value(value_i) {}
};

namespace myre
{

class Regex {
public:
	Regex(std::string expr) : expr_(expr) {}
	std::shared_ptr<SyntaxNode> parse(); // возвращает корень дерева разбора
private:
	std::string expr_;
	size_t curr;
};

} // namespace myre
