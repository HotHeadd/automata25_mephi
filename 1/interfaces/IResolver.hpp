#pragma once

#include <string_view>
#include <unordered_map>

class IResolver{
public:
    virtual bool is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens) = 0;
private:
};


