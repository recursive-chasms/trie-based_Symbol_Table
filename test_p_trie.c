#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "include/defs.h"

//PETER ERNEST CARLE
//Independently-invented trie-based symbol table

/*
This file can be compiled on its own without the makefile. Include a list of
symbols called weird_input.txt in the directory, and enter the term to be
searched for on the command line. If the string is present, it'll print its 
type (10 for CHAR in this case) alongside the number of iterations the 
algorithm took. 

If it's not found, the type will be 0, and the number of iterations will be 
a little over 100, since the algorithm checks a reference list of that length
in case of possible hash collisions. This list could probably be shortened 
considerably, since it exists only for the edge case of multiple strings 
of the same length that end on the same character (e.g., cat, bat, dat, etc.).
Hopefully most source files would include more varied variables.

About weird_input.txt: It's a fragment of a very large dictionary file I 
downloaded. While it may not be particularly representative of a normal list
of variables (since all the words start with "st"), it is good for comparing
the efficiency of the trie code with the linear symtab.c.
*/

//Parse table is a 2D array which is as long as the longest string and as wide as the lower-case ASCII_TAB_SIZE.

//TODO
tab symtab[SYMTAB_SIZE];


void ParseTab_Init()
{
	int index;
	int tab_i;
	int str_i;
	int ref_i;

	//Initializes everything to zero.
	for(tab_i = 0; tab_i < MAX_STRING; tab_i++)
	{
		for(str_i = 0; str_i < ASCII_TAB_SIZE; str_i++)
		{
			parse_table[tab_i][str_i].val = 0;
			for(ref_i = 0; ref_i < MAX_STRING; ref_i++)
				parse_table[tab_i][str_i].type[ref_i] = UNTYPED;
			for(ref_i = 0; ref_i < MAX_STRING; ref_i++)
				parse_table[tab_i][str_i].ref[ref_i] = -1;
		}
	}
	return;
}

void Add_Symbol(char string[MAX_REF], int type)
{ 
	int hash = 0;
	int temp = 0;
	int str_i = 0;
	int ref_i = 0;
	int len;
	arr ptr;

	len = strlen(string);//<--A little optimization. Figured I'd save the compiler the trouble.

	for(str_i = 0; str_i < len; str_i++)
	{
		//iterations++;
		/*Each letter of each symtab entry is placed into its corresponding
		location in the parse table. The index of this location is determined by 
		the value of the character being cast into an integer and subtracted by 
		the value needed to bring the ascii value of 'a' to 0.*/		
		ptr = parse_table[str_i][(int)string[str_i] - CHAR_OFFSET];			
		//I used a temporary pointer here for optimization and readability.
		
		temp = ptr.val = string[str_i];		
		hash = temp ^ hash;					
		parse_table[str_i][(int)string[str_i] - CHAR_OFFSET] = ptr;
	}
	ref_i = 0;
	while(ptr.ref[(hash + ref_i) % MAX_REF] != -1)
	{
		//iterations++;
		/*I'm aware of the possibility of collisions here. Creating a strong 
		hashing function is outside of the scope of this project, since I 
		don't have much knowledge of cryptography right now. A single XOR 
		is about as quick and easy as you can get, methinks. For a toy 
		demonstration of trie functionality, this should suffice.*/
		
		if(ptr.ref[(hash + ref_i) % MAX_REF] == hash)
			goto Duplicate;
				
		ref_i++;
		if(ref_i == MAX_REF)
		{
			puts("ERROR: Maximum number of variables at this character terminus. Try using more variables that end on a different character...or variables with a different number of characters.\n");
				exit(0);
		}
	}
	temp = (hash + ref_i) % MAX_REF;		
	ptr.ref[temp] = hash;
	ptr.type[temp] = type;
		
Duplicate:	//(Just skip the hash entry.)
		
	str_i--;
	parse_table[str_i][(int)string[str_i] - CHAR_OFFSET] = ptr;				
		
	return;
}


int Get_Type(char string[MAX_REF])
{
	int is_first_run = 0;
	int str_i = 0;
	int ref_i = 0;
	int hash = 0;
	int temp = 0;
	
	arr ptr;
	
	iterations++;
	stack_count++;

	while(string[str_i] != '\0' && string[str_i] != '\n')
	{
		temp = 0;
		iterations++;
	//The declaration within the if-statement below was intentional. I probably wouldn't do this in production code.
		if(temp = parse_table[str_i][(int)string[str_i] - CHAR_OFFSET].val)
			str_i++;
		else
			return 0;
			
		hash = temp ^ hash;
	}
	if(!str_i)
		return 0;
	
	str_i--;
	ref_i = 0;
	while(ref_i < MAX_REF)
	{
		iterations++;
		ptr = parse_table[str_i][(int)string[str_i] - CHAR_OFFSET];
		temp = (hash + ref_i) % MAX_REF;
		if(ptr.ref[temp] == hash)
			return ptr.type[temp];
		ref_i++;
		parse_table[str_i][(int)string[str_i] - CHAR_OFFSET] = ptr;
	}

	return 0;
}


int main(int argc, char * argv[])
{
	char buf[MAX_REF];
	int index;
	int file_length = 0;
	int len;
	int str_i;
	int input;
	
	if(argv[1] == NULL)
	{
		puts("Please enter a string of characters to test the algorithm.\n");
		exit(0);
	}

	strncpy(buf, argv[1], MAX_REF);

	iterations = 0;
	//tab* symtab;
	
	arr ptr;
	FILE* fptr;
	
	fptr = fopen("weird_input.txt", "r");
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

	
	fptr = freopen("weird_input.txt", "r", fptr);
	if(fptr == NULL)
	{
		puts("freopen() failed. Exiting.\n");
		exit(1);
	}
	
	/*
	symtab = malloc(file_length * sizeof(tab));
	if(symtab == NULL)
	{
		puts("malloc() failed. Exiting.\n");
		exit(1);
	}
	*/

	index = 0;
	while(index < file_length && index < SYMTAB_SIZE)
	{
		str_i = 0;
		
		while(1)
		{
			if(str_i >= MAX_REF)
				break;
			
			input = fgetc(fptr);
		
			if(input != '\n' && input != '\0')
				symtab[index].str[str_i] = input;
			else
			{
				symtab[index].str[str_i] = '\0';
				symtab[index].type = 1;
				break;
			}			
			str_i++;
		}		
		index++;
	}
	//printf("first string: %s\n", symtab[1].sname);

	ParseTab_Init(parse_table);
	
	for(index = 0; index < file_length; index++)
		Add_Symbol(symtab[index].str, CHAR);
	
	printf("Type: %i\n", Get_Type(buf));
	
	printf("ITERATIONS: %i\n", iterations);
	
	//free(symtab);
	
return 0;
}

