#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userinput.h"

struct tab
{
	char str[STR_SIZE];
	int token;
};
typedef struct tab tab;

tab symtab[SYMTAB_SIZE];

void SymTab_Init(void);

int Sym_Compare(char string[STR_SIZE], int state_array[SYMTAB_SIZE], int match_count, int index, int is_first_run, int iterations);

int String_Compare(char string[STR_SIZE]);
