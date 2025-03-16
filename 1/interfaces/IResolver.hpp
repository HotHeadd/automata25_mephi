#include <string_view>
#include <unordered_map>


namespace resolvers
{

class IResolver{
public:
    virtual bool is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& token_vector) = 0;
private:
};

} // namespace resolvers


