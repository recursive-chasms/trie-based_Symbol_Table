#Makefile by Peter, April 18th.

CC=gcc

#Tells GCC which directory to check for header files
CFLAGS=-g -I./$(IDIR)

#Header and object file directories
IDIR=include
ODIR=obj

#Shared libraries for Lex and YACC
LIBS=-lfl -ly

#Places header files in include/
_DEPS = userinput.h locations.h userinventory.h input.tab.h symbol_table.h help.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

#Places object files in obj/
_OBJ = userinventory.o locations.o lex.yy.o input.tab.o symbol_table.o help.o
OBJ = $(patsubst %,$(ODIR)/%, $(_OBJ))

#Final compilation: compiles main() and links everything together. 
game: $(OBJ) main.c $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Runs YACC to generate parse table for user input
input.tab.c: input.y
	bison -dv input.y
	mv input.tab.h $(IDIR)

./include/input.tab.h: input.y
	bison -dv input.y
	mv input.tab.h $(IDIR)

#Runs Lex to generate lexical scanner
lex.yy.c: input.l ./include/input.tab.h 
	flex  input.l

#5 commands to compile individual object components before they can be integrated with main()

$(ODIR)/lex.yy.o : lex.yy.c ./include/input.tab.h ./include/userinput.h
	$(CC) -c -o $@ lex.yy.c $(CFLAGS)

$(ODIR)/input.tab.o : input.tab.c ./include/userinput.h
	$(CC) -c -o $@ input.tab.c $(CFLAGS)

$(ODIR)/userinventory.o : userinventory.c ./include/userinput.h ./include/locations.h ./include/userinventory.h
	$(CC) -c -o $@ userinventory.c $(CFLAGS)

$(ODIR)/locations.o : locations.c ./include/userinput.h ./include/locations.h ./include/userinventory.h
	$(CC) -c -o $@ locations.c $(CFLAGS)

$(ODIR)/symbol_table.o : symbol_table.c ./include/symbol_table.h ./include/userinput.h
	$(CC) -c -o $@ symbol_table.c $(CFLAGS)

$(ODIR)/help.o : help.c ./include/help.h ./include/userinput.h
	$(CC) -c -o $@ help.c $(CFLAGS)

.PHONY: clean

clean:
	rm -f lex.yy.c 
	rm -f input.output
	rm -f include/input.tab.h
	rm -f input.tab.c
	rm obj/*.o
	rm game


