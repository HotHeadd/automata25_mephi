#pragma once

#include "IResolver.hpp"
#include <regex>


namespace resolvers
{

class RegexResolver : public IResolver{
public:
    bool is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& token_vector) override;
private:
};
} // namespace resolvers


