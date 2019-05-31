//PETER ERNEST CARLE
//Independently-invented trie-based symbol table

/*
p_trie.c: Main symbol table library

TL;DR: I came up with a trie data structure on my own without knowing that's
what it was called. It came primarily from my high-level understanding 
of hash tables and parse tables. I created the structure and source code for 
these projects independently of any specific assignment; it was never taught
in class. The structure itself has theoretically constant look-up time.

The extended backstory behind this is available in the comments in the older
project titled ambig_input_PeterCarle.c. 

This successor project is designed to use trie functionality to provide
a reasonably well-performing symbol table for a compiler. 

I integrated it with the Lex and YACC files from a previous Language Processing
project to test it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "include/defs.h"

/*Initializes everything to zero.*/
void 
ParseTab_Init()
{
	int index;
	int tab_i;
	int str_i;
	int ref_i;
	
	for (tab_i = 0; tab_i < MAX_STRING; tab_i++)
	{
		for (str_i = 0; str_i < ASCII_TAB_SIZE; str_i++)
		{
			/*Parse table is a global 2D array which is as long as the longest
			string and as wide as the lower-case ASCII_TAB_SIZE.*/
			parse_table[tab_i][str_i].val = 0;
			for (ref_i = 0; ref_i < MAX_STRING; ref_i++)
				parse_table[tab_i][str_i].type[ref_i] = UNTYPED;
			for (ref_i = 0; ref_i < MAX_STRING; ref_i++)
				parse_table[tab_i][str_i].ref[ref_i] = -1;
		}
	}
	return;
}

/*This adds a variable and its associated type to the parse table/symbol table.*/
void 
Add_Symbol(char string[MAX_REF], int type)
{ 
	int hash = 0;
	int temp = 0;
	int str_i = 0;
	int ref_i = 0;
	int len;
	arr ptr;

	len = strlen(string);//<--A little optimization.
	for (str_i = 0; str_i < len; str_i++)
	{
		iterations++;
		/*Each letter of each symtab entry is placed into its corresponding
		location in the parse table. The index of this location is determined by 
		the value of the character being cast into an integer and subtracted by 
		33: I eliminated the first 33 ascii characters from the parse table,
		since I have no use for control characters here.*/		
		ptr = parse_table[str_i][(int)string[str_i] - CHAR_OFFSET];			
		/*I used a temporary pointer here for optimization and readability.*/
		
		/*The simple hash is updated with each consecutive character.
		This hash is used at the last character both to index it
		and to uniquely identify it in case multiple symbols of the 
		same length end on the same character.*/
		temp = ptr.val = string[str_i];		
		hash = temp ^ hash;					
		parse_table[str_i][(int)string[str_i] - CHAR_OFFSET] = ptr;
	}
	ref_i = 0;
	while (ptr.ref[(hash + ref_i) % MAX_REF] != -1)
	{/*This loop ideally places the hash at its corresponding index in
	 the hash list. But if an entry is already there (and it isn't a 
	 duplicate), the next highest index is checked to see if it's
	 empty. The loop continues until an empty slot is found.*/
		iterations++;
		
		/*I'm aware of the possibility of collisions here. Creating a strong 
		hashing function is outside of the scope of this project, since I 
		don't have much knowledge of cryptography right now. A single XOR 
		is about as quick and easy as you can get, methinks. For a toy 
		demonstration of trie functionality, this should suffice.*/	
		if (ptr.ref[(hash + ref_i) % MAX_REF] == hash)
			goto Duplicate;//Just one goto, and it's nearby.
				
		ref_i++;
		if (ref_i == MAX_REF)
		{
			puts("ERROR: Maximum number of variables at this character terminus.\
Try using more variables that end on a different character...\
or variables with a different number of characters.\n");
				exit(0);
		}
	}
	temp = (hash + ref_i) % MAX_REF;		
	ptr.ref[temp] = hash;
	ptr.type[temp] = type;
		
Duplicate:	/*(Just skips the hash input.)*/
	
	if (str_i) str_i--;/*Needed to fix off-by-one error.*/
	parse_table[str_i][(int)string[str_i] - CHAR_OFFSET] = ptr;				
		
	return;
}

/*This function performs double duty. It checks to see whether the entry
exists within the parse table. If the entry does exist, its type is returned.
Otherwise zero is returned.*/
int 
Get_Type(char string[MAX_REF])
{
	int str_i = 0;
	int ref_i = 0;
	int hash = 0;
	int temp = 0;
	
	arr ptr;
	
	iterations++;
	stack_count++;

	while (string[str_i] != '\0' && string[str_i] != '\n')
	{
	/*The corresponding location within the parse table of each 
	consecutive character in the input string is checked. If that character
	exists at that location within the parse tree, the algorithm advances
	to the next character/state. If the character doesn't exist in the given
	state, the algorithm rejects the string. If the string is parsed all the
	way through to the end, and the final character exists in the parse table,
	the string is accepted. Classic deterministic finite automaton.
	
	Moreover, the hash of the given string is stored in an array at the final
	character. This array is 100 entries long in case of a massive number of
	hash collisions (which honestly seems rather unlikely). If the hash isn't
	found at its corresponding index, the algorithm iterates through the whole
	array of hashes to check if it's been moved there. This is why non-existent
	strings take longer to process than strings that exist in the table.*/
		temp = 0;
		iterations++;
		
		temp = parse_table[str_i][(int)string[str_i] - CHAR_OFFSET].val
		if (temp) str_i++;
		else return 0;
			
		hash = temp ^ hash;
	}
	if (!str_i) return 0;
	
	str_i--;
	ref_i = 0;
	while (ref_i < MAX_REF)
	{/*In most cases, the hash should be located at its corresponding index.
	But if it isn't, each consecutive entry in the hash reference list should
	be checked until it's found. If the entry isn't found anywhere in the list,
	the entry isn't there.
	*/
		iterations++;
		ptr = parse_table[str_i][(int)string[str_i] - CHAR_OFFSET];
		temp = (hash + ref_i) % MAX_REF;
		if (ptr.ref[temp] == hash)
			return ptr.type[temp];
		ref_i++;
		parse_table[str_i][(int)string[str_i] - CHAR_OFFSET] = ptr;
	}

	return 0;
}


