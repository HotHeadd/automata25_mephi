#include "generated.hpp"
#include <algorithm>

int lex(const char *s, std::string& name, std::string& amount) {
    const char *YYCURSOR = s, *YYMARKER;
    const char* name_end, *amount_st, *amount_end, *lexems_start, *lexems_end;
    unsigned amount_val;
    /*!svars:re2c format = 'const char *@@;\n'; */
    /*!stags:re2c format = 'const char *@@;\n'; */

    /*!re2c
        re2c:yyfill:enable = 0;
        re2c:define:YYCTYPE = char;
        re2c:flags:tags = 1;

        name_pt = [a-zA-Z][a-zA-Z0-9]{0,15};
        amount_pt = [0-9]{0,9};
        lexem = "-"?[0-9]+;
        lexem_list = @lexems_start lexem ("," lexem)* @lexems_end;
        name_amount = name_pt @name_end "[" @amount_st amount_pt @amount_end "]"; 
        init_list = "={" lexem_list? "}";

        fullmatch = name_amount init_list;

        fullmatch [\x00] { 
            name = std::string(s, name_end);
            amount = std::string(amount_st, amount_end);
            if (amount == "0" or amount == ""){
                amount_val = -1;
                if (lexems_start == lexems_end){
                    return 0;
                }
            }
            else{
                amount_val = std::stoi(amount);
            }
            if (amount_val < std::count(lexems_start, lexems_end, ',')+1){
                return 0;
            }
            return 1;
        }
        * { return 0; }
    */
}
