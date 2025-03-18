%option c++
%option noyywrap

NAME [a-zA-Z][a-zA-Z0-9]{0,15}
DIG [0-9]
INIT_LIST (-?[0-9]+(,-?[0-9]+)*)?
%%()
{NAME}\[{DIG}{0,9}\]=\{{INIT_LIST}\} {
    std::cout << yytext << std::endl;
    return true;
}
.* {
    std::cout << yytext << std::endl;
    return false;
}
%%