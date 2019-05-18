#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"


/*
Original symtab code by my Language Processing professor.

See comments on the background to this project; he never meant
to draw attention to this symtab code as a paragon of efficiency. It is very concise and easy to read, though.
*/


struct stelem
{
 char sname[100];
 int  stype; 
};
typedef struct stelem entry;


entry symtab[SYMTAB_SIZE];
int nsym;

int iterations;

void addtab( char *s)
{
	iterations++;
 nsym++;
 strcpy( symtab[nsym].sname, s);
 symtab[nsym].stype = -1;
}

void showtab()
{
 int i;
 for (i = 1; i <= nsym; ++i)
   printf("%d: %s %d\n", i, symtab[i].sname, symtab[i].stype);
}

int intab( char *s)
{
 int i;
 for ( i = 1; i <= nsym; ++i)
 {
   if ( strcmp(symtab[i].sname, s) == 0)
    return 1;
 }
 return 0;

}

int addtype( char *s, int t)
{
 int i, loc = -1;
 for ( i = 1; i <= nsym; ++i)
 {
   if ( strcmp(symtab[i].sname, s) == 0)
    loc = i;
 }
 if (loc > 0)
  {
   //printf("Set type %s to %d\n", s, t);
   symtab[loc].stype = t; 
  }
 else
 {
   //printf("Unable to set type %s to %d\n", s, t);
 } 
}

int gettype( char *s)
{
 int t = -1;
 int i, loc = -1;
 for ( i = 1; i <= nsym; ++i)
 {
 	iterations++;
   if ( strcmp(symtab[i].sname, s) == 0)
   {
    loc = i;
   
   	break;
   }
 }
 if (loc > 0)
  {
   t = symtab[loc].stype;
   //printf("Get type for %s to %d\n", s, t);
  }
   //printf("gettype var %s has type %d\n", s, t);
   
 return t;
}

int main(void)
{
	unsigned long long index;
	unsigned long long tab_i;
	unsigned long long str_i;
	unsigned long long ref_i;

	int len;
	int count;
	int file_length = 0;
	int input;
	
	//entry* symtab;
	
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
	//printf("File length: %i\n", file_length);
	nsym = file_length;
	
	fptr = freopen("weird_input.txt", "r", fptr);
	if(fptr == NULL)
	{
		puts("freopen() failed. Exiting.\n");
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
				symtab[index].sname[str_i] = input;
			else
			{
				symtab[index].sname[str_i] = '\0';
				symtab[index].stype = 1;
				break;
			}			
			str_i++;
		}		
		index++;
	}


	input = gettype("Stoke-upon-Trent");
	printf("In table: %i\n",  input);

	printf("Iterations: %i\n", iterations);

	return 0;
}

