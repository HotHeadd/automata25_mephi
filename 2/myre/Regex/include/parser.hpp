#pragma once

#include <memory>
#include <list>

namespace myre
{

enum class TokenType{
	LPAR,
	RPAR,
	KLEENE,
	OR,
	CONCAT,
	CHAR,
	EPSYLON,
	NONE
};

enum class NodeType{
	KLEENE,
	OR,
	CONCAT,
	CHAR,
	EOS,
	EPSYLON
};

struct SyntaxNode {
	NodeType type;
	char value='\0';
	unsigned start, finish;
	std::shared_ptr<SyntaxNode> left=nullptr, right=nullptr;

	SyntaxNode(NodeType type_i, std::shared_ptr<SyntaxNode> left_kid = nullptr, std::shared_ptr<SyntaxNode> right_kid = nullptr)
	: type(type_i), left(left_kid), right(right_kid)  {}
	SyntaxNode(NodeType type_i, char value_i)
		: type(type_i), value(value_i){}
	SyntaxNode(NodeType type_i, unsigned value_left, unsigned value_right, 
				std::shared_ptr<SyntaxNode> left_kid = nullptr, std::shared_ptr<SyntaxNode> right_kid = nullptr) 
		: type(type_i), start(value_left), finish(value_right), left(left_kid), right(right_kid) {}
};

struct Token{
	TokenType type;
	char value='\0';
	unsigned start, finish;

	Token(TokenType type_i, char value_i='\0')
		: type(type_i), value(value_i) {}
	Token(TokenType type_i, unsigned value_left, unsigned value_right) 
		: type(type_i), start(value_left), finish(value_right) {}

	std::shared_ptr<SyntaxNode> to_node();
};


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
