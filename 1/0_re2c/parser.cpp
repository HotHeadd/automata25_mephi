// re2c $INPUT -o $OUTPUT -i --case-ranges
#include <assert.h>

int lex(const char *s) {
    const char *YYCURSOR = s, *YYMARKER;
    /*!re2c
        re2c:yyfill:enable = 0;
        re2c:define:YYCTYPE = char;
        re2c:flags:tags = 1;

        re2c:define:YYSTAGP = "@@ = YYCURSOR;";
        re2c:define:YYSTAGN = "@@ = NULL;";
        re2c:define:YYMTAGP = "add_mtag(mt, @@, YYCURSOR);";
        re2c:define:YYMTAGN = "add_mtag(mt, @@, NULL);";
        re2c:yyfill:enable = 0;
        re2c:tags = 1;
        name_pt = [a-zA-Z][a-zA-Z0-9]{0,15};
        amount_pt = [0-9]{0,9};
        name_amount = name_pt #name_end "[" #amount_st amount_pt #amount_end "]"; 

        name_amount { return 0; }
        *           { return 1; }
    */
}

int main() {
    assert(lex("aaaaaaaaaaaaaa[1]") == 0);
    return 0;
}

// name_pt = [a-zA-Z][a-zA-Z0-9]{0,15};
// amount_pt = [0-9]{0,9};
// lexem = "-"?[0-9]+;