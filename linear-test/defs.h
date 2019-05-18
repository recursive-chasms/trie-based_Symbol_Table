#ifndef DEF_HEAD
#define DEF_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

arr parse_table[MAX_STRING][ASCII_TAB_SIZE];
int stack_count;
int iterations;

#endif
