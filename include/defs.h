/*defs.h: primary header file*/

#ifndef DEF_HEAD
#define DEF_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*All constants here are referenced by p_trie.c except where 
otherwise noted.*/

/*Prevents overflows for recursive calls. I used a conservative
value.*/
#define SAFE_STACK 20000 

/*The range of ASCII characters we're checking for
is 93 wide. We're ignoring the first 33 control characters.
This is the length of one dimension of the 2D array that 
comprises the central parse table*/
#define ASCII_TAB_SIZE 93 

/*The maximum length of a string stored in the parse table. 
This is the parse table's 2nd dimension.*/
#define MAX_STRING 50

/*The length of the symbol table should be close to the
size of the input dictionary. Used by test_p_trie.c.*/
#define SYMTAB_SIZE 3200

/*Ignoring the first 33 control characters.*/
#define CHAR_OFFSET 33

/*Size of character buffer in toy language.*/
#define STR_SIZE 512

/*The maximum number of references associated with a particular
character or symbol.*/
#define MAX_REF 100

enum type { UNTYPED, INT, CHAR };

/*
#define UNTYPED 0
#define INT 20
#define CHAR 10
*/

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
