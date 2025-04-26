#pragma once

#include <memory>
#include <unordered_map>
#include <set>
#include <vector>
#include "syntax_node.hpp"
#include "context.hpp"

namespace myre
{

class RegexParser {
public:
ContextIndex parse(const std::string& regex, Context& context); // возвращает корень дерева разбора
private:
	char peek() const;
	char next();
	bool consume_if_match(char c);

	std::pair<unsigned, unsigned> parse_range();
	ContextIndex transform_range(unsigned lower, unsigned upper, ContextIndex node, Context& context);
	ContextIndex clone(ContextIndex node, Context& context);

	ContextIndex parse_expression(Context& context);
	ContextIndex parse_term(Context& context);
	ContextIndex parse_atom(Context& context);

	

	std::string regex_;
	unsigned pos=0, paren_balance = 0;
	const unsigned INF = -1;
};

} // namespace myre
