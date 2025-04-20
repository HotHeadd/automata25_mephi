#include <gtest/gtest.h>
#include "parser.hpp"
#include "errors.hpp"

using namespace myre;

TEST(parse, parentheses){
	Context context; RegexParser parser;
	ASSERT_THROW(parser.parse("(aga))", context), ParenthesesError);
	ASSERT_THROW(parser.parse("((abc|d)", context), ParenthesesError);

	ASSERT_NO_THROW(parser.parse("(aga|(cd))", context));
}

TEST(parse, range_errors){
	Context context; RegexParser parser;
	ASSERT_THROW(parser.parse("a{3,2}", context), RangeError);
	ASSERT_THROW(parser.parse("a{bc,3}", context), SyntaxError);
	ASSERT_THROW(parser.parse("a{2,3", context), ParenthesesError);

	ASSERT_NO_THROW(parser.parse("a2,3}", context));
	ASSERT_NO_THROW(parser.parse("a{2,3}", context));
	ASSERT_NO_THROW(parser.parse("a{3,}", context));
	ASSERT_NO_THROW(parser.parse("a{2}", context));
}

TEST(parse, shielding){
	Context context; RegexParser parser;
	ASSERT_THROW(parser.parse("ab#", context), SyntaxError);
}

TEST(parse, range_no_brackets){
	Context context; RegexParser parser;
	std::string expr = "a{2,3}";

	ASSERT_NO_THROW(parser.parse(expr, context));
}

TEST(parse, range_single_value){
	Context context; RegexParser parser;
	std::string expr = "ab{2}";

	ASSERT_NO_THROW(parser.parse(expr, context));
}

TEST(parse, range_with_brackets){
	Context context; RegexParser parser;
	std::string expr = "(ab){2,3}";

	ASSERT_NO_THROW(parser.parse(expr, context));
}

TEST(parse, range_kleenestar){
	Context context; RegexParser parser;
	std::string expr1 = "a{2,}";
	std::string expr2 = "a{,2}";

	ASSERT_NO_THROW(parser.parse(expr1, context));
	ASSERT_NO_THROW(parser.parse(expr2, context));
}