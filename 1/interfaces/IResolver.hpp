#include <string_view>

class IResolver{
public:
    virtual void process_file(std::string_view in_file, std::string_view out_file) = 0;
private:
    virtual bool is_suitable(std::string_view str) = 0;
};