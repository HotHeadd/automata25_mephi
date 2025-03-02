#include <string_view>

namespace resolvers
{

class IResolver{
public:
    virtual void process_file(std::string_view in_file, std::string_view out_file) = 0;
    virtual bool is_suitable(const std::string&) = 0;
private:
};

} // namespace resolvers


