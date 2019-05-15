#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define SAFE_STACK 20000
#define ASCII_TAB_SIZE 93
#define MAX_STRING 50
#define SYMTAB_SIZE 3200
#define NON_CONTROL_CHAR_OFFSET 33
#define STR_SIZE 50
#define MAX_REF 100

int stack_count = 0;
int iterations = 0;

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
to investigate the exact cause of the segfaults on this front. Also,
it's not the HAT-trie algorithm; it doesn't take CPU caching into account. 
(The binary radix trie is  something I would like to study in further depth in 
the future.) 

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

/*
struct stelem
{
 char sname[512];
 int  stype; 
};
typedef struct stelem entry;

entry symtab[100];
int nsym;

void addtab( char *s)

void showtab()

int intab( char *s)

int addtype( char *s, int t)

int gettype( char *s)

*/

struct tab
{
	char str[MAX_REF];
	int token;
};
typedef struct tab tab;

struct arr
{
	int val;
	int count;
	int ref[MAX_REF];
};
typedef struct arr arr;

//Parse table is a 2D array which is as long as the longest string and as wide as the lower-case ASCII_TAB_SIZE.

tab* SymTab_Init(arr parse_table[MAX_STRING][ASCII_TAB_SIZE])
{
	int index;
	int tab_i;
	int str_i;
	int ref_i;

	int len;
	int count;
	int input;
	int file_length = 0;
	
	tab* symtab;
	
	arr ptr;
	FILE* fptr;
	char buf[MAX_REF];
	
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
	
	symtab = malloc(file_length * sizeof(tab));
	if(symtab == NULL)
	{
		puts("malloc() failed. Exiting.\n");
		exit(1);
	}

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
				break;
			}			
			str_i++;
		}		
		index++;
	}
	printf("first string: %s\n", symtab[1].str);

	//Initializes everything to zero.
	for(tab_i = 0; tab_i < MAX_STRING; tab_i++)
	{
		for(str_i = 0; str_i < ASCII_TAB_SIZE; str_i++)
		{
			parse_table[tab_i][str_i].val = 0;
			parse_table[tab_i][str_i].count = 0;
			for(ref_i = 0; ref_i < MAX_REF; ref_i++)
				parse_table[tab_i][str_i].ref[ref_i] = 0;
		}
	}
	
	//Populates the parse table.
	for(tab_i = 0; tab_i < SYMTAB_SIZE; tab_i++)
	{ 
		len = strlen(symtab[tab_i].str);//<--A little optimization. Figured I'd save the compiler the trouble.
		for(str_i = 0; str_i < len; str_i++)
		{
			/*Each letter of each symtab entry is placed into its corresponding
			location in the parse table. The index of this location is determined by the
			value of the character being cast into an integer and subtracted by 
			the value needed to bring the ascii value of 'a' to 0.*/
			ptr = parse_table[str_i][(int)symtab[tab_i].str[str_i] - NON_CONTROL_CHAR_OFFSET];			
			//I used a temporary pointer here for optimization and readability.
			ptr.val = symtab[tab_i].str[str_i];
			ptr.count++;
			
			/*A given character in the parse table may have multiple symtab entries pointing to it.
			We need to collect each of these references and store them next to each other */
			ref_i = 0;
			while(ptr.ref[ref_i] != 0)
				ref_i++;
			ptr.ref[ref_i] = tab_i;//<--This reference is used to index back into the symbol table for printing at the end of the process.
			
			parse_table[str_i][(int)symtab[tab_i].str[str_i] - NON_CONTROL_CHAR_OFFSET] = ptr;
		}	
	}	
	
	//Not strictly necessary, but I like being able to see what the parse table looks like.
	puts("PARSE TABLE:\n");
	for(tab_i = 0; tab_i < MAX_STRING; tab_i++)
	{
		for(str_i = 0; str_i < ASCII_TAB_SIZE; str_i++)
			printf("%c ", parse_table[tab_i][str_i].val);
		putchar('\n');
	}
	
	fclose(fptr);
	
	return symtab;
}

/*
TODO: I believe most of Sym_Compare can be replaced with the simpler 
symbol table functions: In_Table() and Get_Type(). Some refactoring
will be necessary for SymTab_Init(); most of the code will be moved
into Add_Symbol(). 

Add_Symbol() should be consolidated from Tom's two separate functions (addtab
and addtype). Since both additions are made at once, it's not necessary
to separate them out.

The code should be made to work on its own before attempted integration
with the Lex and YACC files for A8 (from Language Processing). 
*/

void Sym_Compare(char string[STR_SIZE], int state_array[SYMTAB_SIZE], int prev_state_array[SYMTAB_SIZE],\
int str_i, int is_first_run, tab* symtab, arr parse_table[MAX_STRING][ASCII_TAB_SIZE])
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
		if(parse_table[str_i][(int)string[str_i] - NON_CONTROL_CHAR_OFFSET].val)
		{	
			ptr = parse_table[str_i][(int)string[str_i] - NON_CONTROL_CHAR_OFFSET];
			
			if(is_first_run)
			{	
				while(ptr.ref[ref_i] != 0 && ref_i < MAX_REF)
				{	
					local_state_array[ref_i] = ptr.ref[ref_i];
					ref_i++;
					local_count++;
					iterations++;
					puts("1111111111 Copy ref\n");
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
					puts("++++++++ Copy other ref\n");
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
		index = 0;
		while(prev_state_array[index] != -1)
		{
			state_array[index] = prev_state_array[index];
			iterations++;
			puts("Final state_array copy.\n");
			index++;
		}
		state_array[index] = -1;
		return;
	}
			
	parse_table[str_i][(int)string[str_i] - NON_CONTROL_CHAR_OFFSET] = ptr;
	is_first_run = 0;
	str_i++;
		
	if(stack_count < SAFE_STACK)
		Sym_Compare(string, state_array, prev_state_array, str_i, is_first_run, symtab, parse_table);
	else
	{
		puts("ERROR: Recursion potentially out of control. Exiting.\n");
		exit(1);
	}
		
	return;
} 


int String_Compare(char string[STR_SIZE], tab* symtab, arr parse_table[MAX_STRING][ASCII_TAB_SIZE])
{		
	int state_array[SYMTAB_SIZE];
	
	int index;
	int match_count = 0;
	
	Sym_Compare(string, state_array, NULL, 0, 1, symtab, parse_table);
	
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
	char buf[MAX_STRING];

	if(argv[1] == NULL)
	{
		puts("Please enter a string of characters to test the algorithm.\n");
		exit(0);
	}

	strncpy(buf, argv[1], MAX_STRING);

	tab* symtab;
	
	arr parse_table[MAX_STRING][ASCII_TAB_SIZE];

	symtab = SymTab_Init(parse_table);
	String_Compare(buf, symtab, parse_table);
	
	free(symtab);
	
return 0;
}

