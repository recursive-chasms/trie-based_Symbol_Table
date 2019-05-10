#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_STACK 20000
#define ALPHABET 26
#define MAX_STRING 11
#define SYMTAB_SIZE 20
#define LOWERCASE_OFFSET 97
#define STR_SIZE 100

int stack_count = 0;
int iterations = 0;

//PETER ERNEST CARLE
//Experimental parse table algorithm

/*
This version of my parse table algorithm, which I later discovered to be
similar to the 

*/


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

//tab symtab[SYMTAB_SIZE];

//Parse table is a 2D array which is as long as the longest string and as wide as the lower-case alphabet.
arr parse_table[MAX_STRING][ALPHABET];

tab* SymTab_Init(void)
{
	int index;
	int tab_i;
	int str_i;
	int ref_i;
	int D_index;
	int len;
	int count;
	int input;
	int file_length = 0;
	
	tab* symtab;
	
	arr ptr;
	FILE* fptr;
	char buf[STR_SIZE];
	
	fptr = fopen("input.txt", "r");
	if(fptr == NULL)
	{
		puts("fopen() failed. Exiting.\n");
		exit(1);
	}
	while(1)
	{
		input = fgetc(fptr);
		if(input == '\n')
			file_length++;
		else if(input == EOF)
			break;
	}
	//printf("FILE LENGTH: %i\n", file_length);
	//fclose(fptr);
	
	fptr = freopen("input.txt", "r", fptr);
	if(fptr == NULL)
	{
		puts("freopen() failed. Exiting.\n");
		exit(1);
	}
	
	symtab = malloc(file_length * sizeof(tab));
	if(symtab == NULL)
	{
		puts("malloc() failed. Exiting.\n");
		exit(1);
	}

	for(index = 0; index < file_length; index++)
	{
		str_i = 0;
		//fgets(&buf, STR_SIZE, fptr);
		
		while(1)
		{
			input = fgetc(fptr);
		
			if(input != '\n')
			{
				symtab[index].str[str_i] = input;
			}
			else
			{
				symtab[index].str[str_i] = '\0';
				break;
			}
			
			str_i++;
			if(str_i >= STR_SIZE)
				break;
		}
		
		//strcpy(symtab[index].str, buf);
	}
	//printf("first string: %s\n", symtab[1].str);

	/*Most of the strings for this experiment are holdovers from the original
	symbol table code from a module I wrote for a group Agile project.*/
	
	/*
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
	*/

	//Initializes everything to zero.
	for(tab_i = 0; tab_i < MAX_STRING; tab_i++)
	{
		for(str_i = 0; str_i < ALPHABET; str_i++)
		{
			parse_table[tab_i][str_i].val = 0;
			parse_table[tab_i][str_i].count = 0;
			for(ref_i = 0; ref_i < SYMTAB_SIZE; ref_i++)
				parse_table[tab_i][str_i].ref[ref_i] = 0;
		}
	}
	
	//Populates the parse table.
	for(tab_i = 0; tab_i < SYMTAB_SIZE; tab_i++)
	{
		ref_i = 0; 
		len = strlen(symtab[tab_i].str);//<--A little optimization. Figured I'd save the compiler the trouble.
		for(str_i = 0; str_i < len; str_i++)
		{
			/*Each letter of each symtab entry is placed into its corresponding
			location in the parse table. The index of this location is determined by the
			value of the character being cast into an integer and subtracted by 
			the value needed to bring the ascii value of 'a' to 0.*/
			ptr = parse_table[str_i][(int)symtab[tab_i].str[str_i] - LOWERCASE_OFFSET];			
			//I used a temporary pointer here for optimization and readability.
			ptr.val = symtab[tab_i].str[str_i];
			ptr.count++;
			
			/*A given character in the parse table may have multiple symtab entries pointing to it.
			We need to collect each of these references and store them next to each other */
			while(ptr.ref[ref_i] != 0)
				ref_i++;
			ptr.ref[ref_i] = tab_i;//<--This reference is used to index back into the symbol table for printing at the end of the process.
			
			parse_table[str_i][(int)symtab[tab_i].str[str_i] - LOWERCASE_OFFSET] = ptr;
		}	
	}	
	
	//Not strictly necessary, but I like being able to see what the parse table looks like.
	puts("PARSE TABLE:\n");
	for(tab_i = 0; tab_i < MAX_STRING; tab_i++)
	{
		for(str_i = 0; str_i < ALPHABET; str_i++)
			printf("%c ", parse_table[tab_i][str_i].val);
		putchar('\n');
	}
	
	fclose(fptr);
	
	return symtab;
}

void Sym_Compare(char string[STR_SIZE], int state_array[SYMTAB_SIZE], int prev_state_array[SYMTAB_SIZE],\
int str_i, int is_first_run, tab* symtab)
{
	/*Uses a state array to track which sets of characters were the final ones to match on the string.
	If necessary, this is used to indicate potential valid inputs to the user among the 
	potentially-matching symbols in the symbol table.*/
	
	int index;	
	int ref_i = 0;
	int local_count = 0;
	int local_state_array[SYMTAB_SIZE];
	arr ptr;
	
	iterations++;
	stack_count++;

	if(string[str_i] != '\0' && string[str_i] != '\n')
	{
		if(parse_table[str_i][(int)string[str_i] - LOWERCASE_OFFSET].val)
		{	
			ptr = parse_table[str_i][(int)string[str_i] - LOWERCASE_OFFSET];
			
			if(is_first_run)
			{	
				while(ptr.ref[ref_i])
				{	
					local_state_array[ref_i] = ptr.ref[ref_i];
					ref_i++;
					local_count++;
					iterations++;
				}
				local_state_array[ref_i] = -1;
				prev_state_array = local_state_array;
			}
			else
			{	
				index = 0;
				while(prev_state_array[index] != -1)
				{
					if(symtab[prev_state_array[index]].str[str_i] == string[str_i])
					{
						local_state_array[ref_i] = prev_state_array[index];
						ref_i++;
						local_count++;
					}
					index++;
					iterations++;
				}
				local_state_array[ref_i] = -1;
				if(local_count)
					prev_state_array = local_state_array;
			}
		}
	}

	if(!local_count && is_first_run)
	{
		state_array[0] = -1;
		return;
	}

	if(!local_count)
	{
		for(index = 0; index < SYMTAB_SIZE; index++)
		{
			state_array[index] = prev_state_array[index];
			iterations++;
		}
		return;
	}
			
	parse_table[str_i][(int)string[str_i] - LOWERCASE_OFFSET] = ptr;
	is_first_run = 0;
	str_i++;
		
	if(stack_count < SAFE_STACK)
		Sym_Compare(string, state_array, prev_state_array, str_i, is_first_run, symtab);
	else
	{
		puts("ERROR: Recursion potentially out of control. Exiting.\n");
		exit(1);
	}
		
	return;
} 


int String_Compare(char string[STR_SIZE], tab* symtab)
{		
	int state_array[SYMTAB_SIZE];
	
	int index;
	int match_count = 0;
	
	Sym_Compare(string, state_array, NULL, 0, 1, symtab);
	
	index = 0;
	while(state_array[index] != -1)
	{
		if(state_array[index])
			match_count++;
		index++;
	}
	
	if(match_count == 0)	
		printf("HELP\n");
	else if(match_count == 1)
	{
		printf("One match.\n");
		index = 0;
		while(state_array[index] != -1)
		{
			if(state_array[index])
				printf(" - %s\n", symtab[state_array[index]].str);
			index++;
		}
	}
	else
	{
		printf("Ambiguous input. Possible options:\n");
		printf("match_count: %i\n", match_count);
		index = 0;
		while(state_array[index] != -1)
		{	
			if(state_array[index])
				printf(" - %s\n", symtab[state_array[index]].str);
			index++;
		}	
	}

	printf("ITERATIONS: %i\n", iterations);

	return 0;
}


int main(int argc, char * argv[])
{
	char buf[STR_SIZE];

	if(argv[1] == NULL)
	{
		puts("Please enter a string of characters to test the algorithm.\n");
		exit(0);
	}

	strncpy(buf, argv[1], STR_SIZE);
	//printf("%s\n", buf);

	tab* symtab;

	symtab = SymTab_Init();
	String_Compare(buf, symtab);
	
	free(symtab);
	
return 0;
}

