//Peter Carle
//Spring 2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_LEN 500
#define STR_SIZE 100

#define SYMTAB_SIZE 16

#define NUMBER_OF_OBJECTS 3
#define NUMBER_OF_ROOMS 4
#define NUMBER_OF_KEYS 3

/*basic user actions*/
#define USE 0
#define GO 1
#define GET 2
#define DROP 3
#define UP 4
#define DOWN 5
#define RIGHT 6
#define LEFT 7

/*meta actions*/
#define HELP 8
#define INVENTORY 9
#define QUIT 14

/*other actions*/
#define UNLOCK 10
#define KILL 11
#define OPEN 12
#define SWIM 13
#define THROW 21

/*attributes*/
#define LOCKED 15
#define SHARP 16
#define HEAVY 17
#define IMPASSABLE 18
#define DARK 19

#define KEY1 20
#define ROOM1 20

