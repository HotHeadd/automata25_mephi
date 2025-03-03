#include <string_view>

namespace resolvers
{

class IResolver{
public:
    virtual void process_file(const std::string& in_file, const std::string& out_file) = 0;
    virtual bool is_suitable(const std::string&) = 0;
private:
};

} // namespace resolvers


