#include <gtest/gtest.h>
#include "parser.hpp"

using namespace myre;

TEST(parse, parentheses){
	RegexParser parser;
	ASSERT_THROW(parser.parse("(aga))"), ParenthesesError);
	ASSERT_THROW(parser.parse("((abc|d)"), ParenthesesError);

	ASSERT_NO_THROW(parser.parse("(aga|(cd))"));
}

TEST(parse, range_errors){
	RegexParser parser;
	ASSERT_THROW(parser.parse("a{3,2}"), RangeError);
	ASSERT_THROW(parser.parse("a{bc,3}"), SyntaxError);
	ASSERT_THROW(parser.parse("a{2,3"), ParenthesesError);

	ASSERT_NO_THROW(parser.parse("a2,3}"));
	ASSERT_NO_THROW(parser.parse("a{2,3}"));
	ASSERT_NO_THROW(parser.parse("a{3,}"));
	ASSERT_NO_THROW(parser.parse("a{2}"));
}

TEST(parse, shielding){
	RegexParser parser;
	ASSERT_THROW(parser.parse("ab#"), SyntaxError);
}

TEST(parse, range_no_brackets){
	RegexParser parser;
	std::string expr = "a{2,3}";

	ASSERT_NO_THROW(parser.parse(expr));
}

TEST(parse, range_single_value){
	RegexParser parser;
	std::string expr = "ab{2}";

	ASSERT_NO_THROW(parser.parse(expr));
}

TEST(parse, range_with_brackets){
	RegexParser parser;
	std::string expr = "(ab){2,3}";

	ASSERT_NO_THROW(parser.parse(expr));
}

TEST(parse, range_kleenestar){
	RegexParser parser;
	std::string expr1 = "a{2,}";
	std::string expr2 = "a{,2}";

	ASSERT_NO_THROW(parser.parse(expr1));
	ASSERT_NO_THROW(parser.parse(expr2));
}