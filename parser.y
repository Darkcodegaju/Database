%{
#include <iostream>
#include <string>

// External functions from Flex and custom error handler
extern int yylex();
void yyerror(const char *s);
%}

%token SELECT FROM WHERE INSERT INTO VALUES STAR COMMA EQUALS SEMICOLON IDENTIFIER NUMBER STRING

%%
program:
    statements
    ;

statements:
    statement SEMICOLON

    | statements statement SEMICOLON
    ;

statement:
    select_stmt { std::cout << "Valid SELECT query!" << std::endl; }
    | insert_stmt { std::cout << "Valid INSERT query!" << std::endl; }
    ;

select_stmt:
    SELECT columns FROM IDENTIFIER where_clause
    ;

columns:
    STAR

    | id_list
    ;

id_list:
    IDENTIFIER
    | id_list COMMA IDENTIFIER
    ;

where_clause:
    /* empty */

    | WHERE IDENTIFIER EQUALS value
    ;

insert_stmt:
    INSERT INTO IDENTIFIER VALUES '(' value_list ')'
    ;

value_list:
    value
    | value_list COMMA value
    ;

value:
    NUMBER

    | STRING
    | IDENTIFIER
    ;

%%
void yyerror(const char *s) {
    std::cerr << "SQL Syntax Error: " << s << std::endl;
}

