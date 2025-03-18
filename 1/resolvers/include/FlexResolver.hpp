#pragma once

#include "IResolver.hpp"
#include <FlexLexer.h>

class FlexResolver : public IResolver{
public:
    bool is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens) override;
private:
    yyFlexLexer lexer;
};