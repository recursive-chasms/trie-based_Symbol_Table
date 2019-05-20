%{

//Peter Carle
//May 2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
//#include "symtab.c"
#include "include/p_trie.h"
#include "include/defs.h"

typedef struct
{
	int ival;
	int type;
	char str[STR_SIZE];
}langstruct;

#define YYSTYPE langstruct

int yylex();
void yyerror(char *s);

%}

%token VARI VARS
%token SET TO 
%token STR_LIT
%token VAR_NAME
%token NUMBER
%token PRINTI PRINTSH PRINTSV

%define locations

%%

program		:
				state_list
							{
								printf("//Peter Carle\n//Spring 2019\n\n#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n\nint main()\n{\nint index = 0;\n%s return 0;\n}\n", $1.str);
							}
			; 

state_list	:
				state_list statement '\n' newline
								{
									strncpy($$.str, $1.str, STR_SIZE);
									strncat($$.str, $2.str, STR_SIZE);
								}			
			|				
				statement '\n' newline
								{ strncpy($$.str, $1.str, STR_SIZE); }				
			|			
				state_list statement ';' newline
								{
									strncpy($$.str, $1.str, STR_SIZE);
									strncat($$.str, $2.str, STR_SIZE);
								}				
			|			
				statement ';' newline
								{ strncpy($$.str, $1.str, STR_SIZE); }				
			|
				state_list declaration '\n' newline
								{
									strncpy($$.str, $1.str, STR_SIZE);
									strncat($$.str, $2.str, STR_SIZE);
								}				
			|
				declaration '\n' newline
								{ strncpy($$.str, $1.str, STR_SIZE); }				
			|
				state_list declaration ';' newline
								{
									strncpy($$.str, $1.str, STR_SIZE);
									strncat($$.str, $2.str, STR_SIZE);
								}
			|
				declaration ';' newline
								{ strncpy($$.str, $1.str, STR_SIZE); }
			;
			
declaration	:
				i_declaration
								{ strncpy($$.str, $1.str, STR_SIZE); }
			|			
				s_declaration
								{ strncpy($$.str, $1.str, STR_SIZE); }
			;

i_declaration :
				i_declaration VAR_NAME
										{
											if(Get_Type($2.str))
												printf("ERROR: %s already declared.\n", $2.str);
											else
											{
												Add_Symbol($2.str, INT);
												snprintf($$.str, STR_SIZE, "int %s;\n", $2.str);
											}
										}				
			|			
				VARI VAR_NAME
										{
											if(Get_Type($2.str))
												printf("ERROR: %s already declared.\n", $2.str);
											else
											{
												Add_Symbol($2.str, INT);
												snprintf($$.str, STR_SIZE, "int %s;\n", $2.str);
											}
										}				
			;
			
s_declaration :
				s_declaration VAR_NAME
										{
											if(Get_Type($2.str))
												printf("ERROR: %s already declared.\n", $2.str);
											else
											{
												Add_Symbol($2.str, CHAR);
												snprintf($$.str, STR_SIZE, "char %s[25];\n", $2.str);
											}
										}				
			|			
				VARS VAR_NAME
										{
											if(Get_Type($2.str))
												printf("ERROR: %s already declared.\n", $2.str);
											else
											{
												Add_Symbol($2.str, CHAR);
												snprintf($$.str, STR_SIZE, "char %s[25];\n", $2.str);
											}
										}
			;


statement	:
				assignment
							{ strncpy($$.str, $1.str, STR_SIZE); }
			|			
				print
							{ strncpy($$.str, $1.str, STR_SIZE); }
			;
			
assignment	:
				dec_assign
							{ strncpy($$.str, $1.str, STR_SIZE); }
			|		
				assign
							{ strncpy($$.str, $1.str, STR_SIZE); }
			;
			
			
dec_assign	:
				VARS VAR_NAME '=' expression 
										{
											if(Get_Type($2.str))
												printf("ERROR: %s already declared.\n", $2.str);
											else if($4.type == CHAR)
											{
												Add_Symbol($2.str, CHAR);
												$$.type = CHAR;
												snprintf($$.str, STR_SIZE, "char * %s = %s;\n", $2.str, $4.str);
											}
											else
												printf("ERROR: Inline declaration failed. Type mismatch.\n");
										}
			|			
				VARI VAR_NAME '=' expression
										{
											if(Get_Type($2.str))
												printf("ERROR: %s already declared.\n", $2.str);
											else if($4.type == INT)
											{
												Add_Symbol($2.str, INT);
												snprintf($$.str, STR_SIZE, "int %s;\n%s = %s;\n", $2.str, $2.str, $4.str);
												$$.type = INT;
											}
											else
												printf("ERROR: Inline declaration failed. Type mismatch.\n");
										}				
			;

assign 		:
				VAR_NAME '=' expression
									{
										if(Get_Type($1.str))
										{
											if(Get_Type($1.str) == INT && $3.type == INT)
											{
												snprintf($$.str, STR_SIZE, "%s = %s;\n", $1.str, $3.str);
												$$.type = $3.type;
											}
											else if(Get_Type($1.str) == CHAR && $3.type == CHAR)
												snprintf($$.str, STR_SIZE, "strncpy(%s, %s, 25);\n", $1.str, $3.str);
											else
												printf("ASSIGNMENT ERROR: var1: %i var2: %i\n", $1.type, $3.type);
										}
										else
											printf("ERROR: %s NOT DECLARED.\n", $1.str);
									}
			|
				SET VAR_NAME TO expression
									{
										if(Get_Type($2.str))
										{
											if(Get_Type($2.str) == INT && $4.type == INT)
											{
												snprintf($$.str, STR_SIZE, "%s = %s;\n", $2.str, $4.str);
												$$.type = $4.type;
											}
											else if(Get_Type($2.str) == CHAR && $4.type == CHAR)
												snprintf($$.str, STR_SIZE, "strncpy(%s, %s, 25);\n", $1.str, $3.str);
											else
												printf("ASSIGNMENT ERROR: var1: %i var2: %i\n", $2.type, $4.type);
										}	
										else
											printf("ERROR: %s NOT DECLARED.\n", $1.str);		
									}
			;
			
expression	:
				expression '+' term
									{ 
										if($1.type == INT && $3.type == INT)
										{
											snprintf($$.str, STR_SIZE, "%s + %s", $1.str, $3.str);
											$$.type = INT;
										}
										else
											printf("TYPE ERROR: op1: %i op2: %i\n", $1.type, $3.type);
									}				
			|
				expression '-' term
									{ 
										if($1.type == INT && $3.type == INT)
										{
											snprintf($$.str, STR_SIZE, "%s - %s", $1.str, $3.str);
											$$.type = INT;
										}
										else
											printf("TYPE ERROR: op1: %i op2: %i\n", $1.type, $3.type);
									}
			|
				term
									{
										snprintf($$.str, STR_SIZE, "%s", $1.str); 
										$$.type = $1.type;
									}	
			|
				STR_LIT
									{ 
										snprintf($$.str, STR_SIZE, "%s", $1.str);
										$$.type = CHAR;
									}							
			;
			
term		:
				term '*' factor
									{ 
										if($1.type == INT && $3.type == INT)
										{
											snprintf($$.str, STR_SIZE, "%s * %s", $1.str, $3.str);
											$$.type = INT;
										}
										else
											printf("TYPE ERROR: op1: %i op2: %i\n", $1.type, $3.type);
									}				
			|
				term '/' factor
									{ 
										if($1.type == INT && $3.type == INT)
										{
											snprintf($$.str, STR_SIZE, "%s / %s", $1.str, $3.str);
											$$.type = INT;
										}
										else
											printf("TYPE ERROR: op1: %i op2: %i\n", $1.type, $3.type);
									}
			|
				term '%' factor
									{ 
										if($1.type == INT && $3.type == INT)
										{
											snprintf($$.str, STR_SIZE, "%s %% %s", $1.str, $3.str);
											$$.type = INT;
										}
										else
											printf("TYPE ERROR: op1: %i op2: %i\n", $1.type, $3.type);
									}
			|
				factor
									{ 
										snprintf($$.str, STR_SIZE, "%s", $1.str); 
										$$.type = $1.type;
									}
			;
			
factor		:
				'(' expression ')'
										{
											snprintf($$.str, STR_SIZE, "(%s)", $2.str);	
											$$.type = $2.type;								
										}
			|
				NUMBER
									{
										snprintf($$.str, STR_SIZE, "%s", $1.str); 
										$$.type = INT;
									}
			|
				VAR_NAME
										{
											snprintf($$.str, STR_SIZE, "%s", $1.str);	
											$$.type = Get_Type($1.str);								
										}								
			;

print		:
				PRINTI VAR_NAME
									{ 
										if(Get_Type($2.str))
										{
											if(Get_Type($2.str) == INT)
												snprintf($$.str, STR_SIZE, "printf(\"%%i\\n\", %s);\n", $2.str);
											else
												printf("PRINT ERROR: Type mismatch.\n");
										}
										else
											printf("PRINT ERROR: Variable undeclared.\n");
									}
			|
				PRINTSH VAR_NAME
									{ 
										if(Get_Type($2.str))
										{
											if(Get_Type($2.str) == CHAR)
												snprintf($$.str, STR_SIZE, "printf(\"%%s\\n\", %s);\n", $2.str);
											else
												printf("PRINT ERROR: Type mismatch.\n");
										}
										else
											printf("PRINT ERROR: Variable undeclared.\n");
									}
			|
				PRINTSV VAR_NAME
									{ 
										if(Get_Type($2.str))
										{
											if(Get_Type($2.str) == CHAR)
											{
												snprintf($$.str, STR_SIZE, "for(index = 0; index < strlen(%s); index++)\n{\nprintf(\"%%c\\n\", %s[index]);\n}\n", $2.str, $2.str);
											}
											else
												printf("PRINT ERROR: Type mismatch.\n");
										}
										else
											printf("PRINT ERROR: Variable undeclared.\n");
									}				
			;

		
newline		: 	
				'\n' newline	
									{ /*Eat the trailing newlines.*/ }
			|						
									
			;


%%

void main()
{
	iterations = 0;
	
	ParseTab_Init();

 	yyparse();
 	
 	//printf("ITERATIONS: %i\n", iterations);
 	
 	return;
}




