//Peter Carle
//Spring 2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
int index = 0;
int x;
int y;
char g[25];
y = 11;
x = 3 * y;
printf("%i\n", x);
strncpy(g, "hello world", 25);
printf("%s\n", g);
for(index = 0; index < strlen(g); index++)
{
printf("%c\n", g[index]);
}
 return 0;
}
