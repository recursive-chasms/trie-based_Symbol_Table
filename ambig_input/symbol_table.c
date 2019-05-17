#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userinput.h"
#include "symbol_table.h"

#define SAFE_STACK 20000

int stack_count = 0;
//int iterations;

/*Symbol table for comparing ambiguous keywords. ~Peter, April 20th*/

void SymTab_Init(void)
{
	int index;
	int B_index;
	int len;

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
	strcpy(symtab[16].str, "unnecessary");
	strcpy(symtab[17].str, "quit");
	strcpy(symtab[18].str, "until");
	strcpy(symtab[19].str, "unti");

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
	//symtab[16].token = MOVE;
	//symtab[17].token = GRAB;
	
	return;
}

int Sym_Compare(char string[STR_SIZE], int state_array[SYMTAB_SIZE], int match_count, int index, int is_first_run, int iterations)
{
	/*Uses a state array to track which sets of characters were the final ones to match on the string.
	If necessary, this is used to indicate potential valid inputs to the user among the 
	potentially-matching symbols in the symbol table.*/
	stack_count++;
	iterations++;
	
	int local_state_array[SYMTAB_SIZE];
	int index_B = 0;
	int local_count = 0;
	int is_first_match = 1;
	
	for(index_B = 0; index_B < SYMTAB_SIZE; index_B++)
		local_state_array[index_B] = 0;
	
	/*Check first layer of string array.*/	
	for(index_B = 0; index_B < SYMTAB_SIZE; index_B++)
	{
		iterations++;
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
	
	if(local_count != 0)
	{
		/*Over-write the previous state array. Otherwise, leave the state array in the last matching state.*/
		for(index_B = 0; index_B < SYMTAB_SIZE; index_B++)
		{
			state_array[index_B] = local_state_array[index_B];
			iterations++;
		}
		
		/*Check next letter in the string*/
		index++;
		if(string[index] != '\0' && string[index] != '\n' && stack_count < SAFE_STACK)
			match_count = Sym_Compare(string, state_array, match_count, index, is_first_run, iterations);
	}
		
	return iterations;
} 

int String_Compare(char string[STR_SIZE])
{		
	int state_array[SYMTAB_SIZE];
	
	int index;
	int match_count;
	int iterations = 0;

	for(index = 0; index < SYMTAB_SIZE; index++)
		state_array[index] = 0;	
	
	iterations = Sym_Compare(string, state_array, 0, 0, 1, 0);
	
	//if(match_count == 0)
	//{	
		//return HELP;
	//}
	//else if(match_count == 1)
	//{
		//for(index = 0; index < SYMTAB_SIZE; index++)
		//{
		//	if(state_array[index] != 0)
				//return symtab[index].token;
		//}
	//}
	//else
	//{
	//	printf("Ambiguous input. Possible options:\n");
	//	for(index = 0; index < SYMTAB_SIZE; index++)
	//	{	
//			if(state_array[index] != 0)
	//			printf(" - %s\n", symtab[index].str);
//		}	
	//}
	
	printf("ITERATIONS: %i\n", iterations);

	return 0;
}


int main()
{
	SymTab_Init();
	String_Compare("unnecessary");
	
return 0;
}

