%{
#include "parser.tab.h"
#include <iostream>
%}

%option noyywrap
%option noinput nounput

%%
[ \t\n]                 ; /* Skip whitespace */
"SELECT"|"select"|"sel"                { return SELECT; }
"FROM"|"from"                  { return FROM; }
"WHERE"|"where"                 { return WHERE; }
"INSERT"|"insert"               { return INSERT; }
"INTO"|"into"                  { return INTO; }
"VALUES"|"values"                { return VALUES; }
"*"                     { return STAR; }
","                     { return COMMA; }
"="                     { return EQUALS; }
";"                     { return SEMICOLON; }
\'[a-zA-Z0-9 ]*\'       { return STRING; }
[0-9]+                  { return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { return IDENTIFIER; }
.                       { std::cerr << "Unknown: " << yytext << std::endl; }
%%
