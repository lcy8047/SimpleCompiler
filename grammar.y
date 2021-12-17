%{
    #include <cstdio>
    #include <iostream>
    #include <fstream>
    #include "ast.hpp"

    int yylex();
    void yyerror( const char *msg );

    Node *root;
%}
%code requires{
    #include <string>
    #include "ast.hpp"
}
%union {
    int          ival;
    char         cval;
    std::string *sval;
    Node *node;
}
%left '+' '-' '*' '/'
%token          VAR FUNC RETURN
%token  <sval>  ID
%token  <ival>  NUM
%type   <node>  root code function_definition function_declaration
  compound_statement
                statement_list statement init_statement expr_statement return_statement
  expr assn_expr add_expr mul_expr unary_expr primary_expr var_id arguments argument_expr_list paramid_list

%start          root
%%
root
    : code                                              { root = new Node( eType::ROOT, $1, NULL ); }
    ;

code
    : function_definition code                          { appendBro( $1, $2 );
                                                          $$ = $1; }
    | function_definition                               { $$ = $1; }
    ;

function_definition
    : function_declaration compound_statement           { appendSon( $1, $2 ); 
                                                          $$ = $1; }
    ;

function_declaration
    : FUNC ID '(' paramid_list ')'                      { $$ = new FuncNode( *$2, $4 ); }
    | FUNC ID '(' ')'                                   { $$ = new FuncNode( *$2, 0 ); }
    ;

compound_statement
    : '{' statement_list '}'                            { $$ = $2; }
    | '{' '}'                                           { ; }
    ;

statement_list
    : statement statement_list                          { appendBro( $1, $2 );
                                                          $$ = $1; }
    | statement                                         { $$ = $1; }
    ;

statement
    : init_statement                        { $$ = $1; }
    | expr_statement                        { $$ = $1; }
    | return_statement                      { $$ = $1; }

paramid_list
    : ID                                    { $$ = new VarNode( *$1 ); }
    | paramid_list ',' ID                   { appendBro( $1, new VarNode( *$3 ) );
                                              $$ = $1; }
    ;

init_statement
    : VAR var_id ';'                        { $$ = $2; }
    ;

expr_statement
    : ';'                                   { ; }
    | expr ';'                              { $$ = $1; }
    ;

return_statement
    : RETURN ';'                            { $$ = new ReturnNode( false ); }
    | RETURN expr ';'                       { $$ = new ReturnNode( true );
                                              appendSon( $$, $2 ); }
    ;

expr
    : assn_expr                             { $$ = $1; }
    ;

assn_expr
    : var_id '=' assn_expr                  { $$ = new BiOpNode(eType::ASSN, $1, $3); }
    | add_expr                              { $$ = $1; }
    ;

add_expr                    
    : add_expr '+' mul_expr                 { $$ = new BiOpNode(eType::ADD, $1, $3); }
    | add_expr '-' mul_expr                 { $$ = new BiOpNode(eType::SUB, $1, $3); }
    | mul_expr                              { $$ = $1; }
    ;

mul_expr
    : mul_expr '*' unary_expr               { $$ = new BiOpNode(eType::MUL, $1, $3); }
    | mul_expr '/' unary_expr               { $$ = new BiOpNode(eType::DIV, $1, $3); }
    | unary_expr                            { $$ = $1; }
    ;                   

unary_expr                  
    : '+' primary_expr                      { $$ = $2; }
    | '-' primary_expr                      { $$ = new UnaryOpNode(eType::NEG, $2); }
    | primary_expr                          { $$ = $1; }
    ;                   

primary_expr                    
    : NUM                                   { $$ = new IntNode( $1 ); }
    | var_id                                { $$ = $1; }
    | '(' expr ')'                          { $$ = $2; }
    | ID '(' arguments ')'                  { $$ = new FunccallNode( *$1, $3 ); }
    ;

var_id
    : ID                                    { $$ = new VarNode( *$1 ); }
    ;

arguments
    : %empty
    | argument_expr_list                    { $$ = $1; }
    ;

argument_expr_list
    : assn_expr                             { $$ = $1; }
    | argument_expr_list ',' assn_expr      { appendBro( $1, $3 );
                                              $$ = $1; }
    ;
%%

int main(int argc, char *argv[])
{   
    ofstream ofs("a.t");
    yyparse();
    ofs << generateCode( root, NULL );

    return 0;
}

void yyerror(const char *msg)
{
    cerr << msg << endl;
}