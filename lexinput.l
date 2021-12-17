%{
    #include <stdlib.h>
    #include "y.tab.h"

    void comment( void );
%}
alpha   [a-zA-Z]
digit   [0-9]
letter  [_a-zA-Z][_a-zA-Z0-9]*
%%
"func"							{ return FUNC; }
"var"							{ return VAR; }
"return"                        { return RETURN; }
"+"                             { return '+'; }
"-"                             { return '-'; }
"*"                             { return '*'; }
"/"                             { return '/'; }
"="                             { return '='; }
"("                             { return '('; }
")"                             { return ')'; }
"{"                             { return '{'; }
"}"                             { return '}'; }
";"                             { return ';'; }
","                             { return ','; }
{digit}+                        { yylval.ival = atoi(yytext); return NUM; }
{letter}                        { yylval.sval = new std::string(yytext); return ID; }
[ \t\n]                         { }
.                               { }
%%
int yywrap() 
{
    return 1;
}