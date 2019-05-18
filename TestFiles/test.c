//Peter Carle
//Spring 2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
int index = 0;
char G[25];
strncpy(G, "consciousness", 25);
for(index = 0; index < strlen(G); index++)
{
printf("%c\n", G[index]);
}
 return 0;
}
