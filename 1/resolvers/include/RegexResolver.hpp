#include "IResolver.hpp"
#include <regex>

namespace resolvers
{

class RegexResolver : public IResolver{
public:
    void process_file(std::string_view in_file, std::string_view out_file) override;
    bool is_suitable(const std::string&) override;
private:
    // с обычными скобочками будет most vexing parce (парсится как функция)
    // инлайн для инициализации прямо тут, а не в cpp
    // constexpr нельзя, потому что не regex вычисляетя во время компиляции
    inline static const std::regex arr_pattern{"abcde", std::regex_constants::icase};
};

} // namespace resolvers


