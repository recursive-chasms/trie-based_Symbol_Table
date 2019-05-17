#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Peter Carle
//May 2019

#define STR_SIZE 512

#define SAFE_STACK 20000
#define ASCII_TAB_SIZE 93
#define MAX_STRING 50
#define SYMTAB_SIZE 3200
#define CHAR_OFFSET 33
#define STR_SIZE 50
#define MAX_REF 100

#define UNTYPED 0
#define INT 10
#define CHAR 20


struct tab
{
	char str[MAX_REF];
	int type;
};
typedef struct tab tab;

struct arr
{
	int val;
	int type;
	int ref[MAX_REF];
};
typedef struct arr arr;

tab* SymTab_Init(arr parse_table[MAX_STRING][ASCII_TAB_SIZE]);

int Get_Type(char string[STR_SIZE], tab* symtab, arr parse_table[MAX_STRING][ASCII_TAB_SIZE]);

