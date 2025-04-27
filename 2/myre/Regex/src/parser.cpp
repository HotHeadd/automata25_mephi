#include "parser.hpp"
#include "errors.hpp"
#include <cctype>

namespace myre
{

ContextIndex RegexParser::parse(const std::string& regex, Context& context){
	regex_ = regex;
	context.reset();
	pos = 0;
	ContextIndex node = parse_expression(context);
	if (pos != regex.size()) {
		if (regex[pos] == ')') throw ParenthesesError(regex_);
		throw SyntaxError(regex_);
	}
	return context.emplace_node(
		NodeType::CONCAT,
		node, 
		context.emplace_node(NodeType::EOS)
	);;
}

ContextIndex RegexParser::parse_expression(Context& context){
	ContextIndex node = parse_term(context);
	ContextIndex right;
	if (node == -1) {
		node = context.emplace_node(NodeType::EPSYLON);
	}
	while (consume_if_match('|')){
		right = parse_term(context);
		if (right == -1) {
			right = context.emplace_node(NodeType::EPSYLON);
		}
		node = context.emplace_node(NodeType::OR, node, right);
	}
	return node;
}

ContextIndex RegexParser::parse_term(Context& context){
	ContextIndex node = -1, right;
	while (true){
		char next_char = peek();
		if (next_char == '\0' or next_char == ')' or next_char == '|'){
			break;
		}
		right = parse_atom(context);
		if (right == -1){
			break;
		}
		if (node == -1) {
			node = right;
		} 
		else {
			node = context.emplace_node(NodeType::CONCAT, node, right);
		}
	}
	return node;
}

ContextIndex RegexParser::parse_atom(Context& context){
	ContextIndex node;
	if (consume_if_match('(')) {
        ++paren_balance;
        // пустые скобки ()
        if (peek() == ')') {
            ++pos;
            --paren_balance;
            node = context.emplace_node(NodeType::EPSYLON);
        }
        else {
			parse_group_name();
            node = parse_expression(context);
            if (!consume_if_match(')')) {
                throw ParenthesesError(regex_);
            }
            --paren_balance;
            if (paren_balance < 0) {
                throw ParenthesesError(regex_);
            }
        }
    }
	else if (consume_if_match(')')){
		throw ParenthesesError(regex_);
	}
	else {
        char c = next();
        if (c == '#') {
            if (pos >= regex_.size()) throw SyntaxError(regex_);
            c = next();
        }
        node = context.emplace_node(NodeType::CHAR, c);
    }
	while (true) {
        if (consume_if_match('*')) {
            node = context.emplace_node(NodeType::KLEENE, node);
        }
        else if (consume_if_match('+')) {
            node = transform_range(1, INF, node, context);
        }
        else if (consume_if_match('?')) {
            node = transform_range(0, 1, node, context);
        }
        else if (peek() == '{') {
            auto [lower, upper] = parse_range();
            node = transform_range(lower, upper, node, context);
        }
        else {
            break;
        }
    }
	return node;
}

void RegexParser::parse_group_name(){
	int bonus = 0;
	while (regex_[pos+bonus] - '0' >= 0 and regex_[pos+bonus] - '9' <= 0){
		++bonus;
	}
	if (regex_[pos+bonus] == ':'){
		++bonus;
		pos += bonus;
	}
}

ContextIndex RegexParser::transform_range(unsigned lower, unsigned upper, ContextIndex base, Context& context){
	ContextIndex node = -1;
	for (int i=0; i<lower; ++i){
		if (i==0){
			node = base;
		}
		else{
			node = context.emplace_node(NodeType::CONCAT, node, clone(base, context));
		}
	}
	if (upper == INF){
		if (node == -1){
			node = context.emplace_node(NodeType::KLEENE, base);
		}
		else{
			node = context.emplace_node(
				NodeType::CONCAT,
				node, 
				context.emplace_node(NodeType::KLEENE, clone(base, context))
			);
		}
	}
	else{
		for (int i=lower; i<upper; ++i){
			if (node == -1){
				node = context.emplace_node(
					NodeType::OR, 
					base,
					context.emplace_node(NodeType::EPSYLON));
			}
			else{
				node = context.emplace_node(
					NodeType::CONCAT,
					node,
					context.emplace_node(
						NodeType::OR,
						clone(base, context),
						context.emplace_node(NodeType::EPSYLON)));
			}
		}
	}
	return node;
}

ContextIndex RegexParser::clone(ContextIndex node_indx, Context& context){
	ContextIndex new_left, new_right, new_node;
	SyntaxNode& node = context.get_node(node_indx);
	if (node.type == NodeType::CONCAT or node.type == NodeType::OR){
		new_left = clone(node.left, context);
		new_right = clone(node.right, context);
		new_node = context.emplace_node(node.type, new_left, new_right);
	}
	else if (node.type == NodeType::KLEENE){
		new_left = clone(node.left, context);
		new_node = context.emplace_node(node.type, new_left);
	}
	else{
		new_node = context.emplace_node(node.type, node.value);
	}
	return new_node;
}

std::pair<unsigned, unsigned> RegexParser::parse_range(){
	std::string lower, upper;
	bool is_upper = false, not_closed = true;
	unsigned i_lower=0, i_upper=0;
	while (pos < regex_.size()){
		char sym = next();
		if (sym == '}'){
			not_closed = false;
			break;
		}
		else if (sym == ',' and is_upper == false){
			is_upper = true;
		}
		else if (std::isdigit(sym)){
			if (is_upper){
				upper += sym;
			}
			else{
				lower += sym;
			}
		}
		else if (sym != '{'){
			throw SyntaxError(regex_);
		}
	}
	if (pos >= regex_.size() and not_closed){
		throw ParenthesesError(regex_);
	}
	if (lower.empty() and upper.empty()){
		lower = "0";
		i_upper = INF;
	}
	else if (lower.empty()){
		lower = "0";
	}
	else if (upper.empty()){
		if (is_upper == true){
			i_upper = INF;
		}
		else{
			upper = lower;
		}
	}
	try{
		i_lower = std::stoull(lower);
		if (!upper.empty()){
			i_upper = std::stoull(upper);
		}
	}
	catch (std::invalid_argument& e){
		throw SyntaxError(regex_);
	}
	if (i_lower > i_upper){
		throw RangeError(i_lower, i_upper);
	}
	return {i_lower, i_upper};
}

char RegexParser::peek() const {
	return pos < regex_.size() ? regex_[pos] : '\0';
}

char RegexParser::next() {
	return pos < regex_.size() ? regex_[pos++] : '\0';
}

bool RegexParser::consume_if_match(char c) {
	if (peek() == c) {
		++pos;
		return true;
	}
	return false;
}

} // namespace myre
