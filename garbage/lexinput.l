%{
    #include <stdlib.h>
    #include "y.tab.h"

    void comment( void );
%}
alpha   [a-zA-Z]
digit   [0-9]
letter  {alpha|_}
%%
"/*"                            { comment(); }

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
{digit}+                        { yylval.ival = atoi(yytext); return NUM; }
.                               { }
%%
int yywrap() 
{
    return 1;
}

void comment( void )
{
	char c, c1;

loop:
	while ((c = input()) != '*' && c != 0)
		putchar(c);

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0)
		putchar(c1);
}