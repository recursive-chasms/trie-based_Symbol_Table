//Peter Carle
//Spring 2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
int index = 0;
int i_NewVar30;
int y;
y = 4 % (((5 + 2) - 2));
i_NewVar30 = 3 * y;
printf("%i\n", i_NewVar30);
char * s_BigString = "Interleaved declaration.";
for(index = 0; index < strlen(s_BigString); index++)
{
printf("%c\n", s_BigString[index]);
}
 return 0;
}
