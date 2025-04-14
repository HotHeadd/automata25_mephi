#pragma once

#include <list>
#include "nodes.hpp"

namespace myre
{


class RegexParser {
public:
	std::shared_ptr<SyntaxNode> parse(const std::string& regex); // возвращает корень дерева разбора
	std::list<std::shared_ptr<Token>> tokenize(const std::string& regex);
private:
	std::string regex_;
	const unsigned INF = -1;
	std::list<std::shared_ptr<Token>> tokens;
	std::pair<unsigned, unsigned> parse_range(const std::string& regex, int& i, std::list<std::shared_ptr<Token>>& tokens);
	std::list<std::shared_ptr<Token>> transform_range(unsigned lower, unsigned upper, std::list<std::shared_ptr<Token>>& tokens, TokenType& prev_type);

	std::shared_ptr<SyntaxNode> parse_expression();
	std::shared_ptr<SyntaxNode> parse_term();
	std::shared_ptr<SyntaxNode> parse_atom();
	std::shared_ptr<Token> consume();
};


class RegexError : public std::runtime_error {
public:
	explicit RegexError(const std::string& message) : std::runtime_error(message) {}
};

class SyntaxError : public RegexError {
public:
	explicit SyntaxError(const std::string& regex) : 
		RegexError("Syntax error in pattern: \"" + regex + "\"") {}
};

class RangeError : public RegexError {
public:
	explicit RangeError(int min, int max) :
		RegexError(
			"Range error: {" + std::to_string(min) + ", " + std::to_string(max)  + "}"
		) {}
};

class ParenthesesError : public RegexError {
public:
	explicit ParenthesesError(const std::string& regex) : 
		RegexError("Unbalanced parenthesis in pattern: \"" + regex + "\"") {}
};

} // namespace myre
