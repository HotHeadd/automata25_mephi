/* Generated by re2c 3.0 on Sat Mar 22 13:13:19 2025 */
#include "generated.hpp"
#include <algorithm>

int lex(const char *s, std::string& name, std::string& amount) {
    const char *YYCURSOR = s, *YYMARKER;
    const char* name_end, *amount_st, *amount_end, *lexems_start, *lexems_end;
    unsigned amount_val;
    /*!svars:re2c format = 'const char *@@;\n'; */
    const char *yyt1;
const char *yyt2;
const char *yyt3;
const char *yyt4;


    
{
	char yych;
	yych = *YYCURSOR;
	switch (yych) {
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy3;
		default: goto yy1;
	}
yy1:
	++YYCURSOR;
yy2:
	{ return 0; }
yy3:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy4;
		case '[': goto yy6;
		default: goto yy2;
	}
yy4:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy7;
		case '[': goto yy6;
		default: goto yy5;
	}
yy5:
	YYCURSOR = YYMARKER;
	goto yy2;
yy6:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
			yyt1 = YYCURSOR;
			goto yy8;
		case ']':
			yyt1 = yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy7:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy10;
		case '[': goto yy6;
		default: goto yy5;
	}
yy8:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy11;
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy9:
	yych = *++YYCURSOR;
	switch (yych) {
		case '=': goto yy12;
		default: goto yy5;
	}
yy10:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy13;
		case '[': goto yy6;
		default: goto yy5;
	}
yy11:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy14;
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy12:
	yych = *++YYCURSOR;
	switch (yych) {
		case '{': goto yy15;
		default: goto yy5;
	}
yy13:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy16;
		case '[': goto yy6;
		default: goto yy5;
	}
yy14:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy17;
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy15:
	yych = *++YYCURSOR;
	switch (yych) {
		case '-':
			yyt4 = YYCURSOR;
			goto yy18;
		case '0' ... '9':
			yyt4 = YYCURSOR;
			goto yy19;
		case '}':
			yyt3 = yyt4 = NULL;
			goto yy20;
		default: goto yy5;
	}
yy16:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy21;
		case '[': goto yy6;
		default: goto yy5;
	}
yy17:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy22;
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy18:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy19;
		default: goto yy5;
	}
yy19:
	yych = *++YYCURSOR;
	switch (yych) {
		case ',': goto yy23;
		case '0' ... '9': goto yy19;
		case '}':
			yyt3 = YYCURSOR;
			goto yy20;
		default: goto yy5;
	}
yy20:
	yych = *++YYCURSOR;
	if (yych <= 0x00) goto yy24;
	goto yy5;
yy21:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy25;
		case '[': goto yy6;
		default: goto yy5;
	}
yy22:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy26;
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy23:
	yych = *++YYCURSOR;
	switch (yych) {
		case '-': goto yy18;
		case '0' ... '9': goto yy19;
		default: goto yy5;
	}
yy24:
	++YYCURSOR;
	amount_st = yyt1;
	amount_end = yyt2;
	lexems_start = yyt4;
	lexems_end = yyt3;
	name_end = yyt1 - 1;
	{ 
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
yy25:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy27;
		case '[': goto yy6;
		default: goto yy5;
	}
yy26:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy28;
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy27:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy29;
		case '[': goto yy6;
		default: goto yy5;
	}
yy28:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy30;
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy29:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy31;
		case '[': goto yy6;
		default: goto yy5;
	}
yy30:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9': goto yy32;
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy31:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy33;
		case '[': goto yy6;
		default: goto yy5;
	}
yy32:
	yych = *++YYCURSOR;
	switch (yych) {
		case ']':
			yyt2 = YYCURSOR;
			goto yy9;
		default: goto yy5;
	}
yy33:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy34;
		case '[': goto yy6;
		default: goto yy5;
	}
yy34:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy35;
		case '[': goto yy6;
		default: goto yy5;
	}
yy35:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy36;
		case '[': goto yy6;
		default: goto yy5;
	}
yy36:
	yych = *++YYCURSOR;
	switch (yych) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z': goto yy37;
		case '[': goto yy6;
		default: goto yy5;
	}
yy37:
	yych = *++YYCURSOR;
	switch (yych) {
		case '[': goto yy6;
		default: goto yy5;
	}
}

}
