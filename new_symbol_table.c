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
	int index;
	int B_index;
	int C_index;
	int D_index;
	int len;
	int count;

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
		{
			hash_array[index][B_index].val = 0;
			hash_array[index][B_index].count = 0;
			for(C_index = 0; C_index < SYMTAB_SIZE; C_index++)
				hash_array[index][B_index].ref[C_index] = 0;
		}
	}
	
	
	for(index = 0; index < SYMTAB_SIZE; index++)
	{
		C_index = 0; 
		len = strlen(symtab[index].str);
		for(B_index = 0; B_index < len; B_index++)
		{
			hash_array[B_index][(int)symtab[index].str[B_index] - LOWERCASE_OFFSET].val = symtab[index].str[B_index];
			hash_array[B_index][(int)symtab[index].str[B_index] - LOWERCASE_OFFSET].count++;
			while(hash_array[B_index][(int)symtab[index].str[B_index] - LOWERCASE_OFFSET].ref[C_index] != 0)
				C_index++;
			hash_array[B_index][(int)symtab[index].str[B_index] - LOWERCASE_OFFSET].ref[C_index] = index;
			
			if(symtab[index].str[B_index] == 'u')
			{
				puts("References for u:\n");
				D_index = 0;
				while(hash_array[B_index][(int)symtab[index].str[B_index] - LOWERCASE_OFFSET].ref[D_index])
				{
					printf("%i\n", hash_array[B_index][(int)symtab[index].str[B_index] - LOWERCASE_OFFSET].ref[D_index]);
					D_index++;
				}
			
			}

			//hash_array[B_index][(int)symtab[index].str[B_index] - LOWERCASE_OFFSET].ref[B_index] = index;
		}	
	}	
	
	puts("HASH ARRAY:\n");
	for(index = 0; index < MAX_STRING; index++)
	{
		for(B_index = 0; B_index < ALPHABET; B_index++)
			printf("%c ", hash_array[index][B_index].val);
		putchar('\n');
	}
	
	return;
}

int Sym_Compare(char string[STR_SIZE], int state_array[SYMTAB_SIZE], int match_count, int index, int is_first_run)
{
	/*Uses a state array to track which sets of characters were the final ones to match on the string.
	If necessary, this is used to indicate potential valid inputs to the user among the 
	potentially-matching symbols in the symbol table.*/
		
	int B_index = 0;
	int local_count = 0;
	int is_first_match = 1;

	
	index = 0;
	while(string[index] != '\0' && string[index] != '\n')
	{
		if(hash_array[index][(int)string[index] - LOWERCASE_OFFSET].val)
		{	
			if(is_first_run)
			{
				B_index = 0;
				while(hash_array[index][(int)string[index] - LOWERCASE_OFFSET].ref[B_index])
				{
					state_array[B_index] = hash_array[index][(int)string[index] - LOWERCASE_OFFSET].ref[B_index];
					printf("State array: %i\n", state_array[B_index]);
					B_index++;
				}
				match_count = hash_array[index][(int)string[index] - LOWERCASE_OFFSET].count;
				local_count++;
			}
			
			if(state_array[index])
			{
				local_count++;	
			}
		}
	
		if(local_count == 0)
			return match_count;
	
		is_first_run = 0;
		index++;
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
	{	
		printf("HELP %i\n", HELP);
		//return HELP;
	}
	else if(match_count == 1)
	{
		printf("One match.\n");
		for(index = 0; index < SYMTAB_SIZE; index++)
		{
			if(state_array[index] != 0) //TODO: Excludes 'down.'
				printf(" - %s\n", symtab[state_array[index]].str);
		}
	}
	else
	{
		printf("Ambiguous input. Possible options:\n");
		printf("match_count: %i\n", match_count);
		for(index = 0; index < SYMTAB_SIZE; index++)
		{	
			//printf("match_count: %i\n", match_count);
			if(state_array[index] != 0) //TODO: Excludes 'down.'
				printf(" - %s\n", symtab[state_array[index]].str);
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
	String_Compare("g");
	
return 0;
}

