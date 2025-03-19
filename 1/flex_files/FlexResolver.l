%option c++
%option noyywrap

NAME [a-zA-Z][a-zA-Z0-9]{0,15}
DIG [0-9]
INIT_LIST (-?[0-9]+(,-?[0-9]+)*)?

%x AMOUNT LEXEMS

%%
<INITIAL>{
{NAME}\[ {
    BEGIN(AMOUNT);
    return 1;
}
. ;
<<EOF>> return 0;
}

<AMOUNT>{
    {DIG}{0,9} {
        BEGIN(LEXEMS);
        return 1;
    }
    . ;
    <<EOF>> {
        BEGIN(INITIAL);
        return 0;
    }
}

<LEXEMS>{
    \]=\{{INIT_LIST}\} {
        BEGIN(INITIAL);
        return 1;
    }
}
. ;
<<EOF>> {
    BEGIN(INITIAL);
    return 0;
}
%%