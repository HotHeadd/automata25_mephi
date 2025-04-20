#include "parser.hpp"
#include "errors.hpp"
#include <cctype>

namespace myre
{

std::shared_ptr<SyntaxNode> RegexParser::parse(const std::string& regex, Context& context){
	regex_ = regex;
	context.reset();
	pos = 0;
	std::shared_ptr<SyntaxNode> node = parse_expression(context);
	if (pos != regex.size()) {
		if (regex[pos] == ')') throw ParenthesesError(regex_);
		throw SyntaxError(regex_);
	}
	return std::make_shared<SyntaxNode>(
				NodeType::CONCAT,
				context,
				node, 
				std::make_shared<SyntaxNode>(NodeType::EOS, context)
	);
}

std::shared_ptr<SyntaxNode> RegexParser::parse_expression(Context& context){
	std::shared_ptr<SyntaxNode> node = parse_term(context);
	std::shared_ptr<SyntaxNode> right;
	if (!node) {
		node = std::make_shared<SyntaxNode>(NodeType::EPSYLON, context);
	}
	while (consume_if_match('|')){
		right = parse_term(context);
		if (!right) {
			right = std::make_shared<SyntaxNode>(NodeType::EPSYLON, context);
		}
		node = std::make_shared<SyntaxNode>(NodeType::OR, context, node, right);
	}
	return node;
}

std::shared_ptr<SyntaxNode> RegexParser::parse_term(Context& context){
	std::shared_ptr<SyntaxNode> node = nullptr, right;
	while (true){
		char next_char = peek();
		if (next_char == '\0' or next_char == ')' or next_char == '|'){
			break;
		}
		right = parse_atom(context);
		if (!right){
			break;
		}
		if (!node) {
			node = right;
		} 
		else {
			node = std::make_shared<SyntaxNode>(NodeType::CONCAT, context, node, right);
		}
	}
	return node;
}

std::shared_ptr<SyntaxNode> RegexParser::parse_atom(Context& context){
	std::shared_ptr<SyntaxNode> node;
	if (consume_if_match('(')) {
        ++paren_balance;
        // пустые скобки ()
        if (peek() == ')') {
            ++pos;            // пропускаем ')'
            --paren_balance;  // баланс вернулся
            node = std::make_shared<SyntaxNode>(NodeType::EPSYLON, context);
        }
        else {
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
        node = std::make_shared<SyntaxNode>(NodeType::CHAR, context, c);
    }
	while (true) {
        if (consume_if_match('*')) {
            node = std::make_shared<SyntaxNode>(NodeType::KLEENE, context, node);
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

std::shared_ptr<SyntaxNode> RegexParser::transform_range(unsigned lower, unsigned upper, std::shared_ptr<SyntaxNode> base, Context& context){
	std::shared_ptr<SyntaxNode> node = nullptr;
	for (int i=0; i<lower; ++i){
		if (i==0){
			node = base;
		}
		else{
			node = std::make_shared<SyntaxNode>(NodeType::CONCAT, context, node, clone(base, context));
		}
	}
	if (upper == INF){
		if (node == nullptr){
			node = std::make_shared<SyntaxNode>(NodeType::KLEENE, context, base);
		}
		else{
			node = std::make_shared<SyntaxNode>(
				NodeType::CONCAT,
				context,
				node, 
				std::make_shared<SyntaxNode>(NodeType::KLEENE,context, clone(base, context))
			);
		}
	}
	else{
		for (int i=lower; i<upper; ++i){
			if (node == nullptr){
				node = std::make_shared<SyntaxNode>(
					NodeType::OR, 
					context,
					base,
					std::make_shared<SyntaxNode>(NodeType::EPSYLON, context));
			}
			else{
				node = std::make_shared<SyntaxNode>(
					NodeType::CONCAT,
					context,
					node,
					std::make_shared<SyntaxNode>(
						NodeType::OR,
						context,
						clone(base, context),
						std::make_shared<SyntaxNode>(NodeType::EPSYLON, context)));
			}
		}
	}
	return node;
}

std::shared_ptr<SyntaxNode> RegexParser::clone(const std::shared_ptr<SyntaxNode>& node, Context& context){
	std::shared_ptr<SyntaxNode> new_left, new_right, new_node;
	if (node->type == NodeType::CONCAT or node->type == NodeType::OR){
		new_left = clone(node->left, context);
		new_right = clone(node->right, context);
		new_node = std::make_shared<SyntaxNode>(node->type, context, new_left, new_right);
	}
	else if (node->type == NodeType::KLEENE){
		new_left = clone(node->left, context);
		new_node = std::make_shared<SyntaxNode>(node->type, context, new_left);
	}
	else{
		new_node = std::make_shared<SyntaxNode>(node->type, context, node->value);
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
