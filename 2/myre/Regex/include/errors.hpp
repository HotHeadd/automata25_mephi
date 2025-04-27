#pragma once

#include <string>
#include <stdexcept>

namespace myre
{

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
