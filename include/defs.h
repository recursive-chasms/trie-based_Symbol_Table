/*defs.h: primary header file*/

#ifndef DEF_HEAD
#define DEF_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*All constants here are referenced by p_trie.c except where 
otherwise noted.*/

enum constants
{
/*Prevents overflows for recursive calls. I used a conservative
value.*/
	SAFE_STACK = 20000, 

/*The range of ASCII characters we're checking for
is 93 wide. We're ignoring the first 33 control characters.
This is the length of one dimension of the 2D array that 
comprises the central parse table*/
	ASCII_TAB_SIZE = 93, 

/*The maximum length of a string stored in the parse table. 
This is the parse table's other dimension.*/
	MAX_STRING = 50,

/*The length of the symbol table should be close to the
size of the input dictionary. Used by test_p_trie.c.*/
	SYMTAB_SIZE = 3200,

/*Ignoring the first 33 control characters.*/
	CHAR_OFFSET = 33,

/*Size of character buffer in toy language.
Referenced by toylang.y.*/
	STR_SIZE = 512,

/*The maximum number of references associated with a particular
character or symbol.*/
	MAX_REF = 100
};

enum type { UNTYPED, INT, CHAR };

/*An entry in the symbol table.*/
struct tab
{
	char str[MAX_REF];
	int type;
};
typedef struct tab tab;

/*A dimension to the parse table.*/
struct arr
{
	int val;
	int type[MAX_REF];
	int ref[MAX_REF];
};
typedef struct arr arr;

/*Parse table is a global 2D array which is as long as the longest
string and as wide as the lower-case ASCII_TAB_SIZE.*/
arr parse_table[MAX_STRING][ASCII_TAB_SIZE];

/*A guard against uncontrolled recursion.*/
int stack_count;

/*I placed this counter at key points in the programs to 
give some indication of the time complexity of the operations
in question.*/
int iterations;

#endif
