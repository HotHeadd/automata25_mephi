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
    // constexpr нельзя, потому что не regex вычисляетя во время компиляции
    inline static const std::regex arr_pattern{"[a-z]\w{0, 15}\[((0?)\]=\{(-[1-9]\d*|[1-9]\d*)(,-[1-9]\d*|[1-9]\d*)*\})|(()\]=\{(-[1-9]\d*|[1-9]\d*)(,-[1-9]\d*|[1-9]\d*)*\})", 
                                               std::regex_constants::icase};
};

} // namespace resolvers


