#pragma once

#include "IResolver.hpp"
#include <regex>

class RegexResolver : public IResolver{
public:
    bool is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens) override;
private:
};


