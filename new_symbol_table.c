#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "userinput.h"
//#include "include/symbol_table.h"

#define SAFE_STACK 20000
#define ALPHABET 26
#define MAX_STRING 9
#define SYMTAB_SIZE 17
#define LOWERCASE_OFFSET 97
#define STR_SIZE 100

int stack_count = 0;

//PETER ERNEST CARLE
//Experimental parse table algorithm

struct tab
{
	char str[STR_SIZE];
	int token;
};
typedef struct tab tab;

struct arr
{
	int val;
	int count;
	int ref[SYMTAB_SIZE];
};
typedef struct arr arr;

tab symtab[SYMTAB_SIZE];

arr hash_array[MAX_STRING][ALPHABET];

void SymTab_Init(void)
{
	int tab_i;
	int str_i;
	int C_index;
	int D_index;
	int len;
	int count;
	
	arr ptr;

	strcpy(symtab[1].str, "down");
	strcpy(symtab[2].str, "drop");
	strcpy(symtab[3].str, "get");
	strcpy(symtab[4].str, "go");
	strcpy(symtab[5].str, "help");
	strcpy(symtab[6].str, "inventory");
	strcpy(symtab[7].str, "kill");
	strcpy(symtab[8].str, "left");
	strcpy(symtab[9].str, "open");
	strcpy(symtab[10].str, "right");
	strcpy(symtab[11].str, "swim");
	strcpy(symtab[12].str, "throw");
	strcpy(symtab[13].str, "unlock");
	strcpy(symtab[14].str, "up");
	strcpy(symtab[15].str, "use");
	strcpy(symtab[16].str, "quit");

	
	for(tab_i = 0; tab_i < MAX_STRING; tab_i++)
	{
		for(str_i = 0; str_i < ALPHABET; str_i++)
		{
			hash_array[tab_i][str_i].val = 0;
			hash_array[tab_i][str_i].count = 0;
			for(C_index = 0; C_index < SYMTAB_SIZE; C_index++)
				hash_array[tab_i][str_i].ref[C_index] = 0;
		}
	}
	
	
	for(tab_i = 0; tab_i < SYMTAB_SIZE; tab_i++)
	{
		C_index = 0; 
		len = strlen(symtab[tab_i].str);
		for(str_i = 0; str_i < len; str_i++)
		{
			ptr = hash_array[str_i][(int)symtab[tab_i].str[str_i] - LOWERCASE_OFFSET];
			
			ptr.val = symtab[tab_i].str[str_i];
			ptr.count++;
			while(ptr.ref[C_index] != 0)
				C_index++;
			ptr.ref[C_index] = tab_i;
			
			hash_array[str_i][(int)symtab[tab_i].str[str_i] - LOWERCASE_OFFSET] = ptr;
		}	
	}	
	
	puts("HASH ARRAY:\n");
	for(tab_i = 0; tab_i < MAX_STRING; tab_i++)
	{
		for(str_i = 0; str_i < ALPHABET; str_i++)
			printf("%c ", hash_array[tab_i][str_i].val);
		putchar('\n');
	}
	
	return;
}

int Sym_Compare(char string[STR_SIZE], int state_array[SYMTAB_SIZE], int match_count, int str_i, int is_first_run)
{
	/*Uses a state array to track which sets of characters were the final ones to match on the string.
	If necessary, this is used to indicate potential valid inputs to the user among the 
	potentially-matching symbols in the symbol table.*/
		
	int ref_i = 0;
	int local_count = 0;
	int is_first_match = 1;
	arr ptr;
	
	str_i = 0;
	while(string[str_i] != '\0' && string[str_i] != '\n')
	{
		if(hash_array[str_i][(int)string[str_i] - LOWERCASE_OFFSET].val)
		{	
			if(is_first_run)
			{
				ref_i = 0;
				ptr = hash_array[str_i][(int)string[str_i] - LOWERCASE_OFFSET];
				
				while(ptr.ref[ref_i])
				{	//TODO: A hash table or BST would probably be more efficient here in cases of multiple references.
					state_array[ref_i] = ptr.ref[ref_i];
					ref_i++;
				}
				match_count = ptr.count;
				local_count++;
			}
			
			if(state_array[str_i])
			{
				local_count++;	
			}
		}
	
		if(local_count == 0)
			return match_count;
	
		hash_array[str_i][(int)string[str_i] - LOWERCASE_OFFSET] = ptr;
		is_first_run = 0;
		str_i++;
	}
		
	return match_count;
} 


int String_Compare(char string[STR_SIZE])
{		
	int state_array[SYMTAB_SIZE];
	
	int index;
	int match_count;

	for(index = 0; index < SYMTAB_SIZE; index++)
		state_array[index] = 0;	
	
	match_count = Sym_Compare(string, state_array, 0, 0, 1);
	
	if(match_count == 0)	
		printf("HELP\n");
	else if(match_count == 1)
	{
		printf("One match.\n");
		for(index = 0; index < SYMTAB_SIZE; index++)
		{
			if(state_array[index] != 0)
				printf(" - %s\n", symtab[state_array[index]].str);
		}
	}
	else
	{
		printf("Ambiguous input. Possible options:\n");
		printf("match_count: %i\n", match_count);
		for(index = 0; index < SYMTAB_SIZE; index++)
		{	
			if(state_array[index] != 0)
				printf(" - %s\n", symtab[state_array[index]].str);
		}	
	}

	return 0;
}


int main()
{
	SymTab_Init();
	String_Compare("u");
	
return 0;
}

