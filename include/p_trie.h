

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

//Peter Carle
//May 2019

//#define STR_SIZE 512

/*
#define SAFE_STACK 20000
#define ASCII_TAB_SIZE 93
#define SYMTAB_SIZE 3200
#define CHAR_OFFSET 33
#define STR_SIZE 512
#define MAX_REF 100
#define MAX_STRING 50

#define UNTYPED 0
#define INT 20
#define CHAR 10

struct tab
{
	char str[MAX_REF];
	int type;
};
typedef struct tab tab;

struct arr
{
	int val;
	int type[MAX_REF];
	int ref[MAX_REF];
};
typedef struct arr arr;
*/

void ParseTab_Init();

void Add_Symbol(char string[MAX_REF], int type);

int Get_Type(char string[MAX_REF]);




