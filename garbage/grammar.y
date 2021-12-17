%{
    #include <stdio.h>
    #include "ast.h"

    int yylex();
    void yyerror( const char *msg );
%}
%union {
    int          ival;
    char         cval;
    char        *sval;
    struct Node *node;
}
%left '+' '-' '*' '/'
%token  <ival>  NUM
%type   <node>  statement_list statement expr_statement
%type   <node>  expr add_expr mul_expr unary_expr primary_expr
%%
statement_list
    : statement
    | statement_list statement
    ;

statement
    : expr_statement
    ;

expr_statement
    : ';'
    | expr ';'                  { generateCode($1); }
    ;

expr
    : add_expr                  { $$ = $1; }
    ;

add_expr
    : add_expr '+' mul_expr     { $$ = makeBiOpNode( '+', $1, $3 ); }
    | add_expr '-' mul_expr     { $$ = makeBiOpNode( '-', $1, $3 ); }
    | mul_expr                  { $$ = $1; }
    ;

mul_expr
    : mul_expr '*' unary_expr   { $$ = makeBiOpNode( '*', $1, $3 ); }
    | mul_expr '/' unary_expr   { $$ = makeBiOpNode( '/', $1, $3 ); }
    | unary_expr                { $$ = $1; }
    ;

unary_expr
    : '+' primary_expr          { $$ = makeUnaryOpNode( '+', $2 ); }
    | '-' primary_expr          { $$ = makeUnaryOpNode( '-', $2 ); }
    | primary_expr              { $$ = $1; }
    ;

primary_expr
    : NUM                       { $$ = makeConstNumNode( $1 ); }
    | '(' expr ')'              { $$ = $2; }
    ;
%%

int main()
{
    yyparse();
    return 0;
}

void yyerror(const char *msg)
{
    fputs(msg, stderr);
}