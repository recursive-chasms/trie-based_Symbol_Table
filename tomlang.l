%{

typedef struct
{
	int ival;
	int type;
	char str[512];
}langstruct;

#define YYSTYPE langstruct
#include <string.h>
#include "tomlang.tab.h"
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

%}

%option yylineno

DIGIT  [0-9]
LETTER [A-Za-z]
WHITE  [ |\t|\f|\r|\v]

%%

vari	{ return VARI; }
vars	{ return VARS; }

set		{ return SET; }
to 		{ return TO; }

\"([^"]*)\" 	{ strcpy(yylval.str, yytext); return STR_LIT; }

printi 	{ return PRINTI; }
printsh	{ return PRINTSH; }
printsv { return PRINTSV; }

\/{WHITE}*\n

\/\/.*\n

\/\*.*\*\/

{WHITE}

{DIGIT}+ { 
			strcpy(yylval.str, yytext);
			return NUMBER; 
		}
		
{LETTER}({LETTER}|{DIGIT}|_)* { strcpy(yylval.str, yytext); return VAR_NAME; } 

\n  { return yytext[0]; }
.	{ return yytext[0]; }

%%


void yyerror(char *s)  /* Called by yyparse on error */
{
  printf ("\terror: %s\n", s);
  printf ("ERROR: %s at line %d\n", s, yylineno);
}
