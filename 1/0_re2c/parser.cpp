// re2c $INPUT -o $OUTPUT -i --case-ranges
#include <assert.h>

int lex(const char *s) {
    const char *YYCURSOR = s, *YYMARKER;
    /*!re2c
        name_pt = [a-zA-Z][a-zA-Z0-9]{0,15};
        re2c:yyfill:enable = 0;
        re2c:define:YYCTYPE = char;

        name_pt"[" { return 0; }
        *           { return 1; }
    */
}

int main() {
    assert(lex("aAAbAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaaaa1c[") == 0);
    return 0;
}

// name_pt = [a-zA-Z][a-zA-Z0-9]{0,15};
// amount_pt = [0-9]{0,9};
// lexem = "-"?[0-9]+;