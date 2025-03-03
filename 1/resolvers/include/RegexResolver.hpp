#include "IResolver.hpp"
#include <regex>

namespace resolvers
{

class RegexResolver : public IResolver{
public:
    void process_file(std::string_view in_file, std::string_view out_file) override;
    bool is_suitable(const std::string&) override;
private:
};
} // namespace resolvers


