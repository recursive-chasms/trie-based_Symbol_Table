#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userinput.h"
//#include "include/symbol_table.h"

#define SAFE_STACK 20000
#define ALPHABET 26
#define MAX_STRING 9
#define SYMTAB_SIZE 16
#define LOWERCASE_OFFSET 97

//#define STR_SIZE 512

int stack_count = 0;

/*Quick and dirty symbol table for comparing ambiguous keywords. No time for a hash table. :/   ~Peter, April 20th*/


struct tab
{
	char str[STR_SIZE];
	int token;
};
typedef struct tab tab;

tab symtab[SYMTAB_SIZE];

int hash_array[MAX_STRING][ALPHABET];

void SymTab_Init(void)
{
	int index;
	int B_index;
	int len;

	strcpy(symtab[0].str, "down");
	strcpy(symtab[1].str, "drop");
	strcpy(symtab[2].str, "get");
	strcpy(symtab[3].str, "go");
	strcpy(symtab[4].str, "help");
	strcpy(symtab[5].str, "inventory");
	strcpy(symtab[6].str, "kill");
	strcpy(symtab[7].str, "left");
	strcpy(symtab[8].str, "open");
	strcpy(symtab[9].str, "right");
	strcpy(symtab[10].str, "swim");
	strcpy(symtab[11].str, "throw");
	strcpy(symtab[12].str, "unlock");
	strcpy(symtab[13].str, "up");
	strcpy(symtab[14].str, "use");
	strcpy(symtab[15].str, "quit");

	symtab[0].token = DOWN;
	symtab[1].token = DROP;
	symtab[2].token = GET;
	symtab[3].token = GO;
	symtab[4].token = HELP;
	symtab[5].token = INVENTORY;
	symtab[6].token = KILL;
	symtab[7].token = LEFT;
	symtab[8].token = OPEN;
	symtab[9].token = RIGHT;
	symtab[10].token = SWIM;
	symtab[11].token = THROW;
	symtab[12].token = UNLOCK;
	symtab[13].token = UP;
	symtab[14].token = USE;
	symtab[15].token = QUIT;
	
	for(index = 0; index < MAX_STRING; index++)
	{
		for(B_index = 0; B_index < ALPHABET; B_index++)
			hash_array[index][B_index] = 0;
	}
	
	for(index = 0; index < SYMTAB_SIZE; index++)
	{
		len = strlen(symtab[index].str);
		for(B_index = 0; B_index < len; B_index++)
		{
			hash_array[index][(int)symtab[index].str[B_index] - LOWERCASE_OFFSET] = 1;
		}	
	}	
	
	puts("HASH ARRAY:\n");
	for(index = 0; index < MAX_STRING; index++)
	{
		for(B_index = 0; B_index < ALPHABET; B_index++)
			printf("%i ", hash_array[index][B_index] = 0);
		putchar('\n');
	}
	
	return;
}

int Sym_Compare(char string[STR_SIZE], int state_array[SYMTAB_SIZE], int match_count, int index, int is_first_run)
{
	/*Uses a state array to track which sets of characters were the final ones to match on the string.
	If necessary, this is used to indicate potential valid inputs to the user among the 
	potentially-matching symbols in the symbol table.*/
	stack_count++;
	
	//int local_state_array[SYMTAB_SIZE];
	int index_B = 0;
	int local_count = 0;
	int is_first_match = 1;
	//int len = strlen(string);
	
	//for(index_B = 0; index_B < SYMTAB_SIZE; index_B++)
	//	local_state_array[index_B] = 0;
	
	index = 0;
	while(string[index] != '\0' && string[index] != '\n')
	{
		//is_first_match = 1;
		if(hash_array[index][(int)string[index] - LOWERCASE_OFFSET])
		{	
			if(is_first_run)
			{
				state_array[index] = 1;	
				match_count++;	
				local_count++;
			}
			
			if(state_array[index])
			{
				//state_array[index] = 1;	

				//match_count++;	
				local_count++;	
			}
		}
	
		if(local_count == 0)
			return match_count;
	
		//for(index_B = 0; index_B < SYMTAB_SIZE; index_B++)
		//	state_array[index_B] = local_state_array[index_B];
	
		is_first_run = 0;
		index++;
	}
	
	
	/*Check first layer of string array.
	for(index_B = 0; index_B < SYMTAB_SIZE; index_B++)
	{
		if(symtab[index_B].str[index] != '\0' && symtab[index_B].str[index] != '\n')
		{
			if(string[index] == symtab[index_B].str[index])
			{	
				if(is_first_match)
				{
					is_first_match = 0;
					match_count = 0;
				}
				if(state_array[index_B] || is_first_run)
				{
					local_state_array[index_B] = 1;	

					match_count++;	
					local_count++;	
				}
			}
		}
	}
	
	is_first_run = 0;
	*/
	
	/*
	if(local_count != 0)
	{
		/*Over-write the previous state array. Otherwise, leave the state array in the last matching state.
		for(index_B = 0; index_B < SYMTAB_SIZE; index_B++)
			state_array[index_B] = local_state_array[index_B];
		
		/*Check next letter in the string
		index++;
		if(string[index] != '\0' && string[index] != '\n' && stack_count < SAFE_STACK)
			match_count = Sym_Compare(string, state_array, match_count, index, is_first_run);
	}
	*/
		
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
	{	
		printf("HELP %i\n", HELP);
		//return HELP;
	}
	else if(match_count == 1)
	{
		printf("One match.\n");
		for(index = 0; index < SYMTAB_SIZE; index++)
		{
			//if(state_array[index] != 0)
				//return symtab[index].token;
		}
	}
	else
	{
		printf("Ambiguous input. Possible options:\n");
		for(index = 0; index < SYMTAB_SIZE; index++)
		{	
			if(state_array[index] != 0)
				printf(" - %s\n", symtab[index].str);
		}	
	}
	//puts("State array:\n");
	//for(index = 0; index < SYMTAB_SIZE; index++)
	//	printf("%i\n", state_array[index]);


	return 0;
}


int main()
{
	SymTab_Init();
	String_Compare("q");
	
return 0;
}

