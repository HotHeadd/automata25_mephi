#pragma once

#include "IResolver.hpp"

class FlexResolver : public IResolver{
public:
    bool is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens) override;
private:
};