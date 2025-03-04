#pragma once

#include "IResolver.hpp"
#include <regex>

namespace resolvers
{

class RegexResolver : public IResolver{
public:
    void process_file(const std::string& in_file, const std::string& out_file) override;
    bool is_suitable(const std::string&) override;
private:
};
} // namespace resolvers


