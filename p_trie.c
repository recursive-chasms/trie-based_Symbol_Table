#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "include/defs.h"

//int stack_count = 0;
//int iterations = 0;

//PETER ERNEST CARLE
//Independently-created trie-based symbol table

/*
This personal research project grew largely out of Professor Tom Halverson's 
symbol table that he presented in his Language Processing class.
He was very self-deprecating about it, for the symtab code was simple and
succinct at the expense of being computationally efficient. Pragmatism prevailed
in that class, for his emphasis was on well-formed language constructs, rather
than the minutia of the supporting code-base. He didn't want us underclassmen 
to get side-tracked too much.

In any case, his symbol table got me thinking about how I would go about doing 
it if I had the time. I used my high-level understanding of hash tables (which
I had never implemented in code) and envisioned a series of single-character
hash tables that were indexed via input at each consecutive character from
the input string. 

The theoretical advantage of this would be *constant* look-up time for a given
string--a performance theoretically superior to that of either a complex hash 
table or a binary search tree.

The idea for the algorithm lay dormant for a couple of months until the 
implementation phase of a group project for my Agile class. I needed to produce
a symbol table to prompt the user for possible keyword inputs in response to 
ambiguous entries, a la command-line GDB.

While the implementation that made it into the project submission had linear
look-up time (due to time constraints), I subsequently completed a functional 
version of the module that incorporated the aforementioned concepts.

I believe it would have succeeded in having constant look-up time--except for
the fact that references to possible keyword entries had to be copied
for each character of the input string. Depending on the number of references,
the runtime could vary.

The name of that module is ambig_input_PeterCarle.c, and it was nearly complete
around Friday, May 10, 2019, when I did some research and discovered 
that the underlying data structure was called a trie (a.k.a., a digital tree,
radix tree or prefix tree)--a deterministic finite automaton that was apparently 
described by René de la Briandais in 1959. I was sixty years late to the party.

Still, I came up with the general concept and the source code for the trie 
independently within my Computer Science minor, using only my conceptual 
understanding of hash tables and parse tables. (I had spent some time earlier 
in the semester looking at the source code for Lex and YACC parse tables as 
well; it's likely my Language Processing class subconsciously affected this creation.) 

For what ambig_input_PeterCarle is, I'm proud of it, though the current limit
it faces is the number of references a single character position in the trie
can take. It doesn't seem to like going much above 100. I'm too burned out on it
to investigate the exact cause of the segfaults on this front. 

On another note, although it doesn't take CPU caching into account, 
the binary radix trie is something I would like to study in further depth in 
the future.

These things aside, I did write it with time-based performance considerations
in mind at every turn, and I included debug output to corroborate this.

---

ambig_input has run its course. I could do more things with it, but I figure
it's time to do away with ambiguous reference tracking and replace it with 
standard symbol table functionality. If it can be integrated as a potential 
symbol table within Lex and YACC for a reasonably-performing compiler, 
then perhaps it can serve as a public demonstration of a personal development
project of reasonable complexity. I imagine it's quite common for Computer
Science undergrads to independently invent algorithms and data structures that
have been around for decades. But I believe it's the drive towards independent
thinking and discovery that counts in this case.

§ Peter Carle §
May 13, 2019

*/

//Parse table is a 2D array which is as long as the longest string and as wide as the lower-case ASCII_TAB_SIZE.

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
		ptr = parse_table[str_i][(int)string[str_i] - CHAR_OFFSET];
		temp = (hash + ref_i) % MAX_REF;
		if(ptr.ref[temp] == hash)
			return ptr.type[temp];
		ref_i++;
		parse_table[str_i][(int)string[str_i] - CHAR_OFFSET] = ptr;
	}

	return 0;
}

/*
int main(int argc, char * argv[])
{
	char buf[MAX_REF];

	if(argv[1] == NULL)
	{
		puts("Please enter a string of characters to test the algorithm.\n");
		exit(0);
	}

	strncpy(buf, argv[1], MAX_REF);

	tab* symtab;
	
	arr parse_table[MAX_REF][ASCII_TAB_SIZE];

	symtab = SymTab_Init(parse_table);
	//String_Compare(buf, symtab, parse_table);
	
	//printf("In table: %i\n", In_Table(buf, symtab, parse_table));
	
	printf("Type: %i\n", Get_Type(buf, symtab, parse_table));
	
	free(symtab);
	
return 0;
}
*/
