#include "RegexResolver.hpp"

namespace resolvers
{

const std::regex RegexResolver::arr_pattern = std::regex{"abcde", std::regex_constants::icase}; 

bool RegexResolver::is_suitable(const std::string& str){
    std::smatch matches;
    return std::regex_match(str, arr_pattern);
}


void RegexResolver::process_file(std::string_view in_file, std::string_view out_file){

}

} // namespace resolvers
