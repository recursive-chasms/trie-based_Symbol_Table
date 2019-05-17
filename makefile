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
_DEPS = toylang.tab.h p_trie.h defs.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

#Places object files in obj/
_OBJ = lex.yy.o p_trie.o toylang.tab.o
OBJ = $(patsubst %,$(ODIR)/%, $(_OBJ))

#Final compilation: compiles main() and links everything together. 
comp: $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Runs YACC to generate parse table for user input
toylang.tab.c: toylang.y include/defs.h
	bison -dv toylang.y
	mv toylang.tab.h $(IDIR)

include/toylang.tab.h: toylang.y
	bison -dv toylang.y
	mv toylang.tab.h $(IDIR)

#Runs Lex to generate lexical scanner
lex.yy.c: toylang.l include/toylang.tab.h
	flex  toylang.l

#5 commands to compile individual object components before they can be integrated with main()

$(ODIR)/lex.yy.o : lex.yy.c include/toylang.tab.h
	$(CC) -c -o $@ lex.yy.c $(CFLAGS)

$(ODIR)/toylang.tab.o : toylang.tab.c include/defs.h
	$(CC) -c -o $@ toylang.tab.c $(CFLAGS)

$(ODIR)/p_trie.o : p_trie.c include/p_trie.h include/defs.h
	$(CC) -c -o $@ p_trie.c $(CFLAGS)

.PHONY: clean

clean:
	rm -f lex.yy.c 
	rm -f toylang.output
	rm -f include/toylang.tab.h
	rm -f toylang.tab.c
	rm obj/*.o
	rm comp


